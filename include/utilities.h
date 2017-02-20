#ifndef UTILITIES_H
#define UTILITIES_H

#include <algorithm>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

namespace utilities {

std::vector<std::string> split(const std::string& s, char delim) {
  std::vector<std::string> result;
  std::stringstream ss;
  ss.str(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    result.push_back(item);
  }
  return result;
}

bool matching(std::string matchingKey, std::string primaryKey) {
  std::vector<std::string> primary = utilities::split(primaryKey, '.');
  std::vector<std::string> matching = utilities::split(matchingKey, '.');
  if (primary.size() == matching.size()) {  // same size
    for (int i = 0; i < primary.size(); i++) {
      if (primary[i] != matching[i]) {  // primary nd matching do not match
        if (primary[i] != "*" && primary[i] != "#") {  // not equal * or #
          return false;
        }
      }
    }
  } else {  // not same size
    int current_matching = 0;
    if (std::find(primary.begin(), primary.end(), "#") != primary.end()) {
      for (int i = 0; i < primary.size(); i++) {
        if (primary[i] != matching[current_matching]) {
          if (primary[i] == "*") {
            current_matching++;
            if (i == primary.size() - 1) {
              return true;
            }
          }
          if (primary[i] == "#") {          // primary is #
            if (i == primary.size() - 1) {  //# is on last place
              return true;
            }
            if (primary[i + 1] == "*") {
              i++;
              current_matching += 2;
            }
            auto it = std::find(matching.begin() + current_matching,
                                matching.end(), primary[i + 1]);
            if (it != matching.end()) {
              current_matching = it - (matching.begin() + current_matching + 1);
              i++;
            } else {
              return false;
            }
          } else {  // are different, not #
            return false;
          }
        } else {
          current_matching++;
        }
      }
    } else {  //# does not exists
      return false;
    }
  }
  return true;
}
}

#endif  // UTILITIES_H
