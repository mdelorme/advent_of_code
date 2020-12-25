#include <bits/stdc++.h>
uint pkey1;
uint pkey2;
uint loop_size1;
uint loop_size2;
uint enc_key1;
uint enc_key2;

uint transform(uint subject_number, uint prev, uint loop_size=1) {
  uint64_t v = prev;
  constexpr uint64_t mod_number = 20201227;

  for (uint i=0; i < loop_size; ++i) {
    v = v*subject_number;
    v = v%mod_number;
  }

  return v;
}

uint get_loop_size(uint pkey) {
  uint loop = 0;
  uint v = 1;
  while (v != pkey) {
    loop++;
    v = transform(7, v);
  }
  return loop;
}

void part1() {
  loop_size1 = get_loop_size(pkey1);
  std::cout << "Loop size for pkey1 is : " << loop_size1 << std::endl;
  loop_size2 = get_loop_size(pkey2);
  std::cout << "Loop size for pkey2 is : " << loop_size2 << std::endl;
  enc_key1 = transform(pkey2, 1, loop_size1);
  enc_key2 = transform(pkey1, 1, loop_size2);
  std::cout << "Encryption key is : " << enc_key1 << " (should be equal to : " << enc_key2 << ")" << std::endl;
}

void part2() {

}

int main(int argc, char **argv) {
  std::ifstream f_in;
  f_in.open("25.in");
  std::string line;
  std::getline(f_in, line);
  pkey1 = std::stoi(line);
  std::getline(f_in, line);
  pkey2 = std::stoi(line);
  f_in.close();

  part1();
  part2();

  return 0;
}