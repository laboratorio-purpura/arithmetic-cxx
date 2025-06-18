module;

#include <span>
#include <tuple>
#include <vector>

export module purple;

export import :mono;
export import :poly;

using namespace std;

export namespace purple
{
    /// poly integer properties

    /// 1 if and only if every term of x is significant, else 0.
    template <typename Integer>
    auto is_compact (vector<Integer> const & x) -> Integer
    {
        auto xs = span( x.begin(), x.end() );
        return is_compact(xs);
    }

    /// 1 if and only if x is the additive identity, else 0.
    template <typename Integer>
    auto is_zero (vector<Integer> const & x) noexcept
    {
        auto xs = span( x.begin(), x.end() );
        return is_zero(xs);
    }

    /// 1 if and only if x is *not* the additive identity, else 0.
    template <typename Integer>
    auto not_zero (vector<Integer> const & x) noexcept
    {
        auto xs = span( x.begin(), x.end() );
        return not_zero(xs);
    }

    /// poly integer arithmetic

    template <typename Integer>
    void sum_accumulate (vector<Integer> & r, vector<Integer> const & x, Integer y) noexcept
    // requires is_compact(x)
    // requires r.size() > x.size()
    {
        auto rs = span( r.begin(), r.end() );
        auto xs = span( x.begin(), x.end() );
        sum_accumulate(rs,xs,y);
    }

    template <typename Integer>
    auto sum (vector<Integer> const & x, Integer y) -> vector<Integer>
    // requires is_compact(x)
    {
        auto r = vector<Integer>( x.size() + 1 );
        sum_accumulate(r,x,y);
        return std::move(r);
    }

    // --

    template <typename Integer>
    void sum_accumulate (vector<Integer> & r, vector<Integer> const & x, vector<Integer> const & y) noexcept
    // requires is_compact(x) && is_compact(y)
    // requires x.size() == y.size()
    // requires r.size() > x.size()
    {
        auto rs = span( r.begin(), r.end() );
        auto xs = span( x.begin(), x.end() );
        auto ys = span( y.begin(), y.end() );
        sum_accumulate(rs,xs,ys);
    }

    template <typename Integer>
    auto sum (vector<Integer> const & x, vector<Integer> const & y) -> vector<Integer>
    // requires is_compact(x) && is_compact(y)
    // requires x.size() == y.size()
    {
        auto r = vector<Integer>( x.size() + 1 );
        sum_accumulate(r,x,y);
        return std::move(r);
    }

    // --

    template <typename Integer>
    void twice_accumulate (vector<Integer> & r, vector<Integer> const & x) noexcept
    // requires is_compact(x)
    // requires r.size() > x.size()
    {
        auto rs = span( r.begin(), r.end() );
        auto xs = span( x.begin(), x.end() );
        twice_accumulate(rs,xs);
    }

    template <typename Integer>
    auto twice (vector<Integer> const & x) -> vector<Integer>
    // requires is_compact(x)
    {
        auto r = vector<Integer>( x.size() + 1 );
        twice_accumulate(r,x);
        return std::move(r);
    }

    // --

    template <typename Integer>
    void product_accumulate (vector<Integer> & r, vector<Integer> const & x, Integer y) noexcept
    // requires is_compact(x)
    // requires r.size() > x.size()
    {
        auto rs = span( r.begin(), r.end() );
        auto xs = span( x.begin(), x.end() );
        product_accumulate(rs,xs,y);
    }

    template <typename Integer>
    auto product (vector<Integer> const & x, Integer y) -> vector<Integer>
    // requires is_compact(x)
    {
        auto r = vector<Integer>( x.size() + 1 );
        product_accumulate(r,x,y);
        return std::move(r);
    }

    // --

    template <typename Integer>
    void product_accumulate (vector<Integer> & r, vector<Integer> const & x, vector<Integer> const & y) noexcept
    // requires is_compact(x) && is_compact(y)
    // requires r.size() > x.size() + y.size()
    {
        auto rs = span( r.begin(), r.end() );
        auto xs = span( x.begin(), x.end() );
        auto ys = span( y.begin(), y.end() );
        product_accumulate(rs,xs,ys);
    }

    template <typename Integer>
    auto product (vector<Integer> const & x, vector<Integer> const & y) -> vector<Integer>
    // requires is_compact(x) && is_compact(y)
    {
        auto r = vector<Integer>( x.size() + y.size() + 1 );
        product_accumulate(r,x,y);
        return std::move(r);
    }

    // --

    template <typename Integer>
    void square_accumulate (vector<Integer> & r, vector<Integer> const & x) noexcept
    // requires is_compact(x)
    // requires r.size() > x.size() + y.size()
    {
        auto rs = span( r.begin(), r.end() );
        auto xs = span( x.begin(), x.end() );
        square_accumulate(rs,xs);
    }

    template <typename Integer>
    auto square (vector<Integer> const & x) -> vector<Integer>
    // requires is_compact(x)
    {
        auto r = vector<Integer>( (2 * x.size()) + 1 );
        square_accumulate(r,x);
        return std::move(r);
    }
}
