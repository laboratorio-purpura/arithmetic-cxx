// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <array>
#include <iomanip>
#include <span>
#include <sstream>
#include <vector>

#include <fmt/format.h>

#include <gmpxx.h>

export module purple.test;

using namespace std;

export namespace purple::test
{
    template <size_t N>
    auto format (array<unsigned,N> const & integer)
    {
        string s;
        for (auto i = integer.size(); i > 0; --i)
            s += fmt::format("{:08X}",integer[i-1]);
        return s;
    }

    auto format (vector<unsigned> const & integer)
    {
        string s;
        for (auto i = integer.size(); i > 0; --i)
            s += fmt::format("{:08X}",integer[i-1]);
        return s;
    }

    auto format (unsigned integer)
    {
        return fmt::format("{:08X}",integer);
    }

    auto format (mpz_class const & integer)
    {
        std::stringstream ss;
        ss << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << integer;
        return ss.str();
    }

    template <size_t SZ, typename AT, size_t AZ>
    auto to_cspan ( array<AT,AZ> const & integer, size_t offset = 0uz ) -> span<AT const,SZ>
    {
        return span<AT const,SZ>( integer.begin() + offset, SZ );
    }

    template <size_t SZ, typename AT>
    auto to_cspan ( vector<AT> const & integer, size_t offset = 0uz ) -> span<AT const,SZ>
    {
        return span<AT const,SZ>( integer.begin() + offset, SZ );
    }

    template <size_t SZ, typename AT, size_t AZ>
    auto to_span ( array<AT,AZ> & integer, size_t offset = 0uz ) -> span<AT,SZ>
    {
        return span<AT,SZ>( integer.begin() + offset, SZ );
    }

    template <size_t SZ, typename AT>
    auto to_span ( vector<AT> & integer, size_t offset = 0uz ) -> span<AT,SZ>
    {
        return span<AT,SZ>( integer.begin() + offset, SZ );
    }

    constexpr unsigned L = 0x80000000U;
    constexpr unsigned M = 0xFFFFFFFFU;

    const vector<unsigned> vE {};

    const vector<unsigned> v0 { 0U };
    const vector<unsigned> v1 { 1U };
    const vector<unsigned> v2 { 2U };
    const vector<unsigned> v3 { 2U };
    const vector<unsigned> vL { L  };
    const vector<unsigned> vM { M  };

    const vector<unsigned> v00 { 0U, 0U };
    const vector<unsigned> v10 { 1U, 0U };
    const vector<unsigned> v20 { 2U, 0U };
    const vector<unsigned> v30 { 2U, 0U };
    const vector<unsigned> vL0 { L,  0U };
    const vector<unsigned> vM0 { M,  0U };

    const vector<unsigned> v01 { 0U, 1U, };
    const vector<unsigned> v02 { 0U, 2U, };
    const vector<unsigned> v03 { 0U, 2U, };
    const vector<unsigned> v0L { 0U, L,  };
    const vector<unsigned> v0M { 0U, M,  };

    const vector<unsigned> v11 { 1U, 1U, };
    const vector<unsigned> v22 { 2U, 2U, };
    const vector<unsigned> vLL { L,  L,  };
    const vector<unsigned> vMM { M,  M,  };
}