#include <bits/stdc++.h>
#include <boost/tokenizer.hpp>

std::vector<uint64_t> numbers;

uint64_t iterate(uint64_t max_ite) {
  std::map<uint64_t, std::pair<uint64_t, uint64_t>> memory;
  std::map<uint64_t, uint64_t> count;
  
  uint64_t ite = 0;
  uint64_t N   = numbers.size();
  uint64_t cur_spoken  = 0;
  uint64_t last_spoken = 0;
  bool dbg = true;

  auto memorize = [&] (const uint64_t n, const uint64_t ite) {
    memory[n].second = memory[n].first;
    memory[n].first  = ite;
  };

  // Counting the numbers for the first time
  for (auto n: numbers) {
    if (count.count(n) == 0)
      count[n] = 1;
    else
      count[n]++;
  }

  while (ite < max_ite) {
    if (ite < N)
      cur_spoken = numbers[ite];
    else {
      if (count[cur_spoken] > 1)
        cur_spoken = memory[cur_spoken].first - memory[cur_spoken].second;
      else
        cur_spoken = 0;

      if (count.count(cur_spoken) == 0)
        count[cur_spoken] = 1;
      else
        count[cur_spoken]++;
    }

    memorize(cur_spoken, ite);

    std::cout << cur_spoken << std::endl;
    if (count[cur_spoken] == 3 && dbg) {
      dbg = false;
      std::cerr << "First at 3 is " << cur_spoken << std::endl;
    }

    ite++;
  }
  
  return cur_spoken;
}

void part1() {
  std::cout << "Part 1 : " << iterate(2020) << std::endl;
}

void part2() {
  std::cout << "Part 2 : " << iterate(30000000) << std::endl;
}

int main(int argc, char **argv) {

  std::ifstream f_in;
  f_in.open("15.in");
  std::string line;
  std::getline(f_in, line);
  f_in.close();

  boost::char_separator<char> sep{",", ""};  
  boost::tokenizer tokenizer{line, sep};
  for (const auto &t: tokenizer)
    numbers.push_back(std::stoi(t));

  part1();
  part2();

  return 0;
}