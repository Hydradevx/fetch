#include "core/enviroment.hpp"
#include "core/hardware.hpp"
#include "core/system.hpp"
#include "util/config.hpp"
#include "util/distro.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void displayAll() {
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

  string packageCount = getPackageCount();
  cout << "Installed Packages: " << packageCount << endl;

  string flatpakCount = getFlatpakCount();
  cout << "Installed Flatpaks: " << flatpakCount << endl;

  string kernelArch = getKernelArchiture();
  cout << "Kernel Architecture: " << kernelArch << endl;
}

vector<string> outLines;

void DisplayRegister() {
  if (config.getBool("show_gpu", false)) {
    vector<string> gpuModels = getGpuModels();
    for (const auto &gpuModel : gpuModels) {
      outLines.push_back("GPU Model: " + gpuModel);
    }
  }

  if (config.getBool("show_cpu", false)) {
    string cpuModel = getCpuModel();
    outLines.push_back("CPU Model: " + cpuModel);
  }

  if (config.getBool("show_ram", false)) {
    string totalRam = getTotalRam();
    outLines.push_back("Total RAM: " + totalRam);
  }

  if (config.getBool("show_used_ram", false)) {
    string usedRam = getUsedRam();
    outLines.push_back("Used RAM: " + usedRam);
  }

  if (config.getBool("show_available_ram", false)) {
    string availableRam = getAvailableRam();
    outLines.push_back("Available RAM: " + availableRam);
  }

  if (config.getBool("show_packages", false)) {
    string packageCount = getPackageCount();
    outLines.push_back("Installed Packages: " + packageCount);

    string flatpakCount = getFlatpakCount();
    outLines.push_back("Installed Flatpaks: " + flatpakCount);
  }
}

void Display() {
  vector<string> logoLines = getDistroLogo();
  size_t maxLogoWidth = 0;
  for (const auto &line : logoLines) {
    if (line.length() > maxLogoWidth) {
      maxLogoWidth = line.length();
    }
  }

  size_t totalLines = max(logoLines.size(), outLines.size());
  for (size_t i = 0; i < totalLines; ++i) {
    string logoPart =
        (i < logoLines.size()) ? logoLines[i] : string(maxLogoWidth, ' ');
    string infoPart = (i < outLines.size()) ? outLines[i] : "";
    cout << logoPart << "   " << infoPart << endl;
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