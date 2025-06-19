#include <cstdio>
#include <iostream>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include <fmt/format.h>

import purple;

using namespace purple;
using namespace std;

namespace
{
    auto format (vector<unsigned> const & integer)
    {
        string s;
        for (auto i = integer.size(); i > 0; --i)
            s += fmt::format("{:08X}",integer[i-1]);
        return s;
    }

    int assert_zero (span<char const *> args)
    {
        unsigned long long number;
        cin >> hex >> number;
        while (number == 0)
            cin >> number;
        return 0;
    }

    int sum (span<char const *> args)
    // requires args.size() > 1
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
        auto r = vector<unsigned>(degree + 1);

        fmt::println("obase=16;");
        fmt::println("ibase=16;");

        for (auto i = 0; i != iterations; ++i)
        {
            auto xz = fread(x.data(), sizeof(unsigned), x.size(), stdin);
            if (xz == -1) break;
            fmt::println("x = {};",format(x));

            auto yz = fread(y.data(), sizeof(unsigned), y.size(), stdin);
            if (yz == -1) break;
            fmt::println("y = {};",format(y));

            memset(r.data(),0,sizeof(unsigned)*r.size());

            sum_accumulate(r,x,y);
            fmt::println("r = {};",format(r));

            fmt::println("(x + y) - r;");
            fflush(stdout);
        }

        return 0;
    }

    int product (span<char const *> args)
    // requires args.size() > 1
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
        auto r = vector<unsigned>((degree * 2) + 1);

        fmt::println("obase=16;");
        fmt::println("ibase=16;");

        for (auto i = 0; i != iterations; ++i)
        {
            auto xz = fread(x.data(), sizeof(unsigned), x.size(), stdin);
            if (xz == -1) break;
            fmt::println("x = {};",format(x));

            auto yz = fread(y.data(), sizeof(unsigned), y.size(), stdin);
            if (yz == -1) break;
            fmt::println("y = {};",format(y));

            memset(r.data(),0,sizeof(unsigned)*r.size());

            product_accumulate(r,x,y);
            fmt::println("r = {};",format(r));

            fmt::println("(x * y) - r;");
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
        else if (command == "sum")
            return sum(args);
        else if (command == "product")
            return product(args);
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
