#include <bits/stdc++.h>
#include <boost/tokenizer.hpp>

std::set<std::string> allergen_list;
std::set<std::string> ingredient_list;

struct Food {
  int id;
  std::vector<std::string> ingredients;
  std::vector<std::string> allergens;

  void read_from_str(int fid, std::string ing, std::string all) {
    id = fid;
    std::istringstream iss(ing);

    // Reading ingredients
    for (std::string i; iss >> i;) {
      ingredients.push_back(i);
      ingredient_list.insert(i);
    }

    // Reading allergens
    boost::char_separator<char> sep{", ", ""};
    boost::tokenizer tokenizer{all, sep};
    for (auto t: tokenizer) {
      allergens.push_back(t);
      allergen_list.insert(t);
    }
  }

  void print() {
    std::cout << "Food item #" << id << std::endl;
    std::cout << " Ingredients : ";
    for (auto i: ingredients)
      std::cout << i << " ";
    std::cout << std::endl;
    std::cout << " Allergens : ";
    for (auto a: allergens)
      std::cout << a << " ";
    std::cout << std::endl;
  }
};
std::vector<Food> food_list;

std::map<std::string, std::vector<int>> allergen_to_food;
std::map<std::string, std::vector<int>> ingredient_to_food;
std::map<std::string, std::string> ingredient_to_allergen;

using IngredientCount = std::map<std::string, int>;
using AllergenCounter = std::map<std::string, IngredientCount>; 
AllergenCounter counter;

void build_lookup() {
  for (auto &food: food_list) {
    for (auto &a: food.allergens)
      allergen_to_food[a].push_back(food.id);
    for (auto &i: food.ingredients)
      ingredient_to_food[i].push_back(food.id);
  }
}

void deduce_allergen() {
  std::string allergen, ingredient;
  for (auto &[a, im]: counter) {
    int should_be = allergen_to_food[a].size();

    std::vector<std::string> choices;
    for (auto &[i, count]: im) {
      if (count == should_be)
        choices.push_back(i);
    }

    // Only one possible choice we're good !
    if (choices.size() == 1) {
      std::string i = choices[0];
      allergen   = a;
      ingredient = i;
      ingredient_to_allergen[i] = a;
      std::cout << " Associating " << i << " with allergen " << a << std::endl;
      counter.erase(a);
      break;
    }
  }

  // And removing ingredient from the list of other allergens
  for (auto &[a, im]: counter) {
    if (im.count(ingredient) == 1)
      im[ingredient] = 0;
  }
}

void part1() {
  build_lookup();
  // Looking for all allergens, and counting the number of times each ingredient appear in each food item
  for (auto a: allergen_list) {
    for (auto fid: allergen_to_food[a]) {
      Food &f = food_list[fid];
      for (auto i: f.ingredients) {
        if (counter[a].count(i) == 0)
          counter[a][i] = 1;
        else
          counter[a][i]++;
      } // for i
    } // for fid
  } // for a

  // We need at most |allergen_list| iterations to converge to the full association list
  std::cout << "Deducing associations : " << std::endl;
  for (int i=0; i < allergen_list.size(); ++i)
    deduce_allergen();

  // Counting the ingredients that don't have a corresponding allergen
  int no_allergens = 0;
  for (auto &f: food_list) {
    for (auto &i: f.ingredients)
      if (ingredient_to_allergen.count(i) == 0)
        no_allergens++;
  }

  std::cout << "Part 1 : " << no_allergens << std::endl;
}

void part2() {
  std::vector<std::string> ilist;
  for (auto i: ingredient_list) {
    if (ingredient_to_allergen.count(i) == 1)
      ilist.push_back(i);
  }

  auto comp = [&](const std::string i1, const std::string i2) {
    return ingredient_to_allergen[i1] < ingredient_to_allergen[i2];
  };

  std::sort(ilist.begin(), ilist.end(), comp);
  std::cout << "Part 2 : ";
  for (uint i=0; i < ilist.size(); ++i) {
    std::cout << ilist[i];
    if (i < ilist.size()-1)
      std::cout << ",";
  }
  std::cout << std::endl;
}

int main(int argc, char **argv) {
  std::ifstream f_in;
  f_in.open("21.in");
  uint fid=0;
  while (!f_in.eof()) {
    std::string line;
    std::getline(f_in, line);
    int pos = line.find("(");
    std::string ingredients = line.substr(0, pos);
    std::string allergens = line.substr(pos+10);
    allergens.pop_back();

    Food f;
    f.read_from_str(fid, ingredients, allergens);
    food_list.push_back(f);
    fid++;
  }
  f_in.close();

  part1();
  part2();

  return 0;
}