// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include <algorithm>
#include <cstdio>
#include <functional>
#include <iomanip>
#include <iostream>
#include <random>
#include <ranges>
#include <span>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include <fmt/format.h>

#include <gmpxx.h>

import purple.arithmetic;

using namespace std;

namespace
{
    auto format (span<unsigned> integer)
    {
        string s;
        for (auto i = integer.size(); i > 0; --i)
            s += fmt::format("{:08X}",integer[i-1]);
        return s;
    }

    auto format (mpz_class const & integer)
    {
        std::stringstream ss;
        ss << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << integer;
        return ss.str();
    }

    using random_integer = minstd_rand;

    int command_degree (
        string_view name, span<char const *> args,
        function<bool (unsigned degree, unsigned iteration, random_integer& random)> f
    )
    {
        if (args.size() < 3) {
            fmt::println("usage: purple-arithmetic-fuzz {} (degree) [iterations]",name);
            return 1;
        }

        auto degree = std::stoi(args[2] );
        if (degree < 1) {
            fmt::println("error: expected degree > 0, actual: {}", degree);
            return 1;
        }

        auto iterations = args.size() < 4 ? 1 : std::stoi( args[3] );
        if (iterations < 1) {
            fmt::println("error: expected iterations > 0, actual: {}", iterations);
            return 1;
        }

        random_device random;
        random_integer integers { random() };

        fmt::println("obase=16;");
        fmt::println("ibase=16;");

        for (auto i = 0; i != iterations; ++i)
        {
            if (! f(degree,i,integers))
                return 1;
        }

        return 0;
    }

    int command (
        string_view name, span<char const *> args,
        function<bool (unsigned iteration, random_integer& random)> f
    )
    {
        if (args.size() < 2) {
            fmt::println("usage: purple-arithmetic-fuzz {} [iterations]",name);
            return 1;
        }

        auto iterations = args.size() < 3 ? 1 : std::stoi( args[2] );
        if (iterations < 1) {
            fmt::println("error: expected iterations > 0, actual: {}", iterations);
            return 1;
        }

        random_device random;
        random_integer integers { random() };

        fmt::println("obase=16;");
        fmt::println("ibase=16;");

        for (auto i = 0; i != iterations; ++i)
        {
            if (! f(i,integers))
                return 1;
        }

        return 0;
    }

    int difference_restricted (span<char const *> args)
    {
        return command_degree("difference-restricted",args,[] (auto degree, auto iteration, auto& random)
        {
            // generate numbers

            auto x = vector<unsigned>(degree);
            ranges::generate(x,ref(random));

            auto y = vector<unsigned>(degree);
            ranges::generate(y,ref(random));

            if ( purple::is_smaller<unsigned>(x,y) ) x.swap(y);

            // compute with purple

            auto r = x;
            auto b = purple::difference_assign<unsigned>(r,y);

            // compute with GMP

            auto gx = mpz_class( format(x), 16 );
            auto gy = mpz_class( format(y), 16 );
            auto gr = gx - gy;

            // compare

            fmt::println("i = {};",iteration);
            fmt::println("x = {}",format(x));
            fmt::println("y = {}",format(y));
            fmt::println("expected = {}",format(gr));
            fmt::println("actual = {}",format(r));

            return ::cmp( gr, mpz_class( format(r), 16 ) ) == 0;
        });
    }

    int half (span<char const *> args)
    {
        return command_degree("half",args,[] (auto degree, auto iteration, auto& random)
        {
            // generate numbers

            auto x = vector<unsigned>(degree);
            ranges::generate(x,ref(random));

            // compute with purple

            auto r = x;
            purple::half_assign<unsigned>(r,1);

            // compute with GMP

            auto gx = mpz_class( format(x), 16 );
            auto gr = gx / 2;

            // compare

            fmt::println("i = {};",iteration);
            fmt::println("x = {};",format(x));
            fmt::println("expected = {}",format(gr));
            fmt::println("actual = {}",format(r));

            return ::cmp( gr, mpz_class( format(r), 16 ) ) == 0;
        });
    }

    int reciprocal_nonzero_1 (span<char const *> args)
    {
        return command("reciprocal-nonzero",args,[] (auto i, auto& random)
        {
            // generate numbers

            auto y = random();

            // requires nonzero
            if ( purple::is_zero(y) ) y = random();

            // compute with purple

            auto r = purple::reciprocal(y);

            // compute with GMP

            auto gy = mpz_class( format(y), 16 );
            auto B = mpz_class("100000000",16);
            auto gr = ( B / gy ) % B;

            // compare

            fmt::println("i = {};",i);
            fmt::println("y = {}",format(y));
            fmt::println("expected = {}",format(gr));
            fmt::println("actual = {}",format(r));

            return ::cmp( gr, mpz_class( format(r), 16 ) ) == 0;
        });
    }

