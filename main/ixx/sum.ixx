// SPDX-FileCopyrightText: 2026 Pedro Lamarão <pedro.lamarao@purpura.dev.br>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <algorithm>
#include <span>
#include <tuple>

export module purple.arithmetics:sum;

import :word_concept;

namespace purple::arithmetics
{
    using std::size;
    using std::span;
    using std::ranges::min;
    using std::tie;

    /// Sum of nonnegative integers `x` and `y`.
    ///
    /// Stores into `r` the `size(r)` least significant words of the result.
    /// Permits aliasing `r` to `x`, in which case it "accumulates" the result.
    /// Returns the "carry" of the top word of the result.
    ///
    /// This implementation applies the "school" method described in Knuth, section 4.3.1.

    export
    template <Word W>
    auto sum ( span<W> r, span<W const> x, W y ) noexcept -> W
    {
        auto carry = W{0};

        auto const sz = size(r);
        auto const xz = size(x);

        // count of result words
        auto const z = min({ sz, xz });

        // add x[0] and y, propagating carry
        if (0 < z)
            tie( r[0], carry ) = sum( x[0], y, W{0} );

        // propagate carry through x
        for (auto i = 1uz; i < z; ++i)
            tie( r[i], carry ) = sum( x[i], W{0}, carry );

        return carry;
    }

    /// Sum of nonnegative integers `x` and `y`.
    ///
    /// Stores into `r` the `size(r)` least significant words of the result.
    /// Permits aliasing `r` to `x`, in which case it "accumulates" the result.
    /// Returns the carry of the top word of the result.
    ///
    /// This implementation applies the "school" method described in Knuth, section 4.3.1.

    export
    template <Word W>
    auto sum ( span<W> r, span<W const> x, span<W const> y ) noexcept -> W
    {
        auto c = W{0};

        auto const rz = size(r);
        auto const xz = size(x);
        auto const yz = size(y);

        // count of result words
        auto const z = min({ rz, xz, yz });

        // add x and y, word by word, propagating carry
        for (auto i = 0uz; i < z; ++i)
            tie( r[i], c ) = sum( x[i], y[i], c );

        // either propagate carry through x
        for (auto i = z, j = min(rz, xz); i < j; ++i)
            tie( r[i], c ) = sum( x[i], W{0}, c );

        // or propagate carry through y
        for (auto i = z, j = min(rz, yz); i < j; ++i)
            tie( r[i], c ) = sum( W{0}, y[i], c );

        return c;
    }
}

