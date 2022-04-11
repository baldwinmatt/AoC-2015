#include "aoc/helpers.h"

#include <set>

namespace {
  using MappedFileSource = aoc::MappedFileSource<char>;

  using Point = std::pair<ssize_t, ssize_t>;
  using VisitedMap = std::set<Point>;
}

int main(int argc, char** argv) {
  aoc::AutoTimer t;

  std::unique_ptr<MappedFileSource>m(new MappedFileSource(argc, argv));
  std::string_view f(m->data(), m->size());

  Point pos{0, 0};
  Point robo_pos{0, 0};
  Point santa_pos{0, 0};
  VisitedMap map;
  VisitedMap robo_map;

  // Initial position gets a present
  map.emplace(pos);
  robo_map.emplace(pos);

  bool santa = true;
  Point* ref = &santa_pos;

  for (const auto& c : f) {
    switch (c) {
      case '>':
        pos.first++;
        ref->first++;
        break;
      case '<':
        pos.first--;
        ref->first--;
        break;
      case '^':
        pos.second--;
        ref->second--;
        break;
      case 'v':
        pos.second++;
        ref->second++;
        break;
      default:
        throw std::runtime_error("Invalid input");
    }

    robo_map.emplace(ref->first, ref->second);

    santa = !santa;
    if (santa) { ref = &santa_pos; } else { ref = &robo_pos; }

    map.emplace(pos);
  }

  size_t part1 = map.size();
  size_t part2 = robo_map.size();

  aoc::print_results(part1, part2);

  return 0;
}

