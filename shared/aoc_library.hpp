#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <sstream>
#include <unordered_map>
#include <ranges>
#include <regex>
#include <chrono>

namespace aoc
{
    namespace ranges = std::ranges;

    inline std::vector<std::string> get_lines(const std::string &filename)
    {
        std::vector<std::string> res = {};
        std::string line;
        std::ifstream file(filename);
        assert(file.is_open());

        while (getline(file, line))
        {
            res.push_back(line);
        }
        file.close();

        return res;
    }

    inline std::vector<uint64_t> transform_int(const std::vector<std::string> &strings)
    {
        const auto to_int = [](const std::string &s)
        { return std::stoull(s); };
        const auto ints = strings | ranges::views::transform(to_int);
        return std::vector<uint64_t>(ints.begin(), ints.end());
    }

    inline std::vector<std::string> split(const std::string &in, const std::string &delim = " ")
    {
        const auto split = ranges::views::split(in, delim) |
                           ranges::views::transform([](const auto &part)
                                                    {
                                                        const auto c = part | ranges::views::common;
                                                        return std::string(c.begin(), c.end());
                                                    });
        std::vector<std::string> splitted(split.begin(), split.end());
        const auto [f, l] = ranges::remove_if(splitted, [](const auto &s)
                                              { return s.empty(); });
        splitted.erase(f, l);
        return splitted;
    }

    template <size_t SIZE>
    std::array<std::string, SIZE> search(const std::string &string, const std::regex &regex)
    {
        std::array<std::string, SIZE> res;
        std::smatch match;
        std::regex_search(string, match, regex);
        for (unsigned int i = 1; auto &s : res)
        {
            s = match[i++];
        }
        return res;
    }

    template <typename T>
    concept os_printable = requires(std::ostream os, T v)
    {
        os << v;
    };

    template <os_printable T>
    inline void assert_equal(const T actual, const T expected)
    {
        if (expected != actual)
        {
            std::stringstream ss;
            ss << "assert_equal failed, actual: " << actual << " expected: " << expected;
            throw std::runtime_error(ss.str());
        }
    }

    template <os_printable... Args>
    inline void printnl(Args &&...args)
    {
        (std::cout << ... << std::forward<Args>(args));
    }

    template <os_printable... Args>
    inline void print(Args &&...args)
    {
        printnl(args...);
        std::cout << std::endl;
    }

    template <os_printable T>
    inline void print(const std::vector<T> &v)
    {
        printnl("[");
        ranges::for_each(v, [](const auto &i)
                         { printnl(i, ", "); });
        print("]");
    }

    template <os_printable T>
    inline void print(const std::vector<std::vector<T>> &v)
    {
        print("[");
        ranges::for_each(v, [](const auto &vi)
                         {
                             printnl(" ");
                             print(vi);
                         });
        print("]");
    }

    template <os_printable T, os_printable K>
    inline void print(const std::pair<T, K> &pair)
    {
        print("{", pair.first, ", ", pair.second, "}, ");
    }

    template <os_printable K, os_printable T>
    inline void print(const std::unordered_map<T, K> &map)
    {
        print("{");
        ranges::for_each(map, [](const auto &entry)
                         { print(entry); });
        print("}");
    }

    class StopWatch
    {
    public:
        StopWatch()
        {
            start_ = std::chrono::high_resolution_clock::now();
        }
        ~StopWatch()
        {
            const auto stop = std::chrono::high_resolution_clock::now();
            const std::chrono::duration<double, std::milli> ms = stop - start_;
            print("Elapsed time: ", ms.count(), "ms");
        }

    private:
        std::chrono::_V2::system_clock::time_point start_;
    };
}