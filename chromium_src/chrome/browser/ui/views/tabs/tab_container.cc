/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/ui/views/tabs/brave_vertical_tab_utils.h"

#define BRAVE_UI_VIEWS_TABS_TAB_CONTAINER_CALCULATE_PREFERRED_SIZE_FOR_VERT_TABS \
  if (ShouldShowVerticalTabs()) {                                                \
    return gfx::Size(                                                            \
        TabStyle::GetStandardWidth(),                                            \
        GetLayoutConstant(TAB_HEIGHT) *                                          \
            (tabs_view_model_.view_size() + group_views_.size()));               \
  }

#define BRAVE_UI_VIEWS_TABS_TAB_CONTAINER_SET_HELPER_TO_USE_VERTICAL_LAYOUT \
  layout_helper_->set_use_vertical_tabs(ShouldShowVerticalTabs());

#define BRAVE_UI_VIEWS_TABS_TAB_CONTAINER_OVERRIDE_TARGET_BOUNDS_FOR_VERTICAL_TABS \
  if (ShouldShowVerticalTabs())                                                    \
    target_bounds.set_y(                                                           \
        (former_model_index > 0)                                                   \
            ? tabs_view_model_.ideal_bounds(former_model_index - 1).bottom()       \
            : 0);                                                                  \
  else

#define BRAVE_UI_VIEWS_TABS_TAB_CONTAINER_SKIP_IF_VERTICAL_TABS \
  if (!ShouldShowVerticalTabs())

#include "src/chrome/browser/ui/views/tabs/tab_container.cc"

#undef BRAVE_UI_VIEWS_TABS_TAB_CONTAINER_SKIP_IF_VERTICAL_TABS
#undef BRAVE_UI_VIEWS_TABS_TAB_CONTAINER_OVERRIDE_TARGET_BOUNDS_FOR_VERTICAL_TABS
#undef BRAVE_UI_VIEWS_TABS_TAB_CONTAINER_SET_HELPER_TO_USE_VERTICAL_LAYOUT
#undef BRAVE_UI_VIEWS_TABS_TAB_CONTAINER_CALCULATE_PREFERRED_SIZE_FOR_VERT_TABS
