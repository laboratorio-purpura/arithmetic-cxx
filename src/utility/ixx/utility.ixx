module;

#include <array>
#include <sstream>
#include <vector>

#include <fmt/format.h>

#include <gmpxx.h>

export module purple.arithmetic.utility;

using std::array;
using std::span;
using std::string;
using std::vector;

namespace purple
{
    export
    template <size_t Z>
    void assign (array<unsigned,Z> & x, mpz_class const & y)
    {
        size_t yz = x.size();
        mpz_export(
            x.data(),
            &yz,
            -1,
            sizeof(unsigned),
            0,
            0,
            y.get_mpz_t()
        );
    }

    export
    void assign (span<unsigned> x, mpz_class const & y)
    {
        size_t yz = x.size();
        mpz_export(
            x.data(),
            &yz,
            -1,
            sizeof(unsigned),
            0,
            0,
            y.get_mpz_t()
        );
    }

    export
    auto format (unsigned integer)
    {
        return fmt::format("{:08X}",integer);
    }

    export
    template <size_t N>
    auto format (array<unsigned,N> const & integer)
    {
        string s;
        size_t i = integer.size();
        while (i > 0 && integer[i-1] == 0)
            --i;
        if (i == 0)
            s = "000000000";
        if (i > 0) {
            s += fmt::format("{:X}",integer[i-1]);
            --i;
        }
        for (; i > 0; --i)
            s += fmt::format("{:08X}",integer[i-1]);
        return s;
    }

    export
    auto format (vector<unsigned> const & integer)
    {
        string s;
        size_t i = integer.size();
        while (i > 0 && integer[i-1] == 0)
            --i;
        if (i == 0)
            s = "000000000";
        if (i > 0) {
            s += fmt::format("{:X}",integer[i-1]);
            --i;
        }
        for (; i > 0; --i)
            s += fmt::format("{:08X}",integer[i-1]);
        return s;
    }

    export
    auto format (mpz_class const & integer)
    {
        std::stringstream ss;
        ss << std::hex << std::uppercase << integer;
        return ss.str();
    }

    export
    auto to_mpz ( unsigned x ) -> mpz_class
    {
        mpz_class r { x };
        return r;
    }

    export
    template <size_t Z>
    auto to_mpz ( array<unsigned,Z> const & x ) -> mpz_class
    {
        mpz_class r {};
        mpz_import(
            r.get_mpz_t(),
            Z,
            -1,
            sizeof(unsigned),
            0,
            0,
            x.data()
        );
        return r;
    }

    export
    auto to_mpz ( vector<unsigned> const & x ) -> mpz_class
    {
        mpz_class r {};
        mpz_import(
            r.get_mpz_t(),
            x.size(),
            -1,
            sizeof(unsigned),
            0,
            0,
            x.data()
        );
        return r;
    }
}