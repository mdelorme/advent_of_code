#include <bits/stdc++.h>

std::vector<std::string> input;

enum Mode {EASY, ADVANCED};
template <Mode mode>
uint64_t solve_ops(std::string &expression) {
  std::regex op_re("([0-9]+) (\\*|\\+) ([0-9]+)");
  std::regex add_re("([0-9]+) \\+ ([0-9]+)");
  std::regex mul_re("([0-9]+) \\* ([0-9]+)");
  std::smatch op_match;
  uint64_t changes = 0;

  if (mode == EASY) {
    while (std::regex_search(expression, op_match, op_re)) {
      changes++;
      uint64_t v1 = std::stoll(op_match[1]);
      std::string op = op_match[2];
      uint64_t v2 = std::stoll(op_match[3]);
      uint64_t result = (op == "+" ? v1+v2 : v1*v2);
      std::ostringstream oss;
      oss << op_match.prefix() << result << op_match.suffix();
      std::cout << "Replaced " << expression << " by " << oss.str() << std::endl;
      expression = oss.str(); 
    } 
  }
  else {
    while (std::regex_search(expression, op_match, add_re)) {
      changes++;
      uint64_t v1 = std::stoll(op_match[1]);
      uint64_t v2 = std::stoll(op_match[2]);
      uint64_t result = v1+v2;
      std::ostringstream oss;
      oss << op_match.prefix() << result << op_match.suffix();
      std::cout << "Replaced " << expression << " by " << oss.str() << std::endl;
      expression = oss.str(); 
    } 
    while (std::regex_search(expression, op_match, mul_re)) {
      changes++;
      uint64_t v1 = std::stoll(op_match[1]);
      uint64_t v2 = std::stoll(op_match[2]);
      uint64_t result = v1*v2;
      std::ostringstream oss;
      oss << op_match.prefix() << result << op_match.suffix();
      std::cout << "Replaced " << expression << " by " << oss.str() << std::endl;
      expression = oss.str(); 
    }    
  }

  return changes;
}

void replace_nested_expressions(std::string &expression) {
  std::regex brackets_re("\\(([0-9]+)\\)");
  std::smatch brackets_match;

  while (std::regex_search(expression, brackets_match, brackets_re)) {
    std::ostringstream oss;
    oss << brackets_match.prefix() << brackets_match[1] << brackets_match.suffix();
    std::cout << "Removed nested block : " << expression << " -> " << oss.str() << std::endl;
    expression = oss.str();
  }
}

template<Mode mode>
std::string parse_expression(std::string expression) {
  bool done = false;
  std::string read = "";
  std::string bracketed = "";
  uint64_t pos = 0;
  uint64_t nested = 0;

  std::cout << "Parsing expression : " << expression << std::endl;

  while (pos < expression.size()) {
    char c = expression[pos];
    if (c == '(') {
      nested++;
      if (nested > 1)
        bracketed += c;
      else
        bracketed = "";
    }
    else if (c == ')') {
      nested--;
      if (nested > 0)
        bracketed += c;
      else {
        std::cout << "Found nested expression " << bracketed << std::endl;
        std::string replacement = parse_expression<mode>(bracketed);
        read += replacement;
      }
    }
    else {
      if (nested > 0)
        bracketed += c;
      else
        read += c;
    }
    pos++;
  }

  std::cout << "Solving ops on " << read << std::endl;
  solve_ops<mode>(read);

  std::cout << "Final expression is " << read << std::endl;
  return read;
}

void part1() {
  uint64_t result = 0;
  for (auto s: input)
    result += std::stoll(parse_expression<EASY>(s));
  std::cout << "Part 1 : " << result << std::endl;
}

void part2() {
  uint64_t result = 0;
  for (auto s: input)
    result += std::stoll(parse_expression<ADVANCED>(s));
  std::cout << "Part 2 : " << result << std::endl;
}

int main(int argc, char **argv) {
  std::ifstream f_in;
  f_in.open("18.in");
  while (!f_in.eof()) {
    std::string line;
    std::getline(f_in, line);
    input.push_back(line);
  }
  f_in.close();

  part1();
  part2();

  return 0;
}