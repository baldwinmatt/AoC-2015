#include "aoc/helpers.h"

namespace {
  using MappedFileSource = aoc::MappedFileSource<char>;
}

int main(int argc, char** argv) {
  aoc::AutoTimer t;

  std::unique_ptr<MappedFileSource>m(new MappedFileSource(argc, argv));
  std::string_view f(m->data(), m->size());

  ssize_t part1 = 0;
  size_t part2 = SIZE_MAX;
  size_t pos = 0;

  for (const auto &c : f) {
    switch (c) {
      case '(':
        part1++;
        break;
      case ')':
        part1--;
        break;
      default:
        throw std::runtime_error("Invalid character");
    }

    pos++;
    if (part2 == SIZE_MAX && part1 < 0) {
      part2 = pos;
    }
  }

  aoc::print_results(part1, part2);

  return 0;
}

