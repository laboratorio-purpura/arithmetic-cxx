// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include <algorithm>
#include <functional>
#include <random>
#include <span>
#include <string>
#include <vector>

#include <fmt/format.h>

#include <gmpxx.h>

import purple.arithmetics;
import purple.arithmetics.utility;

using namespace purple::arithmetics;

using std::array;
using std::function;
using std::ignore;
using std::span;
using std::string_view;
using std::vector;

namespace
{
    constexpr auto bits = sizeof(nullptr) * CHAR_BIT;

    using word = purple::arithmetics::word<bits>;

    std::random_device random_device {};

    template <size_t B> struct primitive {};
    template <> struct primitive <8> { using type = uint8_t; };
    template <> struct primitive <16> { using type = uint16_t; };
    template <> struct primitive <32> { using type = uint32_t; };
    template <> struct primitive <64> { using type = uint64_t; };

    std::independent_bits_engine< std::mt19937_64, bits, primitive<bits>::type > random_engine {
        random_device()
    };
    
    void generate (span<word> s) {
        std::ranges::generate( s, [] { return word { random_engine() }; });        
    }

    int command_degree (
        string_view name, span<char const *> args,
        function<bool (unsigned degree, unsigned iteration)> f
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

        fmt::println("obase=16;");
        fmt::println("ibase=16;");

        for (auto i = 0; i != iterations; ++i)
        {
            if (! f(degree,i))
                return 1;
        }

        return 0;
    }

    int command (
        string_view name, span<char const *> args,
        function<bool (unsigned iteration)> f
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

        fmt::println("obase=16;");
        fmt::println("ibase=16;");

        for (auto i = 0; i != iterations; ++i)
        {
            if (! f(i))
                return 1;
        }

        return 0;
    }

    int difference_restricted (span<char const *> args)
    {
        return command_degree("difference-restricted",args,[] (auto degree, auto iteration)
        {
            // generate numbers

            auto x = vector<word>(degree);
            generate(x);

            auto y = vector<word>(degree);
            generate(y);

            if ( purple::arithmetics::is_smaller<word>(x,y) ) x.swap(y);

            // compute with purple

            auto r = vector<word>(degree);
            auto b = difference<word>(r,x,y);

            // compute with GMP

            auto gx = to_mpz(x);
            auto gy = to_mpz(y);
            auto gr = gx - gy;

            // compare

            fmt::println("i = {};",iteration);
            fmt::println("x = {}",format(x));
            fmt::println("y = {}",format(y));
            fmt::println("expected = {}",format(gr));
            fmt::println("actual = {}",format(r));

            return ::cmp( gr, to_mpz(r) ) == 0;
        });
    }

    int half (span<char const *> args)
    {
        return command_degree("half",args,[] (auto degree, auto iteration)
        {
            // generate numbers

            auto x = vector<word>(degree);
            generate(x);

            // compute with purple

            auto r = vector<word>(degree);
            half<word>(r,x,1);

            // compute with GMP

            auto gx = to_mpz(x);
            auto gr = gx >> 1;

            // compare

            fmt::println("i = {};",iteration);
            fmt::println("x = {};",format(x));
            fmt::println("expected = {}",format(gr));
            fmt::println("actual = {}",format(r));

            return ::cmp( gr, to_mpz(r) ) == 0;
        });
    }

    int reciprocal_normalized_1 (span<char const *> args)
    {
        return command("reciprocal-normalised",args,[] (auto i)
        {
            // generate numbers

            auto y = word { random_engine() };
            y.v |= 0x80000000U; // TODO

            // compute with purple

            auto r = purple::arithmetics::reciprocal_normalized(y);

            // compute with GMP

            auto gy = to_mpz(y);
            auto B = mpz_class("100000000",16);
            auto gr = ( ( ( ( B * B ) - 1 ) / gy ) - B ) % B;

            // compare

            fmt::println("i = {};",i);
            fmt::println("y = {}",format(y));
            fmt::println("expected = {}",format(gr));
            fmt::println("actual = {}",format(r));

            return ::cmp( gr, to_mpz(r) ) == 0;
        });
    }

    int is_greater (span<char const *> args)
    {
        return command_degree("is-greater",args,[] (auto degree, auto iteration)
        {
            // generate numbers

            auto x = vector<word>(degree);
            generate(x);

            auto y = vector<word>(degree);
            generate(y);

            // compute with purple

            auto r = purple::arithmetics::is_greater<word>(x,y);

            // compute with GMP

            auto gx = to_mpz(x);
            auto gy = to_mpz(y);
            auto gr = gx > gy;

            // compare

            fmt::println("i = {};",iteration);
            fmt::println("x = {}",format(x));
            fmt::println("y = {}",format(y));
            fmt::println("expected = {}",gr);
            fmt::println("actual = {}",r);

            return r == gr;
        });
    }

    int is_smaller (span<char const *> args)
    {
        return command_degree("is-smaller",args,[] (auto degree, auto iteration)
        {
            // generate numbers

            auto x = vector<word>(degree);
            generate(x);

            auto y = vector<word>(degree);
            generate(y);

            // compute with purple

            auto r = purple::arithmetics::is_smaller<word>(x,y);

            // compute with GMP

            auto gx = to_mpz(x);
            auto gy = to_mpz(y);
            auto gr = gx < gy;

            // compare

            fmt::println("i = {};",iteration);
            fmt::println("x = {}",format(x));
            fmt::println("y = {}",format(y));
            fmt::println("expected = {}",gr);
            fmt::println("actual = {}",r);

            return r == gr;
        });
    }

