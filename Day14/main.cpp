#include "aoc/helpers.h"

namespace {
  using MappedFileSource = aoc::MappedFileSource<char>;

  struct Reindeer {
    std::string_view name;
    int64_t speed{0};
    int64_t fly_time{0};
    int64_t rest_time{0};

    int64_t distance_traveled{0};
    int64_t ticks{0};
    int64_t points{0};

    bool is_flying{true};

    void tick() {
      if (is_flying) {
        distance_traveled += speed;
      }
      ticks++;
      if (is_flying && ticks >= fly_time) {
        ticks = 0;
        is_flying = false;
      }
      if (!is_flying && ticks >= rest_time) {
        ticks = 0;
        is_flying = true;
      }
    }
  };
}

int main(int argc, char** argv) {
  aoc::AutoTimer t;

  std::unique_ptr<MappedFileSource>m(new MappedFileSource(argc, argv));
  std::string_view f(m->data(), m->size());

  std::string_view line;
  std::vector<Reindeer> reindeers;
  while (aoc::getline(f, line)) {
    reindeers.push_back(Reindeer{});
    auto& r = reindeers.back();
    std::string_view part;
    int idx = 0;
    while (aoc::getline(line, part, " .")) {
      if (idx == 0) {
        r.name = part;
      } else if (idx == 3) {
        r.speed = aoc::stoi(part);
      } else if (idx == 6) {
        r.fly_time = aoc::stoi(part);
      } else if (idx == 13) {
        r.rest_time = aoc::stoi(part);
      }
      idx++;
    }
  }

  for (auto t = 0; t < 2503; t++) {
    auto lead = &reindeers[0];
    int max_distance = 0;
    for (auto& r : reindeers) {
      r.tick();
      if (r.distance_traveled > max_distance) {
        max_distance = r.distance_traveled;
        lead = &r;
      }
    }
    lead->points++;
  }

  int64_t part1 = 0;
  int64_t part2 = 0;
  for (auto& r : reindeers) {
    part1 = std::max(part1, r.distance_traveled);
    part2 = std::max(part2, r.points);
    DEBUG_PRINT(r.name << " " << r.distance_traveled << " points " << r.points);
  }

  aoc::print_results(part1, part2);

  return 0;
}

