// SPDX-FileCopyrightText: 2025 Pedro Lamarão <pedro.lamarao@gmail.com>
// SPDX-License-Identifier: GPL-3.0-only

#include <cstdio>
#include <iostream>
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
        unsigned long long number;
        while (cin) {
            cin >> hex >> number;
            if (number != 0) break;
            ++i;
        }
        if (number == 0) return 0;
        cout << "i = " << i << " -> NONZERO" << endl;
        return 1;
    }

    int sum (span<char const *> args)
    {
        if (args.size() < 3) {
            fmt::println("usage: purple-crypto-fuzz sum (degree) [iterations]");
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

        auto x = vector<unsigned>(degree);
        auto y = vector<unsigned>(degree);

        fmt::println("obase=16;");
        fmt::println("ibase=16;");

        for (auto i = 0; i != iterations; ++i)
        {
            auto xz = fread(x.data(), sizeof(unsigned), x.size(), stdin);
            if (xz == -1) break;
            auto yz = fread(y.data(), sizeof(unsigned), y.size(), stdin);
            if (yz == -1) break;
            auto r = purple::sum(x,y);
            fmt::println("i = {};",i);
            fmt::println("x = {};",format(x));
            fmt::println("y = {};",format(y));
            fmt::println("r = {};",format(r));
            fmt::println("(x + y) - r;");
            fflush(stdout);
        }

        return 0;
    }

    int is_smaller (span<char const *> args)
    {
        if (args.size() < 3) {
            fmt::println("usage: purple-crypto-fuzz is-smaller (degree) [iterations]");
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

        auto x = vector<unsigned>(degree);
        auto y = vector<unsigned>(degree);

        fmt::println("obase=16;");
        fmt::println("ibase=16;");

        for (auto i = 0; i != iterations; ++i)
        {
            auto xz = fread(x.data(), sizeof(unsigned), x.size(), stdin);
            if (xz == -1) break;
            auto yz = fread(y.data(), sizeof(unsigned), y.size(), stdin);
            if (yz == -1) break;
            auto r = purple::is_smaller<unsigned>(x,y);
            fmt::println("i = {};",i);
            fmt::println("x = {};",format(x));
            fmt::println("y = {};",format(y));
            fmt::println("r = {};",r);
            fmt::println("(x < y) - r;");
            fflush(stdout);
        }

        return 0;
    }

    int minus (span<char const *> args)
    {
        if (args.size() < 3) {
            fmt::println("usage: purple-crypto-fuzz minus (degree) [iterations]");
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

        auto x = vector<unsigned>(degree);
        auto y = vector<unsigned>(degree);

        fmt::println("obase=16;");
        fmt::println("ibase=16;");

        for (auto i = 0; i != iterations; ++i)
        {
            auto xz = fread(x.data(), sizeof(unsigned), x.size(), stdin);
            if (xz == -1) break;
            auto yz = fread(y.data(), sizeof(unsigned), y.size(), stdin);
            if (yz == -1) break;
            if (purple::is_smaller<unsigned>(x,y)) swap(x,y);
            auto r = purple::minus(x,y);
            fmt::println("i = {};",i);
            fmt::println("x = {};",format(x));
            fmt::println("y = {};",format(y));
            fmt::println("r = {};",format(r));
            fmt::println("(x - y) - r;");
            fflush(stdout);
        }

        return 0;
    }

    int product (span<char const *> args)
    {
        if (args.size() < 3) {
            fmt::println("usage: purple-crypto-fuzz product (degree) [iterations]");
            return 1;
        }

        auto degree = std::stoi( args[2] );
        if (degree < 1) {
            fmt::println("error: expected degree > 0, actual: {}", degree);
            return 1;
        }

        auto iterations = args.size() < 4 ? 1 : std::stoi( args[3] );
        if (iterations < 1) {
            fmt::println("error: expected iterations > 0, actual: {}", iterations);
            return 1;
        }

        auto x = vector<unsigned>(degree);
        auto y = vector<unsigned>(degree);

        fmt::println("obase=16;");
        fmt::println("ibase=16;");

        for (auto i = 0; i != iterations; ++i)
        {
            auto xz = fread(x.data(), sizeof(unsigned), x.size(), stdin);
            if (xz == -1) break;
            auto yz = fread(y.data(), sizeof(unsigned), y.size(), stdin);
            if (yz == -1) break;
            auto r = purple::product(x,y);
            fmt::println("i = {};",i);
            fmt::println("x = {};",format(x));
            fmt::println("y = {};",format(y));
            fmt::println("r = {};",format(r));
            fmt::println("(x * y) - r;");
            fflush(stdout);
        }

        return 0;
    }

    int product_scalar (span<char const *> args)
    {
        if (args.size() < 3) {
            fmt::println("usage: purple-crypto-fuzz product-scalar (degree) [iterations]");
            return 1;
        }

        auto degree = std::stoi( args[2] );
        if (degree < 1) {
            fmt::println("error: expected degree > 0, actual: {}", degree);
            return 1;
        }

        auto iterations = args.size() < 4 ? 1 : std::stoi( args[3] );
        if (iterations < 1) {
            fmt::println("error: expected iterations > 0, actual: {}", iterations);
            return 1;
        }

        auto x = vector<unsigned>(degree);
        auto y = unsigned();

        fmt::println("obase=16;");
        fmt::println("ibase=16;");

        for (auto i = 0; i != iterations; ++i)
        {
            auto xz = fread(x.data(), sizeof(unsigned), x.size(), stdin);
            if (xz == -1) break;
            auto yz = fread(&y, sizeof(unsigned), 1, stdin);
            if (yz == -1) break;
            auto r = purple::product(x,y);
            fmt::println("i = {};",i);
            fmt::println("x = {};",format(x));
            fmt::println("y = {:08X};",y);
            fmt::println("r = {};",format(r));
            fmt::println("(x * y) - r;");
            fflush(stdout);
        }

        return 0;
    }

    int twice (span<char const *> args)
    {
        if (args.size() < 3) {
            fmt::println("usage: purple-crypto-fuzz twice (degree) [iterations]");
            return 1;
        }

        auto degree = std::stoi( args[2] );
        if (degree < 1) {
            fmt::println("error: expected degree > 0, actual: {}", degree);
            return 1;
        }

        auto iterations = args.size() < 4 ? 1 : std::stoi( args[3] );
        if (iterations < 1) {
            fmt::println("error: expected iterations > 0, actual: {}", iterations);
            return 1;
        }

        auto x = vector<unsigned>(degree);

        fmt::println("obase=16;");
        fmt::println("ibase=16;");

        for (auto i = 0; i != iterations; ++i)
        {
            auto xz = fread(x.data(), sizeof(unsigned), x.size(), stdin);
            if (xz == -1) break;
            auto r = purple::twice(x);
            fmt::println("i = {};",i);
            fmt::println("x = {};",format(x));
            fmt::println("r = {};",format(r));
            fmt::println("(2 * x) - r;");
            fflush(stdout);
        }

        return 0;
    }

    int square (span<char const *> args)
    {
        if (args.size() < 3) {
            fmt::println("usage: purple-crypto-fuzz square (degree) [iterations]");
            return 1;
        }

        auto degree = std::stoi( args[2] );
        if (degree < 1) {
            fmt::println("error: expected degree > 0, actual: {}", degree);
            return 1;
        }

        auto iterations = args.size() < 4 ? 1 : std::stoi( args[3] );
        if (iterations < 1) {
            fmt::println("error: expected iterations > 0, actual: {}", iterations);
            return 1;
        }

        auto x = vector<unsigned>(degree);

        fmt::println("obase=16;");
        fmt::println("ibase=16;");

        for (auto i = 0; i != iterations; ++i)
        {
            auto xz = fread(x.data(), sizeof(unsigned), x.size(), stdin);
            if (xz == -1) break;
            auto r = purple::square(x);
            fmt::println("i = {};",i);
            fmt::println("x = {};",format(x));
            fmt::println("r = {};",format(r));
            fmt::println("(x * x) - r;");
            fflush(stdout);
        }

        return 0;
    }

    int reciprocal_nonzero (span<char const *> args)
    {
        if (args.size() < 2) {
            fmt::println("usage: purple-crypto-fuzz reciprocal_1 [iterations]");
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
            auto x = unsigned();
            auto xn = fread(&x, sizeof(unsigned), 1, stdin);
            if (xn < 1) break;

            auto y = unsigned();
            auto yn = fread(&y, sizeof(unsigned), 1, stdin);
            if (yn < 1) break;
            if (y == 0) y = ~y;

            auto y_ = purple::reciprocal_nonzero(y);
            auto q = ( static_cast< unsigned long long >( x ) * y_ ) >> 32;
            auto r = static_cast< unsigned long long >( x ) - ( y * q );
            if (r >= y) {
                q += 1;
                r -= y;
            }

            fmt::println("i = {};",i);
            fmt::println("x = {:08X};",x);
            fmt::println("y = {:08X};",y);
            fmt::println("q = {:08X};",q);
            fmt::println("r = {:08X};",r);
            fmt::println("( x / y ) - q;");
            fmt::println("( x % y ) - r;");
            fflush(stdout);
        }

        return 0;
    }

    int reciprocal_normalised (span<char const *> args)
    {
        if (args.size() < 2) {
            fmt::println("usage: purple-crypto-fuzz reciprocal_1 [iterations]");
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
            auto x = unsigned();
            auto xn = fread(&x, sizeof(unsigned), 1, stdin);
            if (xn < 1) break;

            auto y = unsigned();
            auto yn = fread(&y, sizeof(unsigned), 1, stdin);
            if (yn < 1) break;
            y |= 0x80000000U;

            auto y_ = purple::reciprocal_normalised(y);
            auto q = ( static_cast< unsigned long long >( x ) * y_ ) >> 32;
            auto r = static_cast< unsigned long long >( x ) - ( y * q );
            if (r >= y) {
                q += 1;
                r -= y;
            }

            fmt::println("i = {};",i);
            fmt::println("x = {:08X};",x);
            fmt::println("y = {:08X};",y);
            fmt::println("q = {:08X};",q);
            fmt::println("r = {:08X};",r);
            fmt::println("( x / y ) - q;");
            fmt::println("( x % y ) - r;");
            fflush(stdout);
        }

        return 0;
    }

    int remainder_scalar (span<char const *> args)
    // requires args.size() > 1
    {
        if (args.size() < 3) {
            fmt::println("usage: purple-crypto-fuzz remainder-scalar (degree) [iterations]");
            return 1;
        }

        auto degree = std::stoi( args[2] );
        if (degree < 1) {
            fmt::println("error: expected degree > 0, actual: {}", degree);
            return 1;
        }

        auto iterations = args.size() < 4 ? 1 : std::stoi( args[3] );
        if (iterations < 1) {
            fmt::println("error: expected iterations > 0, actual: {}", iterations);
            return 1;
        }

        auto x = vector<unsigned>(degree);
        auto y = unsigned();

        fmt::println("obase=16;");
        fmt::println("ibase=16;");

        for (auto i = 0; i != iterations; ++i)
        {
            auto xz = fread(x.data(), sizeof(unsigned), x.size(), stdin);
            if (xz == -1) break;
            auto yz = fread(&y, sizeof(unsigned), 1, stdin);
            if (yz == -1) break;
            auto r = purple::remainder(x,y);
            fmt::println("i = {};",i);
            fmt::println("x = {};",format(x));
            fmt::println("y = {:08X};",y);
            fmt::println("r = {};",format(r));
            fmt::println("(x % y) - r;");
            fflush(stdout);
        }

        return 0;
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
        else if (command == "is-smaller")
            return is_smaller(args);
        else if (command == "minus")
            return minus(args);
        else if (command == "product")
            return product(args);
        else if (command == "product-scalar")
            return product_scalar(args);
        else if (command == "reciprocal-nonzero")
            return reciprocal_nonzero(args);
        else if (command == "reciprocal-normalised")
            return reciprocal_normalised(args);
        else if (command == "remainder-scalar")
            return remainder_scalar(args);
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