    int reciprocal_normalized_1 (span<char const *> args)
    {
        return command("reciprocal-normalised",args,[] (auto i, auto& random)
        {
            // generate numbers

            auto y = random();

            // requires normalized
            y |= 0x80000000U;

            // compute with purple

            auto r = purple::reciprocal_normalized(y);

            // compute with GMP

            auto gy = mpz_class( format(y), 16 );
            auto B = mpz_class("100000000",16);
            auto gr = ( ( ( ( B * B ) - 1 ) / gy ) - B ) % B;

            // compare

            fmt::println("i = {};",i);
            fmt::println("y = {}",format(y));
            fmt::println("expected = {}",format(gr));
            fmt::println("actual = {}",format(r));

            return ::cmp( gr, mpz_class( format(r), 16 ) ) == 0;
        });
    }

    int is_greater (span<char const *> args)
    {
        return command_degree("is-greater",args,[] (auto degree, auto iteration, auto& random)
        {
            // generate numbers

            auto x = vector<unsigned>(degree);
            ranges::generate(x,ref(random));

            auto y = vector<unsigned>(degree);
            ranges::generate(y,ref(random));

            // compute with purple

            auto r = purple::is_greater<unsigned>(x,y);

            // compute with GMP

            auto gx = mpz_class( format(x), 16 );
            auto gy = mpz_class( format(y), 16 );
            auto gr = gx > gy;

            // compare

            fmt::println("i = {};",iteration);
            fmt::println("x = {}",format(x));
            fmt::println("y = {}",format(y));
            fmt::println("expected = {}",format(gr));
            fmt::println("actual = {}",format(r));

            return r == gr;
        });
    }

    int is_smaller (span<char const *> args)
    {
        return command_degree("is-smaller",args,[] (auto degree, auto iteration, auto& random)
        {
            // generate numbers

            auto x = vector<unsigned>(degree);
            ranges::generate(x,ref(random));

            auto y = vector<unsigned>(degree);
            ranges::generate(y,ref(random));

            // compute with purple

            auto r = purple::is_smaller<unsigned>(x,y);

            // compute with GMP

            auto gx = mpz_class( format(x), 16 );
            auto gy = mpz_class( format(y), 16 );
            auto gr = gx < gy;

            // compare

            fmt::println("i = {};",iteration);
            fmt::println("x = {}",format(x));
            fmt::println("y = {}",format(y));
            fmt::println("expected = {}",format(gr));
            fmt::println("actual = {}",format(r));

            return r == gr;
        });
    }

    int product (span<char const *> args)
    {
        return command_degree("product",args,[] (auto degree, auto iteration, auto& random)
        {
            // generate numbers

            auto x = vector<unsigned>(degree);
            ranges::generate(x,ref(random));

            auto y = vector<unsigned>(degree);
            ranges::generate(y,ref(random));

            // compute with purple

            auto r = vector<unsigned>(degree*2);
            auto e = purple::product_accumulate<unsigned>(r,x,y);

            // compute with GMP

            auto gx = mpz_class( format(x), 16 );
            auto gy = mpz_class( format(y), 16 );
            auto gr = gx * gy;

            // compare

            fmt::println("i = {};",iteration);
            fmt::println("x = {}",format(x));
            fmt::println("y = {}",format(y));
            fmt::println("expected = {}",format(gr));
            fmt::println("actual = {}",format(r));

            return ::cmp( gr, mpz_class( format(r), 16 ) ) == 0;
        });
    }

    int product_N_1 (span<char const *> args)
    {
        return command_degree("product-N-1",args,[] (auto degree, auto iteration, auto& random)
        {
            // generate numbers

            auto x = vector<unsigned>(degree);
            ranges::generate(x,ref(random));

            auto y = random();

            // compute with purple

            auto r = vector<unsigned>(degree+1);
            purple::assign<unsigned>(r,x);
            auto e = purple::product_assign<unsigned>(r,y);

            // compute with GMP

            auto gx = mpz_class( format(x), 16 );
            auto gr = gx * y;

            // compare

            fmt::println("i = {};",iteration);
            fmt::println("x = {}",format(x));
            fmt::println("y = {}",format(y));
            fmt::println("expected = {}",format(gr));
            fmt::println("actual = {}",format(r));

            return ::cmp( gr, mpz_class( format(r), 16 ) ) == 0;
        });
    }

    int division_2_1 (span<char const *> args)
    {
        return command("ratio-2-1",args,[] (auto iteration, auto& random)
        {
            // generate numbers

            auto x = array<unsigned,2>();
            ranges::generate(x,ref(random));

            // requires y is normalized
            auto y = random();
            y |= 0x80000000u;

            // requires x ÷ B < y
            while ( purple::not_smaller( x[1], y ) ) x[1] = random();

            // compute with purple

            auto iy = purple::reciprocal_normalized(y);
            auto [q,r] = purple::division_normalized<unsigned,2>(x,y,iy);

            // compute with GMP

            auto gx = mpz_class( format(x), 16 );
            auto gq = gx / y;
            auto gr = gx % y;

            // compare

            fmt::println("i = {};",iteration);
            fmt::println("x = {};",format(x));
            fmt::println("y = {:08X};",y);
            fmt::println("expected, q = {}, r = {}",format(gq),format(gr));
            fmt::println("actual, q = {}, r = {}",format(q),format(r));

            return ::cmp( gq, mpz_class( format(q), 16 ) ) == 0 &&
                ::cmp( gr, mpz_class( format(r), 16 ) ) == 0;
        });
    }

