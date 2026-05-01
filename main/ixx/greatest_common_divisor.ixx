// Copyright (c) 2026 Pedro Lamarão <pedro.lamarao@purpura.dev.br>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <span>
#include <utility>

export module purple.arithmetics:greatest_common_divisor;

import :half;
import :product;
import :square;
import :word_concept;

namespace purple::arithmetics
{
    using std::span;
    using std::swap;

    /// Greatest common divisor of nonnegative integers `x` and `y`.
    ///
    /// "Clobbers" the contents of `x` and `y`.
    /// Returns result aliased to `x` or `y`.
    ///
    /// This implementation applies Euclid's method.

    export
    template <Word W>
    auto greatest_common_divisor_clobber (span<W> x, span<W> y) -> span<W>
    {
        while ( not_zero<W>( y ) ) {
            division<W>( span<W>{}, x, x, y );
            swap( x, y );
        }
        return x;
    }

    /// Greatest common divisor of nonnegative integers `x` and `y`.
    ///
    /// Stores into `r` the `size(r)` least significant words of the result.
    ///
    /// This implementation applies Euclid's method.

    export
    template <Word W>
    void greatest_common_divisor (span<W> r, span<const W> x, span<const W> y)
    {
        W storage [ size(x) + size(y) ];
        auto x_ = span<W>( storage, size(x) );
        assign<W>( x_, x );
        auto y_ = span<W>( storage + size(x), size(y) );
        assign<W>( y_, y );

        auto r_ = greatest_common_divisor_clobber( x_, y_ );
        assign<W>( r, r_ );
    }
}
