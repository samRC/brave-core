/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/ui/views/frame/brave_tab_strip_region_container.h"

#include "brave/browser/ui/views/sidebar/sidebar_button_view.h"
#include "brave/browser/ui/views/tabs/brave_new_tab_button.h"
#include "brave/browser/ui/views/tabs/brave_vertical_tab_utils.h"
#include "chrome/browser/themes/theme_properties.h"
#include "chrome/browser/ui/color/chrome_color_id.h"
#include "chrome/browser/ui/tabs/tab_style.h"
#include "chrome/grit/generated_resources.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/theme_provider.h"
#include "ui/gfx/canvas.h"
#include "ui/views/accessibility/accessibility_paint_checks.h"
#include "ui/views/controls/scroll_view.h"
#include "ui/views/layout/fill_layout.h"
#include "ui/views/layout/flex_layout.h"

namespace {

class ScrollHeaderView : public views::Button {
 public:
  ScrollHeaderView(Button::PressedCallback callback, TabStrip* tab_strip)
      : Button(std::move(callback)), tab_strip_(tab_strip) {
    // TODO(sangwoo.ko) Temporary workaround before we have a proper tooltip
    // text.
    SetProperty(views::kSkipAccessibilityPaintChecks, true);
  }
  ~ScrollHeaderView() override = default;

  // views::Button:
  void OnPaintBackground(gfx::Canvas* canvas) override {
    canvas->DrawColor(GetColorProvider()->GetColor(
        tab_strip_->ShouldPaintAsActiveFrame()
            ? kColorNewTabButtonBackgroundFrameActive
            : kColorNewTabButtonBackgroundFrameInactive));
  }

  void PaintButtonContents(gfx::Canvas* canvas) override {
    // Draw '>'  or '<'
    cc::PaintFlags flags;
    flags.setAntiAlias(true);
    // The same color with NewTabButton::GetForegroundColor()
    flags.setColor(tab_strip_->GetTabForegroundColor(TabActive::kInactive));
    flags.setStrokeCap(cc::PaintFlags::kRound_Cap);
    constexpr int kStrokeWidth = 2;
    flags.setStrokeWidth(kStrokeWidth);

    const int icon_width =
        TabStyle::GetPinnedWidth() - TabStyle::GetTabOverlap();
    const bool expanded = icon_width < width();

    const int icon_inset = ui::TouchUiController::Get()->touch_ui() ? 10 : 9;
    gfx::Rect icon_bounds(gfx::Size(icon_width, height()));
    if (expanded)
      icon_bounds.set_x(width() - icon_width);
    icon_bounds.Inset(gfx::Insets::VH(icon_inset, icon_inset * 1.5));

    if (expanded) {
      // '<
      const gfx::Point start = icon_bounds.top_right();
      const gfx::Point mid = {icon_bounds.x(), icon_bounds.CenterPoint().y()};
      const gfx::Point end = icon_bounds.bottom_right();
      canvas->DrawLine(start, mid, flags);
      canvas->DrawLine(mid, end, flags);
    } else {
      // '>'
      const gfx::Point start = icon_bounds.origin();
      const gfx::Point mid = {icon_bounds.right(),
                              icon_bounds.CenterPoint().y()};
      const gfx::Point end = icon_bounds.bottom_left();
      canvas->DrawLine(start, mid, flags);
      canvas->DrawLine(mid, end, flags);
    }
  }

 private:
  TabStrip* tab_strip_ = nullptr;
};

class ScrollContentsView : public views::View {
 public:
  explicit ScrollContentsView(BraveTabStripRegionContainer* container)
      : container_(container) {}
  ~ScrollContentsView() override = default;

  // views::View:
  void ChildPreferredSizeChanged(views::View* child) override {
    if (height() == GetPreferredSize().height())
      return;

    SetSize({width(), GetPreferredSize().height()});
    container_->Layout();
  }

 private:
  raw_ptr<BraveTabStripRegionContainer> container_ = nullptr;
};

}  // namespace

