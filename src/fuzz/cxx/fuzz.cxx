// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include <algorithm>
#include <cstdio>
#include <functional>
#include <iostream>
#include <random>
#include <ranges>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include <fmt/format.h>

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

    int assert_zero (span<char const *> args)
    {
        size_t i {};
        unsigned long long number; // TODO: number may be larger
        while (cin) {
            cin >> hex >> number;
            if (number != 0) break;
            ++i;
            if (i % 50 == 0)
                fmt::println(".");
            else
                fmt::print(".");
        }
        fflush(stdout);
        if (number == 0) return 0;
        fmt::println("");
        fmt::println("i = {} -> NONZERO\n",i);;
        fflush(stdout);
        return 1;
    }

    using random_integer = minstd_rand;

    int command_degree_iterations (
        string_view command, span<char const *> args,
        function<void (unsigned degree, unsigned iteration, random_integer& random)> f
    )
    {
        if (args.size() < 3) {
            fmt::println("usage: purple-crypto-fuzz {} (degree) [iterations]",command);
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
            f(degree,i,integers);
        }

        return 0;
    }

    int command_iterations (
        string_view command, span<char const *> args,
        function<void (unsigned iteration, random_integer& random)> f
    )
    {
        if (args.size() < 2) {
            fmt::println("usage: purple-crypto-fuzz {} [iterations]",command);
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
            f(i,integers);
        }

        return 0;
    }

    int difference (span<char const *> args)
    {
        return command_degree_iterations("minus",args,[] (auto degree, auto iteration, auto& random)
        {
            auto x = vector<unsigned>(degree);
            ranges::generate(x,ref(random));

            auto y = vector<unsigned>(degree);
            ranges::generate(y,ref(random));

            auto r = purple::difference<unsigned>(x,y);

            fmt::println("i = {};",iteration);
            fmt::println("x = {};",format(x));
            fmt::println("y = {};",format(y));
            fmt::println("r = {};",format(r));
            fmt::println("(x - y) - r;");
        });
    }

    int half (span<char const *> args)
    {
        return command_degree_iterations("half",args,[] (auto degree, auto iteration, auto& random)
        {
            auto x = vector<unsigned>(degree);
            ranges::generate(x,ref(random));

            auto r = x;
            purple::half_assign<unsigned>(r,1);

            fmt::println("i = {};",iteration);
            fmt::println("x = {};",format(x));
            fmt::println("r = {};",format(r));
            fmt::println("(x / 2) - r;");
        });
    }

    int reciprocal_nonzero (span<char const *> args)
    {
        return command_iterations("reciprocal-nonzero",args,[] (auto i, auto& random)
        {
            auto y = random();

            // nonzero
            if (y == 0U) y = 0xFFFFFFFFU;

            auto iy = purple::reciprocal(y);

            fmt::println("i = {};",i);
            fmt::println("y = {:08X};",y);
            fmt::println("iy = {:08X};",iy);
            fmt::println("( 2^32 / y ) - iy");
        });
    }

    int reciprocal_normalised (span<char const *> args)
    {
        return command_iterations("reciprocal-normalised",args,[] (auto i, auto& random)
        {
            auto y = random();

            // "normalise"
            y |= 0x80000000U;

            auto iy = purple::reciprocal_normalized(y);

            fmt::println("i = {};",i);
            fmt::println("b = 100000000;",i);
            fmt::println("y = {:08X};",y);
            fmt::println("iy = {:08X};",iy);
            fmt::println("( ( ( b^2 - 1 ) / y ) - b ) - iy");
        });
    }

    int is_smaller (span<char const *> args)
    {
        return command_degree_iterations("is-smaller",args,[] (auto degree, auto iteration, auto& random)
        {
            auto x = vector<unsigned>(degree);
            ranges::generate(x,ref(random));

            auto y = vector<unsigned>(degree);
            ranges::generate(y,ref(random));

            auto r = purple::is_smaller<unsigned>(x,y);

            fmt::println("i = {};",iteration);
            fmt::println("x = {};",format(x));
            fmt::println("y = {};",format(y));
            fmt::println("r = {};",r);
            fmt::println("(x < y) - r;");
        });
    }

    int product (span<char const *> args)
    {
        return command_degree_iterations("product",args,[] (auto degree, auto iteration, auto& random)
        {
            auto x = vector<unsigned>(degree);
            ranges::generate(x,ref(random));

            auto y = vector<unsigned>(degree);
            ranges::generate(y,ref(random));

            auto r = purple::product<unsigned>(x,y);

            fmt::println("i = {};",iteration);
            fmt::println("x = {};",format(x));
            fmt::println("y = {};",format(y));
            fmt::println("r = {};",format(r));
            fmt::println("(x * y) - r;");
        });
    }

    int product_N_1 (span<char const *> args)
    {
        return command_degree_iterations("product-N-1",args,[] (auto degree, auto iteration, auto& random)
        {
            auto x = vector<unsigned>(degree);
            ranges::generate(x,ref(random));

            auto y = random();

            auto r = purple::product<unsigned>(x,y);

            fmt::println("i = {};",iteration);
            fmt::println("x = {};",format(x));
            fmt::println("y = {:08X};",y);
            fmt::println("r = {};",format(r));
            fmt::println("(x * y) - r;");
        });
    }

    int ratio_2_1 (span<char const *> args)
    {
        return command_iterations("ratio-2-1",args,[] (auto iteration, auto& random)
        {
            auto x = array<unsigned,2>();
            ranges::generate(x,ref(random));

            auto y = random();
            while (y == 0) y = random();

            auto [q,r] = purple::division<unsigned,2>(x,y);

            fmt::println("i = {};",iteration);
            fmt::println("x = {};",format(x));
            fmt::println("y = {:08X};",y);
            fmt::println("q = {};",format(q));
            fmt::println("r = {:08X};",r);
            fmt::println("((x / y) - q) + (x % y) - r");
        });
    }

    int ratio_N_1 (span<char const *> args)
    {
        return command_degree_iterations("ratio-N-1",args,[] (auto degree, auto iteration, auto& random)
        {
            auto x = vector<unsigned>(degree);
            ranges::generate(x,ref(random));

            auto y = random();
            while (y == 0) y = random();

            // "normalise"
            y |= 0x80000000U;

            auto q = vector<unsigned>(degree);
            auto iy = purple::reciprocal_normalized(y);
            auto r = purple::division_normalized<unsigned>( q, x, y, iy );

            fmt::println("i = {};",iteration);
            fmt::println("x = {};",format(x));
            fmt::println("y = {:08X};",y);
            fmt::println("q = {};",format(q));
            fmt::println("r = {:08X};",r);
            fmt::println("((x / y) - q) + ((x % y) - r)");
        });
    }

    int square (span<char const *> args)
    {
        return command_degree_iterations("square",args,[] (auto degree, auto iteration, auto& random)
        {
            auto x = vector<unsigned>(degree);
            ranges::generate(x,ref(random));

            auto r = purple::square<unsigned>(x);

            fmt::println("i = {};",iteration);
            fmt::println("x = {};",format(x));
            fmt::println("r = {};",format(r));
            fmt::println("(x * x) - r;");
        });
    }

    int sum (span<char const *> args)
    {
        return command_degree_iterations("sum",args,[] (auto degree, auto iteration, auto& random)
        {
            auto x = vector<unsigned>(degree);
            ranges::generate(x,ref(random));

            auto y = vector<unsigned>(degree);
            ranges::generate(y,ref(random));

            auto r = purple::sum<unsigned>(x,y);

            fmt::println("i = {};",iteration);
            fmt::println("x = {};",format(x));
            fmt::println("y = {};",format(y));
            fmt::println("r = {};",format(r));
            fmt::println("(x + y) - r;");
        });
    }

    int twice (span<char const *> args)
    {
        return command_degree_iterations("twice",args,[] (auto degree, auto iteration, auto& random)
        {
            auto x = vector<unsigned>(degree);
            ranges::generate(x,ref(random));

            auto r = purple::twice<unsigned>(x);

            fmt::println("i = {};",iteration);
            fmt::println("x = {};",format(x));
            fmt::println("r = {};",format(r));
            fmt::println("(x * 2) - r;");
        });
    }

    int main (span<char const *> args)
    {
        if (args.size() < 2) {
            fmt::println("usage: purple-crypto-fuzz [command]...");
            return 1;
        }

        auto command = string_view( args[1] );
        if (command == "assert-zero")
            return assert_zero(args);
        else if (command == "difference")
            return difference(args);
        else if (command == "half")
            return half(args);
        else if (command == "reciprocal-nonzero")
            return reciprocal_nonzero(args);
        else if (command == "reciprocal-normalised")
            return reciprocal_normalised(args);
        else if (command == "is-smaller")
            return is_smaller(args);
        else if (command == "product")
            return product(args);
        else if (command == "product-N-1")
            return product_N_1(args);
        else if (command == "ratio-2-1")
            return ratio_2_1(args);
        else if (command == "ratio-N-1")
            return ratio_N_1(args);
        else if (command == "square")
            return square(args);
        else if (command == "sum")
            return sum(args);
        else if (command == "twice")
            return twice(args);
        else {
            fmt::println("error: unknown command: {}",command);
            fmt::println("usage: purple-crypto-fuzz [command]...");
            return 1;
        }
    }
}

int main (int argc, char const * argv [])
{
    return main( span( argv, argv + argc) );
}
