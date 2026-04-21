// SPDX-FileCopyrightText: 2026 Pedro Lamarão <pedro.lamarao@purpura.dev.br>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <algorithm>
#include <span>

export module purple.arithmetics:greater;

import :word_concept;

namespace purple::arithmetics
{
    using std::size;
    using std::ranges::min;

    /// Tests if an integer is greater than another.

    export
    template <Word W>
    auto is_greater ( span<W const> x, span<W const> y ) -> bool
    {
        auto const xz = size(x);
        auto const yz = size(y);

        auto const z = min( xz, yz );

        for (auto i = xz; i > z; --i) {
            if ( not_zero( x[i-1] ) )
                return true;
        }

        for (auto i = yz; i > z; --i) {
            if ( not_zero( y[i-1] ) )
                return false;
        }

        for (auto i = z; i > 0; --i) {
            if ( is_smaller( x[i-1], y[i-1] ) )
                return false;
            if ( is_greater( x[i-1], y[i-1] ) )
                return true;
        }

        return false;
    }

    /// Tests if an integer is not greater than another.

    export
    template <Word W>
    auto not_greater ( span<W const> x, span<W const> y ) -> bool
    {
        return ! is_smaller( y, x );
    }
}