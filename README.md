# containerostream
 
This header file provides extensions to the iostream `<<` operator so you can pretty print C++ containers. It has a few interesting examples of template metaprogramming and uses some of the more modern C++ (2017) features.

```
[Vector 4, 2, 17, 14, 15]
[Map a:1, b:2]
(Deque 1, 83, 3, 335, 5)
(T hi, 23, 1.1, (13,bye))
```

## Pedagogical modernization branches

Three branches in this repo show the same library at three points in C++'s
evolution. The observable output is byte-identical across all three; the
implementations get steadily smaller and more idiomatic.

- **`master`** — original C++17 version: 8 `operator<<` overloads + 2
  helper functions.
- **[`cpp20`](https://github.com/adrianfreed/containerostream/tree/cpp20)** —
  collapsed to **one** template constrained by `std::ranges::input_range`,
  plus a tiny `style<C>` trait for per-container labels. Tuples via
  `std::apply` + fold expression.
- **[`cpp23`](https://github.com/adrianfreed/containerostream/tree/cpp23)** —
  most of the library is now redundant: C++23's `<format>` ships native
  range and tuple formatters (P2286, P2585). What remains is 3-line
  `std::formatter` specializations inheriting from
  `std::range_formatter<T>` and calling `set_brackets()`. Test uses
  `std::println`.

See each branch's README for a side-by-side comparison.
