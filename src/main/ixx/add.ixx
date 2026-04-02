// SPDX-FileCopyrightText: 2026 Pedro Lamarão <pedro.lamarao@purpura.dev.br>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <algorithm>
#include <span>

export module purple.arithmetic:add;

import :word_concept;

namespace purple
{
    using std::size;
    using std::ranges::min;

    /// Computes the sum of two integers.
    ///
    /// Add stores into sum the size(sum) least significant words of the result.
    /// It permits aliasing sum to x, in which case it becomes "add accumulate".
    ///
    /// This implementation applies the "school" method described in Knuth, section 4.3.1.

    export
    template <Word W>
    auto add ( span<W> sum, span<W const> x, W y ) noexcept -> W
    {
        auto carry = W{0};

        auto const sz = size(sum);
        auto const xz = size(x);

        // count of result words to compute
        auto const z = min({ sz, xz });

        // add x[0] and y,
        // propagating carry
        if (z > 0)
            carry = add( sum[0], x[0], y, W{0} );

        // propagate carry through x
        for (auto i = 1uz; i != z; ++i)
            carry = add( sum[i], x[i], W{0}, carry );

        return carry;
    }

    /// Computes the sum of two integers.
    ///
    /// Add stores into sum the size(sum) least significant words of the result.
    /// It permits aliasing sum to x, in which case it becomes "add accumulate".
    ///
    /// This implementation applies the "school" method described in Knuth, section 4.3.1.

    export
    template <Word W>
    auto add ( span<W> sum, span<W const> x, span<W const> y ) noexcept -> W
    {
        auto carry = W{0};

        auto const sz = size(sum);
        auto const xz = size(x);
        auto const yz = size(y);

        // count of result words to compute
        auto const z = min({ sz, xz, yz });

        // add x and y word by word,
        // from least to most significant,
        // propagating carry
        for (auto i = 0uz; i != z; ++i)
            carry = add( sum[i], x[i], y[i], carry );

        // either propagate carry through x
        for (auto i = z; i < xz; ++i)
            carry = add( sum[i], x[i], W{0}, carry );

        // or propagate carry through y
        for (auto i = z; i < yz; ++i)
            carry = add( sum[i], W{0}, y[i], carry );

        return carry;
    }
}

