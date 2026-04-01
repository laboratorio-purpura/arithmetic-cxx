// SPDX-FileCopyrightText: 2026 Pedro Lamarão <pedro.lamarao@purpura.dev.br>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <cassert>
#include <span>

export module purple.arithmetic:add;

import :word_concept;

export namespace purple
{
    /// Sum with carry.
    ///
    /// Requires:
    /// size(r) ≥ 2
    ///
    /// Permits aliasing r to x.
    ///
    /// Stores 2 words into counted range [ `begin(r)`, 2 ).
    ///
    /// @return borrow bit.

    template <Word W, size_t Bi>
    requires ( Bi == 2uz )
    auto sum_assign ( span<W> r, span<W const,Bi> x, W y, W carry = W{0u} ) noexcept -> W
    {
        assert( size(r) >= 2 );

        carry = sum_assign( r[0], x[0], y, carry );
        carry = sum_assign( r[1], x[1], carry );
        return carry;
    }

    /// Sum with carry.
    ///
    /// Requires:
    /// size(r) ≥ 2
    ///
    /// Permits aliasing r to x.
    ///
    /// Stores 2 words into counted range [ `begin(r)`, 2 ).
    ///
    /// @return borrow bit.

    template <Word W, size_t Bi>
    requires ( Bi == 2uz )
    auto sum_assign ( span<W> r, span<W const,Bi> x, span<W const,Bi> y, W carry = W{0u} ) noexcept -> W
    {
        assert( size(r) >= 2 );

        carry = sum_assign( r[0], x[0], y[0], carry );
        carry = sum_assign( r[1], x[1], y[1], carry );
        return carry;
    }

    /// Sum with carry.
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
    auto sum_assign ( span<W> r, span<W const> x, W y, W carry = W(0) ) noexcept -> W
    {
        auto const xz = size(x);
        assert( xz >= 1 );
        auto const rz = size(r);
        assert( rz >= xz );

        carry = sum_assign( r[0], x[0], y, carry );
        for (auto i = 1uz; i != xz; ++i)
            carry = sum_assign( r[i], x[i], carry );
        return carry;
    }

    /// Sum with carry.
    ///
    /// Requires:
    /// size(r) ≥ size(x) ≥ size(y)
    ///
    /// Permits aliasing r to x.
    ///
    /// Stores rd = size(x) words into counted range [ begin(r), rd ).
    ///
    /// @return carry bit.

    template <Word W>
    auto sum_assign ( span<W> r, span<W const> x, span<W const> y, W carry = W(0) ) noexcept -> W
    {
        auto const yz = size(y);
        assert( yz >= 1 );
        auto const xz = size(x);
        assert( xz >= yz );
        auto const rz = size(r);
        assert( rz >= xz );

        for (auto i = 0uz; i != yz; ++i)
            carry = sum_assign( r[i], x[i], y[i], carry );
        for (auto i = yz; i != xz; ++i)
            carry = sum_assign( r[i], x[i], carry );
        return carry;
    }
}

