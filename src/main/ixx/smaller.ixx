// SPDX-FileCopyrightText: 2026 Pedro Lamarão <pedro.lamarao@purpura.dev.br>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <cassert>
#include <span>
#include <tuple>

export module purple.arithmetic:smaller;

import :word_concept;

export namespace purple
{
    /// Tests if smaller.
    ///
    /// Requires:
    /// size(x) = size(y)

    template <Word W>
    auto is_smaller_isodegree ( span<W const> x, span<W const> y ) -> bool
    {
        assert( x.size() == y.size() );

        auto const z = y.size();
        auto b = W(0);
        for (auto i = 0uz; i != z; ++i)
            tie( ignore, b ) = difference( x[i], y[i], b );
        return not_zero( b );
    }

    /// Tests if smaller.

    template <Word W>
    auto is_smaller ( span<W const> x, span<W const> y ) -> bool
    {
        auto const xz = x.size();
        auto const yz = y.size();
        if (xz < yz) return is_greater( y, x );
        auto c = is_smaller_isodegree( x.subspan(0,y.size()), y );
        for (auto i = yz; i != xz; ++i)
            c = c && is_zero( x[i] );
        return c;
    }

    /// Tests if *not* smaller.
    ///
    /// Requires:
    /// size(x) = size(y)

    template <Word W>
    auto not_smaller_isodegree ( span<W const> x, span<W const> y ) -> bool
    {
        return ! is_smaller_isodegree( x, y );
    }

    /// Tests if *not* smaller.

    template <Word W>
    auto not_smaller ( span<W const> x, span<W const> y ) -> bool
    {
        return ! is_smaller( x, y );
    }
}