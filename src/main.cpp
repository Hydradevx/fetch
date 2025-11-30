#include <fstream>
#include <iostream>

using namespace std;

#include <fstream>
#include <string>

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

int main() {
  string osName = getOsName();
  cout << "Operating System: " << osName << endl;

  string hostName = getHost();
  cout << "Host Device: " << hostName << endl;

  string kernelVersion = getKernelVersion();
  cout << "Kernel Version: " << kernelVersion << endl;

  string uptime = getUptime();
  cout << "Uptime: " << uptime << endl;

  return 0;
}
