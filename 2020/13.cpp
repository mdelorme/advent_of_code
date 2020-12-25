#include <bits/stdc++.h>
#include <boost/tokenizer.hpp>

int64_t departure_time;
std::vector<int64_t> buses;
std::vector<std::pair<int64_t, int64_t>> buses_ids;

struct Euclid {
  int64_t mi, mj;
};

int64_t next_time(const int64_t bus, int64_t departure_time) {
  return bus * (departure_time / bus + 1) % departure_time;
}

void part1() {
  auto comp = [&] (const int64_t b1, const int64_t b2) {
    return next_time(b1, departure_time) < next_time(b2, departure_time);
  };
  int64_t best_bus = *(std::min_element(buses.begin(), buses.end(), comp));
  int64_t time = next_time(best_bus, departure_time);
  int64_t res = best_bus * time;

  std::cout << "Part 1 : " << res << std::endl;
}

int64_t get_modulo(int64_t ni, int64_t bi) {
  int64_t xi = (ni - bi);
  while (xi < 0)
    xi += ni;
  return xi % ni;
}

void part2() {
  int64_t res = 0;

  bool first = true;
  int64_t xi, ni;

  // Init 
  auto cur = buses_ids.begin();
  ni = cur->second;
  xi = get_modulo(ni, cur->first);
  auto next = std::next(cur);

  // Chinese remainder theorem !
  while (next != buses_ids.end()) {
    int64_t nip1 = next->second;
    int64_t ai = get_modulo(nip1, next->first);
    std::cout << xi << " " << nip1 << " " << ai << std::endl;
    while (xi % nip1 != ai)
      xi += ni;

    ni *= nip1; 

    cur = next;
    std::advance(next, 1);
  }
  
  std::cout << "Part 2 : " << xi << std::endl;
}


int main(int argc, char **argv) {
  std::ifstream f_in;
  f_in.open("13.in");
  std::string line;
  std::getline(f_in, line);
  departure_time = std::stoi(line);
  std::getline(f_in, line);
  f_in.close();
  
  boost::char_separator<char> sep{",", ""};  
  boost::tokenizer tokenizer{line, sep};

  int tok = -1;
  for (const auto &t: tokenizer) {
    tok++;
    if (t == "x")
      continue;
    int64_t bus_id = std::stoi(t);
    buses.push_back(bus_id);
    buses_ids.push_back(std::make_pair(tok, bus_id));
    std::cout << tok << " " << bus_id << std::endl;
  }

  part1();
  part2();

  return 0;
}