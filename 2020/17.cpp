#include <bits/stdc++.h>
using Row   = std::vector<int>;
using Plane = std::vector<Row>;
using Grid  = std::vector<Plane>;

struct HyperGrid {
  std::vector<Grid> data;
  uint Nx, Ny, Nz, Nw;

  HyperGrid() {
    Nx = 0;
    Ny = 0;
    Nz = 0;
    Nw = 0;
  }

  void resize_x(int nx) {
    for (auto &g: data)  // w
      for (auto &p: g)   // z
        for (auto &r: p) // y
          r.resize(nx);  // x
    Nx = nx;
  }

  void resize_y(int ny) {
    for (auto &g: data) // w
      for (auto &p: g)  // z
        p.resize(ny);   // y
    resize_x(Nx);       // x
    Ny = ny;
  }

  void resize_z(int nz) {
    for (auto &g: data) // w
      g.resize(nz);     // z
    resize_y(Ny);       // x-y
    Nz = nz;
  }

  void resize_w(int nw) {
    data.resize(nw); // w
    resize_z(Nz);    // x-y-z
    Nw = nw;
  }

  int& operator()(const int x, const int y, const int z, const int w=0) {
    if (w >= Nw)
      resize_w(w+1);
    if (z >= Nz)
      resize_z(z+1);
    if (y >= Ny)
      resize_y(y+1);
    if (x >= Nx)
      resize_x(x+1);

    return data[w][z][y][x];
  }

  void allocate(uint Nx, uint Ny, uint Nz, uint Nw=1) {
    Row   r(Nx, 0);
    Plane p(Ny, r);
    Grid  g(Nz, p);
    data.clear();
    data.resize(Nw, g);
    this->Nx = Nx;
    this->Ny = Ny;
    this->Nz = Nz;
    this->Nw = Nw;
  }

  void reset() {
    data.clear();
    Nx = 0;
    Ny = 0;
    Nz = 0;
    Nw = 0;
  }

  uint64_t count() {
    uint64_t result=0;
    for (auto g: data)
      for (auto p: g)
        for (auto r: p)
          for (auto c: r)
            result += c;

    return result;
  }

  uint count_neighbors(int x, int y, int z, int w=0) {
    // Can be optimized by "continue-ing" when nz, ny, nx are out of bounds.
    uint count = 0;
    for (int dw=-1; dw < 2; ++dw) {
      int nw = w + dw;
      for (int dz=-1; dz < 2; ++dz) {
        int nz = z + dz;
        for (int dy=-1; dy < 2; ++dy) {
          int ny = y + dy;
          for (int dx=-1; dx < 2; ++dx) {
            int nx = x + dx;
            if (dx == 0 && dy == 0 && dz == 0 && dw == 0)
              continue;
            count += is_active(nx, ny, nz, nw);
          }
        }
      }
    }

    return count;
  }

  bool is_active(int x, int y, int z, int w=0) {
    if (x < 0 || x >= Nx || y < 0 || y >= Ny || z < 0 || z >= Nz || w < 0 || w >= Nw)
      return false;

    return data[w][z][y][x] == 1;
  }
};

HyperGrid grid_3d, grid_4d;

void iterate_3d() {
  HyperGrid new_grid;
  uint Nx = grid_3d.Nx;
  uint Ny = grid_3d.Ny;
  uint Nz = grid_3d.Nz;

  new_grid.allocate(Nx+2, Ny+2, Nz+2);
  for (uint z=0; z < Nz+2; ++z) {
    for (uint y=0; y < Ny+2; ++y) {
      for (uint x=0; x < Nx+2; ++x) {
        uint ncount = grid_3d.count_neighbors(x-1, y-1, z-1);
        bool active = grid_3d.is_active(x-1, y-1, z-1);
        int new_value = 0;

        if (active && (ncount == 2 || ncount == 3))
          new_value = 1;
        else if (!active && ncount == 3)
          new_value = 1;
        
        new_grid(x, y, z) = new_value;
      }
    }
  }

  grid_3d = std::move(new_grid);
}

void iterate_4d() {
  HyperGrid new_grid;
  uint Nx = grid_4d.Nx;
  uint Ny = grid_4d.Ny;
  uint Nz = grid_4d.Nz;
  uint Nw = grid_4d.Nw;

  new_grid.allocate(Nx+2, Ny+2, Nz+2, Nw+2);
  for (uint w=0; w < Nw+2; ++w) {
    for (uint z=0; z < Nz+2; ++z) {
      for (uint y=0; y < Ny+2; ++y) {
        for (uint x=0; x < Nx+2; ++x) {
          uint ncount = grid_4d.count_neighbors(x-1, y-1, z-1, w-1);
          bool active = grid_4d.is_active(x-1, y-1, z-1, w-1);
          int new_value = 0;

          if (active && (ncount == 2 || ncount == 3))
            new_value = 1;
          else if (!active && ncount == 3)
            new_value = 1;
          
          new_grid(x, y, z, w) = new_value;
        }
      }
    }
  }

  grid_4d = std::move(new_grid);
}

void part1() {
  for (int i=0; i < 6; ++i)
    iterate_3d();
  
  uint count = grid_3d.count();
  std::cout << "Part 1 : " << count << std::endl;
}

void part2() {
  for (int i=0; i < 6; ++i)
    iterate_4d();
  
  uint64_t count = grid_4d.count();
  std::cout << "Part 2 : " << count << std::endl;

}

int main(int argc, char **argv) {
  std::ifstream f_in;
  f_in.open("17.in");
  bool first = true;
  Plane p;
  int y = 0;
  while (!f_in.eof()) {
    std::string line;
    std::getline(f_in, line);
    int x = 0;
    for (auto c: line) {
      grid_3d(x, y, 0) = (c == '#' ? 1 : 0);
      grid_4d(x, y, 0, 0) = (c == '#' ? 1 : 0);
      x++;
    }
    y++;
  }
  f_in.close();

  part1();
  part2();

  return 0;
}