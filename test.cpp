#include "container_o_stream.h"

#include <print>
#include <ranges>

void basicexamples();
void customclassexample();

int main() {
    basicexamples();
    customclassexample();
}

void basicexamples() {
    std::list l{1, 2, 3, 5, 5};
    std::println("{}", l);

    std::forward_list fl{1, 3, 3, 335, 5};
    std::println("{}", fl);

    std::deque d{1, 83, 3, 335, 5};
    std::println("{}", d);

    std::tuple pa{"hi", 23, 1.1, std::pair(13, "bye")};
    std::println("{}", pa);

    std::vector vec{4, 2, 17, 14, 15};
    std::println("{}", vec);

    std::array ar{1, 4, 2, 17, 11, 15};
    std::println("{}", ar);

    std::set set{1, 4, 2, 17, 17, 11, 15};
    std::println("{}", set);

    std::multiset mset{2, 4, 2, 17, 17, 11, 15};
    std::println("{}", mset);

    std::map<char, int> map{{'a', 1}, {'b', 2}};
    std::println("{}", map);

    const std::unordered_map<char, int> umap{{'a', 4}, {'5', 2}};
    std::println("{}", umap);

    std::multimap<char, int> mmap{{'a', 4}, {'b', 2}, {'a', 5}};
    std::println("{}", mmap);

    std::array data{std::pair(1, 3), std::pair(3, 4)};
    std::println("{}", data);
}

namespace pointexample {
// Custom class. Model a range and provide one std::formatter specialization
// — the same path any user type takes in modern C++.
template <typename T = double, int N = 3>
struct point {
    static_assert(std::is_arithmetic_v<T>, "arithmetic type required");
    std::array<T, N> data;

    template <typename... Args>
    explicit point(Args... args) : data({args...}) {}

    auto begin() const { return data.begin(); }
    auto end() const { return data.end(); }

    point& operator+=(point const& rhs) & {
        for (int i = 0; i < N; ++i) data[i] += rhs.data[i];
        return *this;
    }
    friend point operator+(point lhs, point const& rhs) { lhs += rhs; return lhs; }

    point& operator*=(point const& rhs) & {
        for (int i = 0; i < N; ++i) data[i] *= rhs.data[i];
        return *this;
    }
    friend point operator*(point lhs, point const& rhs) { lhs *= rhs; return lhs; }
};
}  // namespace pointexample

template <typename T, int N>
struct std::formatter<pointexample::point<T, N>> : std::range_formatter<T> {
    constexpr formatter() { this->set_brackets("[point ", "]"); }
};

void customclassexample() {
    using namespace pointexample;
    //    point<char *, 1> ps{"fred"} ;  // test static assert

    point p3{0.1, 1.2, 2.3};
    point<double> p{0.1, 1.2, 2.3};
    point<int, 4> pf{4, 1, 2, 4};

    std::println("{}", p + p3);
    std::println("{}", pf);

    point<int, 1> pe{4};
    std::println("{}", pe);
}
