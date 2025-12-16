#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

class Config {
public:
  bool load();
  bool exists() const;

  std::string getString(const std::string &path,
                        const std::string &fallback = "") const;
  int getInt(const std::string &path, int fallback = 0) const;

  std::vector<std::string> getModules() const;

private:
  nlohmann::json root;
  std::string getConfigPath() const;

  const nlohmann::json *resolvePath(const std::string &path) const;
};

extern Config config;