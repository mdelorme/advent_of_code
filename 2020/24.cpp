#include <bits/stdc++.h>

using Position = std::pair<int, int>;
std::set<Position> visited;
enum Direction : int8_t {E=0, W=1, NE=2, NW=3, SE=4, SW=5};
std::string dir_str[] {"E", "W", "NE", "NW", "SE", "SW"};

using MoveSet = std::vector<Direction>;
std::vector<MoveSet> input;

void print_moveset(const MoveSet &ms) {
  for (uint i=0; i < ms.size(); ++i) {
    std::cout << dir_str[ms[i]];
    if (i < ms.size()-1)
      std::cout << " -> ";
  }
  std::cout << std::endl;
}

Position move(const Position &from, Direction dir) {
  if (dir == E)
    return Position{from.first+1, from.second};
  else if (dir == W)
    return Position{from.first-1, from.second};
  else {
    // There should be a way to simplify this but ... eh
    bool odd_r = (from.second&1);
    if (odd_r) {
      switch (dir) {
        case NE: return Position{from.first+1, from.second-1}; break;
        case NW: return Position{from.first,   from.second-1}; break;
        case SW: return Position{from.first,   from.second+1}; break;
        case SE: return Position{from.first+1, from.second+1}; break;
      }
    }
    else {
      switch(dir) {
        case NE: return Position{from.first,   from.second-1}; break;
        case NW: return Position{from.first-1, from.second-1}; break;
        case SW: return Position{from.first-1, from.second+1}; break;
        case SE: return Position{from.first,   from.second+1}; break;
      }
    }
  }

  // Unknown dir ?
  return from;
}

std::set<Position> black_tiles;

std::set<Position> get_eligible_positions() {
  std::set<Position> result;
  for (auto t: black_tiles) {
    for (int d=0; d < 6; ++d) {
      Position p = move(t, static_cast<Direction>(d));
      result.insert(p);
    }
    result.insert(t);
  }
  return result;
}

int count_neighbors(Position p) {
  int res=0;
  for (int d=0; d < 6; ++d) {
    Position np = move(p, static_cast<Direction>(d));
    if (black_tiles.count(np))
      res++;
  }

  return res;
}

void part2() {
  constexpr int ndays=100;
  for (int i=0; i < ndays; ++i) {
    std::cout << "Day #" << i+1 << std::endl;
    std::set<Position> new_black_tiles;
    std::set<Position> eligible = get_eligible_positions();

    for (auto &p: eligible) {
      int n = count_neighbors(p);
      bool is_black = black_tiles.count(p);
      if (is_black && (n>0 && n<=2))
        new_black_tiles.insert(p);
      else if (!is_black && n==2)
        new_black_tiles.insert(p);
    }  

    black_tiles = new_black_tiles;
  }

  std::cout << "Part 2 : " << black_tiles.size() << std::endl;
}

void part1() {
  for (auto &ms: input) {
    Position pos{0,0};
    for (auto d: ms)
      pos = move(pos, d);

    if (black_tiles.count(pos) == 1)
      black_tiles.erase(pos);
    else
      black_tiles.insert(pos);
  }

  std::cout << "Part 1 : " << black_tiles.size() << std::endl;
}

int main(int argc, char **argv) {
  std::ifstream f_in;
  f_in.open("24.in");
  for (std::string line; f_in >> line;) {
    MoveSet ms;
    int dir = 0;
    for (auto c: line) {
      switch (c) {
        case 'n': dir  = 2; break;
        case 's': dir  = 4; break;
        case 'e': break;
        case 'w': dir += 1; break; 
      }
      if (c=='e' || c=='w') {
        ms.push_back(static_cast<Direction>(dir));
        dir = 0;
      }
    }

    input.push_back(ms);
  }
  f_in.close();

  part1();
  part2();

  return 0;
}