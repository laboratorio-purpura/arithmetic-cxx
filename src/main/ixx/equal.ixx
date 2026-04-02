// SPDX-FileCopyrightText: 2026 Pedro Lamarão <pedro.lamarao@purpura.dev.br>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <algorithm>
#include <span>

export module purple.arithmetic:equal;

import :word_concept;

namespace purple
{
    using std::size;
    using std::ranges::min;

    /// Tests if two integers are equal.

    export
    template <Word W>
    auto is_equal ( span<W const> x, span<W const> y ) -> bool
    {
        auto const xz = size(x);
        auto const yz = size(y);

        auto const z = min( xz, yz );

        for (auto i = 0uz; i != z; ++i) {
            if ( not_equal( x[i], y[i] ) )
                return false;
        }

        for (auto i = z; i != xz; ++i) {
            if ( not_zero( x[i] ) )
                return false;
        }

        for (auto i = z; i != yz; ++i) {
            if ( not_zero( y[i] ) )
                return false;
        }

        return true;
    }

    /// Tests if two integers are not equal.

    export
    template <Word W>
    auto not_equal ( span<W const> x, span<W const> y ) -> bool
    {
        return ! is_equal( x, y );
    }
}
