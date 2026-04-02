// SPDX-FileCopyrightText: 2026 Pedro Lamarão <pedro.lamarao@purpura.dev.br>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <algorithm>
#include <cassert>
#include <span>
#include <tuple>

export module purple.arithmetic:multiply;

import :word_concept;

namespace purple
{
    using std::ignore;
    using std::size;

    /// Computes the product of two integers.
    ///
    /// Multiply adds into product the size(product) least significant words of the result.
    /// It forbids aliasing product to x.
    /// It permits product to start nonzero, in which case it becomes "multiply and add".
    ///
    /// This implementation applies the "school" method described in Knuth, section 4.3.1.

    export
    template <Word W>
    auto multiply ( span<W> product, span<W const> x, W y ) noexcept -> W
    {
        auto excess = W{0};

        auto const pz = size(product);
        auto const xz = size(x);

        // TODO: lift this restriction
        assert(pz >= xz);

        // multiply x and y word by word,
        // from least to most significant
        for (auto i = 0uz; i != xz; ++i)
            excess = multiply( product[i], x[i], y, excess );

        return excess;
    }

    /// Computes the product of two integers.
    ///
    /// Multiply adds into product the size(product) least significant words of the result.
    /// It forbids aliasing product to x.
    /// It permits product to start nonzero, in which case it becomes "multiply and add".
    ///
    /// This implementation applies the "school" method described in Knuth, section 4.3.1.

    export
    template <Word W>
    void multiply ( span<W> product, span<W const> x, span<W const> y ) noexcept;

    template <Word W>
    void multiply ( span<W> result, span<W const> x, span<W const> y ) noexcept
    {
        auto const rz = size(result);
        auto const xz = size(x);
        auto const yz = size(y);

        // TODO: lift this restriction
        assert(rz >= xz + yz);

        // multiply x and y word by word,
        // from least to most significant
        for (auto j = 0uz; j != yz; ++j)
        {
            auto excess = W{0};
            for (auto i = 0uz; i != xz; ++i)
            {
                // x[i] × y[j] + excess
                auto p = product( x[i], y[j], excess );
                // store low word, propagate high word
                auto carry = add( result[i+j], result[i+j], p[0] );
                ignore = add( excess, p[1], carry );
            }
            // store excess
            ignore = add( result[xz+j], result[xz+j], excess );
        }
    }
}