    int division_N_1 (span<char const *> args)
    {
        return command_degree("division-N-1",args,[] (auto degree, auto iteration, auto& random)
        {
            // generate numbers

            auto x = vector<unsigned>(degree);
            ranges::generate(x,ref(random));

            // requires y is normalized
            auto y = random();
            y |= 0x80000000u;

            // requires x ÷ B < y
            while ( purple::not_smaller( x[degree-1], y ) ) x[degree-1] = random();

            // compute with purple

            auto iy = purple::reciprocal_normalized(y);
            auto q = vector<unsigned>(degree+1);
            auto r = purple::division_normalized<unsigned>(span(q),x,y,iy);

            // compute with GMP

            auto gx = mpz_class( format(x), 16 );
            auto gq = gx / y;
            auto gr = gx % y;

            // compare

            fmt::println("i = {};",iteration);
            fmt::println("x = {};",format(x));
            fmt::println("y = {:08X};",y);
            fmt::println("expected, q = {}, r = {}",format(gq),format(gr));
            fmt::println("actual, q = {}, r = {}",format(q),format(r));

            return ::cmp( gq, mpz_class( format(q), 16 ) ) == 0 &&
                ::cmp( gr, mpz_class( format(r), 16 ) ) == 0;
        });
    }

    int square (span<char const *> args)
    {
        return command_degree("square",args,[] (auto degree, auto iteration, auto& random)
        {
            // generate numbers

            auto x = vector<unsigned>(degree);
            ranges::generate(x,ref(random));

            // compute with purple

            auto r = vector<unsigned>(degree*2);
            auto e = purple::square_accumulate<unsigned>(r,x);

            // compute with GMP

            auto gx = mpz_class( format(x), 16 );
            auto gr = gx * gx;

            // compare

            fmt::println("i = {};",iteration);
            fmt::println("x = {}",format(x));
            fmt::println("expected = {}",format(gr));
            fmt::println("actual = {}",format(r));

            return ::cmp( gr, mpz_class( format(r), 16 ) ) == 0;
        });
    }

    int sum (span<char const *> args)
    {
        return command_degree("sum",args,[] (auto degree, auto iteration, auto& random)
        {
            // generate numbers

            auto x = vector<unsigned>(degree);
            ranges::generate(x,ref(random));

            auto y = vector<unsigned>(degree);
            ranges::generate(y,ref(random));

            // compute with purple

            auto r = x;
            auto c = purple::sum_assign<unsigned>(r,y);

            // compute with GMP

            auto gx = mpz_class( format(x), 16 );
            auto gy = mpz_class( format(y), 16 );
            auto gr = gx + gy;

            // compare

            fmt::println("i = {};",iteration);
            fmt::println("x = {}",format(x));
            fmt::println("y = {}",format(y));
            fmt::println("expected = {}",format(gr));
            fmt::println("actual = {}",format(r));

            return ::cmp( gr, mpz_class( format(r), 16 ) ) == 0;
        });
    }

    int twice (span<char const *> args)
    {
        return command_degree("twice",args,[] (auto degree, auto iteration, auto& random)
        {
            // generate numbers

            auto x = vector<unsigned>(degree);
            ranges::generate(x,ref(random));

            // compute with purple

            auto r = x;
            auto e = purple::twice_assign<unsigned>(r,1uz);

            // compute with GMP

            auto gx = mpz_class( format(x), 16 );
            auto gr = gx * 2;

            // compare

            fmt::println("i = {};",iteration);
            fmt::println("x = {}",format(x));
            fmt::println("purple = {}",format(r));
            fmt::println("gmp = {}",format(gr));

            return ::cmp( gr, mpz_class( format(r), 16 ) ) == 0;
        });
    }

    int main (span<char const *> args)
    {
        if (args.size() < 2) {
            fmt::println("usage: purple-arithmetic-fuzz [command]...");
            return 1;
        }

        auto command = string_view( args[1] );
        if (command == "difference-restricted")
            return difference_restricted(args);
        else if (command == "half")
            return half(args);
        else if (command == "reciprocal-nonzero-1")
            return reciprocal_nonzero_1(args);
        else if (command == "reciprocal-normalized-1")
            return reciprocal_normalized_1(args);
        else if (command == "is-greater")
            return is_greater(args);
        else if (command == "is-smaller")
            return is_smaller(args);
        else if (command == "product")
            return product(args);
        else if (command == "product-N-1")
            return product_N_1(args);
        else if (command == "division-2-1")
            return division_2_1(args);
        else if (command == "division-N-1")
            return division_N_1(args);
        else if (command == "square")
            return square(args);
        else if (command == "sum")
            return sum(args);
        else if (command == "twice")
            return twice(args);
        else {
            fmt::println("error: unknown command: {}",command);
            fmt::println("usage: purple-arithmetic-fuzz [command]...");
            return 1;
        }
    }
}

int main (int argc, char const * argv [])
{
    return main( span( argv, argv + argc) );
}
