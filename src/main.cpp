#include "core/enviroment.hpp"
#include "core/hardware.hpp"
#include "core/system.hpp"
#include "util/config.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

Config config;

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

void Display() {
  if (config.getBool("show_gpu", false)) {
    vector<string> gpuModels = getGpuModels();
    for (const auto &gpuModel : gpuModels) {
      cout << "GPU Model: " << gpuModel << endl;
    }
  }

  if (config.getBool("show_cpu", false)) {
    string cpuModel = getCpuModel();
    cout << "CPU Model: " << cpuModel << endl;
  }

  if (config.getBool("show_ram", false)) {
    string totalRam = getTotalRam();
    cout << "Total RAM: " << totalRam << endl;

    string usedRam = getUsedRam();
    cout << "Used RAM: " << usedRam << endl;

    string availableRam = getAvailableRam();
    cout << "Available RAM: " << availableRam << endl;
  }

  if (config.getBool("show_packages", false)) {
    string packageCount = getPackageCount();
    cout << "Installed Packages: " << packageCount << endl;

    string flatpakCount = getFlatpakCount();
    cout << "Installed Flatpaks: " << flatpakCount << endl;
  }
}

int main() {
  if (!config.load()) {
    cerr << "Failed to load config." << endl;
    return 1;
  }
  Display();
  return 0;
}