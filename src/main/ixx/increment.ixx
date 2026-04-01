// SPDX-FileCopyrightText: 2026 Pedro Lamarão <pedro.lamarao@purpura.dev.br>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <cassert>
#include <span>

export module purple.arithmetic:increment;

import :word_concept;

export namespace purple
{
    /// Next with carry.
    ///
    /// Requires:
    /// size(r) ≥ 2
    ///
    /// Permits aliasing r to x.
    ///
    /// Stores 2 words into counted range [ `begin(r)`, 2 ).
    ///
    /// @return carry bit.

    template <Word W, size_t Bi>
    requires ( Bi == 2uz )
    auto increment ( span<W> r, span<W const,Bi> x, W carry = W{0u} ) -> W
    {
        assert( size(r) >= 2 );

        carry = increment( r[0], x[0], carry );
        carry = add( r[1], x[1], carry );
        return carry;
    }

    /// Next with carry.
    ///
    /// Requires:
    /// size(r) ≥ size(x)
    ///
    /// Permits aliasing r to x.
    ///
    /// Stores rd = size(x) words into counted range [ begin(r), rd ).
    ///
    /// @return carry bit.

    template <Word W>
    auto increment ( span<W> r, span<W const> x, W carry = W(0) ) -> W
    {
        auto const xz = size(x);
        assert( xz >= 1 );
        auto const rz = size(r);
        assert( rz >= xz );

        carry = increment( r[0], x[0], carry );
        for (auto i = 1uz; i != xz; ++i)
            carry = add( r[i], x[i], carry );
        return carry;
    }
}
