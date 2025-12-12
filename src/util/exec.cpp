#include "util/exec.hpp"
#include <array>
#include <cstdio>
#include <memory>

using namespace std;

string execCmd(const char *cmd) {
  array<char, 128> buffer{};
  string result;
  unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
  if (!pipe)
    return "";
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }
  return result;
}
