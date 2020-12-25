#include <bits/stdc++.h>

using Grid = std::vector<std::string>;
Grid grid;
using Position = std::pair<int, int>;
std::vector<Position> seats;

int H, W;

enum CountMode {MODE_ADJACENT, MODE_RAYTRACING};

void print_grid(const Grid &g) {
  for (auto line: g)
    std::cout << line << std::endl;
}

void find_seats() {
  for (int y=0; y < H; ++y)
    for (int x=0; x < W; ++x)
      if (grid[y][x] == 'L')
        seats.push_back(std::make_pair(x, y));
}

template<CountMode mode>
int count_neighbours(const Grid &g, Position p) {
  int res = 0;

  if (mode == MODE_ADJACENT) {
    for (int dx=-1; dx < 2; ++dx) {
      for (int dy=-1; dy < 2; ++dy) {
        if (dx==0 && dy == 0)
          continue;
        
        int x = p.first + dx;
        int y = p.second + dy;
        if (x < 0 || x > W-1 || y < 0 || y > H-1)
          continue;
        res += (g[y][x] == '#');
      }
    }
  }
  else if (mode == MODE_RAYTRACING) {
    int x0 = p.first;
    int y0 = p.second;

    int dxt[8] {0, 1, 1, 1, 0, -1, -1, -1};
    int dyt[8] {1, 1, 0, -1, -1, -1, 0, 1};

    for (int dir=0; dir < 8; ++dir){
      int dx = dxt[dir];
      int dy = dyt[dir];
      int x = x0;
      int y = y0;
      
      while (true) {
        x += dx;
        y += dy;

        if (x < 0 || x > W-1 || y < 0 || y > H-1)
          break;
        
        if (g[y][x] == 'L')
          break;
        if (g[y][x] == '#') {
          res++;
          break;
        }
      }
    }
  }

  return res;
}

template<CountMode mode>
int iterate(Grid &g) {
  Grid ng(g);
  int changes = 0;

  for (auto p: seats) {
    int x = p.first;
    int y = p.second;

    bool occ = g[y][x] == '#';
    int nn = count_neighbours<mode>(g, p);

    int occ_limit = (mode == MODE_ADJACENT ? 4 : 5);

    if (!occ && nn==0) {
      ng[y][x] = '#';
      changes++;
    }
    else if (occ && nn >= occ_limit) {
      ng[y][x] = 'L';
      changes++;
    }
  }

  g = ng;
  return changes;
}

void part1() {
  Grid g(grid);
  while (iterate<MODE_ADJACENT>(g) != 0);

  int nocc = 0;
  for (auto p: seats) {
    int x = p.first;
    int y = p.second;

    nocc += (g[y][x] == '#');
  }

  std::cout << "Part 1 : " << nocc << std::endl;
}

void part2() {
  Grid g(grid);
  while (iterate<MODE_RAYTRACING>(g) != 0);

  print_grid(g);

  int nocc = 0;
  for (auto p: seats) {
    int x = p.first;
    int y = p.second;

    nocc += (g[y][x] == '#');
  }

  std::cout << "Part 2 : " << nocc << std::endl;
}

int main(int argc, char **argv) {
  std::ifstream f_in;
  f_in.open("11.in");
  int cur_line = 0;
  for (std::string line; f_in >> line;)
    grid.push_back(line);

  H = grid.size();
  W = grid[0].size();
  f_in.close();

  find_seats();

  part1();
  part2();

  return 0;
}