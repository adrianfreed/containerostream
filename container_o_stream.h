///
// Created by AdrianFreed on 9/24/20.
// C++23 modernization: std::formatter + std::range_formatter
//
// Pedagogical notes (vs. master / cpp20 branches):
//
//   1. Most of the library is no longer needed. In C++23, <format> ships
//      with default formatters for ranges (P2286) and tuples/pairs (P2585):
//
//          std::vector v{1,2,3};
//          std::println("{}", v);   // prints: [1, 2, 3]
//
//          std::map m{{'a',1},{'b',2}};
//          std::println("{}", m);   // prints: {'a': 1, 'b': 2}
//
//      No library code is required for that.
//
//   2. The only remaining value of this library is the *custom labels*
//      ("[Vector ...]", "(Deque ...)", "[Map ...]") that the original
//      C++17 version was designed around.
//
//   3. The modern way to add a label is to specialize std::formatter
//      and inherit from std::range_formatter<T>. Each container becomes
//      a 3-line specialization that just calls set_brackets().
//      Compare to the C++17 version's 4-line operator<< + helper-call
//      per container, or the cpp20 version's 1-line style<C> trait
//      specialization. C++23's approach also gains:
//        - works with std::format / std::print / std::println
//        - composes with format-spec syntax ("{:n}" for no-brackets,
//          "{:e}" for escaped strings, width/fill, ...)
//        - no global-namespace operator<< overloads on std types
//          (which the standard technically forbids).
//
//   4. std::formatter specializations live in namespace std — the
//      standard explicitly permits this for user code that wants to
//      format library types in a custom way.
//
//   5. Maps don't use std::range_formatter because we want "key:value"
//      (no space, matching the original output) rather than the
//      "key: value" format range_format::map produces by default.
//

#ifndef CONTAINEROSTREAM_H
#define CONTAINEROSTREAM_H

#include <array>
#include <deque>
#include <forward_list>
#include <format>
#include <list>
#include <map>
#include <ranges>
#include <set>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

// ---------- pair: print as (first,second) ----------
// Overrides the default C++23 std::pair formatter, which would yield
// "(1, 2)" with a space after the comma.
//
// Note: parse() and format() are templated on context types so they
// satisfy the std::formattable concept, which probes the formatter
// against multiple context instantiations.
template <typename A, typename B>
struct std::formatter<std::pair<A, B>> {
    constexpr auto parse(auto& ctx) { return ctx.begin(); }
    auto format(const std::pair<A, B>& p, auto& ctx) const {
        return std::format_to(ctx.out(), "({},{})", p.first, p.second);
    }
};

// ---------- tuple: print as (T a, b, c) ----------
// Overrides the default C++23 std::tuple formatter, which would yield
// "(a, b, c)" — we want the leading "T " marker the original library used.
template <typename... Ts>
struct std::formatter<std::tuple<Ts...>> {
    constexpr auto parse(auto& ctx) { return ctx.begin(); }
    auto format(const std::tuple<Ts...>& t, auto& ctx) const {
        auto out = ctx.out();
        out = std::format_to(out, "(T ");
        std::apply(
            [&out](const auto&... xs) {
                std::size_t n = 0;
                ((out = std::format_to(out, "{}{}", n++ ? ", " : "", xs)), ...);
            },
            t);
        return std::format_to(out, ")");
    }
};

// ---------- sequence-style: vector, array, set, deque, list, ... ----------
// Each is a 3-line specialization that inherits from std::range_formatter
// and overrides the brackets. The standard library does all the heavy
// lifting (iteration, separator, nested-formatter dispatch).

template <typename T, typename A>
struct std::formatter<std::vector<T, A>> : std::range_formatter<T> {
    constexpr formatter() { this->set_brackets("[Vector ", "]"); }
};

template <typename T, std::size_t N>
struct std::formatter<std::array<T, N>> : std::range_formatter<T> {
    constexpr formatter() { this->set_brackets("[Array ", "]"); }
};

template <typename T, typename A>
struct std::formatter<std::deque<T, A>> : std::range_formatter<T> {
    constexpr formatter() { this->set_brackets("(Deque ", ")"); }
};

template <typename T, typename A>
struct std::formatter<std::list<T, A>> : std::range_formatter<T> {
    constexpr formatter() { this->set_brackets("(List ", ")"); }
};

template <typename T, typename A>
struct std::formatter<std::forward_list<T, A>> : std::range_formatter<T> {
    constexpr formatter() { this->set_brackets("(ForwardList ", ")"); }
};

template <typename T, typename C, typename A>
struct std::formatter<std::set<T, C, A>> : std::range_formatter<T> {
    constexpr formatter() { this->set_brackets("[Set ", "]"); }
};

template <typename T, typename C, typename A>
struct std::formatter<std::multiset<T, C, A>> : std::range_formatter<T> {
    constexpr formatter() { this->set_brackets("[Multiset ", "]"); }
};

template <typename T, typename H, typename E, typename A>
struct std::formatter<std::unordered_set<T, H, E, A>> : std::range_formatter<T> {
    constexpr formatter() { this->set_brackets("[UnorderedSet ", "]"); }
};

// ---------- maps: key:value (no space, matching original) ----------
namespace containerstream::detail {
template <typename Map, typename Ctx>
auto format_map(const Map& m, Ctx& ctx, std::string_view label) {
    auto out = std::format_to(ctx.out(), "[{} ", label);
    bool first = true;
    for (const auto& [k, v] : m) {
        out = std::format_to(out, "{}{}:{}", first ? "" : ", ", k, v);
        first = false;
    }
    return std::format_to(out, "]");
}
}  // namespace containerstream::detail

template <typename K, typename V, typename C, typename A>
struct std::formatter<std::map<K, V, C, A>> {
    constexpr auto parse(auto& ctx) { return ctx.begin(); }
    auto format(const std::map<K, V, C, A>& m, auto& ctx) const {
        return containerstream::detail::format_map(m, ctx, "Map");
    }
};

template <typename K, typename V, typename C, typename A>
struct std::formatter<std::multimap<K, V, C, A>> {
    constexpr auto parse(auto& ctx) { return ctx.begin(); }
    auto format(const std::multimap<K, V, C, A>& m, auto& ctx) const {
        return containerstream::detail::format_map(m, ctx, "Multimap");
    }
};

template <typename K, typename V, typename H, typename E, typename A>
struct std::formatter<std::unordered_map<K, V, H, E, A>> {
    constexpr auto parse(auto& ctx) { return ctx.begin(); }
    auto format(const std::unordered_map<K, V, H, E, A>& m, auto& ctx) const {
        return containerstream::detail::format_map(m, ctx, "Unorderedmap");
    }
};

#endif  // CONTAINEROSTREAM_H
