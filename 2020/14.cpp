#include <bits/stdc++.h>

using BitSet = std::bitset<36>;

struct Instruction {
  std::string mask;
  uint64_t address;
  uint64_t value;
  bool is_mask;
};

std::vector<Instruction> instructions;
std::map<uint64_t, BitSet> memory_p1;
std::map<uint64_t, uint64_t> memory_p2;

void apply_value(uint64_t address, uint64_t value, std::string cur_mask) {
  BitSet b(value);
  for (int i=0; i < 36; ++i) {
    if (cur_mask[35-i] == '0')
      b[i] = false;
    else if (cur_mask[35-i] == '1')
      b[i] = true;
  }

  memory_p1[address] = b;
}

void part1() {
  std::string cur_mask;
  memory_p1.clear();
  for (auto &i: instructions) {
    if (!i.is_mask) {
      apply_value(i.address, i.value, cur_mask);
    }
    else
      cur_mask = i.mask;
  }

  uint64_t sum = 0;
  for (auto &[a, v] : memory_p1)
    sum += v.to_ullong();
  
  std::cout << "Part 1 : " << sum << std::endl;
}

uint64_t tot_count = 0;
uint64_t sum_memory(std::string value, int pos) {
  if (pos == 36) {
    BitSet b(value);
    tot_count++;
    return b.to_ullong();
  }

  if (value[pos] == 'X') {
    std::string v1 = value;
    std::string v2 = value;
    v1[pos] = '0';
    v2[pos] = '1';
    return sum_memory(v1, pos+1) + sum_memory(v2, pos+1);
  }
  uint64_t res = sum_memory(value, pos+1);
  return res;
}

std::string get_address_mask(uint address, std::string mask) {
  BitSet b(address);
  std::string res = b.to_string();
  for (int i=0; i < 36; ++i) {
    if (mask[i] != '0')
      res[i] = mask[i];
  }

  return res;
}

void parse_addresses(std::vector<uint64_t> &addresses, std::string cur_addr, int pos) {
  if (pos == 36) {
    BitSet b(cur_addr);
    addresses.push_back(b.to_ullong());
    return;
  }

  if (cur_addr[pos] == 'X') {
    cur_addr[pos] = '0';
    parse_addresses(addresses, cur_addr, pos+1);
    cur_addr[pos] = '1';
    parse_addresses(addresses, cur_addr, pos+1);
  }
  else
    parse_addresses(addresses, cur_addr, pos+1);
}

void part2() {
  std::string cur_mask;
  memory_p2.clear();
  for (auto &i: instructions) {
    if (!i.is_mask) {
      std::string addr_mask = get_address_mask(i.address, cur_mask);
      std::vector<uint64_t> addresses;
      parse_addresses(addresses, addr_mask, 0);
      for (auto addr: addresses)
        memory_p2[addr] = i.value;
    }
    else
      cur_mask = i.mask;
  }

  // Enumerating memory
  uint64_t sum = 0;
  for (auto &[a, v]: memory_p2) {
    sum += v;
  }

  std::cout << "Part 2 : " << sum << std::endl;
}

int main(int argc, char **argv) {
  std::ifstream f_in;
  f_in.open("14.in");
  std::regex memory_re("mem\\[([0-9]+)\\] = ([0-9]+)");
  while (f_in.good()) {
    std::string line;
    Instruction ins;
    std::getline(f_in, line);
    
    std::smatch match;
    std::regex_match(line, match, memory_re);

    if (match.empty()) {
      if (line == "")
        break;
      ins.is_mask = true;
      ins.mask    = line.substr(7, 36);
    }
    else {
      ins.address = std::stoi(match[1]);
      ins.value   = std::stoi(match[2]);
      ins.is_mask = false;
    }

    instructions.push_back(ins);
  }
  f_in.close();

  part1();
  part2();

  return 0;
}