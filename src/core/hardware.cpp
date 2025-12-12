#include "core/hardware.hpp"
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

string getCpuModel() {
  ifstream cpuinfo("/proc/cpuinfo");
  string line;
  while (getline(cpuinfo, line)) {
    if (line.find("model name") != string::npos) {
      size_t colon = line.find(":");
      if (colon != string::npos) {
        return line.substr(colon + 2); // Skip ": "
      }
    }
  }
  return "Unknown CPU Model";
}

string getCpuCoreCount() {
  ifstream cpuinfo("/proc/cpuinfo");
  string line;
  int count = 0;
  while (getline(cpuinfo, line)) {
    if (line.find("processor") != string::npos) {
      count++;
    }
  }
  return to_string(count);
}

std::vector<std::string> getGpuModels() {
  std::string raw = execCmd("lspci | grep -Ei 'vga|3d|display'");
  std::vector<std::string> list;

  size_t start = 0;
  while (true) {
    size_t end = raw.find('\n', start);
    if (end == std::string::npos)
      break;
    std::string line = raw.substr(start, end - start);
    start = end + 1;

    size_t spacePos = line.find(' ');
    if (spacePos != std::string::npos)
      line = line.substr(spacePos + 1);

    size_t colonPos = line.find(": ");
    if (colonPos != std::string::npos)
      line = line.substr(colonPos + 2);

    list.push_back(line);
  }

  return list;
}

string getTotalRam() {
  ifstream meminfo("/proc/meminfo");
  string line;
  while (getline(meminfo, line)) {
    if (line.find("MemTotal:") == 0) {
      size_t start = line.find_first_of("0123456789");
      size_t end = line.find(" kB", start);
      if (start != string::npos && end != string::npos) {
        string memKbStr = line.substr(start, end - start);
        long memKb = stol(memKbStr);
        long memMb = memKb / 1024;
        return to_string(memMb) + " MB";
      }
    }
  }
  return "Unknown RAM";
}

string getAvailableRam() {
  ifstream meminfo("/proc/meminfo");
  string line;
  while (getline(meminfo, line)) {
    if (line.find("MemAvailable:") == 0) {
      size_t start = line.find_first_of("0123456789");
      size_t end = line.find(" kB", start);
      if (start != string::npos && end != string::npos) {
        string memKbStr = line.substr(start, end - start);
        long memKb = stol(memKbStr);
        long memMb = memKb / 1024;
        return to_string(memMb) + " MB";
      }
    }
  }
  return "Unknown Available RAM";
}

string getUsedRam() {
  string total = getTotalRam();
  string available = getAvailableRam();
  if (total == "Unknown RAM" || available == "Unknown Available RAM")
    return "Unknown Used RAM";
  long totalMb = stol(total.substr(0, total.find(" ")));
  long availableMb = stol(available.substr(0, available.find(" ")));
  long usedMb = totalMb - availableMb;
  return to_string(usedMb) + " MB";
}

string getTotalSwap() {
  ifstream meminfo("/proc/meminfo");
  string line;
  while (getline(meminfo, line)) {
    if (line.find("SwapTotal:") == 0) {
      size_t start = line.find_first_of("0123456789");
      size_t end = line.find(" kB", start);
      if (start != string::npos && end != string::npos) {
        string swapKbStr = line.substr(start, end - start);
        long swapKb = stol(swapKbStr);
        long swapMb = swapKb / 1024;
        return to_string(swapMb) + " MB";
      }
    }
  }
  return "Unknown Swap";
}

string getSwapFree() {
  ifstream meminfo("/proc/meminfo");
  string line;
  while (getline(meminfo, line)) {
    if (line.find("SwapFree:") == 0) {
      size_t start = line.find_first_of("0123456789");
      size_t end = line.find(" kB", start);
      if (start != string::npos && end != string::npos) {
        string swapKbStr = line.substr(start, end - start);
        long swapKb = stol(swapKbStr);
        long swapMb = swapKb / 1024;
        return to_string(swapMb) + " MB";
      }
    }
  }
  return "Unknown Swap Free";
}

string getDiskSize() {
  string cmd = "df -h  | awk 'NR==2 {print $2}'";
  string size = execCmd(cmd.c_str());
  return trim(size);
}

string getDiskUsed() {
  string cmd = "df -h  | awk 'NR==2 {print $3}'";
  string used = execCmd(cmd.c_str());
  return trim(used);
}

string getResolution() {
  string cmd = "xdpyinfo | grep dimensions | awk '{print $2}'";
  string res = execCmd(cmd.c_str());
  return trim(res);
}