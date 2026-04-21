// SPDX-FileCopyrightText: 2026 Pedro Lamarão <pedro.lamarao@purpura.dev.br>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <algorithm>
#include <span>
#include <tuple>

export module purple.arithmetics:subtract;

import :word_concept;

namespace purple::arithmetics
{
    using std::size;
    using std::ranges::min;
    using std::tie;

    /// Computes the difference between two integers.
    ///
    /// Subtract stores into difference the size(difference) least significant words of the result.
    /// It permits aliasing difference to x, in which case it becomes "subtract accumulate".
    ///
    /// This implementation applies the "school" method described in Knuth, section 4.3.1.

    export
    template <Word W>
    auto subtract ( span<W> difference, span<W const> x, W y ) noexcept -> W;

    export
    template <Word W>
    auto subtract ( span<W> result, span<W const> x, W y ) noexcept -> W
    {
        auto borrow = W{0};

        auto const sz = size(result);
        auto const xz = size(x);

        // count of result words to compute
        auto const z = min({ sz, xz });

        // subtract x[0] and y,
        // propagating borrow
        if (z > 0)
            tie( result[0], borrow ) = difference( x[0], y, W{0} );

        // propagate borrow through x
        for (auto i = 1uz; i != z; ++i)
            tie( result[i], borrow ) = difference( x[i], W{0}, borrow );

        return borrow;
    }

    /// Computes the difference between two integers.
    ///
    /// Subtract stores into difference the size(difference) least significant words of the result.
    /// It permits aliasing difference to x, in which case it becomes "subtract accumulate".
    ///
    /// This implementation applies the "school" method described in Knuth, section 4.3.1.

    export
    template <Word W>
    auto subtract ( span<W> difference, span<W const> x, span<W const> y ) noexcept -> W;

    template <Word W>
    auto subtract ( span<W> result, span<W const> x, span<W const> y ) noexcept -> W
    {
        auto borrow = W{0};

        auto const dz = size(result);
        auto const xz = size(x);
        auto const yz = size(y);

        // count of result words to compute
        auto const z = min({ dz, xz, yz });

        // subtract x and y word by word,
        // from least to most significant,
        // propagating borrow
        for (auto i = 0uz; i != z; ++i)
            tie( result[i], borrow ) = difference( x[i], y[i], borrow );

        // either propagate borrow through x
        for (auto i = z; i < xz; ++i)
            tie( result[i], borrow ) = difference( x[i], W{0}, borrow );

        // or propagate borrow through y
        for (auto i = z; i < yz; ++i)
            tie( result[i], borrow ) = difference( W{0}, y[i], borrow );

        return borrow;
    }
}