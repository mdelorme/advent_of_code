#include <bits/stdc++.h>

struct Policy {
  char c;
  int v1;
  int v2;
  std::string password;
};

std::vector<Policy> policies;

void part1() {
  int valid_passwords = 0;

  // For each policy
  for (auto p:policies) {
    std::map<char, int> counter;

    // Counting the letters in the password
    for (auto c:p.password) {
      if (counter.find(c) == counter.end())
        counter[c] = 1;
      else
        counter[c]++;
    }

    // Checking the word is valid
    if (counter.find(p.c) != counter.end()) {
      if (counter[p.c] >= p.v1 && counter[p.c] <= p.v2)
        valid_passwords++;
    }
  }

  std::cout << "Part 1 : " << valid_passwords << std::endl;
}

void part2() {
  int valid_passwords = 0;

  // For each policy
  for (auto p:policies) {
    // XOR
    if ((p.password[p.v1-1] == p.c) != (p.password[p.v2-1] == p.c))
      valid_passwords++;
  }

  std::cout << "Part 2 : " << valid_passwords << std::endl;
}

int main(int argc, char** argv) {
  std::ifstream f_in;
  f_in.open("02.in");
  while (true) {
    Policy p;
    char tmp;
    f_in >> p.v1 >> tmp >> p.v2 >> p.c >> tmp >> p.password;
    policies.push_back(p);
    if (f_in.eof())
      break;
  }
  f_in.close();

  part1();
  part2();

  return 0;
}