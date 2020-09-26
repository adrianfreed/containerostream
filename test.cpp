#include "container_o_stream.h"


int main() {
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
    std::cout << data;


    return 0;
}