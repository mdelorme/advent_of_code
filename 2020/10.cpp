#include <bits/stdc++.h>

std::vector<int> chargers;
std::map<int, int> cmap;
std::map<int, uint64_t> memo;

void part1() {
  int cur = 0;
  int n_1 = 0;
  int n_3 = 1;

  for (auto c: chargers) {
    if (cur == (c-1))
      n_1++;
    else if (cur == (c-3))
      n_3++;
    cur = c;
  }

  std::cout << "Part 1 : " << n_1*n_3 << std::endl;
}

uint64_t explore(int cur_charger) {
  // Return conditions
  if (cur_charger == chargers.size()-1)
    return 1;
  
  if (memo.find(cur_charger) != memo.end())
    return memo[cur_charger];
  

  // testing subsets
  int cur_charge = chargers[cur_charger];
  uint64_t result = 0;
  for (int i=1; i < 4; ++i) {
    if (cmap.find(cur_charge+i) != cmap.end())
      result += explore(cmap[cur_charge+i]);
  }

  memo[cur_charger] = result;
  return result;
}

void part2() {
  for (uint i=0; i < chargers.size(); ++i)
    cmap[chargers[i]] = i;

  cmap[-1] = 0;

  uint64_t result = explore(-1);
  std::cout << "Part 2 : " << result << std::endl;
}

int main(int argc, char **argv) {
  std::ifstream f_in;
  f_in.open("10.in");
  for (int charger; f_in >> charger;)
    chargers.push_back(charger);
  std::sort(chargers.begin(), chargers.end());
  f_in.close();

  part1();
  part2();

  return 0;
}