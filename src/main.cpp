#include "core/enviroment.hpp"
#include "core/hardware.hpp"
#include "core/system.hpp"
#include "util/config.hpp"
#include "util/distro.hpp"
#include "util/strings.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void displayAll() {

  string hostName = getHost();
  cout << "Host Device: " << hostName << endl;

  string kernelVersion = getKernelVersion();
  cout << "Kernel Version: " << kernelVersion << endl;

  string uptime = getUptime();
  cout << "Uptime: " << uptime << endl;

  string cpuCores = getCpuCoreCount();
  cout << "CPU Cores: " << cpuCores << endl;

  string totalSwap = getTotalSwap();
  cout << "Total Swap: " << totalSwap << endl;

  string swapFree = getSwapFree();
  cout << "Swap Free: " << swapFree << endl;

  string diskSize = getDiskSize();
  cout << "Disk Size: " << diskSize << endl;

  string diskUsed = getDiskUsed();
  cout << "Disk Used: " << diskUsed << endl;

  string resolution = getResolution();
  cout << "Screen Resolution: " << resolution << endl;

  string shell = getShell();
  cout << "Shell: " << shell << endl;

  string terminal = getTerminal();
  cout << "Terminal: " << terminal << endl;

  string wm = GetWM();
  cout << "Window Manager: " << wm << endl;

  string flatpakCount = getFlatpakCount();
  cout << "Installed Flatpaks: " << flatpakCount << endl;

  string kernelArch = getKernelArchiture();
  cout << "Kernel Architecture: " << kernelArch << endl;
}

vector<string> outLines;

void DisplayRegister() {
  for (const auto &mod : config.getModules()) {

    if (mod == "os") {
      outLines.push_back("OS: " + getOsName());
    } else if (mod == "cpu") {
      outLines.push_back("CPU: " + getCpuModel());
    } else if (mod == "gpu") {
      for (const auto &gpu : getGpuModels())
        outLines.push_back("GPU: " + gpu);
    } else if (mod == "ram") {
      outLines.push_back("RAM: " + getUsedRam() + " / " + getTotalRam());
    } else if (mod == "packages") {
      outLines.push_back("Packages: " + getPackageCount());
      outLines.push_back("Flatpaks: " + getFlatpakCount());
    }
  }
}

void Display() {
  vector<string> logoLines = getDistroLogo();
  size_t maxLogoWidth = 0;
  for (const auto &line : logoLines) {
    if (visibleLength(line) > maxLogoWidth) {
      maxLogoWidth = visibleLength(line);
    }
  }

  int pad_left = config.getInt("general.padding_left", 0);
  int pad_right = config.getInt("general.padding_right", 0);
  size_t totalLines = max(logoLines.size(), outLines.size());
  for (size_t i = 0; i < totalLines; ++i) {
    string logoPart;
    if (i < logoLines.size()) {
      logoPart = logoLines[i];
      size_t pad = maxLogoWidth - visibleLength(logoPart);
      logoPart += string(pad, ' ');
    } else {
      logoPart = string(maxLogoWidth, ' ');
    }
    string infoPart = (i < outLines.size()) ? outLines[i] : "";

    cout << string(pad_left, ' ') << logoPart << string(pad_right, ' ')
         << infoPart << '\n';
  }
}

int main() {
  if (!config.load()) {
    cerr << "Failed to load config." << endl;
    return 1;
  }
  DisplayRegister();
  Display();
  return 0;
}