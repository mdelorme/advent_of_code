#include <bits/stdc++.h>
constexpr int tile_s = 10;
constexpr int grid_s = 12;
constexpr int line_s = tile_s-2;
constexpr int map_s  = grid_s * line_s;
std::string monster[] {"                  # ",
                      "#    ##    ##    ###",
                      " #  #  #  #  #  #   "};

constexpr int my=3;
constexpr int mx=20;
int monster_count = 0;

using Tileset = std::array<std::array<char, tile_s>, tile_s>;
enum Dir : int8_t {LEFT=0, RIGHT=1, TOP=2, BOTTOM=3};
Dir opp_dir[] {RIGHT, LEFT, BOTTOM, TOP};

struct Tile;
struct Grid;
struct Map;

std::vector<Tile> tiles;
using GridRow = std::array<int64_t, grid_s>;


struct Tile {
  int id = 0;
  Tileset tiles;
  int cur_state = 0;

  std::string extract_border(Dir d) const {
    std::string res = "";
    for (int i=0; i < tile_s; ++i) {
      switch(d) {
        case TOP:    res += tiles[0][i];        break;
        case BOTTOM: res += tiles[tile_s-1][i]; break;
        case LEFT:   res += tiles[i][0];        break;
        case RIGHT:  res += tiles[i][tile_s-1]; break;
      }
    }
    return res;
  }

  void flip_h() {
    Tileset new_tiles;
    for (int i=0; i < tile_s; ++i)
      for (int j=0; j < tile_s; ++j)
        new_tiles[i][j] = tiles[i][tile_s-1-j]; 
    tiles = new_tiles;
  }

  void flip_v() {
    Tileset new_tiles;
    for (int i=0; i < tile_s; ++i)
      for (int j=0; j < tile_s; ++j)
        new_tiles[i][j] = tiles[tile_s-1-i][j]; 
    tiles = new_tiles;    
  }

  void rotate() {
    Tileset new_tiles;
    for (int i=0; i < tile_s; ++i)
      for (int j=0; j < tile_s; ++j)
        new_tiles[i][j] = tiles[tile_s-1-j][i]; 
    tiles = new_tiles;   
  }

  bool matches_tile(Tile &o, Dir d) const {
    Dir opp = opp_dir[d];
    std::string b1 = extract_border(d);
    std::string b2 = o.extract_border(opp);

    return (b1 == b2);
  }

  void next_state() {
    cur_state++;
    cur_state = cur_state % 16;
    int flip_state = cur_state%4;
    switch(flip_state) {
      case 0: rotate(); break;
      case 1: flip_h(); break;
      case 2: flip_v(); break;
      case 3: flip_h(); break;
    }
  }

  void print() {
    for (auto &r: tiles) {
      for (auto c: r)
        std::cout << c;
      std::cout << std::endl;
    }
  }

  bool operator==(const Tile &o) const {
    return o.id == id;
  }
};

struct Grid {
  std::array<GridRow, grid_s> tiles;

  void shift_rows_down() {
    for (int i=grid_s-1; i > 0; --i)
      for (int j=0; j < grid_s; ++j)
        tiles[i][j] = tiles[i-1][j];
    for (int i=0; i < grid_s; ++i)
      tiles[0][i] = -1;
  }

  void shift_cols_right() {
    for (int i=grid_s-1; i > 0; --i)
      for (int j=0; j < grid_s; ++j)
        tiles[j][i] = tiles[j][i-1];
    
    for (int i=0; i < grid_s; ++i)
      tiles[i][0] = -1;
  }

  GridRow& operator[](const int i) {
    return tiles[i];
  }
  
  void print() {
    for (int i=0; i < grid_s; ++i) {
      for (int j=0; j < grid_s; ++j) {
        std::cout << std::setprecision(4) << std::setfill(' ') << tiles[i][j] << " ";
      }
      std::cout << std::endl;
    }
  }

};

Tile null_tile;
Tile get_tile(int id) {
  for (auto &t : tiles)
    if (t.id == id)
      return t;
  return null_tile;
}

Grid grid;


///////////////////////// MAP

struct Map {
  using Line = std::array<char, map_s>;
  using MapData = std::array<Line, map_s>;
  MapData map;
  int cur_state = 0;

  void next_state() {
    cur_state++;
    cur_state = cur_state % 16;
    int flip_state = cur_state%4;
    switch(flip_state) {
      case 0: rotate(); break;
      case 1: flip_h(); break;
      case 2: flip_v(); break;
      case 3: flip_h(); break;
    }
  }

  void flip_h() {
    MapData new_map;
    for (int i=0; i < map_s; ++i)
      for (int j=0; j < map_s; ++j)
        new_map[i][j] = map[i][map_s-1-j]; 
    map = new_map;
  }

  void flip_v() {
    MapData new_map;
    for (int i=0; i < map_s; ++i)
      for (int j=0; j < map_s; ++j)
        new_map[i][j] = map[map_s-1-i][j]; 
    map = new_map; 
  }

