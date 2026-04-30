// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

module;

#include <array>
#include <memory>
#include <mutex>
#include <random>
#include <string>
#include <vector>

#include <gmpxx.h>

#include <gtest/gtest.h>

#include <hegel/hegel.h>

export module purple.test;

import purple.arithmetics;
import purple.arithmetics.utility;

using namespace hegel::generators;
using namespace std;

template <size_t Bits> struct primitive;
template<> struct primitive<8> { using type = uint8_t; };
template<> struct primitive<16> { using type = uint16_t; };
template<> struct primitive<32> { using type = uint32_t; };
template<> struct primitive<64> { using type = uint64_t; };
template <size_t Bits> using primitive_type = typename primitive<Bits>::type;

export namespace purple::arithmetics
{
    template <size_t Bits>
    constexpr
    bool operator== (word<Bits> x, word<Bits> y) noexcept {
        return x.v == y.v;
    }
    template <size_t Bits>
    constexpr
    bool operator!= (word<Bits> x, word<Bits> y) noexcept {
        return x.v != y.v;
    }
}

export namespace purple::test
{
    constexpr size_t hegel_cases = 1000;

    constexpr size_t random_cases = 10000;

    template <size_t Bits>
    class WordGenerator : public IGenerator<arithmetics::word<Bits>> {
    public:
        explicit WordGenerator(IntegersParams<primitive_type<Bits>> params = {})
            : params_(std::move(params)) {
            primitive_type<Bits> min_val =
                params_.min_value.value_or(std::numeric_limits<primitive_type<Bits>>::min());
            primitive_type<Bits> max_val =
                params_.max_value.value_or(std::numeric_limits<primitive_type<Bits>>::max());
            if (min_val > max_val)
                throw std::invalid_argument("Cannot have max_value < min_value");
        }

        std::optional<BasicGenerator<arithmetics::word<Bits>>> as_basic() const override {
            primitive_type<Bits> min_val =
                params_.min_value.value_or(std::numeric_limits<primitive_type<Bits>>::min());
            primitive_type<Bits> max_val =
                params_.max_value.value_or(std::numeric_limits<primitive_type<Bits>>::max());
            return hegel::generators::BasicGenerator<arithmetics::word<Bits>>{
                {{"type", "integer"},
                {"min_value", min_val},
                {"max_value", max_val}},
                &hegel::generators::default_parse_raw<primitive_type<Bits>>
            };
        }

    private:
        IntegersParams<primitive_type<Bits>> params_;
    };

    template <size_t Bits>
    Generator<arithmetics::word<Bits>> words(IntegersParams<primitive_type<Bits>> params = {}) {
        return Generator<arithmetics::word<Bits>>(new WordGenerator<Bits>(std::move(params)));
    }

    struct PurpleTest : testing::Test { };

    template <typename>
    struct PurpleHegelTest : testing::Test {};

    template <typename>
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
    static void generate (span<purple::arithmetics::word<B>> x) noexcept
    {
        init();
        auto const xz = x.size();
        mpz_class t {};
        mpz_rrandomb(t.get_mpz_t(),state,xz*B);
        purple::arithmetics::assign(x,t);
    }

    template <size_t B, size_t Z>
    static void generate (array<purple::arithmetics::word<B>,Z> & x) noexcept
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
    static void generate (span<purple::arithmetics::word<B>> x) noexcept
    {
        init();
        auto const xz = x.size();
        mpz_class t {};
        mpz_urandomb(t.get_mpz_t(),state,xz*B);
        purple::arithmetics::assign(x,t);
    }

    template <size_t B, size_t Z>
    static void generate (array<purple::arithmetics::word<B>,Z> & x) noexcept
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

using HegelTypes = ::testing::Types<
    tuple< bits<8> >,
    tuple< bits<16> >,
    tuple< bits<32> >,
    tuple< bits<64> >
>;

export namespace purple::test
{
    TYPED_TEST_SUITE(PurpleHegelTest,HegelTypes);
}

using RandomTypes = ::testing::Types<
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
    TYPED_TEST_SUITE(PurpleRandomTest,RandomTypes);
}