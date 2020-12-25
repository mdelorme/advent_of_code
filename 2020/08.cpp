#include <bits/stdc++.h>

enum InstructionType {NOOP, ACC, JMP};
struct Instruction {
  InstructionType type;
  int value;

  Instruction(std::string type, int value) {
    if (type == "nop")
      this->type = NOOP;
    else if (type == "acc")
      this->type = ACC;
    else
      this->type = JMP;
    this->value = value;
  }
};

std::vector<Instruction> instructions;
int global;
int position;
int term_pos;

bool advance() {
  Instruction &i = instructions[position];
  switch(i.type) {
    case NOOP: 
      position++; 
      break;
    case ACC:  
      global += i.value; 
      position++;
    break;
    case JMP:  
      position += i.value;
      break;
  }

  return position == term_pos;
}

void reset() {
  position = 0;
  global   = 0;
}

bool run() {
  reset();
  std::set<int> visited;
  bool term = false;
  while (!term && visited.count(position) == 0) {
    visited.insert(position);
    term = advance();
  }

  return term;
}

void part1() {
  run();
  std::cout << "Part 1 : " << global << std::endl;
}

void part2() {
  // Mapping the positions of jmps and nops
  std::vector<int> positions;
  for (uint i=0; i < term_pos; ++i) {
    if (instructions[i].type != ACC)
      positions.push_back(i);
  }

  // Starting by jmps, easier
  for (auto pos: positions) {
    if (instructions[pos].type == JMP)
      instructions[pos].type = NOOP;
    else
      instructions[pos].type = JMP;
    
    bool term = run();
    if (term)
      break;

    // Rolling back
    if (instructions[pos].type == JMP)
      instructions[pos].type = NOOP;
    else
      instructions[pos].type = JMP;
  }

  std::cout << "Part 2 : " << global << " (" << position << ")" << std::endl;
}


int main(int argc, char **argv) {
  std::ifstream f_in;
  f_in.open("08.in");
  while (f_in.good()) {
    std::string instype;
    int value;
    char sgn;
    f_in >> instype >> value;
    Instruction ins{instype, value};
    instructions.push_back(ins);
  }
  term_pos = instructions.size();
  f_in.close();

  part1();
  part2();
  return 0;
}