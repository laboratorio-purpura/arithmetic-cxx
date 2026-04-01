// SPDX-FileCopyrightText: 2026 Pedro Lamarão <pedro.lamarao@purpura.dev.br>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <cassert>
#include <span>

export module purple.arithmetic:decrement;

import :word_concept;

export namespace purple
{
    /// Previous with borrow.
    ///
    /// Requires:
    /// size(r) ≥ size(x)
    ///
    /// Permits aliasing r to x.
    ///
    /// Stores rd = size(x) words into counted range [ begin(r), rd ).
    ///
    /// @return borrow bit.

    template <Word W>
    auto previous_assign ( span<W> r, span<W const> x, W borrow = W(0) ) -> W
    {
        auto const xz = size(x);
        assert( xz >= 1 );
        auto const rz = size(r);
        assert( rz >= xz );

        borrow = previous_assign( r[0], x[0], borrow );
        for (auto i = 1uz; i != xz; ++i)
            borrow = difference_assign( r[i], x[i], borrow );
        return borrow;
    }

}
