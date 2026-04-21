// SPDX-FileCopyrightText: 2026 Pedro Lamarão <pedro.lamarao@purpura.dev.br>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <span>

export module purple.arithmetics:normal;

import :word_concept;

export namespace purple::arithmetics
{
    /// Tests if normalized.
    ///
    /// Let N = size(y).
    /// Normalized means B ÷ 2 ≤ y[N-1] < B.
    ///
    /// In a binary machine, this means the most significant bit is 1.

    template <Word W>
    auto is_normalized ( span<W const> y ) noexcept -> bool
    {
        return is_normalized( y[ y.size() - 1 ] );
    }
}
