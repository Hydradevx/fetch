#include "util/config.hpp"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>

using namespace std;
namespace fs = std::filesystem;

static const char *DEFAULT_CONFIG = R"(show_gpu: true
show_cpu: true
show_ram: true
show_used_ram: true
show_available_ram: true
show_packages: true

color_primary: "#8aadf4"
color_secondary: "#c6d0f5"

padding_left: 2
padding_right: 2
logo: ubuntu
)";

string Config::getConfigPath() const {
  const char *home = getenv("HOME");
  if (!home)
    return "";
  return string(home) + "/.config/hyprfetch/config.yaml";
}

bool Config::exists() const { return fs::exists(getConfigPath()); }

bool Config::load() {
  string path = getConfigPath();
  fs::path cfgPath(path);

  // Create default config if missing
  if (!fs::exists(cfgPath)) {
    try {
      fs::create_directories(cfgPath.parent_path());

      ofstream out(path);
      out << DEFAULT_CONFIG;
      out.close();

      cout << "Created default config at " << path << endl;
    } catch (const fs::filesystem_error &e) {
      cerr << "Config creation failed: " << e.what() << endl;
      return false;
    }
  }

  // Load config
  try {
    root = YAML::LoadFile(path);
  } catch (const YAML::Exception &e) {
    cerr << "YAML parse error: " << e.what() << endl;
    return false;
  }

  return true;
}

bool Config::getBool(const string &key, bool fallback) const {
  if (!root[key])
    return fallback;
  return root[key].as<bool>();
}

string Config::getString(const string &key, const string &fallback) const {
  if (!root[key])
    return fallback;
  return root[key].as<string>();
}

int Config::getInt(const string &key, int fallback) const {
  if (!root[key])
    return fallback;
  return root[key].as<int>();
}

Config config;