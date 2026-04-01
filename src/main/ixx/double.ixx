// SPDX-FileCopyrightText: 2026 Pedro Lamarão <pedro.lamarao@purpura.dev.br>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <cassert>
#include <span>

export module purple.arithmetic:double_;

import :word_concept;

export namespace purple
{
    /// Twice with excess.
    ///
    /// Requires:
    /// z < log2(B)
    /// size(r) ≥ 2
    ///
    /// Permits aliasing r to x.
    ///
    /// Stores 2 words into counted range [ `begin(r)`, 2 ).
    ///
    /// @return excess word.

    template <Word W, size_t Bi>
    requires ( Bi == 2uz )
    auto double_ ( span<W> r, span<W const,Bi> x, size_t z, W excess = W{0u} ) noexcept -> W
    {
        assert( size(r) >= 2 );

        excess = double_( r[0], x[0], z, excess );
        excess = double_( r[1], x[1], z, excess );
        return excess;
    }

    /// Twice with excess.
    ///
    /// Requires:
    /// z < log2(B)
    /// size(r) ≥ size(x)
    ///
    /// Permits aliasing r to x.
    ///
    /// Stores rd = size(x) words into counted range [ begin(r), rd ).
    ///
    /// @return excess word.

    template <Word W>
    auto double_ ( span<W> r, span<W const> x, size_t z, W excess = W(0) ) noexcept -> W
    {
        auto const xz = size(x);
        assert( xz >= 1 );
        auto const rz = size(r);
        assert( rz >= xz );

        for (auto i = 0uz; i != xz; ++i)
            excess = double_( r[i], x[i], z, excess );
        return excess;
    }
}