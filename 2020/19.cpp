#include <bits/stdc++.h>

// TODO : Check part 1 ... Should not be working, but it is working ...

std::vector<std::string> messages;
enum RuleType {SubRule, Leaf};

struct Rule;
std::map<int, Rule> rule_map;
std::set<std::string> valid;

int max_len = 0;

struct Rule {
  int id;
  std::string original_rule;
  RuleType type;
  std::vector<std::string> values;
  std::vector<int> sub_rules1;
  std::vector<int> sub_rules2;
  bool recursive = false;

  void read_sub_rule(std::string rule, int sub=1) {
    std::istringstream iss(rule);
    for (int v; iss >> v;) {
      if (v == id)
        recursive = true;
      switch(sub) {
        case 1: sub_rules1.push_back(v); break;
        case 2: sub_rules2.push_back(v); break;
      }
    }
  }

  void read_from_str(int id, std::string rule) {
    this->id = id;
    original_rule = rule;
    if (rule[0] == '"') {
      type = Leaf;
      values.push_back(rule.substr(1, 1));
    }
    else {
      type = SubRule;
      int pos = rule.find("|");
      if (pos == std::string::npos)
        read_sub_rule(rule);
      else {
        std::string rule1 = rule.substr(0, pos);
        std::string rule2 = rule.substr(pos+1);
        read_sub_rule(rule1, 1);
        read_sub_rule(rule2, 2);
      }
    }
  }

  void build_values_vector(std::vector<int> sub_rules) {
    std::vector<std::string> res;
    res.push_back("");
    for (auto sr: sub_rules) {
      std::vector<std::string> new_v;
      
      Rule &r = rule_map[sr];
      for (auto s1: res) {
        for (auto s2: r.values) {
          std::string r = s1+s2;
          if (r.size() > max_len)
            continue;

          new_v.push_back(r);
        }
      }
      
      res = new_v;
    }
    for (auto c: res)
      values.push_back(c);
  }

  void unfold() {
    if (type == Leaf)
      return;

    for (auto sr: sub_rules1) {
      if (sr == id)
        continue;

      Rule &r = rule_map[sr];
      if (r.type != Leaf)
        r.unfold();
    }
    for (auto sr: sub_rules2) {
      if (sr == id)
        continue;

      Rule &r = rule_map[sr];
      if (r.type != Leaf)
        r.unfold(); 
    }

    values.clear();
    build_values_vector(sub_rules1);
    build_values_vector(sub_rules2);

    type = Leaf;
  }
};

void part1() {
  std::cout << "Unfolding rules ... " << std::endl;
  rule_map[0].unfold();
  std::cout << "Mapping and counting ... " << std::endl;
  for (auto c: rule_map[0].values)
    valid.insert(c);


  uint64_t count = 0;
  for (auto m: messages)
    if (valid.count(m) == 1)
      count++;
  std::cout << "Part 1 : " << count << std::endl;
}

bool match(std::string expr, int n42, int n31) {
  // Termination condition
  if (expr == "")
    return (n42 > n31 && n31 > 0);

  // Matching with 42 if we haven't started the streak of 31
  if (n31 == 0) {
    for (auto &rule: rule_map[42].values) {
      // Are we matching the start of the string ?
      if (expr.starts_with(rule) && rule.size() == 8) {
        std::string nexpr = expr.substr(rule.size());
        bool res = match(nexpr, n42+1, n31);
        if (res)
          return true;
      }
    }
  }

  // If by this point we don't have at least 2 rules 42, no use to continue, we won't match
  if (n42 < 2)
    return false;

  // Same thing if we have more 31 than 42, we're done
  if (n42 <= n31)
    return false;

  // Matching with 31 now
  for (auto &rule: rule_map[31].values) {
    if (expr.starts_with(rule) && rule.size() == 8) {
      std::string nexpr = expr.substr(rule.size());
      bool res = match(nexpr, n42, n31+1);
      if (res)
        return true;
    }
  }

  return false;
}

void part2() {
  // Rule 0 is now 8 11
  // Rule 8 becomes 42 | 42 8
  //  -> This implies (42)+
  // Rule 11 becomes 42 31 | 42 11 31
  //  -> This implies (42)+(31)+
  //  -> With number of 42 == number of 31
  // In total, rule 0 is thus: 
  // (42)+(31)+
  // With |(42)+| > |(31)+| 
  std::cout << "Matching new recursive rules" << std::endl;
  uint64_t count = 0;
  for (auto m: messages) {
    if (match(m, 0, 0))
      count++;
  }
  std::cout << "Part 2 : " << count << std::endl;
}

int main(int argc, char **argv) {
  std::ifstream f_in;
  f_in.open("19.in");
  int mode = 0;
  while (!f_in.eof()) {
    std::string line;
    std::getline(f_in, line);
    if (mode == 0) {
      if (line == "")
        mode++;
      else {
        int pos = line.find(":");
        std::string rule = line.substr(pos+2);
        int id = std::stoi(line.substr(0, pos));
        Rule r;
        r.read_from_str(id, rule);
        rule_map[id] = r;
      }
    }       
    else {
      messages.push_back(line);
      int len = line.size();
      max_len = std::max(max_len, len);
    }
  }
  f_in.close();

  part1();
  part2();

  return 0;
}