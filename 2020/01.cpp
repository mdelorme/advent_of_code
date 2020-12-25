#include <bits/stdc++.h>

int part1(std::vector<int> &input) {
  size_t N = input.size();
  for (size_t i=0; i < N-1; ++i) {
    for (size_t j=i+1; j < N; ++j) {
      if (input[i]+input[j] == 2020) {
        return input[i]*input[j];
      }
    }
  }
  return -1;
}

int part2(std::vector<int> &input) {
  size_t N = input.size();
  for (size_t i=0; i < N-2; ++i) {
    int v1 = input[i];
    for (size_t j=i+1; j < N-1; ++j) {
      int v2 = input[j];
      for (size_t k=j+1; k < N; ++k) {
        int v3 = input[k];

        if (v1+v2+v3 == 2020) {
          return v1*v2*v3;
        }
      }
    }
  }  

  return -1;
}

int main(int argc, char **argv) {

  std::vector<int> input;
  std::ifstream f_in;
  f_in.open("01.in");
  for (int v; f_in >> v;)
    input.push_back(v);
  f_in.close();

  std::cout << "Part 1 : " << part1(input) << std::endl;
  std::cout << "Part 2 : " << part2(input) << std::endl;

  return 0;
}