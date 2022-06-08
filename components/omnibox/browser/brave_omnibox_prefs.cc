/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/omnibox/browser/brave_omnibox_prefs.h"

#include "components/prefs/pref_registry_simple.h"

namespace omnibox {

const char kAutocompleteEnabled[] = "brave.autocomplete_enabled";
const char kTopSiteSuggestionsEnabled[] = "brave.top_site_suggestions_enabled";
const char kBraveSuggestedSiteSuggestionsEnabled[] =
    "brave.brave_suggested_site_suggestions_enabled";
const char kHistorySuggestionsEnabled[] =
    "brave.omnibox.history_suggestions_enabled";
const char kBookmarkSuggestionsEnabled[] =
    "brave.omnibox.bookmark_suggestions_enabled";

void RegisterBraveProfilePrefs(PrefRegistrySimple* registry) {
  registry->RegisterBooleanPref(kAutocompleteEnabled, true);
  registry->RegisterBooleanPref(kTopSiteSuggestionsEnabled, true);
  registry->RegisterBooleanPref(kBraveSuggestedSiteSuggestionsEnabled, false);
  registry->RegisterBooleanPref(kHistorySuggestionsEnabled, true);
  registry->RegisterBooleanPref(kBookmarkSuggestionsEnabled, true);
}

}  // namespace omnibox
