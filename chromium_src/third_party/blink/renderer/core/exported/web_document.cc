/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/public/web/web_document.h"
#include "third_party/blink/renderer/platform/bindings/script_state.h"
#include "third_party/blink/renderer/core/html/plugin_document.h"

#define IsPluginDocument IsDOMFeaturePolicyEnabled(                          \
        v8::Local<v8::Context> context,                                      \
        const String& feature) {                                             \
    blink::ScriptState* script_state = blink::ScriptState::From(context);    \
    Document* document = Unwrap<Document>();                                 \
    return document->featurePolicy()->allowsFeature(script_state, feature);  \
  }                                                                          \
  bool WebDocument::IsPluginDocument

#include "../../../../../../../third_party/blink/renderer/core/exported/web_document.cc"

#undef IsPluginDocument
