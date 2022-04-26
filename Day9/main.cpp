#include "aoc/helpers.h"

#include <algorithm>
#include <vector>
#include <map>

namespace {
  using MappedFileSource = aoc::MappedFileSource<char>;

  using PlaceMap = std::map<std::string_view, std::map<std::string_view, size_t>>;
}

int main(int argc, char** argv) {
  aoc::AutoTimer t;

  std::unique_ptr<MappedFileSource>m(new MappedFileSource(argc, argv));
  std::string_view f(m->data(), m->size());

  PlaceMap distances;
  std::string_view line;
  while (aoc::getline(f, line)) {
    std::string_view parts[3];
    std::string_view skip;
    aoc::getline(line, parts[0], " ");
    aoc::getline(line, skip, " ");
    assert(skip == std::string_view("to"));
    aoc::getline(line, parts[1], " ");
    aoc::getline(line, skip, " ");
    assert(skip == std::string_view("="));
    aoc::getline(line, parts[2], " ");

    size_t dist = aoc::stoi(parts[2]);
    const auto from = parts[0];
    const auto to = parts[1];
    distances[from][to] = distances[to][from] = dist;

    DEBUG_PRINT("From: " << parts[0] << " to " << parts[1] << " = " << dist);
  }

  std::vector<std::string_view> cities(distances.size());

  std::transform(distances.begin(), distances.end(), cities.begin(), [](auto p){ DEBUG_PRINT("City: " << p.first); return p.first; });

  size_t part1 = SIZE_MAX;
  size_t part2 = 0;
  do {
    size_t dis = 0;
    for (auto it = cities.begin(); it != (cities.end() - 1); ++it) {
      dis += distances[*it][*(it+1)];
    }

    part1 = std::min(dis, part1);
    part2 = std::max(dis, part2);
  } while (std::next_permutation(cities.begin(), cities.end()));

  aoc::print_results(part1, part2);

  return 0;
}

