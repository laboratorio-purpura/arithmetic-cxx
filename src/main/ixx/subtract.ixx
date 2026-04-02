// SPDX-FileCopyrightText: 2026 Pedro Lamarão <pedro.lamarao@purpura.dev.br>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <algorithm>
#include <span>

export module purple.arithmetic:subtract;

import :word_concept;

namespace purple
{
    using std::size;
    using std::ranges::min;

    /// Computes the difference between two integers.
    ///
    /// Subtract stores into difference the size(difference) least significant words of the result.
    /// It permits aliasing difference to x, in which case it becomes "subtract accumulate".
    ///
    /// This implementation applies the "school" method described in Knuth, section 4.3.1.

    export
    template <Word W>
    auto subtract ( span<W> difference, span<W const> x, W y ) noexcept -> W
    {
        auto borrow = W{0};

        auto const sz = size(difference);
        auto const xz = size(x);

        // count of result words to compute
        auto const z = min({ sz, xz });

        // subtract x[0] and y,
        // propagating borrow
        if (z > 0)
            borrow = subtract( difference[0], x[0], y, W{0} );

        // propagate borrow through x
        for (auto i = 1uz; i != z; ++i)
            borrow = subtract( difference[i], x[i], W{0}, borrow );

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
    auto subtract ( span<W> difference, span<W const> x, span<W const> y ) noexcept -> W
    {
        auto borrow = W{0};

        auto const dz = size(difference);
        auto const xz = size(x);
        auto const yz = size(y);

        // count of result words to compute
        auto const z = min({ dz, xz, yz });

        // subtract x and y word by word,
        // from least to most significant,
        // propagating borrow
        for (auto i = 0uz; i != z; ++i)
            borrow = subtract( difference[i], x[i], y[i], borrow );

        // either propagate borrow through x
        for (auto i = z; i < xz; ++i)
            borrow = subtract( difference[i], x[i], W{0}, borrow );

        // or propagate borrow through y
        for (auto i = z; i < yz; ++i)
            borrow = subtract( difference[i], W{0}, y[i], borrow );

        return borrow;
    }
}