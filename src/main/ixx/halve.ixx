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