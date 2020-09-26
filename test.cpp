#include "container_o_stream.h"
void basicexamples();
void customclassexample();

int main()
{
    basicexamples();
    customclassexample();

    return 0;
}

void basicexamples()
{



    std::list l{1, 2, 3, 5, 5};
    std::cout << l << "\n";


    std::forward_list fl{1, 3, 3, 335, 5};
    std::cout << fl << "\n";

    std::deque d{1, 83, 3, 335, 5};
    std::cout << d << "\n";


    std::tuple pa{"hi", 23, 1.1, std::pair(13, "bye")};
    std::cout << pa << "\n";

    std::vector vec{4, 2, 17, 14, 15};
    std::cout << vec << "\n";

    std::array ar{1, 4, 2, 17, 11, 15};
    std::cout << ar << "\n";

    std::set set{1, 4, 2, 17, 17, 11, 15};
    std::cout << set << "\n";

    std::multiset mset{2, 4, 2, 17, 17, 11, 15};
    std::cout << mset << "\n";

    std::map<char, int> map{{'a', 1},
                            {'b', 2}};
    std::cout << map << "\n";

    const std::unordered_map<char, int> umap{{'a', 4},
                                             {'5', 2}};
    std::cout << umap << "\n";

    std::multimap<char, int> mmap{{'a', 4},
                                  {'b', 2},
                                  {'a', 5}};
    std::cout << mmap << "\n";

    std::array data{std::pair(1, 3), std::pair(3, 4)};
    std::cout << data<< "\n";

}


namespace pointexample {
template<typename T=double, int N = 3>
struct point {
    static_assert(std::is_arithmetic_v<T>, "arithmetic type required");
    std::array<T, N> data;

    template<typename... Args>
    explicit point(Args... args) : data({args...}) {
    };

    point<T, N> &operator+=(point<T, N> const &rhs) &{
        for (auto i = 0; i < N; ++i)
            data[i] += rhs.data[i];

        return *this;
    }

    friend point<T, N> operator+(point<T, N> lhs, point<T, N> const &rhs) {
        lhs += rhs;
        return lhs;
    }

    point<T, N> &operator*=(const point<T, N> &rhs) &{
        for (auto i = 0; i < N; ++i)
            data[i] *= rhs.data[i];

        return *this;
    }

    friend point<T, N> operator*(point<T, N> lhs, const point<T, N> &rhs) {
        lhs *= rhs;
        return lhs;
    }
};

template<typename T, int N>
std::ostream &operator<<(std::ostream &os, point<T, N> d) {
    return containerstream::arrayhelper(os, d.data, "[point ");
}



}

void customclassexample() {
    using namespace pointexample;
//    point<char *, 1> ps{"fred"} ;  // test static assert

    point p3{0.1, 1.2, 2.3};
    point<double> p{0.1, 1.2, 2.3};
    point<int, 4> pf{4, 1, 2, 4};

    std::cout << p + p3 << "\n";
    std::cout << pf << "\n";

    point<int, 1> pe{4};
    std::cout << pe << "\n";



}
