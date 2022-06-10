/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/ui/views/tabs/brave_vertical_tab_utils.h"

#define BRAVE_SKIP_IF_VERTICAL_TAB \
  if (ShouldShowVerticalTabs())    \
    return top;

#include "src/chrome/browser/ui/views/frame/browser_view_layout.cc"

#undef BRAVE_SKIP_IF_VERTICAL_TAB
