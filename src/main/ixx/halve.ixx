// SPDX-FileCopyrightText: 2026 Pedro Lamarão <pedro.lamarao@purpura.dev.br>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <cassert>
#include <span>
#include <tuple>

export module purple.arithmetic:halve;

import :word_concept;

export namespace purple
{
    /// Half with remainder.
    ///
    /// Requires:
    /// z < log2(B)
    /// size(q) ≥ 2
    ///
    /// Permits aliasing r to x.
    ///
    /// Stores 2 words into counted range [ `begin(q)`, 2 ).
    ///
    /// @return remainder word.

    template <Word W, size_t Bi>
    requires ( Bi == 2uz )
    auto halve ( span<W> q, span<W const,Bi> x, size_t z = 1 ) noexcept -> W
    {
        auto const B = sizeof(W) * 8uz;

        assert( size(q) >= 2 );

        auto r1 = W{0u};
        tie( q[1], r1 ) = half( x[1], z );
        tie( r1, ignore ) = twice( r1, B-z );

        auto r0 = W{0u};
        tie( q[0], r0 ) = half( x[0], z );
        tie( q[0], ignore ) = sum( q[0], r1 );

        return r0;
    }

    /// Half with remainder.
    ///
    /// Requires:
    /// size(q) ≥ size(x)
    ///
    /// Permits aliasing r to x.
    ///
    /// Stores rd = size(x) words into counted range [ begin(r), rd ).
    ///
    /// @return remainder word.

    template <Word W>
    auto halve ( span<W> q, span<W const> x, size_t z ) noexcept -> W
    {
        auto const B = sizeof(W) * 8uz;
        auto const xz = size(x);
        assert( xz >= 1 );
        auto const rz = size(q);
        assert( rz >= xz );

        auto r = W(0);
        for (auto i = xz; i != 0; --i) {
            // halve current word
            auto r0 = W(0);
            tie( q[i-1], r0 ) = half( x[i-1], z );
            // add previous remainder
            auto [ r1, _ ] = twice( r, B - z );
            tie( q[i-1], ignore ) = sum( q[i-1], r1 );
            r = r0;
        }
        return r;
    }
}