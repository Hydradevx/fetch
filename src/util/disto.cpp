#include "util/config.hpp"
#include "util/distro.hpp"
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

string getLogoPath(const string &name) {
  const char *home = getenv("HOME");
  if (!home)
    return "";

  return string(home) + "/.config/hyprfetch/logos/" + name + ".txt";
}

string stripQuotes(string s) {
  if (!s.empty() && s.front() == '"')
    s = s.substr(1, s.size() - 2);
  return s;
}

string getDistroId() {
  ifstream file("/etc/os-release");
  string line;

  while (getline(file, line)) {
    if (line.rfind("ID=", 0) == 0) {
      return stripQuotes(line.substr(3));
    }
  }
  return "linux";
}

string getDistroIdLike() {
  ifstream file("/etc/os-release");
  string line;

  while (getline(file, line)) {
    if (line.rfind("ID_LIKE=", 0) == 0) {
      string val = stripQuotes(line.substr(8));
      return val.substr(0, val.find(' '));
    }
  }
  return "";
}

vector<string> getDistroLogo() {
  string id = config.getString("general.logo", "auto");

  if (id == "auto") {
    id = getDistroId();
  } else {
    id = stripQuotes(id);
  }
  fstream logoFile(getLogoPath(id));

  if (!logoFile.is_open()) {
    string distroLike = getDistroIdLike();
    logoFile.open(getLogoPath(distroLike));
    if (!logoFile.is_open()) {
      logoFile.open(getLogoPath("default"));
    }
  }

  vector<string> lines;
  string line;
  while (getline(logoFile, line)) {
    lines.push_back(line);
  }

  if (lines.empty()) {
    lines.push_back("  Love from Hydra \n  ");
  }
  return lines;
}