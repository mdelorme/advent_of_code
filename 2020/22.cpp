#include <bits/stdc++.h>
using Deck = std::vector<char>;
Deck deck1;
Deck deck2;
using Config = std::pair<Deck, Deck>;

void iterate(Deck &d1, Deck &d2) {
  int c1 = d1.front();
  int c2 = d2.front();
  d1.erase(d1.begin());
  d2.erase(d1.begin());

  if (c1 > c2) {
    d1.push_back(c1);
    d1.push_back(c2);
  }
  else if (c2 > c1) {
    d2.push_back(c2);
    d2.push_back(c1);
  }
  else {
    d1.push_back(c1);
    d2.push_back(c2);
  }
}

void part1() {
  Deck d1(deck1);
  Deck d2(deck2);
  int ite = 0;
  while(!d1.empty() && !d2.empty()) {
    iterate(d1, d2);
    ite++;
  }

  std::cout << "Finished in " << ite << " iterations " << std::endl;

  Deck &winning = (d1.empty() ? d2 : d1);
  uint64_t count = winning.size();
  uint64_t score = 0;
  while (!winning.empty()) {
    uint64_t v = winning.front();
    score += v * count;
    count--;
    winning.erase(winning.begin());
  }

  std::cout << "Part 1 : " << score << std::endl;
}

void write_deck(Deck &d) {
  std::cout << "[ ";
  for (auto c: d)
    std::cout << (int)c << " ";
  std::cout << "]" << std::endl;
}

int lvl=0;
int recursive_game(Deck &deck1, Deck &deck2, bool first=false) {
  // result : 
  // 0 -> game won by p1
  // 1 -> game won by p2
  lvl++;
  //std::cout << "Recursive call at lvl " << lvl << " with |d1|=" << deck1.size() << "; |d2|=" << deck2.size() << std::endl;

  bool done = deck1.empty() || deck2.empty();
  std::set<Config> configs;

  int ite = 0;
  while (!done) {
    if (lvl <= 1) {
      ite++;
      std::cout << "Iteration #" << ite << " for lvl " << lvl << std::endl;
      std::cout << " - D1 = ";
      write_deck(deck1);
      std::cout << " - D2 = ";
      write_deck(deck2);
    }
    Config cfg{deck1, deck2};
    if (configs.count(cfg) == 1) {
      lvl--;
      return 0;
    }
    configs.insert(cfg);

    int c1 = deck1.front();
    int c2 = deck2.front();
    deck1.erase(deck1.begin());
    deck2.erase(deck2.begin());

    int winner;
    if (c1 <= deck1.size() && c2 <= deck2.size()) {
      Deck d1;
      Deck d2;
      for (int i=0; i < c1; ++i)
        d1.push_back(deck1[i]);
      for (int i=0; i < c2; ++i)
        d2.push_back(deck2[i]);
      winner = recursive_game(d1, d2);
    }
    else
    {
      //std::cout << "At lvl " << lvl << " solving round with standard rules " << std::endl;
      winner = (c1 >= c2 ? 0 : 1);
    }

    if (winner == 0) {
      deck1.push_back(c1);
      deck1.push_back(c2);
    }
    else if (winner) {
      deck2.push_back(c2);
      deck2.push_back(c1);
    }

    done = (deck1.empty() || deck2.empty());
  }

  lvl--;
  int winner = (deck1.empty() ? 1 : 0);
  return (deck1.empty() ? 1 : 0);
}

void part2() {
  Deck d1(deck1);
  Deck d2(deck2);

  int winner = recursive_game(d1, d2, true);

  std::cout << "Final results :" << std::endl;
  std::cout << " - D1 = ";
  write_deck(d1);
  std::cout << " - D2 = ";
  write_deck(d2);

  Deck &winning = (winner == 0 ? d1 : d2);
  uint64_t count = winning.size();
  uint64_t score = 0;
  for (uint64_t i=0; i < winning.size(); ++i)
    score += (count-i) * winning[i];

  std::cout << "Part 2 : " << score << std::endl;
}

int main(int argc, char **argv) {
  std::ifstream f_in;
  f_in.open("22.in");
  int player = 0;
  while(!f_in.eof()) {
    std::string line;
    std::getline(f_in, line);
    if (line == "")
      player++;
    else if (line.find("Player") != std::string::npos)
      continue;
    else {
      if (player == 0)
        deck1.push_back(std::stoi(line));
      else
        deck2.push_back(std::stoi(line));
    }
  }
  f_in.close();

  //part1();
  part2();

  return 0;
}