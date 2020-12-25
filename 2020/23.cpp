#include <bits/stdc++.h>

std::string input;

struct Cups {
  std::vector<int> next;
  int cur;
  int max;

  Cups(uint size, std::string input) {
    max=0;
    int last=-1;
    int first;

    next.resize(size);
    
    for (auto c: input) {
      int v = c-'0';
      if (v > max)
        max = v;
      if (last != -1)
        next[last] = v;
      else
        first = v;
      
      last = v;
    }
    for (int i=max+1; i<=size; ++i) {
      next[last] = i;
      last = i;
    }
    next[last] = first;
    max = std::max(max, last);
    cur = first;
    std::cout << "First is " << first << std::endl;
    std::cout << "Max is " << max << std::endl;
  }

  void print(int base) {
    int v = base;
    std::cout << v << " ";
    while (next[v] != base) {
      std::cout << next[v] << " ";
      v = next[v];
    }
    std::cout << std::endl;
  }

  int get_next(int cur, int picked[]) {
    if (cur == 0)
      cur = max;
    while (picked[0] == cur || picked[1] == cur || picked[2] == cur) {
      cur--;
      if (cur < 1)
        cur = max;
    }
    return cur;
  }

  void solve(uint iterations, int part) {
    for (int i=0; i < iterations; ++i) {
      int picked[3];

      // Picking up the three next values
      picked[0] = next[cur];
      picked[1] = next[picked[0]];
      picked[2] = next[picked[1]];

      // Linking current with the following value
      next[cur] = next[picked[2]];
      
      // Computing the next position
      int next_p = get_next(cur-1, picked);
      int next_save = next[next_p];
      next[next_p] = picked[0];
      next[picked[2]] = next_save;

      // Getting the next current value
      cur = next[cur];
    }

    std::cout << "All good !" << std::endl;

    if (part == 1) {
      std::ostringstream oss;
      int cur = 1;
      while (next[cur] != 1) {
        //std::cout << cur << " ";
        //std::flush(std::cout);
        oss << next[cur];
        cur = next[cur];
      }
      std::cout << "Part 1 : " << oss.str() << std::endl;
    }
    else {
      uint64_t res = next[1];
      res *= next[res];
      std::cout << "Part 2 : " << res << std::endl;
    }
  }

};

void part2() {
  Cups cups{1000000, input};
  cups.solve(10000000, 2);
}

void part1() {
  uint size = input.size();
  Cups cups{size, input};
  cups.solve(100, 1);
}

int main(int argc, char **argv) {
  std::ifstream f_in;
  f_in.open("23.in");
  std::getline(f_in, input);
  f_in.close();

  part1();
  part2();
  return 0;
}