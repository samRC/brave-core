/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_IOS_BROWSER_API_BRAVE_WALLET_BRAVE_WALLET_UTILS_IOS_H_
#define BRAVE_IOS_BROWSER_API_BRAVE_WALLET_BRAVE_WALLET_UTILS_IOS_H_

#import <Foundation/Foundation.h>
#import "brave_wallet.mojom.objc.h"

NS_ASSUME_NONNULL_BEGIN

@interface BraveWalletNetworkInfo (Utils)

/// Whether or not this network is a bundled/known network
@property(readonly) bool isKnownNetwork;

@end

NS_ASSUME_NONNULL_END

#endif
