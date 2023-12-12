#include "aoc/helpers.h"
#include "aoc/padded_matrix.h"

namespace {
  using MappedFileSource = aoc::MappedFileSource<char>;

  using Grid = aoc::PaddedMatrix<bool, 1>;

  const auto LoadInput = [](std::string_view f) {
    Grid g;
    std::string_view line;
    while (aoc::getline(f, line)) {
      g.set_width(line.size());
      const auto y = g.add_row();
      ssize_t x = 0;
      for (auto c : line) {
        g.add(x++, y, c == '#');
      }
    }
    g.fill_pddding(false);
    return g;
  };

  const auto CountOnNeigbors = [](Grid& g, const aoc::point& pt) {
    int count = 0;

    for (const auto& d : aoc::point_directions_8) {
      count += g.at(pt + d);
    }

    return count;
  };

  const auto Step = [](Grid& g, bool corners_fixed = false) {
    Grid next(g.get_height(), g.get_width());

    for (ssize_t y = 0; y < g.get_height(); ++y) {
      for (ssize_t x = 0; x < g.get_width(); ++x) {
        if (corners_fixed && ((x == 0 && y == 0) ||
                              (x == 0 && y == g.get_height() - 1) ||
                              (x == g.get_width() - 1 && y == 0) ||
                              (x == g.get_width() - 1 && y == g.get_height() - 1)))
      {
          next.add(x, y, true);
          continue;
        }
        const auto pt = aoc::point(x, y);
        const auto on = g.at(pt);
        const auto n = CountOnNeigbors(g, pt);
        next.add(x, y, (on && n == 2) || n == 3);
      }
    }

    return next;
  };

  const auto CountOn = [](Grid& g) {
    int count = 0;
    for (ssize_t y = 0; y < g.get_height(); ++y) {
      for (ssize_t x = 0; x < g.get_width(); ++x) {
        count += g.at(x, y) == true ? 1 : 0;
      }
    }
    return count;
  };
}

int main(int argc, char** argv) {
  aoc::AutoTimer t;

  std::unique_ptr<MappedFileSource>m(new MappedFileSource(argc, argv));
  std::string_view f(m->data(), m->size());

  Grid g = LoadInput(f);
  Grid g2 = g;

  g2.add(0, 0, true);
  g2.add(0, g2.get_height() - 1, true);
  g2.add(g2.get_width() - 1, 0, true);
  g2.add(g2.get_width() - 1, g2.get_height() - 1, true);

  for (int i = 0; i < 100; ++i) {
    g = Step(g);
    g2 = Step(g2, true);
  };

  int part1 = CountOn(g);
  int part2 = CountOn(g2);
  aoc::print_results(part1, part2);

  return 0;
}