  void rotate() {
    MapData new_map;
    for (int i=0; i < map_s; ++i)
      for (int j=0; j < map_s; ++j)
        new_map[i][j] = map[map_s-1-j][i]; 
    map = new_map; 
  }

  bool is_a_monster_place(int x, int y) {
    for (int i=0; i < my; ++i) {
      for (int j=0; j < mx; ++j) {
        if (monster[i][j] == '#' && map[y+i][x+j] != '#')
          return false;
      }
    }
    return true;
  }

  void replace_monster(int x, int y) {
    for (int i=0; i < my; ++i) {
      for (int j=0; j < mx; ++j) {
        if (monster[i][j] == '#')
          map[y+i][x+j] = 'O';
      }
    }
  }

  void find_monsters() {
    print();
    for (int st=0; st < 16; ++st) {
      for (int i=0; i < map_s-my; ++i) {
        for (int j=0; j < map_s-mx; ++j) {
          if (is_a_monster_place(j, i)) {
            monster_count++;
            replace_monster(j, i);
          }
        }
      }

      if (monster_count > 0)
        break;
      next_state();
    }

    print();
    std::cout << "Monster count : " << monster_count << std::endl;
  }

  void print() {
    for (int i=0; i < map_s; ++i) {
      for (int j=0; j < map_s; ++j)
        std::cout << map[i][j];
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }

  uint count_spaces() {
    uint count = 0;
    for (int i=0; i < map_s; ++i)
      for (int j=0; j < map_s; ++j)
        count += (map[i][j] == '#');
    return count;
  }
  
  void build() {
    for (int i=0; i < grid_s; ++i) {
      for (int j=0; j < grid_s; ++j) {
        uint tid = grid[i][j];
        int base_row = i*line_s;
        int base_col = j*line_s;
        Tile t = get_tile(tid);
        for (int k=0; k < line_s; ++k)
          for (int l=0; l < line_s; ++l)
            map[base_row+k][base_col+l] = t.tiles[k+1][l+1];
      }
    }
  }
};

int find_neighbor(const Tile &t, Dir d) {
  for (auto &t2: tiles) {
    if (t.id == t2.id)
      continue;
    for (int i=0; i < 16; ++i) {
      if (t.matches_tile(t2, d))
        return t2.id;
      t2.next_state();
    }
  }

  return -1;
}


void build_grid() {
  int ntiles = tiles.size();
  Tile cur_tile = tiles[0];
  grid[0][0] = cur_tile.id;

  // Going all the way to the left
  int neigh = find_neighbor(cur_tile, LEFT);
  while (neigh != -1){
    grid.shift_cols_right();
    grid[0][0] = neigh;
    cur_tile = get_tile(neigh);
    neigh = find_neighbor(cur_tile, LEFT);
  }

  // Going all the way to the top
  neigh = find_neighbor(cur_tile, TOP);
  while (neigh != -1){
    grid.shift_rows_down();
    grid[0][0] = neigh;
    cur_tile = get_tile(neigh);
    neigh = find_neighbor(cur_tile, TOP);
  }
  
  // Now parsing every row and going right to find the neighbors
  for (int i=0; i < grid_s; ++i) {
    cur_tile = get_tile(grid[i][0]);

    // Going all the way to the right
    for (int j=0; j < grid_s-1; ++j) {
      neigh      = find_neighbor(cur_tile, RIGHT);
      grid[i][j+1] = neigh;
      cur_tile   = get_tile(neigh);
    }
    // Checking if the bottom row is filled
    if (i < grid_s-1 && grid[i+1][0] == -1) {
      cur_tile = get_tile(grid[i][0]);
      neigh = find_neighbor(cur_tile, BOTTOM);
      grid[i+1][0] = neigh;
    }
  }
  grid.print();
}

void part1() {
  build_grid();
  int L = grid_s-1;
  uint64_t res = grid[0][0] * grid[L][0] * grid[L][L] * grid[0][L];
  std::cout << "Part 1 : " << res << std::endl;
}

void part2() {
  Map map;
  map.build();
  map.find_monsters();
  std::cout << "Part 2 : " << map.count_spaces() << std::endl;
}

int main(int argc, char **argv) {
  std::ifstream f_in;
  f_in.open("20.in");
  while (!f_in.eof()) {
    std::string line;
    std::getline(f_in, line);
    line = line.substr(5);
    line = line.substr(0, line.size()-1);
    Tile t;
    t.id = std::stoi(line);

    for (int i=0; i < tile_s; ++i) {
      std::getline(f_in, line);
      for (auto j=0; j < tile_s; ++j)
        t.tiles[i][j] = line[j];
    }
    
    std::getline(f_in, line);
    tiles.push_back(t);
  }
  f_in.close();
  
  part1();
  part2();

  return 0;
}
