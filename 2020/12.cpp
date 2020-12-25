#include <bits/stdc++.h>

struct Instruction {
  char action;
  int count;
};

std::vector<Instruction> instructions;

int dx[4] {1, 0, -1,  0};
int dy[4] {0, 1,  0, -1};

void part1() {
  int x = 0;
  int y = 0;
  int d = 0;

  for (auto &i: instructions) {
    switch (i.action) {
      case 'E': x += i.count; break;
      case 'W': x -= i.count; break;
      case 'N': y += i.count; break;
      case 'S': y -= i.count; break;
      case 'L': d = (d + i.count / 90) % 4; break;
      case 'R': d = (d - i.count / 90 + 4) % 4; break;
      case 'F':
       x += dx[d] * i.count;
       y += dy[d] * i.count;
       break;
    }
  }

  int dist = abs(x) + abs(y);
  std::cout << "Part 1 : " << dist << std::endl;
}

void part2() {
  int wpx = 10;
  int wpy = 1;
  int x = 0;
  int y = 0;

  auto move_waypoint = [&] (const int x, const int y) {
    wpx += x;
    wpy += y;
  };

  auto move_ship = [&] (const int count) {
    int dx = wpx - x;
    int dy = wpy - y;

    x += dx * count;
    y += dy * count;
    wpx = x + dx;
    wpy = y + dy;
  };

  auto rotate_waypoint = [&] (const int angle) {
    int a = angle;
    if (a < 0)
      a += 360;

    int nwpx, nwpy;
    wpx -= x;
    wpy -= y;
    while (a > 0) {
      nwpx = -wpy;
      nwpy =  wpx;
      a -= 90;
      wpx = nwpx;
      wpy = nwpy;
    }

    wpx += x;
    wpy += y;
  };

  for (auto &i: instructions) {
    switch(i.action) {
      case 'E': move_waypoint(i.count, 0);  break;
      case 'W': move_waypoint(-i.count, 0); break;
      case 'N': move_waypoint(0, i.count);  break;
      case 'S': move_waypoint(0, -i.count); break;
      case 'F': move_ship(i.count); break;
      case 'L': rotate_waypoint(i.count); break;
      case 'R': rotate_waypoint(-i.count); break;
    }
  }

  int dist = abs(x) + abs(y);
  std::cout << "Part 2 : " << dist << std::endl;
}

int main(int argc, char **argv) {
  std::ifstream f_in;
  f_in.open("12.in");
  while (!f_in.eof()) {
    Instruction ins;
    f_in >> ins.action >> ins.count;
    instructions.push_back(ins);
  }
  f_in.close();

  part1();
  part2();
  return 0;
}