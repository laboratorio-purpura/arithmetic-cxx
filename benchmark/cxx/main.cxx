// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include <algorithm>
#include <array>
#include <cstdint>
#include <random>
#include <ranges>
#include <span>

#include <gmpxx.h>

#include <benchmark/benchmark.h>

import purple.arithmetics;

using std::array;
using std::ignore;
using std::span;
using std::vector;

namespace
{
    constexpr auto bits = sizeof(nullptr) * CHAR_BIT;

    using word = purple::arithmetics::word<bits>;

    std::random_device cxx_random_device {};

    template <size_t B> struct primitive {};
    template <> struct primitive <8> { using type = uint8_t; };
    template <> struct primitive <16> { using type = uint16_t; };
    template <> struct primitive <32> { using type = uint32_t; };
    template <> struct primitive <64> { using type = uint64_t; };

    std::independent_bits_engine< std::mt19937_64, bits, primitive<bits>::type > cxx_random_engine {
        cxx_random_device()
    };

    void generate (vector<word> & v) {
        std::ranges::generate( v, [] { return word { cxx_random_engine() }; } );
    }

    gmp_randclass gmp_random {
        gmp_randinit_default
    };

    struct gmp_random_seeder_t {
        gmp_random_seeder_t () noexcept {
            gmp_random.seed( cxx_random_device() );
        }
    }
    gmp_random_seeder;

    static_assert( sizeof(mp_limb_t) == sizeof(word), "unexpected size of mp_limb_t" );

    // assign

