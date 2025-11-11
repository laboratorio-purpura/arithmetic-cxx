// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <array>
#include <memory>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include <gmpxx.h>

export module purple.test;

using std::array;
using std::span;
using std::string;
using std::vector;

export namespace purple::test
{
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

    struct PurpleTest : ::testing::Test { };

    struct WordGenerator
    {
        virtual ~WordGenerator () noexcept = default;

        virtual auto name () -> string = 0;

        virtual void generate (span<unsigned> x) noexcept = 0;

        virtual void generate (mpz_class & x, size_t z, size_t B) noexcept = 0;
    };

    using WordGeneratorPtr = std::shared_ptr<WordGenerator>;

    struct PurpleRandomTest : testing::TestWithParam<WordGeneratorPtr> { };
}