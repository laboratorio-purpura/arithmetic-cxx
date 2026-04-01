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

export module purple.arithmetic:word_type;

using std::array;
using std::span;
using std::tuple;

export namespace purple
{
    template <size_t B>
    union word
    {
        unsigned _BitInt(B) v;
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
    auto is_equal (word<B> x, word<B> y) noexcept -> bool
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

    /// Transform procedures.

    /// Reciprocal approximation.
    ///
    /// Reciprocal is the multiplicative inverse.
    ///
    /// Requires:
    /// y is nonzero

    template <size_t B>
    auto reciprocal ( word<B> y ) noexcept -> word<B>
    {
        // ( B - 1 ) ÷ y
        return { 0xFFFFFFFFu / y.v };
    }

    /// Normalized reciprocal approximation.
    ///
    /// Reciprocal is the multiplicative inverse.
    ///
    /// Requires:
    /// y is normalized

    template <size_t B>
    auto reciprocal_normalized ( word<B> y ) noexcept -> word<B>
    {
        // ( ( ( B^2 - 1 ) ÷ y ) ÷ B ) - B
        unsigned _BitInt(2*B) t ( -1 );
        auto x = ( ( t - y.v ) << B ) | t;
        auto q = x / y.v;
        return { static_cast< unsigned _BitInt(B) >( q ) };
    }

    /// Expand procedures.
    ///
    /// These procedures increase values, producing a "carry" or an "excess".

    /// Next with carry.

    template <size_t B>
    auto next ( word<B> x ) noexcept -> array< word<B>, 2 >
    {
        unsigned _BitInt(B) r {};
        unsigned _BitInt(1) c = __builtin_add_overflow(x.v,1,&r);
        return { r, c };
    }

    /// Next with carry.

    template <size_t B>
    auto next ( word<B> x, word<B> c ) noexcept -> array< word<B>, 2 >
    {
        unsigned _BitInt(B) r {};
        unsigned _BitInt(1) c1 = __builtin_add_overflow(x.v,1,&r);
        unsigned _BitInt(1) c2 = __builtin_add_overflow(r.v,c.v,&r.v);
        return { r, c1 | c2 };
    }

    /// Next with carry.
    ///
    /// Permits aliasing r to x.

    template <size_t B>
    auto increment ( word<B> & r, word<B> x ) noexcept -> word<B>
    {
        unsigned _BitInt(1) c = __builtin_add_overflow(x.v,1,&r.v);
        return { c };
    }

    /// Next with carry.
    ///
    /// Permits aliasing r to x.

    template <size_t B>
    auto increment ( word<B> & r, word<B> x, word<B> c ) noexcept -> word<B>
    {
        unsigned _BitInt(1) c1 = __builtin_add_overflow(x.v,1,&r.v);
        unsigned _BitInt(1) c2 = __builtin_add_overflow(r.v,c.v,&r.v);
        return { c1 | c2 };
    }

    /// Sum with carry.

