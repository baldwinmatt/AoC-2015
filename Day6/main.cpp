#include "aoc/helpers.h"
#include <algorithm>
#include <numeric>
#include <array>

namespace {
  using MappedFileSource = aoc::MappedFileSource<char>;

  class LightGrid {
    static constexpr size_t _width = 1000;
    static constexpr size_t _height = 1000;

    std::array<bool, _width * _height> _grid;
    std::array<int, _width * _height> _brightGrid;

    size_t to_offset(size_t x, size_t y) const {
      assert(x < _width);
      assert(y < _height);

      return _width * y + x;
    }

  public:
    LightGrid()
      : _grid()
    {
      for (size_t i = 0; i < _grid.size(); i++) {
        _grid[i] = false;
        _brightGrid[i] = 0;
      }
    }

    size_t width() const { return _width; }
    size_t height() const { return _height; }

    bool at(size_t x, size_t y) const {
      return _grid[to_offset(x, y)];
    }

    void toggle(size_t x, size_t y) {
      const auto n = to_offset(x, y);
      _grid[n] = !_grid[n];
      _brightGrid[n] += 2;
    }

    void turnOn(size_t x, size_t y) {
      const auto n = to_offset(x, y);
      _grid[n] = true;
      _brightGrid[n] ++;
    }

    void turnOff(size_t x, size_t y) {
      const auto n = to_offset(x, y);
      _grid[n] = false;
      _brightGrid[n] -= !!_brightGrid[n];
    }

    size_t countOn() const {
      size_t n = 0;
      return std::accumulate(_grid.cbegin(), _grid.cend(), n, [](const size_t n, const auto b)
        { return n + b; } );
    }

    size_t brightness() const {
      size_t n = 0;
      return std::accumulate(_brightGrid.cbegin(), _brightGrid.cend(), n, [](const size_t n, const auto b)
        { return n + b; } );
    }
  };

  enum class Instruction {
    None,
    Toggle,
    TurnOn,
    TurnOff
  };
}

std::ostream& operator<<(std::ostream& os, const LightGrid& g) {
  for (size_t y = 0; y < g.height(); y++) {
    for (size_t x = 0; x < g.width(); x++) {
      if (g.at(x, y)) {
        os << "#";
      } else {
        os << " ";
      }
    }
    os << std::endl;
  }
  os << std::endl;
  return os;
}

int main(int argc, char** argv) {
  aoc::AutoTimer t;

  std::unique_ptr<MappedFileSource>m(new MappedFileSource(argc, argv));
  std::string_view f(m->data(), m->size());

  LightGrid ig{};
  std::string_view line;

  constexpr std::string_view TOGGLE("toggle");
  constexpr std::string_view TURN("turn");
  constexpr std::string_view ON("on");
  constexpr std::string_view OFF("off");
  constexpr std::string_view THROUGH("through");

  while (aoc::getline(f, line)) {
    std::string_view part;
    aoc::getline(line, part, " ");
    Instruction i = Instruction::None;

    if (part == TOGGLE) {
      i = Instruction::Toggle;
      goto coords;
    } else if (part != TURN) {
      throw std::runtime_error("Bad input");
    }
    aoc::getline(line, part, " ");
    if (part == ON) {
      i = Instruction::TurnOn;
    } else if (part == OFF) {
      i = Instruction::TurnOff;
    }

coords:
    // x,y
    aoc::getline(line, part, " ");
    auto pos = part.find(',');
    if (pos == std::string_view::npos) {
      throw std::runtime_error("Bad input");
    }
    const aoc::Point p1{
      aoc::stoi(part.substr(0, pos)),
      aoc::stoi(part.substr(pos + 1))
    };
    
    // through
    aoc::getline(line, part, " ");
    (void)THROUGH;
    assert(part == THROUGH);
    
    // x,y
    aoc::getline(line, part, " ");
    pos = part.find(',');
    if (pos == std::string_view::npos) {
      throw std::runtime_error("Bad input");
    }
    const aoc::Point p2{
      aoc::stoi(part.substr(0, pos)), 
      aoc::stoi(part.substr(pos + 1))
    };

    using Func = std::function<void(size_t x, size_t y)>;
    const auto toggle = [&](size_t x, size_t y) {
      ig.toggle(x, y);
    };
    const auto turnOn = [&](size_t x, size_t y) {
      ig.turnOn(x, y);
    };
    const auto turnOff = [&](size_t x, size_t y) {
      ig.turnOff(x, y);
    };

    Func func;
    switch (i) {
      case Instruction::Toggle:
        func = toggle;
        break;
      case Instruction::TurnOn:
        func = turnOn;
        break;
      case Instruction::TurnOff:
        func = turnOff;
        break;
      default:
        throw std::runtime_error("Bad Input");
    }

    for (auto x = p1.first; x <= p2.first; x++) {
      for (auto y = p1.second; y <= p2.second; y++) {
        func(x, y);
      }
    }
    
    DEBUG(std::cout << aoc::cls << ig;);
  }

  size_t part1 = ig.countOn();
  size_t part2 = ig.brightness();

  aoc::print_results(part1, part2);

  return 0;
}

