/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/ios/browser/api/brave_wallet/brave_wallet_utils_ios.h"

#include "base/strings/sys_string_conversions.h"
#include "brave/components/brave_wallet/browser/brave_wallet_utils.h"

@implementation BraveWalletNetworkInfo (Utils)

/// Whether or not this network is a bundled/known network
- (bool)isKnownNetwork {
  return brave_wallet::KnownEthChainExists(
            base::SysNSStringToUTF8(self.chainId));
}

@end
