///
// Created by AdrianFreed on 9/24/20.
// << overloads for printing containers
// C++20 modernization: concepts + ranges + std::apply fold
//
// Pedagogical notes (vs. the original C++17 version):
//   * 8 near-identical operator<< overloads (vector/set/list/deque/...)
//     collapse to ONE template constrained by std::ranges::input_range.
//   * The recursive tupleElementPrint helper is replaced by std::apply
//     plus a C++17 fold expression. (Valid in C++17 but more idiomatic
//     once you have ranges to lean on for the rest.)
//   * Per-container labels move into a tiny customization-point trait,
//     so adding a new container is a 1-line specialization, not a 4-line
//     operator<<.
//   * The single overload is constrained to exclude std::string and
//     anything convertible to std::string_view, so we don't hijack
//     string output.
//

#ifndef CONTAINEROSTREAM_H
#define CONTAINEROSTREAM_H

#include <array>
#include <concepts>
#include <deque>
#include <forward_list>
#include <iostream>
#include <list>
#include <map>
#include <ranges>
#include <set>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace containerstream {

// Per-container styling. Default is a square-bracketed list.
// Specialize this trait to give a container a custom label or brackets.
template <typename C>
struct style {
    static constexpr const char* open  = "[";
    static constexpr const char* close = "]";
    static constexpr const char* sep   = ", ";
};

// Sequence-style
template <typename T, typename A> struct style<std::vector<T, A>> {
    static constexpr const char* open = "[Vector ";       static constexpr const char* close = "]"; static constexpr const char* sep = ", ";
};
template <typename T, std::size_t N> struct style<std::array<T, N>> {
    static constexpr const char* open = "[Array ";        static constexpr const char* close = "]"; static constexpr const char* sep = ", ";
};
template <typename T, typename A> struct style<std::deque<T, A>> {
    static constexpr const char* open = "(Deque ";        static constexpr const char* close = ")"; static constexpr const char* sep = ", ";
};
template <typename T, typename A> struct style<std::list<T, A>> {
    static constexpr const char* open = "(List ";         static constexpr const char* close = ")"; static constexpr const char* sep = ", ";
};
template <typename T, typename A> struct style<std::forward_list<T, A>> {
    static constexpr const char* open = "(ForwardList ";  static constexpr const char* close = ")"; static constexpr const char* sep = ", ";
};

// Set-style
template <typename T, typename C, typename A> struct style<std::set<T, C, A>> {
    static constexpr const char* open = "[Set ";          static constexpr const char* close = "]"; static constexpr const char* sep = ", ";
};
template <typename T, typename C, typename A> struct style<std::multiset<T, C, A>> {
    static constexpr const char* open = "[Multiset ";     static constexpr const char* close = "]"; static constexpr const char* sep = ", ";
};
template <typename T, typename H, typename E, typename A> struct style<std::unordered_set<T, H, E, A>> {
    static constexpr const char* open = "[UnorderedSet "; static constexpr const char* close = "]"; static constexpr const char* sep = ", ";
};

// Map-style (key:value pairs)
template <typename K, typename V, typename C, typename A> struct style<std::map<K, V, C, A>> {
    static constexpr const char* open = "[Map ";          static constexpr const char* close = "]"; static constexpr const char* sep = ", ";
};
template <typename K, typename V, typename C, typename A> struct style<std::multimap<K, V, C, A>> {
    static constexpr const char* open = "[Multimap ";     static constexpr const char* close = "]"; static constexpr const char* sep = ", ";
};
template <typename K, typename V, typename H, typename E, typename A> struct style<std::unordered_map<K, V, H, E, A>> {
    static constexpr const char* open = "[Unorderedmap "; static constexpr const char* close = "]"; static constexpr const char* sep = ", ";
};

// Concept: a printable container is any input_range we haven't already
// promised to stream as text. Excluding string-like types prevents this
// overload from stealing std::string / std::string_view output.
template <typename T>
concept printable_container =
    std::ranges::input_range<T> &&
    !std::convertible_to<std::remove_cvref_t<T>, std::string_view> &&
    !std::is_array_v<std::remove_cvref_t<T>>;

// Map-like: an associative container with key->value mapping
// (std::map, std::multimap, std::unordered_map, std::unordered_multimap).
// Detected via the standard mapped_type/key_type member typedefs, which
// distinguishes a real map from a sequence-of-pairs like std::vector<pair>.
template <typename R>
concept map_like = requires {
    typename std::remove_cvref_t<R>::key_type;
    typename std::remove_cvref_t<R>::mapped_type;
};

}  // namespace containerstream

// std::pair — printed as (first,second)
template <typename A, typename B>
std::ostream& operator<<(std::ostream& os, const std::pair<A, B>& p) {
    return os << '(' << p.first << ',' << p.second << ')';
}

// std::tuple — printed as (T a, b, c) using std::apply + fold expression.
// Replaces the original recursive tupleElementPrint<> helper.
template <typename... Ts>
std::ostream& operator<<(std::ostream& os, const std::tuple<Ts...>& t) {
    os << "(T ";
    std::apply(
        [&os](const auto&... xs) {
            std::size_t n = 0;
            (((n++ ? os << ", " : os) << xs), ...);
        },
        t);
    return os << ")";
}

// One template for every range-shaped container.
// Replaces 8 separate operator<< overloads in the C++17 version.
template <containerstream::printable_container C>
std::ostream& operator<<(std::ostream& os, const C& c) {
    using S = containerstream::style<std::remove_cvref_t<C>>;
    os << S::open;
    bool first = true;
    for (const auto& x : c) {
        if (!first) os << S::sep;
        if constexpr (containerstream::map_like<C>)
            os << x.first << ':' << x.second;
        else
            os << x;
        first = false;
    }
    return os << S::close;
}

#endif  // CONTAINEROSTREAM_H