    template <size_t B>
    auto sum ( word<B> x, word<B> y ) noexcept -> array< word<B>, 2 >
    {
        unsigned _BitInt(B) r {};
        unsigned _BitInt(1) c = __builtin_add_overflow(x.v,y.v,&r);
        return { r, c };
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

    /// Sum with carry.
    ///
    /// Permits aliasing r to x.

    template <size_t B>
    auto add ( word<B> & r, word<B> x, word<B> y ) noexcept -> word<B>
    {
        unsigned _BitInt(1) c = __builtin_add_overflow(x.v,y.v,&r.v);
        return { c };
    }

    /// Sum with carry.
    ///
    /// Permits aliasing r to x.

    template <size_t B>
    auto add ( word<B> & r, word<B> x, word<B> y, word<B> c ) noexcept -> word<B>
    {
        unsigned _BitInt(1) c1 = __builtin_add_overflow(x.v,y.v,&r.v);
        unsigned _BitInt(1) c2 = __builtin_add_overflow(r.v,c.v,&r.v);
        return { c1 | c2 };
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

    /// Product with excess.

    template <size_t B>
    auto product ( word<B> x, word<B> y, word<B> excess ) noexcept -> array< word<B>, 2 >
    {
        unsigned _BitInt(2*B) x_ { x.v };
        unsigned _BitInt(2*B) r = ( x_ * y.v ) + excess.v;
        return {
            static_cast< unsigned _BitInt(B) >( r ),
            static_cast< unsigned _BitInt(B) >( r >> B )
        };
    }

    /// Product with excess.
    ///
    /// Permits aliasing r to x.

    template <size_t B>
    auto multiply ( word<B> & r, word<B> x, word<B> y ) noexcept -> word<B>
    {
        unsigned _BitInt(2*B) x_ { x.v };
        unsigned _BitInt(2*B) t = x_ * y.v;
        r = word<B> { static_cast< unsigned _BitInt(B) >( t ) };
        return { static_cast< unsigned _BitInt(B) >( t >> B ) };
    }

    /// Product with excess.
    ///
    /// Permits aliasing r to x.

    template <size_t B>
    auto multiply ( word<B> & r, word<B> x, word<B> y, word<B> excess ) noexcept -> word<B>
    {
        unsigned _BitInt(2*B) x_ { x.v };
        unsigned _BitInt(2*B) t = ( x_ * y.v ) + excess.v;
        r = word<B> { static_cast< unsigned _BitInt(B) >( t ) };
        return { static_cast< unsigned _BitInt(B) >( t >> B ) };
    }

    /// Twice with excess.
    ///
    /// Requires:
    /// z < B

    template <size_t B>
    auto twice ( word<B> x, size_t z ) noexcept -> array< word<B>, 2uz >
    {
        unsigned _BitInt(2*B) x_ { x.v };
        unsigned _BitInt(2*B) r = ( x_ << z );
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
    auto twice ( word<B> x, size_t z, word<B> excess ) noexcept -> array< word<B>, 2uz >
    {
        unsigned _BitInt(2*B) x_ { x.v };
        unsigned _BitInt(2*B) r = ( x_ << z ) + excess.v;
        return {
            static_cast< unsigned _BitInt(B) >( r ),
            static_cast< unsigned _BitInt(B) >( r >> B )
        };
    }

    /// Twice with excess.
    ///
    /// Permits aliasing r to x.
    ///
    /// Requires:
    /// z < B

    template <size_t B>
    auto double_ ( word<B> & r, word<B> x, size_t z ) noexcept -> word<B>
    {
        unsigned _BitInt(2*B) x_ { x.v };
        unsigned _BitInt(2*B) t = ( x_ << z );
        r = word<B> { static_cast< unsigned _BitInt(B) >( t ) };
        return { static_cast< unsigned _BitInt(B) >( t >> B ) };
    }

    /// Twice with excess.
    ///
    /// Permits aliasing r to x.
    ///
    /// Requires:
    /// z < B

    template <size_t B>
    auto double_ ( word<B> & r, word<B> x, size_t z, word<B> excess ) noexcept -> word<B>
    {
        unsigned _BitInt(2*B) x_ { x.v };
        unsigned _BitInt(2*B) t = ( x_ << z ) + excess.v;
        r = word<B> { static_cast< unsigned _BitInt(B) >( t ) };
        return { static_cast< unsigned _BitInt(B) >( t >> B ) };
    }

    /// Reduce procedures.
    ///
    /// These procedures decrease values, requiring a "borrow" or leaving a "remainder".

    /// Previous with borrow.

    template <size_t B>
    auto previous ( word<B> x ) noexcept -> array< word<B>, 2 >
    {
        unsigned _BitInt(B) r {};
        unsigned _BitInt(1) c = __builtin_sub_overflow(x.v,1,&r);
        return { r, c };
    }

    /// Previous with borrow.

    template <size_t B>
    auto previous ( word<B> x, word<B> b ) noexcept -> array< word<B>, 2 >
    {
        unsigned _BitInt(B) r {};
        unsigned _BitInt(1) c1 = __builtin_sub_overflow(x.v,1,&r);
        unsigned _BitInt(1) c2 = __builtin_sub_overflow(r.v,b.v,&r.v);
        return { r, c1 | c2 };
    }

    /// Previous with borrow.
    ///
    /// Permits aliasing r to x.

    template <size_t B>
    auto decrement ( word<B> & r, word<B> x ) noexcept -> word<B>
    {
        unsigned _BitInt(1) c = __builtin_sub_overflow(x.v,1,&r.v);
        return { c };
    }

    /// Previous with borrow.
    ///
    /// Permits aliasing r to x.

    template <size_t B>
    auto decrement ( word<B> & r, word<B> x, word<B> b ) noexcept -> word<B>
    {
        unsigned _BitInt(1) c1 = __builtin_sub_overflow(x.v,1,&r.v);
        unsigned _BitInt(1) c2 = __builtin_sub_overflow(r.v,b.v,&r.v);
        return { c1 | c2 };
    }

    /// Difference with carry.

    template <size_t B>
    auto difference ( word<B> x, word<B> y ) noexcept -> array< word<B>, 2 >
    {
        unsigned _BitInt(B) r {};
        unsigned _BitInt(1) c = __builtin_sub_overflow(x.v,y.v,&r);
        return { r, c };
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

    /// Difference with carry.
    ///
    /// Permits aliasing r to x.

    template <size_t B>
    auto subtract ( word<B> & r, word<B> x, word<B> y ) noexcept -> word<B>
    {
        unsigned _BitInt(1) c = __builtin_sub_overflow(x.v,y.v,&r.v);
        return { c };
    }

    /// Difference with carry.
    ///
    /// Permits aliasing r to x.

    template <size_t B>
    auto subtract ( word<B> & r, word<B> x, word<B> y, word<B> b ) noexcept -> word<B>
    {
        unsigned _BitInt(1) c1 = __builtin_sub_overflow(x.v,y.v,&r.v);
        unsigned _BitInt(1) c2 = __builtin_sub_overflow(r.v,b.v,&r.v);
        return { c1 | c2 };
    }

    /// Division with remainder.

    template <size_t B>
    auto division ( word<B> x, word<B> y ) noexcept -> tuple< word<B>, word<B> >
    {
        auto q = x.v / y.v;
        auto r = x.v % y.v;
        return { { q }, { r } };
    }

    /// Division with remainder.

    template <size_t B>
    auto division ( array<word<B>,2> x, word<B> y ) noexcept -> tuple< array< word<B>, 2 >, word<B> >
    {
        using dword = unsigned _BitInt(2*B);
        auto x_ = x[0].v | ( dword { x[1].v } << B );
        auto q = x_ / y.v;
        auto r = x_ % y.v;
        return {
            array<word<B>,2> {
                static_cast< unsigned _BitInt(B) >( q ),
                static_cast< unsigned _BitInt(B) >( q >> B ),
            },
            {
                static_cast< unsigned _BitInt(B) >( r )
            }
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

    /// Half with remainder.
    ///
    /// Permits aliasing q to x.

    template <size_t B>
    auto halve ( word<B> & q, word<B> x, size_t z ) noexcept -> word<B>
    {
        constexpr unsigned _BitInt(B) one { 1 };
        unsigned _BitInt(B) q_ = x.v >> z;
        unsigned _BitInt(B) r = x.v % (one << z);
        q.v = q_;
        return { r };
    }
}