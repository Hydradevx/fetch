#pragma once
#include <string>
#include <yaml-cpp/yaml.h>

class Config {
public:
  bool load();
  bool exists() const;

  // accessors
  bool getBool(const std::string &key, bool fallback) const;
  std::string getString(const std::string &key,
                        const std::string &fallback) const;
  int getInt(const std::string &key, int fallback) const;

private:
  YAML::Node root;
  std::string getConfigPath() const;
};

extern Config config;