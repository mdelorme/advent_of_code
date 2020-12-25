#include <bits/stdc++.h>

std::vector<std::string> passports;

bool is_valid(std::string str) {
  str = str.substr(1);
  std::istringstream iff(str);

  bool valid=true;
  try {
    for (std::string s; iff >> s;) {
      size_t pos = s.find(":");
      std::string key   = s.substr(0, pos);
      std::string value = s.substr(pos+1);

      if (key == "byr") {
        int byr = std::stoi(value);
        if (byr < 1920 || byr > 2002)
          valid = false;
      }
      else if (key == "iyr") {
        int iyr = std::stoi(value);
        if (iyr < 2010 || iyr > 2020)
          valid = false;
      }
      else if (key == "eyr") {
        int eyr = std::stoi(value);
        if (eyr < 2020 || eyr > 2030)
          valid = false;
      }
      else if (key == "hgt") {
        int height = std::stoi(value.substr(0, value.size()-2));
        std::string unit = value.substr(value.size()-2);

        if (unit == "in") {
          valid = (height >= 59 && height <= 76);
        }
        else if (unit == "cm") {
          valid = (height >= 150 && height <= 193);
        }
        else
          valid = false;
      }
      else if (key == "hcl") {
        std::regex regexp("#[[:xdigit:]]{6}");
        valid = std::regex_match(value, regexp);
      }
      else if (key == "ecl"){
        std::array<std::string, 7> valid_eyes {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
        valid = (std::find(valid_eyes.begin(), valid_eyes.end(), value) != valid_eyes.end());
      }
      else if (key == "pid") {
        std::regex regexp("[[:digit:]]{9}");
        valid = std::regex_match(value, regexp);
      }

      if (!valid)
        return false;
    }
  }
  catch(...) {
    return false;
  }

  return valid;
}

void part1() {
  uint nvalid=0;
  std::string needed_keys[] {"byr", "iyr", "eyr", "hgt", "ecl", "hcl", "pid"};
  for (auto p: passports) {
    bool valid = true;
    for (auto k: needed_keys) {
      if (p.find(k) == std::string::npos) {
        valid = false;
        break;
      } 
    }

    if (valid)
      nvalid++;
  }

  std::cout << "Part 1 : " << nvalid << std::endl;
}

void part2() {
  uint nvalid=0;
  std::string needed_keys[] {"byr", "iyr", "eyr", "hgt", "ecl", "hcl", "pid"};
  for (auto p: passports) {
    bool valid = true;
    for (auto k: needed_keys) {
      if (p.find(k) == std::string::npos) {
        valid = false;
        break;
      } 
    }

    if (valid && is_valid(p))
      nvalid++;
  }

  std::cout << "Part 2 : " << nvalid << std::endl;
}

int main(int argc, char **argv) {
  std::ifstream f_in;
  f_in.open("04.in");

  while (!f_in.eof()) {
    std::string line;
    std::getline(f_in, line);

    std::string passport_line = "";
    
    while (line != "") {
      passport_line += " " + line;
      std::getline(f_in, line);
    }

    passports.push_back(passport_line);
  }
  f_in.close();

  part1();
  part2();

  return 0;
}
