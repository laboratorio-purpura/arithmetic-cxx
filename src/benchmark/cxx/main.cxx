// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include <algorithm>
#include <array>
#include <random>
#include <ranges>
#include <span>

#include <gmpxx.h>

#include <benchmark/benchmark.h>

import purple.arithmetic;

using std::array;
using std::default_random_engine;
using std::ignore;
using std::ranges::generate;
using std::random_device;
using std::span;
using std::vector;

using random_integer = std::linear_congruential_engine<unsigned,48271UL,0UL,2147483647UL>;

namespace
{
    random_device cxx_random_device {};

    default_random_engine cxx_random_engine { cxx_random_device() };

    gmp_randclass gmp_random { gmp_randinit_default };

    struct gmp_random_seeder_t {
        gmp_random_seeder_t () noexcept {
            gmp_random.seed( cxx_random_device() );
        }
    }
    gmp_random_seeder;

    static_assert( sizeof(mp_limb_t) == 8, "unexpected size of mp_limb_t" );

    // assign

    void gmp_assign (benchmark::State& state)
    {
        auto words = state.range(0);

        mpz_class x = gmp_random.get_z_bits(words*64);

        mpz_class r {};
        mpz_realloc2(r.get_mpz_t(),words*64);

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

        auto x = vector<unsigned>(words);
        generate(x,ref(cxx_random_engine));

        auto r = vector<unsigned>(words);

        for (auto _ : state)
        {
            purple::assign<unsigned>(r,x);

            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(purple_assign)->RangeMultiplier(2)->Range(8,64);

    // next

    void gmp_next (benchmark::State& state)
    {
        auto words = state.range(0);

        mpz_class x = gmp_random.get_z_bits(words*64);

        mpz_class r {};
        mpz_realloc2(r.get_mpz_t(),(words+1)*64);

        for (auto _ : state)
        {
            mpz_add_ui(r.get_mpz_t(),x.get_mpz_t(),1u);

            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(gmp_next)->RangeMultiplier(2)->Range(8,64);

    void purple_next (benchmark::State& state)
    {
        auto words = state.range(0);

        auto x = vector<unsigned>(words);
        generate(x,ref(cxx_random_engine));

        auto r = vector<unsigned>(words+1);

        for (auto _ : state)
        {
            purple::next_assign<unsigned>(r,x);

            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(purple_next)->RangeMultiplier(2)->Range(8,64);

    // sum N x 1

    void gmp_sum_N_1 (benchmark::State& state)
    {
        auto words = state.range(0);

        mpz_class x = gmp_random.get_z_bits(words*64);

        mpz_class y_ = gmp_random.get_z_bits(64u);
        auto y = y_.get_ui();

        mpz_class r {};
        mpz_realloc2(r.get_mpz_t(),(words+1)*64);

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

        auto x = vector<unsigned>(words);
        generate(x,ref(cxx_random_engine));

        auto y = cxx_random_engine();

        auto r = vector<unsigned>(words+1);

        for (auto _ : state)
        {
            purple::sum_assign<unsigned>(r,x,y);

            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(purple_sum_N_1)->RangeMultiplier(2)->Range(8,64);

    // sum N x N

    void gmp_sum_N_N (benchmark::State& state)
    {
        auto words = state.range(0);

        mpz_class x = gmp_random.get_z_bits(words*64);

        mpz_class y = gmp_random.get_z_bits(words*64);

        mpz_class r {};
        mpz_realloc2(r.get_mpz_t(),(words+1)*64);

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

        auto x = vector<unsigned>(words);
        generate(x,ref(cxx_random_engine));

        auto y = vector<unsigned>(words);
        generate(y,ref(cxx_random_engine));

        auto r = vector<unsigned>(words+1);

        for (auto _ : state)
        {
            purple::sum_assign<unsigned>(r,x,y);

            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(purple_sum_N_N)->RangeMultiplier(2)->Range(8,64);

    // sum 2N x N

    void gmp_sum_2N_N (benchmark::State& state)
    {
        auto words = state.range(0);

        mpz_class x = gmp_random.get_z_bits(2*words*64);

        mpz_class y = gmp_random.get_z_bits(words*64);

        mpz_class r {};
        mpz_realloc2(r.get_mpz_t(),(2*words+1)*64);

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

        auto x = vector<unsigned>(2*words);
        generate(x,ref(cxx_random_engine));

        auto y = vector<unsigned>(words);
        generate(y,ref(cxx_random_engine));

        auto r = vector<unsigned>((2*words)+1);

        for (auto _ : state)
        {
            purple::sum_assign<unsigned>(r,x,y);

            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(purple_sum_2N_N)->RangeMultiplier(2)->Range(8,64);

    // product N x 1

    void gmp_product_N_1 (benchmark::State& state)
    {
        auto words = state.range(0);

        mpz_class x = gmp_random.get_z_bits(words*64);

        mpz_class y_ = gmp_random.get_z_bits(64u);
        auto y = y_.get_ui();

        mpz_class r {};
        mpz_realloc2(r.get_mpz_t(),(words+1)*64);

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

        auto x = vector<unsigned>(words);
        generate(x,ref(cxx_random_engine));

        auto y = cxx_random_engine();

        auto r = vector<unsigned>(words+1);

        for (auto _ : state)
        {
            purple::product_assign<unsigned>(r,x,y);

            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(purple_product_N_1)->RangeMultiplier(2)->Range(8,64);

    // product N x N

    void gmp_product_N_N (benchmark::State& state)
    {
        auto words = state.range(0);

        mpz_class x = gmp_random.get_z_bits(words*64);

        mpz_class y = gmp_random.get_z_bits(words*64);

        mpz_class r {};
        mpz_realloc2(r.get_mpz_t(),((2*words)+1)*64);

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

        auto x = vector<unsigned>(words);
        generate(x,ref(cxx_random_engine));

        auto y = vector<unsigned>(words);
        generate(y,ref(cxx_random_engine));

        auto r = vector<unsigned>((2*words)+1);

        for (auto _ : state)
        {
            purple::product_accumulate<unsigned>(r,x,y);

            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(purple_product_N_N)->RangeMultiplier(2)->Range(8,64);

    // twice N x 1

    void gmp_twice_N_1 (benchmark::State& state)
    {
        auto words = state.range(0);

        mpz_class x = gmp_random.get_z_bits(words*64);

        mpz_class r {};
        mpz_realloc2(r.get_mpz_t(),(words+1)*64);

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

        auto x = vector<unsigned>(words);
        generate(x,ref(cxx_random_engine));

        auto r = vector<unsigned>(words+1);

        for (auto _ : state)
        {
            purple::twice_assign<unsigned>(r,x,31uz);

            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(purple_twice_N_1)->RangeMultiplier(2)->Range(8,64);

    // previous

    void gmp_previous (benchmark::State& state)
    {
        auto words = state.range(0);

        mpz_class x = gmp_random.get_z_bits(words*64);

        mpz_class r {};
        mpz_realloc2(r.get_mpz_t(),words*64);

        for (auto _ : state)
        {
            mpz_sub_ui(r.get_mpz_t(),x.get_mpz_t(),1u);

            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(gmp_previous)->RangeMultiplier(2)->Range(8,64);

    void purple_previous (benchmark::State& state)
    {
        auto words = state.range(0);

        auto x = vector<unsigned>(words);
        generate(x,ref(cxx_random_engine));

        auto r = vector<unsigned>(words);

        for (auto _ : state)
        {
            purple::previous_assign<unsigned>(r,x);

            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(purple_previous)->RangeMultiplier(2)->Range(8,64);

    // difference N x 1

    void gmp_difference_N_1 (benchmark::State& state)
    {
        auto words = state.range(0);

        mpz_class x = gmp_random.get_z_bits(words*64);

        mpz_class y_ = gmp_random.get_z_bits(64u);
        auto y = y_.get_ui();

        mpz_class r {};
        mpz_realloc2(r.get_mpz_t(),words*64);

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

        auto x = vector<unsigned>(words);
        generate(x,ref(cxx_random_engine));

        auto y = cxx_random_engine();

        auto r = vector<unsigned>(words);

        for (auto _ : state)
        {
            purple::difference_assign<unsigned>(r,x,y);

            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(purple_difference_N_1)->RangeMultiplier(2)->Range(8,64);

    // difference N x N

    void gmp_difference_N_N (benchmark::State& state)
    {
        auto words = state.range(0);

        mpz_class x = gmp_random.get_z_bits(words*64);

        mpz_class y = gmp_random.get_z_bits(words*64);

        mpz_class r {};
        mpz_realloc2(r.get_mpz_t(),words*64);

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

        auto x = vector<unsigned>(words);
        generate(x,ref(cxx_random_engine));

        auto y = vector<unsigned>(words);
        generate(y,ref(cxx_random_engine));

        auto r = vector<unsigned>(words);

        for (auto _ : state)
        {
            purple::difference_assign<unsigned>(r,x,y);

            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(purple_difference_N_N)->RangeMultiplier(2)->Range(8,64);

    // difference 2N x N

    void gmp_difference_2N_N (benchmark::State& state)
    {
        auto words = state.range(0);

        mpz_class x = gmp_random.get_z_bits(2*words*64);

        mpz_class y = gmp_random.get_z_bits(words*64);

        mpz_class r {};
        mpz_realloc2(r.get_mpz_t(),2*words*64);

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

        auto x = vector<unsigned>(2*words);
        generate(x,ref(cxx_random_engine));

        auto y = vector<unsigned>(words);
        generate(y,ref(cxx_random_engine));

        auto r = vector<unsigned>(2*words);

        for (auto _ : state)
        {
            purple::difference_assign<unsigned>(r,x,y);

            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(purple_difference_2N_N)->RangeMultiplier(2)->Range(8,64);

    // division N x 1

    void gmp_division_N_1 (benchmark::State& state)
    {
        auto words = state.range(0);

        mpz_class x = gmp_random.get_z_bits(words*64);

        mpz_class y_ = gmp_random.get_z_bits(64u);
        auto y = y_.get_ui();

        mpz_class q {};
        mpz_realloc2(q.get_mpz_t(),words*64);

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

        auto x = vector<unsigned>(words);
        generate(x,ref(cxx_random_engine));

        auto y = cxx_random_engine();

        auto q = vector<unsigned>(words);

        for (auto _ : state)
        {
            auto r = purple::division_assign<unsigned>(q,x,y);

            benchmark::DoNotOptimize(q);
            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(purple_division_N_1)->RangeMultiplier(2)->Range(8,64);

    // division 2N x N

    void gmp_division_2N_N (benchmark::State& state)
    {
        auto words = state.range(0);

        mpz_class x = gmp_random.get_z_bits(2*words*64);

        mpz_class y = gmp_random.get_z_bits(words*64);

        mpz_class q {};
        mpz_realloc2(q.get_mpz_t(),((2*words)+1)*64);

        mpz_class r {};
        mpz_realloc2(r.get_mpz_t(),((2*words)+1)*64);

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

        auto x = vector<unsigned>(2*words);
        generate(x,ref(cxx_random_engine));

        auto y = vector<unsigned>(words);
        generate(y,ref(cxx_random_engine));

        auto q = vector<unsigned>((2*words)+1);
        auto r = vector<unsigned>((2*words)+1);

        for (auto _ : state)
        {
            purple::division_assign<unsigned>(q,r,x,y);

            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(purple_division_2N_N)->RangeMultiplier(2)->Range(8,64);

    // half N x 1

    void gmp_half_N_1 (benchmark::State& state)
    {
        auto words = state.range(0);

        mpz_class x = gmp_random.get_z_bits(words*64);

        mpz_class r {};
        mpz_realloc2(r.get_mpz_t(),words*64);

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

        auto x = vector<unsigned>(words);
        generate(x,ref(cxx_random_engine));

        auto r = vector<unsigned>(words);

        for (auto _ : state)
        {
            purple::half_assign<unsigned>(r,x,31uz);

            benchmark::DoNotOptimize(r);
        }
    }

    BENCHMARK(purple_half_N_1)->RangeMultiplier(2)->Range(8,64);
}

BENCHMARK_MAIN();
