#include <bits/stdc++.h>

struct Seat {
  std::string str;
  uint row;
  uint col;
  uint id;
};

bool cmp(const Seat &s1, const Seat &s2) {
  return s1.id < s2.id;
}

std::vector<Seat> seats;

Seat dehash(std::string seat) {
  Seat out;
  out.str = seat;

  uint row_min = 0;
  uint row_max = 128;
  uint col_min = 0;
  uint col_max = 8;

  for (int i=0; i < 6; ++i) {
    uint mid = (row_min + row_max) / 2;

    if (seat[i] == 'F')
      row_max = mid;
    else
      row_min = mid;
  }

  out.row = (seat[6] == 'F' ? row_min : row_max-1);

  for (int i=0; i < 2; ++i) {
    uint mid = (col_min + col_max) / 2;

    if (seat[7+i] == 'L')
      col_max = mid;
    else
      col_min = mid;
  }

  out.col = (seat[9] == 'L' ? col_min : col_max-1);
  out.id = out.row * 8 + out.col;

  return std::move(out);
}

void part1() {
  auto ite = std::max_element(seats.begin(), seats.end(), cmp);
  std::cout << "Part 1 : " << ite->id << std::endl;
}

void part2() {
  bool map[128][8] {false};
  for (auto &s: seats)
    map[s.row][s.col] = true;
  
  bool found = false;
  Seat mine;
  for (int row=1; row < 127; ++row){
    for (int col=0; col < 8; ++col) {
      if (!map[row][col]) {
        if (map[row-1][col] && map[row+1][col]) {
          mine.col = col;
          mine.row = row;
          mine.id = row * 8 + col;
          found = true;
          break;
        }
      }
    }

    if (found)
      break;
  }

  std::cout << "Part 2 : " << mine.id << std::endl;
}

int main(int argc, char **argv) {
  std::ifstream f_in;
  f_in.open("05.in");
  for (std::string line; f_in >> line;) {
    Seat s = dehash(line);
    seats.push_back(s);
  }
  f_in.close();

  part1();
  part2();

  return 0;
}