#include "aoc/helpers.h"

#include <map>

namespace {
  using MappedFileSource = aoc::MappedFileSource<char>;

  enum class Comp {
    Eq = 0,
    Gt = 1,
    Lt = 2,
  };

  using Value = std::pair<int, Comp>;
  using Aunt = std::map<std::string, Value, std::less<>>;

  const Aunt sue = {
    { "children", { 3, Comp::Eq } },
    { "cats", { 7, Comp::Gt } },
    { "samoyeds", { 2, Comp::Eq } },
    { "pomeranians", { 3, Comp::Lt } },
    { "akitas", { 0, Comp::Eq } },
    { "vizslas", { 0, Comp::Eq } },
    { "goldfish", { 5, Comp::Lt } },
    { "trees", { 3, Comp::Gt } },
    { "cars", { 2, Comp::Eq } },
    { "perfumes", { 1, Comp::Eq } },
  };

  const auto CompareWithAunt = [](const Value& p, int val) {
    switch (p.second) {
      case Comp::Eq:
        return p.first == val;
      case Comp::Gt:
        return val > p.first;
      case Comp::Lt:
        return val < p.first;
    }
    throw std::runtime_error("Invalid comparison");
  };
}


int main(int argc, char** argv) {
  aoc::AutoTimer t;

  std::unique_ptr<MappedFileSource>m(new MappedFileSource(argc, argv));
  std::string_view f(m->data(), m->size());
  std::string_view line;

  int64_t part1{-1};
  int64_t part2{-1};
  while (aoc::getline(f, line) && (part1 == -1 || part2 == -1)) {
    const auto parts = aoc::split(line, ", :");

    const auto it1 = sue.find(parts[2]);
    const auto it2 = sue.find(parts[4]);
    const auto it3 = sue.find(parts[6]);

    if (part1 == -1) {
      if (it1->second.first == aoc::stoi(parts[3]) &&
          it2->second.first == aoc::stoi(parts[5]) &&
          it3->second.first == aoc::stoi(parts[7])) {
        part1 = aoc::stoi(parts[1]);
      }
    }
    if (part2 == -1) {
      if (CompareWithAunt(it1->second, aoc::stoi(parts[3])) &&
          CompareWithAunt(it2->second, aoc::stoi(parts[5])) &&
          CompareWithAunt(it3->second, aoc::stoi(parts[7]))) {
        part2 = aoc::stoi(parts[1]);
      }
    }
  }

  aoc::print_results(part1, part2);

  return 0;
}

