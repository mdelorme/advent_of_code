#include <bits/stdc++.h>

using Form  = std::set<char>;
using Group = std::vector<Form>;
std::vector<Group> groups;

void part1() {
  uint total = 0;
  for (auto &g: groups) {
    Form any_yes;
    for (auto &f: g)
      any_yes.insert(f.begin(), f.end());
    total += any_yes.size();
  }
  std::cout << total << std::endl;
}

void part2() {
  uint total = 0;
  for (auto &g: groups) {
    std::vector<char> all_yes;
    std::string lower = "abcdefghjiklmnopqrstuvwxyz";

    for (auto c : lower) {
      bool ok = true;
      for (auto &f: g) {
        if (f.find(c) == f.end()) {
          ok = false;
          break;
        }
      }

      if (ok)
        all_yes.push_back(c);
    }

    total += all_yes.size();
  }
  std::cout << total << std::endl;
}

int main(int argc, char **argv) {
  std::ifstream f_in;
  f_in.open("06.in");

  Group g;
  while (!f_in.eof()) {
    std::string line;
    std::getline(f_in, line);

    if (line == "") {
      groups.push_back(g);
      g = Group();
    }
    else {
      Form f;
      for (auto c: line)
        f.insert(c);
      g.push_back(f);
    }
  }

  f_in.close();

  part1();
  part2();

  return 0;
}