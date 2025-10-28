// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <array>
#include <span>
#include <tuple>

export module purple.arithmetic:duo.span;

import :duo;

using std::array;
using std::size_t;
using std::span;
using std::tuple;

/// Type conversion: span.

export namespace purple
{
    /// Test procedures.

    template <typename XT, typename YT, size_t Degree>
    requires ( Degree == 2uz )
    auto is_greater ( span<XT,Degree> x, span<YT,Degree> y ) noexcept
    {
        auto x_ = span<XT const,Degree>(x);
        auto y_ = span<YT const,Degree>(y);
        return is_greater( x_, y_ );
    }

    template <typename XT, typename YT, size_t Degree>
    requires ( Degree == 2uz )
    auto not_smaller ( span<XT,Degree> x, span<YT,Degree> y ) noexcept
    {
        auto x_ = span<XT const,Degree>(x);
        auto y_ = span<YT const,Degree>(y);
        return not_smaller( x_, y_ );
    }

    /// Increase procedures.

    template <typename Word, size_t Degree>
    requires ( Degree == 2uz )
    auto sum_assign ( span<Word,Degree> x, span<Word,Degree> y, Word carry = Word(0) ) noexcept
    {
        auto y_ = span<Word const,Degree>(y);
        return sum_assign( x, y_, carry );
    }

    /// Decrease procedures.

    template <typename Word, size_t Degree>
    requires ( Degree == 2uz )
    auto difference_assign ( span<Word,Degree> x, span<Word,Degree> y, Word borrow = Word(0) ) noexcept
    {
        auto y_ = span<Word const,Degree>(y);
        return difference_assign( x, y_, borrow );
    }
}