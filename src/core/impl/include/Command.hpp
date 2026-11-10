// Copyright (c) 2026 Sebastian Pineda (spineda.wpi.alum@gmail.com)
//
// Command.hpp - Base class for executable game commands
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef SRC_CORE_INCLUDE_COMMAND_HPP_
#define SRC_CORE_INCLUDE_COMMAND_HPP_

namespace arigato::core {
class Command {
 public:
    /// \brief required due to this class being virtual.
    virtual ~Command() = default;
    virtual void operator()() noexcept = 0;
};

class NullCommand final : public Command {
    void operator()() noexcept override {}
};
}  // namespace arigato::core

#endif  // SRC_CORE_INCLUDE_COMMAND_HPP_
