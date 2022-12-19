#include <vector>
#include <string>
#include <sstream>

std::vector<std::string> Tokenise(std::string& string) {
  std::vector<std::string> tokens;
  std::stringstream stream(string);
  std::string token;
  while (stream >> token) {
    tokens.push_back(token);
  }
  return tokens;
}
