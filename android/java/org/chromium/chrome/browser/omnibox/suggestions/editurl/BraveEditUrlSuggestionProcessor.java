/**
 * Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 */

package org.chromium.chrome.browser.omnibox.suggestions.editurl;

import android.content.Context;

import org.chromium.base.supplier.Supplier;
import org.chromium.chrome.browser.omnibox.suggestions.OmniboxSuggestion;
import org.chromium.chrome.browser.omnibox.suggestions.UrlBarDelegate;
import org.chromium.chrome.browser.omnibox.suggestions.basic.SuggestionHost;
import org.chromium.chrome.browser.ui.favicon.LargeIconBridge;

public class BraveEditUrlSuggestionProcessor extends EditUrlSuggestionProcessor {
    private boolean mHasClearedOmniboxForFocus;

    public BraveEditUrlSuggestionProcessor(Context context, SuggestionHost suggestionHost,
            UrlBarDelegate locationBarDelegate, Supplier<LargeIconBridge> iconBridgeSupplier) {
        super(context, suggestionHost, locationBarDelegate, iconBridgeSupplier);
    }

    @Override
    public boolean doesProcessSuggestion(OmniboxSuggestion suggestion) {
        // We don't want to clear omnibox for focus, so just pretend that it's already happened
        mHasClearedOmniboxForFocus = true;
        return super.doesProcessSuggestion(suggestion);
    }
}
