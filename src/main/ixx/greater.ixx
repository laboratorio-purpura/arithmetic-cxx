// SPDX-FileCopyrightText: 2026 Pedro Lamarão <pedro.lamarao@purpura.dev.br>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <span>
#include <tuple>

export module purple.arithmetic:greater;

import :word_concept;

export namespace purple
{
    /// Tests if greater.
    ///
    /// Requires:
    /// size(x) = size(y)

    template <Word W>
    auto is_greater_isodegree ( span<W const> x, span<W const> y ) -> bool
    {
        return is_smaller_isodegree( y, x );
    }

    /// Tests if greater.

    template <Word W>
    auto is_greater ( span<W const> x, span<W const> y ) -> bool
    {
        auto const xz = x.size();
        auto const yz = y.size();
        if (xz < yz) return is_smaller( y, x );
        auto c = is_greater_isodegree( x.subspan(0,y.size()), y );
        for (auto i = yz; i != xz; ++i)
            c = c || not_zero( x[i] );
        return c;
    }

    /// Tests if *not* greater.
    ///
    /// Requires:
    /// size(x) = size(y)

    template <Word W>
    auto not_greater_isodegree ( span<W const> x, span<W const> y ) -> bool
    {
        return ! is_smaller_isodegree( y, x );
    }

    /// Tests if *not* greater.

    template <Word W>
    auto not_greater ( span<W const> x, span<W const> y ) -> bool
    {
        return ! is_smaller( y, x );
    }
}