BraveTabStripRegionContainer::BraveTabStripRegionContainer(
    TabStripRegionView* region_view)
    : region_view_(region_view) {
  scroll_view_ = AddChildView(std::make_unique<views::ScrollView>());
  scroll_view_header_ =
      scroll_view_->SetHeader(std::make_unique<ScrollHeaderView>(
          base::BindRepeating(
              [](BraveTabStripRegionContainer* container,
                 const ui::Event& event) {
                container->SetState(
                    container->state_ ==
                            BraveTabStripRegionContainer::State::kCollapsed
                        ? BraveTabStripRegionContainer::State::kExpanded
                        : BraveTabStripRegionContainer::State::kCollapsed);
              },
              this),
          region_view_->tab_strip_));

  auto* scroll_contents =
      scroll_view_->SetContents(std::make_unique<ScrollContentsView>(this));
  scroll_contents->SetLayoutManager(std::make_unique<views::FillLayout>());
  scroll_view_->SetVerticalScrollBarMode(
      views::ScrollView::ScrollBarMode::kHiddenButEnabled);

  new_tab_button_ = AddChildView(std::make_unique<BraveNewTabButton>(
      region_view_->tab_strip_,
      base::BindRepeating(&TabStrip::NewTabButtonPressed,
                          base::Unretained(region_view_->tab_strip_))));
  new_tab_button_->SetTooltipText(
      l10n_util::GetStringUTF16(IDS_TOOLTIP_NEW_TAB));
  new_tab_button_->SetAccessibleName(
      l10n_util::GetStringUTF16(IDS_ACCNAME_NEWTAB));

  UpdateLayout();
}

BraveTabStripRegionContainer::~BraveTabStripRegionContainer() {}

void BraveTabStripRegionContainer::SetState(State state) {
  if (state_ == state)
    return;

  state_ = state;
  InvalidateLayout();
}

gfx::Size BraveTabStripRegionContainer::CalculatePreferredSize() const {
  if (!ShouldShowVerticalTabs())
    return {};

  if (state_ == State::kExpanded) {
    return {TabStyle::GetStandardWidth(),
            View::CalculatePreferredSize().height()};
  }

  DCHECK_EQ(state_, State::kCollapsed)
      << "If a new state was added, " << __FUNCTION__
      << " should be revisited.";

  return {TabStyle::GetPinnedWidth() - TabStyle::GetTabOverlap(),
          View::CalculatePreferredSize().height()};
}

void BraveTabStripRegionContainer::Layout() {
  // As we have to update ScrollView's viewport size and its contents size,
  // layouting children manually will be more handy.

  // 1. New tab should be fixed at the bottom of container.
  auto contents_bounds = GetContentsBounds();
  new_tab_button_->SetSize(
      {contents_bounds.width(), new_tab_button_->GetPreferredSize().height()});
  new_tab_button_->SetPosition(
      {contents_bounds.x(),
       contents_bounds.bottom() - new_tab_button_->height()});

  // 2. ScrollView takes the rest of space.
  // Set preferred size for scroll view to know this.
  const gfx::Size header_size{scroll_view_->width(),
                              new_tab_button_->GetPreferredSize().height()};
  scroll_view_header_->SetPreferredSize(header_size);
  scroll_view_header_->SetSize(header_size);
  scroll_view_->SetSize({contents_bounds.width(),
                         contents_bounds.height() - new_tab_button_->height()});
  scroll_view_->SetPosition(contents_bounds.origin());
  scroll_view_->ClipHeightTo(0, scroll_view_->height() - header_size.height());

  auto* scroll_view_contents = scroll_view_->contents();
  scroll_view_contents->SetSize(
      {scroll_view_->width(), scroll_view_contents->height()});

  UpdateNewTabButtonVisibility();
}

void BraveTabStripRegionContainer::UpdateLayout() {
  if (ShouldShowVerticalTabs()) {
    if (!Contains(region_view_)) {
      original_parent_of_region_view_ = region_view_->parent();
      original_parent_of_region_view_->RemoveChildView(region_view_);
      scroll_view_->contents()->AddChildView(region_view_.get());
    }
    region_view_->layout_manager_->SetOrientation(
        views::LayoutOrientation::kVertical);
  } else {
    if (Contains(region_view_)) {
      scroll_view_->contents()->RemoveChildView(region_view_);
      original_parent_of_region_view_->AddChildView(region_view_.get());
    }
    region_view_->layout_manager_->SetOrientation(
        views::LayoutOrientation::kHorizontal);
  }
  Layout();
}

void BraveTabStripRegionContainer::OnThemeChanged() {
  View::OnThemeChanged();

  auto background_color =
      GetThemeProvider()->GetColor(ThemeProperties::COLOR_TOOLBAR);
  SetBackground(views::CreateSolidBackground(background_color));
  scroll_view_->SetBackgroundColor(background_color);

  new_tab_button_->FrameColorsChanged();
}

void BraveTabStripRegionContainer::UpdateNewTabButtonVisibility() {
  bool overflowed =
      scroll_view_->GetMaxHeight() < scroll_view_->contents()->height();
  region_view_->new_tab_button()->SetVisible(!overflowed);
  new_tab_button_->SetVisible(overflowed);
}
