#include "util/config.hpp"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>

using namespace std;
namespace fs = std::filesystem;
using json = nlohmann::json;

static const char *DEFAULT_CONFIG = R"(
{
  "general": {
    "logo": "auto",
    "padding_left": 2,
    "padding_right": 2
  },
  "theme": {
    "color_primary": "#8aadf4",
    "color_secondary": "#c6d0f5"
  },
  "modules": [
    "os",
    "cpu",
    "gpu",
    "ram",
    "packages"
  ]
}
)";

string Config::getConfigPath() const {
  const char *home = getenv("HOME");
  return home ? string(home) + "/.config/hyprfetch/config.json" : "";
}

bool Config::exists() const { return fs::exists(getConfigPath()); }

bool Config::load() {
  string path = getConfigPath();
  fs::create_directories(fs::path(path).parent_path());

  if (!fs::exists(path)) {
    ofstream out(path);
    out << DEFAULT_CONFIG;
    out.close();
    cout << "Created default config at " << path << endl;
  }

  try {
    ifstream in(path);
    in >> root;
  } catch (const std::exception &e) {
    cerr << "Config parse error: " << e.what() << endl;
    return false;
  }

  return true;
}

const json *Config::resolvePath(const string &path) const {
  const json *current = &root;
  size_t start = 0;

  while (true) {
    size_t dot = path.find('.', start);
    string key = path.substr(start, dot - start);

    if (!current->contains(key))
      return nullptr;

    current = &(*current)[key];

    if (dot == string::npos)
      break;
    start = dot + 1;
  }
  return current;
}

string Config::getString(const string &path, const string &fallback) const {
  const json *node = resolvePath(path);
  return (node && node->is_string()) ? node->get<string>() : fallback;
}

int Config::getInt(const string &path, int fallback) const {
  const json *node = resolvePath(path);
  return (node && node->is_number_integer()) ? node->get<int>() : fallback;
}

vector<string> Config::getModules() const {
  vector<string> mods;
  if (!root.contains("modules") || !root["modules"].is_array())
    return mods;

  for (const auto &m : root["modules"])
    mods.push_back(m.get<string>());

  return mods;
}

Config config;