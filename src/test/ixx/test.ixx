// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <array>
#include <memory>
#include <mutex>
#include <random>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include <gmpxx.h>

export module purple.test;

import purple.arithmetic;
import purple.arithmetic.utility;

using std::array;
using std::span;
using std::string;
using std::tuple;
using std::vector;

export namespace purple::test
{
    struct PurpleTest : testing::Test { };

    template <typename T>
    struct PurpleRandomTest : testing::Test {};
}

struct gmp_random
{
    static std::once_flag once;

    static gmp_randstate_t state;

    static void init ()
    {
        std::call_once( once, [] {
            std::random_device random;
            gmp_randinit_default( state );
            gmp_randseed_ui( state, random() );
        });
    }
};

std::once_flag gmp_random::once {};

gmp_randstate_t gmp_random::state {};

struct gmp_rrandomb : gmp_random
{
    static auto name () -> string { return "gmp_rrandomb"; };

    template <size_t B>
    static void generate (span<purple::word<B>> x) noexcept
    {
        init();
        auto const xz = x.size();
        mpz_class t {};
        mpz_rrandomb(t.get_mpz_t(),state,xz*B);
        purple::assign(x,t);
    }

    template <size_t B, size_t Z>
    static void generate (array<purple::word<B>,Z> & x) noexcept
    {
        generate<B>( span(x) );
    }

    static void generate (mpz_class & x, size_t z, size_t B) noexcept
    {
        init();
        mpz_rrandomb(x.get_mpz_t(),state,z*B);
    }
};

struct gmp_urandomb : gmp_random
{
    static auto name () -> string { return "gmp_urandomb"; };

    template <size_t B>
    static void generate (span<purple::word<B>> x) noexcept
    {
        init();
        auto const xz = x.size();
        mpz_class t {};
        mpz_urandomb(t.get_mpz_t(),state,xz*B);
        purple::assign(x,t);
    }

    template <size_t B, size_t Z>
    static void generate (array<purple::word<B>,Z> & x) noexcept
    {
        generate<B>( span(x) );
    }

    static void generate (mpz_class & x, size_t z, size_t B) noexcept
    {
        init();
        mpz_urandomb(x.get_mpz_t(),state,z*B);
    }
};

template <size_t Z>
struct bits : std::integral_constant<size_t,Z> {};

using Types = ::testing::Types<
    tuple< bits<8>,  gmp_rrandomb >,
    tuple< bits<8>,  gmp_urandomb >,
    tuple< bits<16>, gmp_rrandomb >,
    tuple< bits<16>, gmp_urandomb >,
    tuple< bits<32>, gmp_rrandomb >,
    tuple< bits<32>, gmp_urandomb >,
    tuple< bits<64>, gmp_rrandomb >,
    tuple< bits<64>, gmp_urandomb >
>;

export namespace purple::test
{
    TYPED_TEST_SUITE(PurpleRandomTest,Types);
}