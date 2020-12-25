#include <bits/stdc++.h>
#include <boost/tokenizer.hpp>

struct FieldSpec {
  std::string field_name;
  std::vector<std::pair<uint, uint>> ranges;
  uint field_id;

  bool is_value_ok(uint value) {
    for (auto &[a, b]: ranges) 
      if (value >= a && value <= b)
        return true;
    return false;
  }
};
bool operator==(const FieldSpec &f1, const FieldSpec &f2) {
  return f1.field_name == f2.field_name;
}
bool operator<(const FieldSpec &f1, const FieldSpec &f2) {
  return f1.field_name < f2.field_name;
}

std::vector<FieldSpec> fields;

using Ticket = std::vector<uint>;
Ticket my_ticket;
std::vector<Ticket> tickets; 
std::vector<Ticket> valid_tickets;


void part1() {
  uint error = 0;
  for (auto &t: tickets) {
    bool is_valid = true;
    for (auto v: t) {
      bool valid = false; 
      for (auto &f: fields) {
        valid = f.is_value_ok(v);
        if (valid)
          break;
      }

      if (!valid) {
        error += v;
        is_valid = false;
      }
    }

    if (is_valid)
      valid_tickets.push_back(t);
  }

  std::cout << "Part 1 : " << error << std::endl;
}

using Set = std::set<int>;
void eliminate() {
  const uint Nf = fields.size();

  std::vector<bool> def(Nf, false);
  std::vector<std::vector<bool>> valid(Nf, def);

  // 1- Initialize
  std::vector<int> count_field;

  auto update_counts = [&]() {
    count_field.clear();
    count_field.resize(Nf, 0);

    for (uint i=0; i < Nf; ++i) {
      for (uint j=0; j < Nf; ++j) {
        if (valid[i][j])
          count_field[i]++;
      }
    }
  };

  // 2- Filling validity matrix
  for (uint fi=0; fi < Nf; ++fi) {
    FieldSpec &f = fields[fi];

    for (uint pos=0; pos < Nf; ++pos) {
      bool is_valid = true;
      for (auto t: valid_tickets) {
        if (!f.is_value_ok(t[pos])) {
          is_valid = false;
          break;
        }
      }

      valid[fi][pos] = is_valid;
    }
  }

  auto done = [&]() {
    bool res = true;
    for (auto c: count_field)
      if (c!=0)
        return false;
    
    return true;
  };

  // And iterating on solutions 
  update_counts();
  while (!done()) {
    int idf = -1;
    // Finding the field that is alone
    for (int i=0; i < Nf; ++i) {
      if (count_field[i] == 1) {
        idf = i;
        break;
      }
    }

    // Finding its position
    int pos_id;
    for (int j=0; j < Nf; ++j) {
      if (valid[idf][j]) {
        pos_id = j;
        break;
      }
    }

    // Associating then removing it from the table
    fields[idf].field_id = pos_id;

    for (int i=0; i < Nf; ++i) {
      valid[idf][i] = false;
      valid[i][pos_id] = false;
    }

    //std::cout << "Field #" << pos_id << " is " << fields[idf].field_name << std::endl;
    // And looping
    update_counts();
  }
}

void part2() {
  eliminate();

  uint64_t res = 1;
  for (auto f: fields) {
    if (f.field_name.find("departure") != std::string::npos) {
      res *= my_ticket[f.field_id];
    }
  }

  std::cout << "Part 2 : " << res << std::endl;
} 

int mode = 0;

std::regex field_re("([a-z\\ ]+):(.*)");
std::regex range_re("([0-9]+)-([0-9]+)");

void read_field_spec(std::string line) {
  std::smatch match;
  std::regex_search(line, match, field_re);
  FieldSpec field;
  field.field_name = match[1];
  std::string ranges = match[2];
  std::smatch range_match;

  while(std::regex_search(ranges, range_match, range_re)) {
    uint min_val = std::stoi(range_match[1]);
    uint max_val = std::stoi(range_match[2]);
    field.ranges.push_back(std::make_pair(min_val, max_val));
    ranges = range_match.suffix();
  }

  fields.push_back(field);
}

void read_ticket(std::string line, bool mine) {
  boost::char_separator<char> sep{","};  
  boost::tokenizer tokenizer{line, sep};
  Ticket ticket;
  for (const auto &t: tokenizer)
    ticket.push_back(std::stoi(t));

  if (mine)
    my_ticket = ticket;
  else 
    tickets.push_back(ticket);
}

int main(int argc, char **argv) {
  std::ifstream f_in;
  f_in.open("16.in");

  while (!f_in.eof()) {
    std::string line;
    std::getline(f_in, line);

    if (line == "") {
      mode++;
      if (mode > 0)
        std::getline(f_in, line);
    }
    else {
      switch(mode) {
        case 0: read_field_spec(line);    break;
        case 1: read_ticket(line, true);  break;
        case 2: read_ticket(line, false); break;
      }
    }
  }
  f_in.close();

  part1();
  part2();
  return 0;
}