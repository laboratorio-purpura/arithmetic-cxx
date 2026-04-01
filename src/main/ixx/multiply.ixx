// SPDX-FileCopyrightText: 2026 Pedro Lamarão <pedro.lamarao@purpura.dev.br>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <cassert>
#include <span>
#include <tuple>

export module purple.arithmetic:multiply;

import :word_concept;

export namespace purple
{
    /// Product with excess.
    ///
    /// Requires:
    /// words(r) ≥ size(x)
    ///
    /// Permits aliasing r to x.
    ///
    /// Stores rd = size(x) words into counted range [ begin(r), rd ).
    ///
    /// @return excess word.

    template <Word W>
    auto product_assign ( span<W> r, span<W const> x, W y, W excess = W(0) ) noexcept -> W
    {
        auto const xz = size(x);
        assert( xz >= 1 );
        auto const rz = size(r);
        assert( rz >= xz );

        for (auto i = 0uz; i != xz; ++i)
            excess = product_assign( r[i], x[i], y, excess );
        return excess;
    }

    /// Product.
    ///
    /// Requires:
    /// size(r) ≥ size(x) + size(y)
    ///
    /// Stores rd = size(x) + size(y) words into counted range [ begin(r), rd ).

    template <Word W>
    void product_accumulate ( span<W> r, span<W const> x, span<W const> y ) noexcept
    {
        auto const xz = size(x);
        assert( xz >= 1 );
        auto const yz = size(y);
        assert( yz >= 1 );
        auto const rz = size(r);
        assert( rz >= xz + yz );
        assert( is_zero<W>( r.subspan(xz) ) );

        for (auto j = 0uz; j != yz; ++j)
        {
            auto excess = W{0u};
            for (auto i = 0uz; i != xz; ++i)
            {
                // x[i] × y[j]
                auto p = product( x[i], y[j], excess );
                // store into r[i+j]
                auto ri = i+j;
                auto c = sum_assign( r[ri], r[ri], p[0] );
                ignore = sum_assign( excess, p[1], c );
            }
            // store excess into r
            r[xz+j] = excess;
        }
    }
}
