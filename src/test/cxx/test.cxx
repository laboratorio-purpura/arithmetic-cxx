// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include <random>

#include <gmpxx.h>

#include <gtest/gtest.h>

import purple.arithmetic.utility;
import purple.test;

using purple::assign;
using purple::test::PurpleRandomTest;
using purple::test::WordGenerator;
using purple::test::WordGeneratorPtr;
using std::random_device;
using std::span;
using std::string;

struct RGenerator final : WordGenerator
{
    random_device device {};

    gmp_randstate_t state {};

    RGenerator ()
    {
        gmp_randinit_default(state);
        gmp_randseed_ui(state,device());
    }

    ~RGenerator () noexcept override = default;

    auto name () -> string override { return "rrandom"; };

    void generate (span<unsigned> x) noexcept override
    {
        constexpr auto B = sizeof(unsigned) * 8;
        auto const xz = x.size();
        mpz_class t {};
        mpz_rrandomb(t.get_mpz_t(),state,xz*8);
        assign(x,t);
    }

    void generate (mpz_class & x, size_t z, size_t B) noexcept override
    {
        mpz_rrandomb(x.get_mpz_t(),state,z*B);
    }
};

auto const rgenerator = std::make_shared<RGenerator>();

struct UGenerator final : WordGenerator
{
    random_device device {};

    gmp_randstate_t state {};

    UGenerator ()
    {
        gmp_randinit_default(state);
        gmp_randseed_ui(state,device());
    }

    ~UGenerator () noexcept override = default;

    auto name () -> string override { return "urandom"; };

    void generate (span<unsigned> x) noexcept override
    {
        constexpr auto B = sizeof(unsigned) * 8;
        auto const xz = x.size();
        mpz_class t {};
        mpz_urandomb(t.get_mpz_t(),state,xz*8);
        assign(x,t);
    }

    void generate (mpz_class & x, size_t z, size_t B) noexcept override
    {
        mpz_urandomb(x.get_mpz_t(),state,z*B);
    }
};

auto const ugenerator = std::make_shared<UGenerator>();

INSTANTIATE_TEST_SUITE_P(
    PurpleRandomTest,
    PurpleRandomTest,
    testing::Values(rgenerator,ugenerator),
    [] (const testing::TestParamInfo<purple::test::WordGeneratorPtr>& info) {
        return info.param->name();
    }
);