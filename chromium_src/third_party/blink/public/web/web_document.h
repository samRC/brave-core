/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CHROMIUM_SRC_THIRD_PARTY_BLINK_PUBLIC_WEB_WEB_DOCUMENT_H_
#define CHROMIUM_SRC_THIRD_PARTY_BLINK_PUBLIC_WEB_WEB_DOCUMENT_H_

#include "third_party/blink/renderer/core/permissions_policy/dom_feature_policy.h"

#define IsPluginDocument IsDOMFeaturePolicyEnabled(           \
      v8::Local<v8::Context> context, const String& feature); \
  BLINK_EXPORT bool IsPluginDocument

#include "../../../../../../third_party/blink/public/web/web_document.h"

#undef IsPluginDocument

#endif
