// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <array>
#include <sstream>
#include <vector>

#include <fmt/format.h>

#include <gmpxx.h>

export module purple.arithmetic.utility;

import purple.arithmetic;

using std::array;
using std::span;
using std::string;
using std::vector;

namespace purple
{
    export
    template <Word W, size_t Z>
    void assign (array<W,Z> & x, mpz_class const & y)
    {
        size_t yz = x.size();
        mpz_export(
            x.data(),
            &yz,
            -1,
            sizeof(W),
            0,
            0,
            y.get_mpz_t()
        );
    }

    export
    template <Word W>
    void assign (span<W> x, mpz_class const & y)
    {
        size_t yz = x.size();
        mpz_export(
            x.data(),
            &yz,
            -1,
            sizeof(W),
            0,
            0,
            y.get_mpz_t()
        );
    }

    export
    auto format (word<8> integer)
    {
        return fmt::format("{:2X}",integer.v);
    }

    export
    auto format (word<16> integer)
    {
        return fmt::format("{:4X}",integer.v);
    }

    export
    auto format (word<32> integer)
    {
        return fmt::format("{:8X}",integer.v);
    }

    export
    auto format (word<64> integer)
    {
        return fmt::format("{:16X}",integer.v);
    }

    export
    auto format (span<const word<8>> integer)
    {
        string s;
        size_t i = integer.size();
        while (i > 0 && integer[i-1].v == 0)
            --i;
        if (i == 0)
            s = "0";
        if (i > 0) {
            s += fmt::format("{:2X}",integer[i-1].v);
            --i;
        }
        for (; i > 0; --i)
            s += fmt::format("{:02X}",integer[i-1].v);
        return s;
    }

    export
    auto format (span<const word<16>> integer)
    {
        string s;
        size_t i = integer.size();
        while (i > 0 && integer[i-1].v == 0)
            --i;
        if (i == 0)
            s = "0";
        if (i > 0) {
            s += fmt::format("{:4X}",integer[i-1].v);
            --i;
        }
        for (; i > 0; --i)
            s += fmt::format("{:04X}",integer[i-1].v);
        return s;
    }

    export
    auto format (span<const word<32>> integer)
    {
        string s;
        size_t i = integer.size();
        while (i > 0 && integer[i-1].v == 0)
            --i;
        if (i == 0)
            s = "0";
        if (i > 0) {
            s += fmt::format("{:8X}",integer[i-1].v);
            --i;
        }
        for (; i > 0; --i)
            s += fmt::format("{:08X}",integer[i-1].v);
        return s;
    }

    export
    auto format (span<const word<64>> integer)
    {
        string s;
        size_t i = integer.size();
        while (i > 0 && integer[i-1].v == 0)
            --i;
        if (i == 0)
            s = "0";
        if (i > 0) {
            s += fmt::format("{:16X}",integer[i-1].v);
            --i;
        }
        for (; i > 0; --i)
            s += fmt::format("{:016X}",integer[i-1].v);
        return s;
    }

    export
    template <size_t Z>
    auto format (array<word<64>,Z> const & integer)
    {
        return format( span(integer) );
    }

    export
    template <size_t B>
    auto format (vector<word<B>> const & integer)
    {
        return format( span(integer) );
    }

    export
    auto format (mpz_class const & integer)
    {
        std::stringstream ss;
        ss << std::hex << std::uppercase << integer;
        return ss.str();
    }

    export
    template <size_t B>
    auto to_mpz ( word<B> x ) -> mpz_class
    {
        mpz_class r {};
        mpz_import(
            r.get_mpz_t(),
            1,
            -1,
            sizeof(word<B>),
            0,
            0,
            &x
        );
        return r;
    }

    export
    template <size_t B, size_t Z>
    auto to_mpz ( array<word<B>,Z> const & x ) -> mpz_class
    {
        mpz_class r {};
        mpz_import(
            r.get_mpz_t(),
            Z,
            -1,
            sizeof(word<B>),
            0,
            0,
            x.data()
        );
        return r;
    }

    export
    template <size_t B>
    auto to_mpz ( span<word<B>> const & x ) -> mpz_class
    {
        mpz_class r {};
        mpz_import(
            r.get_mpz_t(),
            x.size(),
            -1,
            sizeof(word<B>),
            0,
            0,
            x.data()
        );
        return r;
    }

    export
    template <size_t B>
    auto to_mpz ( vector<word<B>> const & x ) -> mpz_class
    {
        mpz_class r {};
        mpz_import(
            r.get_mpz_t(),
            x.size(),
            -1,
            sizeof(word<B>),
            0,
            0,
            x.data()
        );
        return r;
    }
}