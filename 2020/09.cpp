#include <bits/stdc++.h>

std::vector<uint64_t> input;
constexpr int preamble_length = 25;
uint64_t invalid;

bool valid(uint position) {
  uint64_t ref = input[position];
  bool found = false;
  for (uint i=position-preamble_length; i < position-1; ++i) {
    uint64_t n1 = input[i];
    for (uint j=i+1; j < position; ++j) {
      if (ref == n1 + input[j])
        return true;
    }
  }

  return false;
}

void part1() {
  uint N=input.size();
  for (uint i=preamble_length; i < N; ++i) {
    if (!valid(i)) {
      invalid = input[i];
      break;
    }
  }

  std::cout << "Part 1 : " << invalid << std::endl;
}

void part2() {
  uint N=input.size();
  uint64_t res;
  for (uint i=0; i < N; ++i) {
    uint64_t acc = 0;
    uint64_t smallest = -1;
    uint64_t largest  = 0;

    uint j=i;
    while (acc < invalid) {
      uint64_t cur = input[j];
      smallest = std::min(cur, smallest);
      largest  = std::max(cur, largest);
      acc += cur;
      j++;
    }
    
    if (acc == invalid) {
      res = smallest + largest;
      break;
    }
  }

  std::cout << "Part 2 : " << res << std::endl;
}

int main(int argc, char **argv) {
  std::ifstream f_in;
  f_in.open("09.in");
  for (uint64_t in; f_in >> in;)
    input.push_back(in);
  f_in.close();
  part1();
  part2();
  return 0;
}