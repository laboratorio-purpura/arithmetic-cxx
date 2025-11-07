// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <array>
#include <iomanip>
#include <random>
#include <span>
#include <sstream>
#include <vector>

#include <fmt/format.h>

#include <gtest/gtest.h>

#include <gmpxx.h>

export module purple.test;

using namespace std;

export namespace purple::test
{
    template <size_t N>
    auto format (array<unsigned,N> const & integer)
    {
        string s;
        size_t i = integer.size();
        for (; integer[i-1] == 0 && i > 0; --i)
            continue;
        if (i == 0)
            s = "00000000";
        for (; i > 0; --i)
            s += fmt::format("{:08X}",integer[i-1]);
        return s;
    }

    auto format (vector<unsigned> const & integer)
    {
        string s;
        size_t i = integer.size();
        for (; integer[i-1] == 0 && i > 0; --i)
            continue;
        if (i == 0)
            s = "000000000";
        for (; i > 0; --i)
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

    struct PurpleTest : ::testing::Test
    {
        random_device random;
        linear_congruential_engine<unsigned,48271ul,0ul,2147483647ul> generator { random() };

        auto generate () {
            return generator();
        }

        template <typename T, size_t N>
        void generate (std::array<T,N> & a) {
            std::ranges::generate(a,std::ref(generator));
        }
    };
}