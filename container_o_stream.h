///
// Created by AdrianFreed on 9/24/20.
// << overloads for printing containers
// requires C++17
//

#ifndef TEST_CONTAINER_O_STREAM_H
#define TEST_CONTAINER_O_STREAM_H
#include <iostream>
#include <array>
#include <vector>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <tuple>
#include <deque>
#include <list>
#include <forward_list>

// readable text for container content:  pairs
template<typename TA, typename TB>
auto &operator<<(std::ostream &os, const std::pair<TA, TB> &p) {
    os << '(' << p.first << ',' << p.second << ')';
    return os;
}

namespace containerstream {
/// variadic template metaprogram to iterate over the tuple elements
// requires C++17, replace with messier folds if using earlier version
    template<size_t I = 0, typename... Tp>
    void tupleElementPrint(const std::tuple<Tp...> &t, std::ostream &os) {
        os << std::get<I>(t);
        if constexpr(I + 1 != sizeof...(Tp)) {
            os << ", ";
            tupleElementPrint<I + 1>(t, os);
        }
    }
}
///  readable text for container content:  tuples
template<typename ...Types>
auto &operator<<(  std::ostream &os, const std::tuple<Types ...> &t) {
    os << "(T ";
    containerstream::tupleElementPrint(t, os);
    os << ")";
    return os;
}

namespace containerstream {

///  maps have key:value pairs
    template<typename C>
    const std::ostream &maphelper(std::ostream &os, C &v,
                            const char *first,
                            const char *separator=", ",
                            const char *last="]") {
        os << first;
        for (auto it = v.cbegin(); it != v.cend(); ++it) {
            os << it->first << ':' << it->second;
            if (std::next(it) != v.cend())
                os << separator;
        }
        os << last;
        return os;
    }
}
///  readable text for container content:  unordered_maps
template<typename T, typename Tbis>
std::ostream &operator<<(const std::ostream &os, const std::unordered_map<T, Tbis> &v) {
    return containerstream::maphelper(os, v, "[Unorderedmap ");
}
///  readable text for container content:  maps
template<typename T, typename Tbis>
std::ostream &operator<<(const std::ostream &os, const std::map<T, Tbis> &v) {
    return containerstream::maphelper(os, v, "[Map ", ", ", "]");
}
///  readable text for container content:  multimap
template<typename T, typename Tbis>
std::ostream &operator<<(const std::ostream &os, const std::multimap<T, Tbis> &v) {
    return containerstream::maphelper(os, v, "[Multimap ", ", ", "]");
}

namespace containerstream {

// Most containers are just output as a bracketed, sequence with a separator
    template<typename C>
     std::ostream &
    arrayhelper( std::ostream &os, C &v,
                const char *first,
                const char *separator= ", ",
                const char *last="]") {
        os << first;
        for (auto it = v.cbegin(); it != v.cend(); ++it) {
            os << *it;
            if (std::next(it) != v.cend())
                os << separator;
        }
        os << last;
        return os;
    }

}

// There may appear to be a lot of repetition in these functions
// The idea behind that is that you are invited to change the styling of
// each separator and bracket according to your own aesthetics or conventions

///  readable text for container content:  Array
template<typename T, std::size_t N>
auto &operator<<(const std::ostream &os, const std::array<T, N> &a) {
    return containerstream::arrayhelper(os, a, "[Array ");
}

///  readable text for container content:  Vector
template<typename T>
std::ostream &operator<<(const std::ostream &os, const std::vector<T> &v) {
    return containerstream::arrayhelper(os, v, "[Vector ");
}
///  readable text for container content:  Set
template<typename T>
std::ostream &operator<<(const std::ostream &os, const std::set<T> &v) {
    return containerstream::arrayhelper(os, v, "[Set ");
}
///  readable text for container content:  Multiset
template<typename T>
std::ostream &operator<<(const std::ostream &os, const std::multiset<T> &v) {
    return containerstream::arrayhelper(os, v, "[Multiset ");
}
///  readable text for container content:  List
template<typename T>
std::ostream &operator<<(const std::ostream &os, const std::list<T> &l) {
    return containerstream::arrayhelper(os, l, "(List ", ", ", ")");
}
///  readable text for container content:  Forward lists
template<typename T>
std::ostream &operator<<(const std::ostream &os, const std::forward_list<T> &l) {
    return containerstream::arrayhelper(os, l, "(ForwardList ", ", ", ")");
}
///  readable text for container content:  Deque
template<typename T>
std::ostream &operator<<(const std::ostream &os, const std::deque<T> &d) {
    return containerstream::arrayhelper(os, d, "(Deque ", ", ", ")");
}

#endif //TEST_CONTAINER_O_STREAM_H
