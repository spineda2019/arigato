// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// Component.hpp - Common tools for components in the system
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef SRC_CORE_INCLUDE_COMPONENTS_COMPONENT_HPP_
#define SRC_CORE_INCLUDE_COMPONENTS_COMPONENT_HPP_

#include <type_traits>

namespace arigato::component {
/// \brief Common constraints for components
///
/// Components are most powerful when stored in a contiguous container (for DoD
/// and cache-locality and such). These constraints ensure that common
/// containers (e.g. vector) can work with these types as cheaply/efficiently as
/// possible
template <class T>
concept Component = std::is_trivially_constructible_v<T> &&
                    std::is_trivially_copy_constructible_v<T> &&
                    std::is_trivially_copyable_v<T> &&
                    std::is_trivially_move_constructible_v<T> &&
                    std::is_trivially_destructible_v<T>;
}  // namespace arigato::component

#endif  // SRC_CORE_INCLUDE_COMPONENTS_COMPONENT_HPP_
