// SPDX-FileCopyrightText: 2026 Pedro Lamarão <pedro.lamarao@purpura.dev.br>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <span>

export module purple.arithmetics:odd;

import :word_concept;

export namespace purple
{
    /// Tests if *not* odd.
    ///
    /// In a binary machine, this means the least significant bit is 0.

    template <Word W, size_t Bi>
    requires ( Bi == 2uz )
    auto not_odd ( span<W const,Bi> x ) noexcept -> bool
    {
        return not_odd( x[0] );
    }
}