#include "aoc/helpers.h"
#include <map>

namespace {
  using MappedFileSource = aoc::MappedFileSource<char>;

  enum class Operation {
    None,
    AND,
    OR,
    NOT,
    LSHIFT,
    RSHIFT,
    ASSIGN,
  };

  constexpr std::string_view AND("AND");
  constexpr std::string_view OR("OR");
  constexpr std::string_view NOT("NOT");
  constexpr std::string_view LSHIFT("LSHIFT");
  constexpr std::string_view RSHIFT("RSHIFT");
  constexpr std::string_view ASSIGN("->");

  const auto OperationFromString = [](const auto& v) {
    switch (v.size()) {
      case 2:
        if (v == OR) { return Operation::OR; }
        else if (v == ASSIGN) { return Operation::ASSIGN; }
        break;
      case 3:
        if (v == AND) { return Operation::AND; }
        else if (v == NOT) { return Operation::NOT; }
        break;
      case 6:
        if (v == LSHIFT) { return Operation::LSHIFT; }
        else if (v == RSHIFT) { return Operation::RSHIFT; }
        break;
      default:
        break;
    }
    return Operation::None;
  };

  class Gate {
    Operation op;

    std::string_view lhs;
    std::string_view rhs;

    bool stored;

    uint16_t value;

  public:
    Gate(Operation op, std::string_view lhs, std::string_view rhs)
      : op(op)
      , lhs(lhs)
      , rhs(rhs)
      , stored(false)
      , value(0)
    { }

    Gate(Operation op, std::string_view lhs)
      : Gate(op, lhs, std::string_view())
    { }

    Gate()
      : Gate(Operation::None, std::string_view(), std::string_view())
    { }

    bool is_memoized() const { return stored; }
    uint16_t get_value() const { return value; }
    Operation get_operation() const { return op; }
    std::string_view get_lhs() const { return lhs; }
    std::string_view get_rhs() const { return rhs; }
    void assign(uint16_t val) { 
      value = val;
      stored = true;
    }
    void reset() {
      stored = false;
    }
  };

  using WireMap = std::map<std::string_view, Gate>;

  class Circuit {
    WireMap _circuit;
    
  public:
    void reset() {
      for (auto& [k,v] : _circuit) {
        (void)k;
        v.reset();
      }
    }

    void update(std::string_view wire, Gate&& g) {
      Gate noop;
      auto it = _circuit.emplace(wire, noop);
      it.first->second = g;
    }

    void add(std::string_view wire, Gate&& g) {
      if (!_circuit.emplace(wire, g).second) {
        throw std::runtime_error("Duplicate output wire");
      }
    }

    uint16_t value_of(std::string_view s) {
      if (aoc::is_numeric(s)) {
        return aoc::stoi(s);
      }

      return get_output(s);
    }

    uint16_t get_output(std::string_view wire) {

      auto it = _circuit.find(wire);
      assert(it != _circuit.end());

      Gate& g = it->second;

      if (g.is_memoized()) { return g.get_value(); }

      switch (g.get_operation()) {
        case Operation::None:
          throw std::logic_error("Bad Input");
        case Operation::ASSIGN:
          g.assign(this->value_of(g.get_lhs()));
          break;
        case Operation::NOT:
          g.assign(~this->value_of(g.get_lhs()));
          break;
        case Operation::AND:
          g.assign(this->value_of(g.get_lhs()) & this->value_of(g.get_rhs()));
          break;
        case Operation::OR:
          g.assign(this->value_of(g.get_lhs()) | this->value_of(g.get_rhs()));
          break;
        case Operation::LSHIFT:
          g.assign(this->value_of(g.get_lhs()) << this->value_of(g.get_rhs()));
          break;
        case Operation::RSHIFT:
          g.assign(this->value_of(g.get_lhs()) >> this->value_of(g.get_rhs()));
          break;
      }
      return g.get_value();
    }
  };
}

int main(int argc, char** argv) {
  aoc::AutoTimer t;

  std::unique_ptr<MappedFileSource>m(new MappedFileSource(argc, argv));
  std::string_view f(m->data(), m->size());

  std::string_view line;

  Circuit circuit;
  while (aoc::getline(f, line)) {
    // parse a line, which is space delimited
    // Each line is either 3 4 or 5 words
    std::string_view parts[5];
    size_t i;
    for (i = 0; i < 5; i++) {
      if (!aoc::getline(line, parts[i], " ")) { break; }
    }

    Gate g;
    Operation op;
    std::string_view assign;
    switch (i) {
      case 3:
        op = OperationFromString(parts[1]);
        assert(op == Operation::ASSIGN);
        g = Gate(op, parts[0]);
        assign = parts[2];
        break;
      case 4:
        op = OperationFromString(parts[0]);
        assert(op == Operation::NOT);
        g = Gate(op, parts[1]);
        assert(OperationFromString(parts[2]) == Operation::ASSIGN);
        assign = parts[3];
        break;
      case 5:
        op = OperationFromString(parts[1]);
        assert(op == Operation::None);
        g = Gate(op, parts[0], parts[2]);
        assert(OperationFromString(parts[3]) == Operation::ASSIGN);
        assign = parts[4];
        break;
      default:
        throw std::runtime_error("Bad Input");
    }
    circuit.add(assign, std::move(g));
  }

  uint16_t part1 = circuit.get_output("a");

  Gate b;
  b.assign(part1);
  circuit.reset();
  circuit.update("b", std::move(b));

  uint16_t part2 = circuit.get_output("a");

  aoc::print_results(part1, part2);

  return 0;
}

