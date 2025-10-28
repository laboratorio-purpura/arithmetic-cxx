// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <array>
#include <span>
#include <tuple>

export module purple.arithmetic:duo.array;

import :duo;

using std::array;
using std::span;
using std::tuple;

/// Type conversion: array.

export namespace purple
{
    /// Test procedures.

    template <typename Word, size_t Degree>
    requires ( Degree == 2uz )
    auto is_zero ( array<Word,Degree> const & x ) noexcept -> bool
    {
        auto x_ = span<Word const,Degree>(x);
        return is_zero(x_);
    }

    template <typename Word, size_t Degree>
    requires ( Degree == 2uz )
    auto not_zero ( array<Word,Degree> const & x ) noexcept -> bool
    {
        auto x_ = span<Word const,Degree>(x);
        return not_zero(x_);
    }

    template <typename Word, size_t Degree>
    requires ( Degree == 2uz )
    auto is_odd ( array<Word,Degree> const & x ) noexcept -> bool
    {
        auto x_ = span<Word const,Degree>(x);
        return is_odd(x_);
    }

    template <typename Word, size_t Degree>
    requires ( Degree == 2uz )
    auto not_odd ( array<Word,Degree> const & x ) noexcept -> bool
    {
        auto x_ = span<Word const,Degree>(x);
        return not_odd(x_);
    }

    template <typename Word, size_t Degree>
    requires ( Degree == 2uz )
    auto is_normalized ( array<Word,Degree> const & x ) noexcept -> bool
    {
        auto xx = span<Word const,Degree>(x);
        return is_normalized(xx);
    }

    template <typename Word, size_t Degree>
    requires ( Degree == 2uz )
    auto not_normalized ( array<Word,Degree> const & x ) noexcept -> bool
    {
        auto x_ = span<Word const,Degree>(x);
        return not_normalized(x_);
    }

    /// Transform procedures.

    template <typename Word, size_t Degree>
    requires ( Degree == 2uz )
    auto reciprocal_normalized ( array<Word,Degree> const & y ) noexcept
    {
        auto y_ = span<Word const,Degree>( y );
        return reciprocal_normalized( y_ );
    }

    /// Expand procedures.

    template <typename Word, size_t Degree>
    requires ( Degree == 2uz )
    auto sum_assign ( array<Word,Degree> const & x, array<Word,Degree> const & y, Word c = Word(0) ) noexcept
    {
        auto x_ = span<Word const,Degree>( x );
        auto y_ = span<Word const,Degree>( y );
        return sum_assign( x_, y_, c);
    }
}