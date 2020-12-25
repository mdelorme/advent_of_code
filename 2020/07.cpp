#include <bits/stdc++.h>

using Bag     = std::string;
using Storage = std::pair<Bag, int>;

std::map<Bag, std::vector<Storage>> bags; 
Bag readBag(std::istream &is) {
  Bag b;
  std::string type, color;
  is >> type >> color;
  b = type + " " + color; 
  return std::move(b);
}

bool findShiny(std::string bag) {
  if (bag == "shiny gold")
    return true;

  for (auto [b, n]: bags[bag]) {
    if (findShiny(b))
      return true;
  }

  return false;
}

uint countSubBags(std::string bag) {
  uint tot = 0;
  for (auto [b, n] : bags[bag])
    tot += n * (countSubBags(b) + 1);
  
  return tot;
}

void part1() {
  uint tot = 0;
  for (auto [b, v]: bags) {
    if (b != "shiny gold")
      tot += findShiny(b);
  }

  std::cout << "Part 1 : " << tot << std::endl;
}

void part2() {
  uint tot = countSubBags("shiny gold");
  std::cout << "Part 2 : " << tot << std::endl;
}

int main(int argc, char **argv) {
  std::ifstream f_in;
  f_in.open("07.in");
  while (true) {
    Bag bag = readBag(f_in);
    if (!f_in.good())
      break; 

    std::string tmp;
    f_in >> tmp >> tmp; // bag contains
    std::string line;
    std::getline(f_in, line);
    std::istringstream iss(line);
    while (true) {
      int qty;
      Bag b;
      std::string first, type, color, term;
      iss >> first;
      b = readBag(iss);
      if (!iss.good())
        break; 
      iss >> term;
      qty = std::stoi(first);
      bags[bag].push_back(std::make_pair(b, qty));
      if (bags.find(b) == bags.end()) {
        bags[b].resize(0); // Making sure the name appears in the map keys
      }

      if (term.back() == '.')
        break;
    }
  }
  f_in.close();

  part1();
  part2();

  return 0;
}