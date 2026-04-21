// SPDX-FileCopyrightText: 2026 Pedro Lamarão <pedro.lamarao@purpura.dev.br>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <span>

export module purple.arithmetics:zero;

import :word_concept;

namespace purple::arithmetics
{
    using std::size;

    /// Tests if an integer is zero.

    export
    template <Word W>
    auto is_zero ( span<W const> x ) noexcept -> bool
    {
        auto const xz = size(x);

        // integer is zero if and only if every word is zero
        for (auto i = 0u; i != xz; ++i) {
            if ( not_zero( x[i] ) )
                return false;
        }

        return true;
    }

    /// Tests if an integer is not zero.

    export
    template <Word W>
    auto not_zero ( span<W const> x ) noexcept -> bool
    {
        auto const xz = size(x);

    	// integer is not zero if and only if any word is nonzero
        for (auto i = 0u; i != xz; ++i) {
            if ( not_zero( x[i] ) )
                return true;
        }

        return false;
    }
}
