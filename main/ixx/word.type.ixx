// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <array>
#include <span>
#include <tuple>

/// Singular nonnegative integer arithmetics.
///
/// This module partition defines `type template word<B>` satisfying `concept Word`
/// representing words in binary base 2^B.
///
/// The template implementation makes use of clang extensions and builtins,
/// such as `_BitInt(B)` and `__builtin_clzg`,
/// to satisfy the Word concept efficiently on a generic machine.

export module purple.arithmetics:word_type;

using std::array;
using std::span;
using std::tuple;

export namespace purple::arithmetics
{
    template <size_t B>
    union word
    {
        using type = unsigned _BitInt(B);

        type v;

        constexpr
        word () noexcept = default;

        constexpr
        word ( type x ) noexcept : v { x } {}
    };

    /// Query procedures.

    template <size_t B>
    auto leading_zero_bits (word<B> x) noexcept -> size_t
    {
        return __builtin_clzg( x.v );
    }

    template <size_t B>
    auto trailing_zero_bits (word<B> x) noexcept -> size_t
    {
        return __builtin_ctzg( x.v );
    }

    /// Test procedures.

    /// Tests if zero.

    template <size_t B>
    auto is_zero (word<B> x) noexcept -> bool
    {
        return x.v == 0;
    }

    /// Tests if *not* zero.

    template <size_t B>
    auto not_zero (word<B> x) noexcept -> bool
    {
        return x.v != 0;
    }

    /// Tests if odd.

    template <size_t B>
    auto is_odd ( word<B> x ) noexcept -> bool
    {
        return ( x.v & 1u ) == 1u;
    }

    /// Tests if *not* odd.

    template <size_t B>
    auto not_odd ( word<B> x ) noexcept -> bool
    {
        return ( x.v & 1u ) == 0u;
    }

    /// Tests if normalized.

    template <size_t B>
    auto is_normalized ( word<B> x ) noexcept -> bool
    {
        return ( x.v >> (B-1) ) == 1u;
    }

    /// Tests if *not* normalized.

    template <size_t B>
    auto not_normalized ( word<B> x ) noexcept -> bool
    {
        return ( x.v >> (B-1) ) == 0u;
    }

    /// Comparison procedures.

    /// Tests if equal.

    template <size_t B>
    auto are_equal (word<B> x, word<B> y) noexcept -> bool
    {
        return x.v == y.v;
    }

    /// Tests if *not* equal.

    template <size_t B>
    auto not_equal (word<B> x, word<B> y) noexcept -> bool
    {
        return x.v != y.v;
    }

    /// Tests if smaller.

    template <size_t B>
    auto is_smaller (word<B> x, word<B> y) noexcept -> bool
    {
        return x.v < y.v;
    }

    /// Tests if *not* smaller.

    template <size_t B>
    auto not_smaller (word<B> x, word<B> y) noexcept -> bool
    {
        return x.v >= y.v;
    }

    /// Tests if greater.

    template <size_t B>
    auto is_greater (word<B> x, word<B> y) noexcept -> bool
    {
        return x.v > y.v;
    }

    /// Tests if *not* greater.

    template <size_t B>
    auto not_greater (word<B> x, word<B> y) noexcept -> bool
    {
        return x.v <= y.v;
    }

    /// Expand procedures.
    ///
    /// These procedures increase values, producing a "carry" or an "excess".

    /// Next with carry.

    template <size_t B>
    auto next ( word<B> x, word<B> c ) noexcept -> array< word<B>, 2 >
    {
        unsigned _BitInt(B) r {};
        unsigned _BitInt(1) c1 = __builtin_add_overflow(x.v,1,&r);
        unsigned _BitInt(1) c2 = __builtin_add_overflow(r,c.v,&r);
        return { r, c1 | c2 };
    }

    /// Sum with carry.

    template <size_t B>
    auto sum ( word<B> x, word<B> y, word<B> c ) noexcept -> array< word<B>, 2 >
    {
        unsigned _BitInt(B) r {};
        unsigned _BitInt(1) c1 = __builtin_add_overflow(x.v,y.v,&r);
        unsigned _BitInt(1) c2 = __builtin_add_overflow(r,c.v,&r);
        return { r, c1 | c2 };
    }

    /// Product with excess.

    template <size_t B>
    auto product ( word<B> x, word<B> y ) noexcept -> array< word<B>, 2 >
    {
        unsigned _BitInt(2*B) x_ { x.v };
        unsigned _BitInt(2*B) r = x_ * y.v;
        return {
            static_cast< unsigned _BitInt(B) >( r ),
            static_cast< unsigned _BitInt(B) >( r >> B )
        };
    }

    /// Twice with excess.
    ///
    /// Requires:
    /// z < B

    template <size_t B>
    auto twice ( word<B> x, size_t z ) noexcept -> array< word<B>, 2uz >
    {
        unsigned _BitInt(2*B) x_ { x.v };
        unsigned _BitInt(2*B) r = x_ << z;
        return {
            static_cast< unsigned _BitInt(B) >( r ),
            static_cast< unsigned _BitInt(B) >( r >> B )
        };
    }

    /// Reduce procedures.
    ///
    /// These procedures decrease values, requiring a "borrow" or leaving a "remainder".

    /// Previous with borrow.

    template <size_t B>
    auto previous ( word<B> x, word<B> b ) noexcept -> array< word<B>, 2 >
    {
        unsigned _BitInt(B) r {};
        unsigned _BitInt(1) c1 = __builtin_sub_overflow(x.v,1,&r);
        unsigned _BitInt(1) c2 = __builtin_sub_overflow(r,b.v,&r);
        return { r, c1 | c2 };
    }

    /// Difference with carry.

    template <size_t B>
    auto difference ( word<B> x, word<B> y, word<B> b ) noexcept -> array< word<B>, 2 >
    {
        unsigned _BitInt(B) r {};
        unsigned _BitInt(1) c1 = __builtin_sub_overflow(x.v,y.v,&r);
        unsigned _BitInt(1) c2 = __builtin_sub_overflow(r,b.v,&r);
        return { r, c1 | c2 };
    }

    /// Division with remainder.

    template <size_t B>
    auto division ( array<word<B>,2> x, word<B> y ) noexcept -> tuple< word<B>, word<B> >
    {
        using dword = unsigned _BitInt(2*B);
        auto x_ = x[0].v | ( dword { x[1].v } << B );
        auto q = x_ / y.v;
        auto r = x_ % y.v;
        return {
            word<B>{ static_cast< unsigned _BitInt(B) >( q ) },
            word<B>{ static_cast< unsigned _BitInt(B) >( r ) }
        };
    }

    /// Half with remainder.

    template <size_t B>
    auto half ( word<B> x, size_t z ) noexcept -> tuple< word<B>, word<B> >
    {
        constexpr unsigned _BitInt(B) one { 1 };
        unsigned _BitInt(B) q = x.v >> z;
        unsigned _BitInt(B) r = x.v % (one << z);
        return { { q }, { r } };
    }
}