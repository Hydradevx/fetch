#pragma once

#include <string>
#include <vector>

// Utility
std::string readTrim(const std::string &path);
bool invalid(const std::string &s);
std::string execCmd(const char *cmd);
std::string trim(const std::string &s);

// CPU
std::string getCpuModel();
std::string getCpuCoreCount();

// GPU
std::vector<std::string> getGpuModels();

// RAM
std::string getTotalRam();
std::string getAvailableRam();
std::string getUsedRam();

// Swap
std::string getTotalSwap();
std::string getSwapFree();

// Disk
std::string getDiskSize();
std::string getDiskUsed();

// Display
std::string getResolution();