    void gmp_assign (benchmark::State& state)
    {
        auto words = state.range(0);

        mpz_class x = gmp_random.get_z_bits(words*bits);

        mpz_class r {};
        mpz_realloc2(r.get_mpz_t(),words*bits);

        for (auto _ : state)
        {
            mpz_set(r.get_mpz_t(),x.get_mpz_t());

            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(gmp_assign)->RangeMultiplier(2)->Range(8,64);

    void purple_assign (benchmark::State& state)
    {
        auto words = state.range(0);

        auto x = vector<word>(words);
        generate(x);

        auto r = vector<word>(words);

        for (auto _ : state)
        {
            r = x;

            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(purple_assign)->RangeMultiplier(2)->Range(8,64);

    // sum N x 1

    void gmp_sum_N_1 (benchmark::State& state)
    {
        auto words = state.range(0);

        mpz_class x = gmp_random.get_z_bits(words*bits);

        mpz_class y_ = gmp_random.get_z_bits(bits);
        auto y = y_.get_ui();

        mpz_class r {};
        mpz_realloc2(r.get_mpz_t(),(words+1)*bits);

        for (auto _ : state)
        {
            mpz_add_ui(r.get_mpz_t(),x.get_mpz_t(),y);

            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(gmp_sum_N_1)->RangeMultiplier(2)->Range(8,64);

    void purple_sum_N_1 (benchmark::State& state)
    {
        auto words = state.range(0);

        auto x = vector<word>(words);
        generate(x);

        auto y = word { cxx_random_engine() };

        auto r = vector<word>(words+1);

        for (auto _ : state)
        {
            r[words] = sum<word>(r,x,y);

            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(purple_sum_N_1)->RangeMultiplier(2)->Range(8,64);

    // sum N x N

    void gmp_sum_N_N (benchmark::State& state)
    {
        auto words = state.range(0);

        mpz_class x = gmp_random.get_z_bits(words*bits);

        mpz_class y = gmp_random.get_z_bits(words*bits);

        mpz_class r {};
        mpz_realloc2(r.get_mpz_t(),(words+1)*bits);

        for (auto _ : state)
        {
            mpz_add(r.get_mpz_t(),x.get_mpz_t(),y.get_mpz_t());

            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(gmp_sum_N_N)->RangeMultiplier(2)->Range(8,64);

    void purple_sum_N_N (benchmark::State& state)
    {
        auto words = state.range(0);

        auto x = vector<word>(words);
        generate(x);

        auto y = vector<word>(words);
        generate(y);

        auto r = vector<word>(words+1);

        for (auto _ : state)
        {
            sum<word>(r,x,y);

            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(purple_sum_N_N)->RangeMultiplier(2)->Range(8,64);

    // sum 2N x N

    void gmp_sum_2N_N (benchmark::State& state)
    {
        auto words = state.range(0);

        mpz_class x = gmp_random.get_z_bits(2*words*bits);

        mpz_class y = gmp_random.get_z_bits(words*bits);

        mpz_class r {};
        mpz_realloc2(r.get_mpz_t(),(2*words+1)*bits);

        for (auto _ : state)
        {
            mpz_add(r.get_mpz_t(),x.get_mpz_t(),y.get_mpz_t());

            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(gmp_sum_2N_N)->RangeMultiplier(2)->Range(8,64);

    void purple_sum_2N_N (benchmark::State& state)
    {
        auto words = state.range(0);

        auto x = vector<word>(2*words);
        generate(x);

        auto y = vector<word>(words);
        generate(y);

        auto r = vector<word>((2*words)+1);

        for (auto _ : state)
        {
            sum<word>(r,x,y);

            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(purple_sum_2N_N)->RangeMultiplier(2)->Range(8,64);

    // product N x 1

    void gmp_product_N_1 (benchmark::State& state)
    {
        auto words = state.range(0);

        mpz_class x = gmp_random.get_z_bits(words*bits);

        mpz_class y_ = gmp_random.get_z_bits(bits);
        auto y = y_.get_ui();

        mpz_class r {};
        mpz_realloc2(r.get_mpz_t(),(words+1)*bits);

        for (auto _ : state)
        {
            mpz_mul_ui(r.get_mpz_t(),x.get_mpz_t(),y);

            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(gmp_product_N_1)->RangeMultiplier(2)->Range(8,64);

    void purple_product_N_1 (benchmark::State& state)
    {
        auto words = state.range(0);

        auto x = vector<word>(words);
        generate(x);

        auto y = word { cxx_random_engine() };

        auto r = vector<word>(words+1);

        for (auto _ : state)
        {
            product<word>(r,x,y);

            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(purple_product_N_1)->RangeMultiplier(2)->Range(8,64);

    // product N x N

    void gmp_product_N_N (benchmark::State& state)
    {
        auto words = state.range(0);

        mpz_class x = gmp_random.get_z_bits(words*bits);

        mpz_class y = gmp_random.get_z_bits(words*bits);

        mpz_class r {};
        mpz_realloc2(r.get_mpz_t(),((2*words)+1)*bits);

        for (auto _ : state)
        {
            mpz_mul(r.get_mpz_t(),x.get_mpz_t(),y.get_mpz_t());

            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(gmp_product_N_N)->RangeMultiplier(2)->Range(8,64);

    void purple_product_N_N (benchmark::State& state)
    {
        auto words = state.range(0);

        auto x = vector<word>(words);
        generate(x);

        auto y = vector<word>(words);
        generate(y);

        auto r = vector<word>((2*words)+1);

        for (auto _ : state)
        {
            product<word>(r,x,y);

            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(purple_product_N_N)->RangeMultiplier(2)->Range(8,64);

    // twice N x 1

    void gmp_twice_N_1 (benchmark::State& state)
    {
        auto words = state.range(0);

        mpz_class x = gmp_random.get_z_bits(words*bits);

        mpz_class r {};
        mpz_realloc2(r.get_mpz_t(),(words+1)*bits);

        for (auto _ : state)
        {
            mpz_mul_2exp(r.get_mpz_t(),x.get_mpz_t(),31);

            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(gmp_twice_N_1)->RangeMultiplier(2)->Range(8,64);

    void purple_twice_N_1 (benchmark::State& state)
    {
        auto words = state.range(0);

        auto x = vector<word>(words);
        generate(x);

        auto r = vector<word>(words+1);

        for (auto _ : state)
        {
            twice<word>(r,x,31uz);

            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(purple_twice_N_1)->RangeMultiplier(2)->Range(8,64);

    // square N

    void gmp_square_N (benchmark::State& state)
    {
        auto words = state.range(0);

        mpz_class x = gmp_random.get_z_bits(words*bits);

        mpz_class r {};
        mpz_realloc2(r.get_mpz_t(),((2*words)+1)*bits);

        for (auto _ : state)
        {
            mpz_pow_ui(r.get_mpz_t(),x.get_mpz_t(),2);

            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(gmp_square_N)->RangeMultiplier(2)->Range(8,64);

    void purple_square_N (benchmark::State& state)
    {
        auto words = state.range(0);

        auto x = vector<word>(words);
        generate(x);

        auto r = vector<word>((2*words)+1);

        for (auto _ : state)
        {
            for (auto& it : r) it = word{}; // TODO: clear(r)

            square_accumulate<word>(r,x);

            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(purple_square_N)->RangeMultiplier(2)->Range(8,64);

    // difference N x 1

    void gmp_difference_N_1 (benchmark::State& state)
    {
        auto words = state.range(0);

        mpz_class x = gmp_random.get_z_bits(words*bits);

        mpz_class y_ = gmp_random.get_z_bits(bits);
        auto y = y_.get_ui();

        mpz_class r {};
        mpz_realloc2(r.get_mpz_t(),words*bits);

        for (auto _ : state)
        {
            mpz_sub_ui(r.get_mpz_t(),x.get_mpz_t(),y);

            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(gmp_difference_N_1)->RangeMultiplier(2)->Range(8,64);

    void purple_difference_N_1 (benchmark::State& state)
    {
        auto words = state.range(0);

        auto x = vector<word>(words);
        generate(x);

        auto y = word { cxx_random_engine() };

        auto r = vector<word>(words);

        for (auto _ : state)
        {
            difference<word>(r,x,y);

            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(purple_difference_N_1)->RangeMultiplier(2)->Range(8,64);

    // difference N x N

    void gmp_difference_N_N (benchmark::State& state)
    {
        auto words = state.range(0);

        mpz_class x = gmp_random.get_z_bits(words*bits);

        mpz_class y = gmp_random.get_z_bits(words*bits);

        mpz_class r {};
        mpz_realloc2(r.get_mpz_t(),words*bits);

        for (auto _ : state)
        {
            mpz_sub(r.get_mpz_t(),x.get_mpz_t(),y.get_mpz_t());

            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(gmp_difference_N_N)->RangeMultiplier(2)->Range(8,64);

    void purple_difference_N_N (benchmark::State& state)
    {
        auto words = state.range(0);

        auto x = vector<word>(words);
        generate(x);

        auto y = vector<word>(words);
        generate(y);

        auto r = vector<word>(words);

        for (auto _ : state)
        {
            difference<word>(r,x,y);

            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(purple_difference_N_N)->RangeMultiplier(2)->Range(8,64);

    // difference 2N x N

    void gmp_difference_2N_N (benchmark::State& state)
    {
        auto words = state.range(0);

        mpz_class x = gmp_random.get_z_bits(2*words*bits);

        mpz_class y = gmp_random.get_z_bits(words*bits);

        mpz_class r {};
        mpz_realloc2(r.get_mpz_t(),2*words*bits);

        for (auto _ : state)
        {
            mpz_sub(r.get_mpz_t(),x.get_mpz_t(),y.get_mpz_t());

            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(gmp_difference_2N_N)->RangeMultiplier(2)->Range(8,64);

    void purple_difference_2N_N (benchmark::State& state)
    {
        auto words = state.range(0);

        auto x = vector<word>(2*words);
        generate(x);

        auto y = vector<word>(words);
        generate(y);

        auto r = vector<word>(2*words);

        for (auto _ : state)
        {
            difference<word>(r,x,y);

            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(purple_difference_2N_N)->RangeMultiplier(2)->Range(8,64);

    // division N x 1

    void gmp_division_N_1 (benchmark::State& state)
    {
        auto words = state.range(0);

        mpz_class x = gmp_random.get_z_bits(words*bits);

        mpz_class y_ = gmp_random.get_z_bits(bits);
        auto y = y_.get_ui();

        mpz_class q {};
        mpz_realloc2(q.get_mpz_t(),words*bits);

        for (auto _ : state)
        {
            auto r = mpz_div_ui(q.get_mpz_t(),x.get_mpz_t(),y);

            benchmark::DoNotOptimize(q);
            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(gmp_division_N_1)->RangeMultiplier(2)->Range(8,64);

    void purple_division_N_1 (benchmark::State& state)
    {
        auto words = state.range(0);

        auto x = vector<word>(words);
        generate(x);

        auto y = word { cxx_random_engine() };

        auto q = vector<word>(words);

        for (auto _ : state)
        {
            auto r = division<word>(q,x,y);

            benchmark::DoNotOptimize(q);
            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(purple_division_N_1)->RangeMultiplier(2)->Range(8,64);

    // division 2N x N

    void gmp_division_2N_N (benchmark::State& state)
    {
        auto words = state.range(0);

        mpz_class x = gmp_random.get_z_bits(2*words*bits);

        mpz_class y = gmp_random.get_z_bits(words*bits);

        mpz_class q {};
        mpz_realloc2(q.get_mpz_t(),((2*words)+1)*bits);

        mpz_class r {};
        mpz_realloc2(r.get_mpz_t(),((2*words)+1)*bits);

        for (auto _ : state)
        {
            mpz_tdiv_qr(q.get_mpz_t(),r.get_mpz_t(),x.get_mpz_t(),y.get_mpz_t());

            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(gmp_division_2N_N)->RangeMultiplier(2)->Range(8,64);

    void purple_division_2N_N (benchmark::State& state)
    {
        auto words = state.range(0);

        auto x = vector<word>(2*words);
        generate(x);

        auto y = vector<word>(words);
        generate(y);

        auto q = vector<word>((2*words)+1);
        auto r = vector<word>((2*words)+1);

        for (auto _ : state)
        {
            division<word>(q,r,x,y);

            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(purple_division_2N_N)->RangeMultiplier(2)->Range(8,64);

    // half N x 1

    void gmp_half_N_1 (benchmark::State& state)
    {
        auto words = state.range(0);

        mpz_class x = gmp_random.get_z_bits(words*bits);

        mpz_class r {};
        mpz_realloc2(r.get_mpz_t(),words*bits);

        for (auto _ : state)
        {
            mpz_div_2exp(r.get_mpz_t(),x.get_mpz_t(),31);

            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(gmp_half_N_1)->RangeMultiplier(2)->Range(8,64);

    void purple_half_N_1 (benchmark::State& state)
    {
        auto words = state.range(0);

        auto x = vector<word>(words);
        generate(x);

        auto r = vector<word>(words);

        for (auto _ : state)
        {
            half<word>(r,x,31uz);

            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(purple_half_N_1)->RangeMultiplier(2)->Range(8,64);
}

BENCHMARK_MAIN();
