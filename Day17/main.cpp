#include "aoc/helpers.h"

namespace {
  using MappedFileSource = aoc::MappedFileSource<char>;

  using Quantities = std::vector<int64_t>;

  template<typename FnBase, typename FnAdd>
  int64_t count_combinations(Quantities::const_reverse_iterator it,
                              Quantities::const_reverse_iterator end,
                              int64_t target,
                              int64_t depth,
                              FnBase base,
                              FnAdd add,
                              int64_t init) {
    const auto q = *it;
    if (q > target) {
      return init;
    } else if (q == target) {
      return base(depth);
    }

    target -= q;

    auto count = init;
    while (++it != end) {
      const auto n = count_combinations(it, end, target, depth + 1, base, add, init);
      count = add(count, n);
    }
    return count;
  }

  int64_t count_combinations(const Quantities& containers, int64_t target) {
    int64_t count = 0;
    for (auto it = containers.rbegin(); it != containers.rend(); ++it) {
      count += count_combinations(it, containers.rend(), target, 0,
        [](int64_t) { return 1; },
        [](int64_t n, int64_t c) { return n + c; },
        0);
    }
    return count;
  }

  template<typename FnBase, typename FnAdd>
  int64_t count_combinations(const Quantities& containers, int64_t target, FnBase base, FnAdd add, int64_t init) {
    auto count = init;

    for (auto it = containers.rbegin(); it != containers.rend(); ++it) {
      const auto r = count_combinations(it, containers.rend(), target, 0, base, add, init);
      count = add(count, r);
    }
    return count;
  }

  int64_t shortest_combination(const Quantities& containers, int64_t target) {
    return count_combinations(containers, target,
      [](int64_t d) { return d; },
      [](int64_t n, int64_t c){ return std::min(n, c); },
      INT64_MAX);
  }

  int64_t count_shortest_combinations(const Quantities& containers, int64_t target) {
    const auto shortest = shortest_combination(containers, target);

    DEBUG_PRINT("shortest: " << shortest);

    return count_combinations(containers, target,
      [shortest](int64_t d) { return d == shortest; },
      [](int64_t n, int64_t c) { return n + c; },
      0);
  }
}

int main(int argc, char** argv) {
  aoc::AutoTimer t;

  std::unique_ptr<MappedFileSource>m(new MappedFileSource(argc, argv));
  std::string_view f(m->data(), m->size());

  Quantities containers;
  aoc::parse_as_integers(f, [&containers](const auto n){ DEBUG_PRINT(n); containers.push_back(n); });

  std::sort(containers.begin(), containers.end());

  int64_t part1 = count_combinations(containers, 150);  
  int64_t part2 = count_shortest_combinations(containers, 150);

  aoc::print_results(part1, part2);
  return 0;
}

