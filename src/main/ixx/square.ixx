// SPDX-FileCopyrightText: 2026 Pedro Lamarão <pedro.lamarao@purpura.dev.br>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <cassert>
#include <span>
#include <tuple>

export module purple.arithmetic:square;

import :word_concept;

export namespace purple
{
    /// Square.
    ///
    /// Requires:
    /// size(r) ≥ size(x) × 2
    ///
    /// Stores rd = size(x) + size(y) words into counted range [ begin(r), rd ).

    template <Word W>
    void square_accumulate ( span<W> r, span<W const> x ) noexcept
    {
        auto const xz = size(x);
        assert( xz >= 1 );
        auto const rz = size(r);
        assert( rz >= xz*2 );

        W storage [ 3 ];

        for (auto xi = 0uz; xi != xz; ++xi)
        {
            // xi ^ 2
            {
                // xi ^ 2 + carry
                auto p = product( x[xi], x[xi], W{0} );
                // store
                auto ri = xi+xi;
                auto rs = r.subspan(ri);
                ignore = add<W>( rs, rs, p );
            }
            // 2 * xi * xj
            for (auto xj = xi + 1uz; xj != xz; ++xj)
            {
                // xi * xj
                auto p = product( x[xi], x[xj], W{0} );
                // 2 * xi * xj
                auto t = span<W>( storage, 3 );
                t[2] = double_<W>( t, p, 1uz );
                // store
                auto ri = xi+xj;
                auto rs = r.subspan(ri);
                ignore = add<W>( rs, rs, t );
            }
        }
    }
}