// SPDX-FileCopyrightText: 2026 Pedro Lamarão <pedro.lamarao@purpura.dev.br>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <algorithm>
#include <span>

export module purple.arithmetic:smaller;

import :word_concept;

namespace purple
{
    using std::size;
    using std::ranges::min;

    /// Tests if an integer is smaller than another.

    export
    template <Word W>
    auto is_smaller ( span<W const> x, span<W const> y ) -> bool
    {
        auto const xz = size(x);
        auto const yz = size(y);

        auto const z = min( xz, yz );

        for (auto i = xz; i > z; --i) {
            if ( not_zero( x[i-1] ) )
                return false;
        }

        for (auto i = yz; i > z; --i) {
            if ( not_zero( y[i-1] ) )
                return true;
        }

        for (auto i = z; i > 0; --i) {
            if ( is_smaller( x[i-1], y[i-1] ) )
                return true;
            if ( is_greater( x[i-1], y[i-1] ) )
                return false;
        }

        return false;
    }

    /// Tests if an integer is not smaller than another.

    export
    template <Word W>
    auto not_smaller ( span<W const> x, span<W const> y ) -> bool
    {
        return ! is_smaller( x, y );
    }
}