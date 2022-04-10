#include "aoc/helpers.h"
#include <vector>

namespace {
  using MappedFileSource = aoc::MappedFileSource<char>;
}

int main(int argc, char** argv) {
  aoc::AutoTimer t;

  std::unique_ptr<MappedFileSource>m(new MappedFileSource(argc, argv));
  std::string_view f(m->data(), m->size());

  size_t part1 = 0;
  size_t part2 = 0;
  std::string_view line;
  while (aoc::getline(f, line)) {
    std::vector<size_t>parts;
    aoc::parse_as_integers(line, "x", [&parts](int n){parts.push_back(n);});

    if (parts.size() != 3) { throw std::runtime_error("Bad input"); }

    size_t slack = SIZE_MAX;
    const size_t areas[] = {
      parts[0] * parts[1],
      parts[1] * parts[2],
      parts[0] * parts[2],
    };
    for (size_t i = 0; i < sizeof(areas) / sizeof(areas[0]); i++) {
      slack = std::min(slack, areas[i]);
      part1 += 2 * areas[i];
    }

    part1 += slack;

    const size_t perims[] = {
      2 * parts[0] + 2 * parts[1],
      2 * parts[1] + 2 * parts[2],
      2 * parts[0] + 2 * parts[2],
    };

    size_t ribbon = SIZE_MAX;
    for (size_t i = 0; i < sizeof(perims) / sizeof(perims[0]); i++) {
      ribbon = std::min(ribbon, perims[i]);
    }

    ribbon += parts[0] * parts[1] * parts[2];
    part2 += ribbon;
  }

  aoc::print_results(part1, part2);

  return 0;
}

