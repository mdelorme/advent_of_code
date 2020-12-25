#include <bits/stdc++.h>

std::vector<std::string> map;

int travel(int dx, int dy) {
  int x, y;
  int N = map.size();
  int L = map[0].size();
  x = 0;
  y = 0;

  int ntrees = 0;
  while (y < N) {
    if (map[y][x] == '#')
      ntrees++;

    y += dy;
    x = (x+dx)%L;
  }

  return ntrees;
}

void part1() {
  std::cout << "Part 1 : " << travel(3, 1) << std::endl;
}

void part2() {
  int dx[5] {1, 3, 5, 7, 1};
  int dy[5] {1, 1, 1, 1, 2};

  uint32_t ntrees = 1;
  for (int i=0; i < 5; ++i)
    ntrees *= travel(dx[i], dy[i]);

  std::cout << "Part 2 : " << ntrees << std::endl;
}

int main(int argc, char **argv) {
  std::ifstream f_in;
  f_in.open("03.in");
  for (std::string s; f_in >> s;)
    map.push_back(s);

  part1();
  part2();

  return 0;
}
