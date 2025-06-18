module;

#include <tuple>

export module purple:mono;

using std::tuple;

export namespace purple
{
    static_assert( sizeof(unsigned int) == 4, "oops");
    static_assert( sizeof(unsigned long long) == 8, "oops");

    /// mono integer properties

    /// 1 if and only if x is zero, else 0.
    constexpr
    auto is_zero (unsigned int x) noexcept -> unsigned int
    {
        x |= -x;
        x >>= 31U;
        return 1 - x;
    }

    /// 1 if and only if x is *not* zero, else 0.
    constexpr
    auto not_zero (unsigned int x) noexcept -> unsigned int
    {
        x |= -x;
        return x >> 31U;
    }

    /// mono integer arithmetic

    constexpr
    auto sum (unsigned int x, unsigned int y, unsigned int carry = 0) noexcept -> tuple< unsigned int, unsigned int >
    {
        auto r = __builtin_addc(x,y,carry,&carry);
        return { r, carry };
    }

    constexpr
    auto sum_accumulate (unsigned int & x, unsigned int y, unsigned int carry = 0) noexcept -> unsigned int
    {
        x = __builtin_addc(x,y,carry,&carry);
        return carry;
    }

    constexpr
    auto twice (unsigned int x, unsigned int carry = 0) noexcept -> tuple< unsigned int, unsigned int >
    {
        auto r = ( static_cast<unsigned long long>(x) * 2 ) + carry;
        return { r, r >> 32 };
    }

    constexpr
    auto twice_accumulate (unsigned int & x, unsigned int carry = 0) noexcept -> unsigned int
    {
        auto r = ( static_cast<unsigned long long>(x) * 2 ) + carry;
        x = r;
        return r >> 32;
    }

    constexpr
    auto product (unsigned int x, unsigned int y, unsigned int carry = 0) noexcept -> tuple< unsigned int, unsigned int >
    {
        auto r = ( static_cast<unsigned long long>(x) * y ) + carry;
        return { r, r >> 32 };
    }

    constexpr
    auto product_accumulate (unsigned int & x, unsigned int y, unsigned int carry = 0) noexcept -> unsigned int
    {
        auto r = ( static_cast<unsigned long long>(x) * y ) + carry;
        x = r;
        return r >> 32;
    }

    constexpr
    auto square (unsigned int x, unsigned int carry = 0) noexcept -> tuple< unsigned int, unsigned int >
    {
        auto r = ( static_cast<unsigned long long>(x) * x ) + carry;
        return { r, r >> 32 };
    }

    constexpr
    auto square_accumulate (unsigned int & x, unsigned int carry = 0) noexcept -> unsigned int
    {
        auto r = ( static_cast<unsigned long long>(x) * x ) + carry;
        x = r;
        return r >> 32;
    }

    constexpr
    auto ratio (unsigned int x, unsigned int y) noexcept -> tuple< unsigned int, unsigned int >
    {
        auto q = x / y;
        auto r = x % y;
        return { q, r };
    }
}
