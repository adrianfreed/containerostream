# containerostream — C++23 branch

Header-only formatters that print STL containers with readable, labeled output:

```
[Vector 4, 2, 17, 14, 15]
[Map a:1, b:2]
(Deque 1, 83, 3, 335, 5)
(T hi, 23, 1.1, (13,bye))
```

```cpp
#include "container_o_stream.h"
#include <print>

std::vector v{4, 2, 17, 14, 15};
std::println("{}", v);          // [Vector 4, 2, 17, 14, 15]
```

This branch is the third step in a pedagogical modernization sequence:
`master` (C++17) → `cpp20` → `cpp23` (this branch). The user-visible output
is byte-identical across all three, but the implementation gets steadily
smaller and more idiomatic.

## What C++23 buys us

The headline: **most of this library is now in the standard library.**

```cpp
std::vector v{1, 2, 3};
std::println("{}", v);            // → [1, 2, 3]   (built-in, P2286)

std::map m{{'a', 1}, {'b', 2}};
std::println("{}", m);            // → {'a': 1, 'b': 2}   (built-in)

std::tuple t{1, "hi", 3.14};
std::println("{}", t);            // → (1, "hi", 3.14)   (P2585)
```

No `containerostream.h` required. The only reason to use this library
in C++23 is if you want the **custom labels** the original was designed
around (`[Vector ...]`, `(Deque ...)`, `[Map ...]`).

## How the labels are added

Each container becomes a 3-line `std::formatter` specialization that
inherits from `std::range_formatter<T>` and calls `set_brackets()`:

```cpp
template <typename T, typename A>
struct std::formatter<std::vector<T, A>> : std::range_formatter<T> {
    constexpr formatter() { this->set_brackets("[Vector ", "]"); }
};
```

That's the whole specialization. The standard library handles iteration,
the separator, and recursive dispatch to per-element formatters.

## Side-by-side: same operation, three eras

| Concern                          | C++17 (master)                | C++20 (`cpp20`)                       | C++23 (this branch)                          |
| -------------------------------- | ----------------------------- | ------------------------------------- | -------------------------------------------- |
| Format machinery                 | `operator<<` overloads         | One concept-constrained `operator<<`  | `std::formatter` specializations              |
| Per-container code               | ~4 lines of `operator<<`       | 1-line `style<C>` trait specialization | 3-line formatter inheriting `range_formatter` |
| Lives in namespace               | Global (technically UB on std types) | Global (same)                  | `std` (explicitly permitted)                 |
| Iteration logic                  | Hand-written in `arrayhelper`  | Range-based for in single template    | None — `range_formatter` does it             |
| Tuple printing                   | Recursive `if constexpr`       | `std::apply` + fold                   | `std::apply` + fold (inside formatter)        |
| Output API                       | `std::cout <<`                 | `std::cout <<`                        | `std::print` / `std::println`                |
| Composes with format-specs       | No                             | No                                    | Yes (`{:n}`, `{:e}`, width/fill, ...)         |

## See also

- `master` — the original C++17 version
- `cpp20` — concepts + ranges modernization
