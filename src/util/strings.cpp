#include "util/strings.hpp"
#include "fstream"
#include <regex>

using namespace std;

bool invalid(const string &s) {
  if (s.empty())
    return true;
  if (s == "To Be Filled By O.E.M." || s == "To be filled by O.E.M.")
    return true;
  return false;
}

string trim(const string &s) {
  size_t start = s.find_first_not_of(" \t\n\r");
  size_t end = s.find_last_not_of(" \t\n\r");
  if (start == string::npos)
    return "";
  return s.substr(start, end - start + 1);
}

string readTrim(const string &path) {
  ifstream f(path);
  if (!f.is_open())
    return "";
  string s;
  getline(f, s);
  return s;
}

std::string stripAnsi(const std::string &s) {
  static const std::regex ansi(R"(\x1B\[[0-9;]*[A-Za-z])");
  return std::regex_replace(s, ansi, "");
}

size_t visibleLength(const std::string &s) { return stripAnsi(s).length(); }

string ansiFgHex(const string &hex) {
  int r = stoi(hex.substr(1, 2), nullptr, 16);
  int g = stoi(hex.substr(3, 2), nullptr, 16);
  int b = stoi(hex.substr(5, 2), nullptr, 16);
  return "\033[38;2;" + to_string(r) + ";" + to_string(g) + ";" + to_string(b) +
         "m";
}