    int product (span<char const *> args)
    {
        return command_degree("product",args,[] (auto degree, auto iteration)
        {
            // generate numbers

            auto x = vector<word>(degree);
            generate(x);

            auto y = vector<word>(degree);
            generate(y);

            // compute with purple

            auto r = vector<word>(degree*2);
            product<word>(r,x,y);

            // compute with GMP

            auto gx = to_mpz(x);
            auto gy = to_mpz(y);
            auto gr = gx * gy;

            // compare

            fmt::println("i = {};",iteration);
            fmt::println("x = {}",format(x));
            fmt::println("y = {}",format(y));
            fmt::println("expected = {}",format(gr));
            fmt::println("actual = {}",format(r));

            return ::cmp( gr, to_mpz(r) ) == 0;
        });
    }

    int product_N_1 (span<char const *> args)
    {
        return command_degree("product-N-1",args,[] (auto degree, auto iteration)
        {
            // generate numbers

            auto x = vector<word>(degree);
            generate(x);

            auto y = word { random_engine() };

            // compute with purple

            auto r = vector<word>(degree+1);
            r[degree] = product<word>(r,x,y);

            // compute with GMP

            auto gx = to_mpz(x);
            auto gy = to_mpz(y);
            auto gr = gx * gy;

            // compare

            fmt::println("i = {};",iteration);
            fmt::println("x = {}",format(x));
            fmt::println("y = {}",format(y));
            fmt::println("expected = {}",format(gr));
            fmt::println("actual = {}",format(r));

            return ::cmp( gr, to_mpz(r) ) == 0;
        });
    }

    int division_2_1 (span<char const *> args)
    {
        return command("ratio-2-1",args,[] (auto iteration)
        {
            // generate numbers

            auto y = word { random_engine() };
            y.v |= 0x80000000u; // TODO

            auto x = array<word,2>();
            generate(x);
            if ( not_smaller( x[1], y ) )
                tie( x[1], ignore ) = difference( x[1], y, word{0} );

            // compute with purple

            auto iy = reciprocal_normalized(y);
            auto [q,r] = division_normal_strict<word,2>(x,y,iy);

            // compute with GMP

            auto gx = to_mpz(x);
            auto gy = to_mpz(y);
            auto gq = gx / gy;
            auto gr = gx % gy;

            // compare

            fmt::println("i = {};",iteration);
            fmt::println("x = {};",format(x));
            fmt::println("y = {};",format(y));
            fmt::println("expected, q = {}, r = {}",format(gq),format(gr));
            fmt::println("actual, q = {}, r = {}",format(q),format(r));

            return ::cmp( gq, to_mpz(q) ) == 0 && ::cmp( gr, to_mpz(r) ) == 0;
        });
    }

    int division_N_1 (span<char const *> args)
    {
        return command_degree("division-N-1",args,[] (auto degree, auto iteration)
        {
            // generate numbers

            auto x = vector<word>(degree);
            generate(x);

            auto y = word{0u};
            while ( is_zero(y) )
                y = word { random_engine() };

            // compute with purple

            auto q = vector<word>(degree+1);
            auto r = division<word>(span(q),x,y);

            // compute with GMP

            auto gx = to_mpz(x);
            auto gy = to_mpz(y);
            auto gq = gx / gy;
            auto gr = gx % gy;

            // compare

            fmt::println("i = {};",iteration);
            fmt::println("x = {};",format(x));
            fmt::println("y = {};",format(y));
            fmt::println("expected, q = {}, r = {}",format(gq),format(gr));
            fmt::println("actual, q = {}, r = {}",format(q),format(r));

            return ::cmp( gq, to_mpz(q) ) == 0 && ::cmp( gr, to_mpz(r) ) == 0;
        });
    }

    int square (span<char const *> args)
    {
        return command_degree("square",args,[] (auto degree, auto iteration)
        {
            // generate numbers

            auto x = vector<word>(degree);
            generate(x);

            // compute with purple

            auto r = vector<word>(degree*2);
            purple::arithmetics::square<word>(r,x);

            // compute with GMP

            auto gx = to_mpz(x);
            auto gr = gx * gx;

            // compare

            fmt::println("i = {};",iteration);
            fmt::println("x = {}",format(x));
            fmt::println("expected = {}",format(gr));
            fmt::println("actual = {}",format(r));

            return ::cmp( gr, to_mpz(r) ) == 0;
        });
    }

    int sum (span<char const *> args)
    {
        return command_degree("sum",args,[] (auto degree, auto iteration)
        {
            // generate numbers

            auto x = vector<word>(degree);
            generate(x);

            auto y = vector<word>(degree);
            generate(y);

            // compute with purple

            auto r = vector<word>(degree+1);
            r[degree] = sum<word>(r,x,y);

            // compute with GMP

            auto gx = to_mpz(x);
            auto gy = to_mpz(y);
            auto gr = gx + gy;

            // compare

            fmt::println("i = {};",iteration);
            fmt::println("x = {}",format(x));
            fmt::println("y = {}",format(y));
            fmt::println("expected = {}",format(gr));
            fmt::println("actual = {}",format(r));

            return ::cmp( gr, to_mpz(r) ) == 0;
        });
    }

    int twice (span<char const *> args)
    {
        return command_degree("twice",args,[] (auto degree, auto iteration)
        {
            // generate numbers

            auto x = vector<word>(degree);
            generate(x);

            // compute with purple

            auto r = vector<word>(degree+1);
            r[degree] = twice<word>(r,x,1uz);

            // compute with GMP

            auto gx = to_mpz(x);
            auto gr = gx << 1;

            // compare

            fmt::println("i = {};",iteration);
            fmt::println("x = {}",format(x));
            fmt::println("purple = {}",format(r));
            fmt::println("gmp = {}",format(gr));

            return ::cmp( gr, to_mpz(r) ) == 0;
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
