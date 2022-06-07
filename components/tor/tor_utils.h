/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_TOR_TOR_UTILS_H_
#define BRAVE_COMPONENTS_TOR_TOR_UTILS_H_

#include <string>
#include <vector>

#include "base/values.h"

class PrefService;

namespace tor {

struct BridgesConfig {
  enum class Usage {
    kNotUsed,
    kBuiltIn,
    kRequest,
    kProvide,
  };

  enum class BuiltinType {
    kSnowflake,
    kObfs4,
    kMeekAzure,
  };

  BridgesConfig();
  BridgesConfig(BridgesConfig&&) noexcept;
  BridgesConfig(const BridgesConfig&) = delete;
  ~BridgesConfig();

  BridgesConfig& operator=(BridgesConfig&&) noexcept;
  BridgesConfig& operator=(const BridgesConfig&) = delete;

  const std::vector<std::string>& GetBuiltinBridges() const;

  static absl::optional<BridgesConfig> FromValue(const base::Value* v);
  base::Value::Dict ToDict() const;
  base::Value ToValue() const;

  Usage use_bridges = Usage::kNotUsed;
  BuiltinType use_builtin = BuiltinType::kObfs4;
  std::vector<std::string> requested_bridges;
  std::vector<std::string> provided_bridges;
};

void MigrateLastUsedProfileFromLocalStatePrefs(PrefService* local_state);

}  // namespace tor

#endif  // BRAVE_COMPONENTS_TOR_TOR_UTILS_H_
