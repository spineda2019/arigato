/// \file
/// \brief Common types and values for ID coherence b/w game and rendering
///
/// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
///
/// This Source Code Form is subject to the terms of the Mozilla Public
/// License, v. 2.0. If a copy of the MPL was not distributed with this
/// file, You can obtain one at https://mozilla.org/MPL/2.init/.

#ifndef SRC_COMMON_TYPES_ARIGATO_ID_HPP_
#define SRC_COMMON_TYPES_ARIGATO_ID_HPP_

#include <cstdint>

namespace arigato::id {
using Id_t = std::uint16_t;

enum class Id : Id_t {
    Unknown,
    Kitters,
    CafeBar,
};

}  // namespace arigato::id

#endif  // SRC_COMMON_TYPES_ARIGATO_ID_HPP_
