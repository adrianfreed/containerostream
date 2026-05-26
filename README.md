# containerostream — C++20 branch

Header-only `operator<<` overloads that print STL containers with readable,
labeled output:

```
[Vector 4, 2, 17, 14, 15]
[Map a:1, b:2]
(Deque 1, 83, 3, 335, 5)
(T hi, 23, 1.1, (13,bye))
```

This branch is a pedagogical modernization of the original C++17 version
(`master`). The output is byte-identical, but the implementation is much
smaller.

## What C++20 buys us

| Concern                       | C++17 (master)                                          | C++20 (this branch)                                |
| ----------------------------- | ------------------------------------------------------- | -------------------------------------------------- |
| Container coverage            | 8 separate `operator<<` overloads + 2 helper functions  | **One** template constrained by `std::ranges::input_range` |
| Tuple printing                | Recursive `tupleElementPrint<I>` with `if constexpr`    | `std::apply` + fold expression — no helper         |
| Per-container labels          | Hard-coded in each overload                             | `containerstream::style<C>` trait — 1-line spec    |
| Adding a new container        | Write a new operator<<                                  | Either model `std::ranges::range`, or add a `style` specialization |
| String safety                 | Implicit (no overload took strings)                     | Explicit concept exclusion: `!convertible_to<string_view>` |

## See also

- `master` — the original C++17 version
- `cpp23` — further modernization using `std::formatter` + `std::print`
