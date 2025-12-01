#include <array>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

string readTrim(const string &path) {
  ifstream f(path);
  if (!f.is_open())
    return "";
  string s;
  getline(f, s);
  return s;
}

bool invalid(const string &s) {
  if (s.empty())
    return true;
  if (s == "To Be Filled By O.E.M." || s == "To be filled by O.E.M.")
    return true;
  return false;
}

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

string trim(const string &s) {
  size_t start = s.find_first_not_of(" \t\n\r");
  size_t end = s.find_last_not_of(" \t\n\r");
  if (start == string::npos)
    return "";
  return s.substr(start, end - start + 1);
}

string getHost() {
  string name = readTrim("/sys/devices/virtual/dmi/id/product_name");
  if (!invalid(name))
    return name;

  string board = readTrim("/sys/devices/virtual/dmi/id/board_name");
  if (!invalid(board))
    return board;

  string vendor = readTrim("/sys/devices/virtual/dmi/id/sys_vendor");
  if (!invalid(vendor))
    return vendor;

  return "Unknown Device";
}

string getOsName() {
  ifstream osfile("/etc/os-release");
  string line;
  while (getline(osfile, line)) {
    if (line.find("PRETTY_NAME=") == 0) {
      size_t start = line.find("\"") + 1;
      size_t end = line.rfind("\"");
      return line.substr(start, end - start);
    }
  }
  return "Unknown OS";
}

string getKernelVersion() { return readTrim("/proc/sys/kernel/osrelease"); }

string formatUptime(long seconds) {
  long days = seconds / 86400;
  seconds %= 86400;
  long hours = seconds / 3600;
  seconds %= 3600;
  long minutes = seconds / 60;
  seconds %= 60;

  char buffer[100];
  snprintf(buffer, sizeof(buffer),
           "%ld days, %ld hours, %ld minutes, %ld seconds", days, hours,
           minutes, seconds);
  return string(buffer);
}

string getUptime() {
  string uptimeStr = readTrim("/proc/uptime");
  long s = stol(uptimeStr);
  return formatUptime(s);
}

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

int main() {
  string osName = getOsName();
  cout << "Operating System: " << osName << endl;

  string hostName = getHost();
  cout << "Host Device: " << hostName << endl;

  string kernelVersion = getKernelVersion();
  cout << "Kernel Version: " << kernelVersion << endl;

  string uptime = getUptime();
  cout << "Uptime: " << uptime << endl;

  string cpuModel = getCpuModel();
  cout << "CPU Model: " << cpuModel << endl;

  string cpuCores = getCpuCoreCount();
  cout << "CPU Cores: " << cpuCores << endl;

  vector<string> gpuModels = getGpuModels();
  for (const auto &gpuModel : gpuModels) {
    cout << "GPU Model: " << gpuModel << endl;
  }

  string totalRam = getTotalRam();
  cout << "Total RAM: " << totalRam << endl;

  string usedRam = getUsedRam();
  cout << "Used RAM: " << usedRam << endl;

  string availableRam = getAvailableRam();
  cout << "Available RAM: " << availableRam << endl;

  return 0;
}
