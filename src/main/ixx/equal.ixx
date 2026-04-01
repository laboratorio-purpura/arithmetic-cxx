// SPDX-FileCopyrightText: 2026 Pedro Lamarão <pedro.lamarao@purpura.dev.br>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <span>

export module purple.arithmetic:equal;

import :word_concept;

export namespace purple
{
    /// Tests if equal.

    template <Word W, size_t Bi>
    requires ( Bi == 2uz )
    auto is_equal ( span<W const,Bi> x, span<W const,Bi> y ) noexcept -> bool
    {
        bool r = true;
        r = r && is_equal( x[0], y[0] );
        r = r && is_equal( x[1], y[1] );
        return r;
    }

    /// Tests if equals.
    ///
    /// Requires:
    /// size(x) = size(y)

    template <Word W>
    auto is_equal_isodegree ( span<W const> x, span<W const> y ) -> bool
    {
        auto const z = y.size();
        for (auto i = 0uz; i != z; ++i) {
            if ( not_equal( x[i], y[i] ) )
                return false;
        }
        return true;
    }

    /// Tests if equals.

    template <Word W>
    auto is_equal ( span<W const> x, span<W const> y ) -> bool
    {
        auto const xz = x.size();
        auto const yz = y.size();
        if (xz < yz) return is_equal(y,x);
        if (! is_equal_isodegree( x, y ))
            return false;
        for (auto i = yz; i != xz; ++i) {
            if ( not_zero( x[i] ) )
                return false;
        }
        return true;
    }

    /// Tests if *not* equal.

    template <Word W, size_t Bi>
    requires ( Bi == 2uz )
    auto not_equal ( span<W const,Bi> x, span<W const,Bi> y ) noexcept -> bool
    {
        bool r = false;
        r = r || not_equal( x[0], y[0] );
        r = r || not_equal( x[1], y[1] );
        return r;
    }

    /// Tests if *not* equals.
    ///
    /// Requires:
    /// size(x) = size(y)

    template <Word W>
    auto not_equal_isodegree ( span<W const> x, span<W const> y ) -> bool
    {
        return ! is_equal_isodegree( x, y );
    }

    /// Tests if *not* equals.

    template <Word W>
    auto not_equal ( span<W const> x, span<W const> y ) -> bool
    {
        return ! is_equal( x, y );
    }
}
