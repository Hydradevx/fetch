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

vector<string> applyLogoColors(const vector<string> &logo,
                               const vector<string> &colors) {
  vector<string> result;

  for (string line : logo) {
    for (size_t i = 0; i < colors.size(); ++i) {
      string token = "$" + to_string(i + 1);
      size_t pos;
      while ((pos = line.find(token)) != string::npos) {
        line.replace(pos, token.length(), ansiFgHex(colors[i]));
      }
    }
    line += "\033[0m"; // reset per-line
    result.push_back(line);
  }

  return result;
}

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

  string kernelArch = getKernelArchiture();
  cout << "Kernel Architecture: " << kernelArch << endl;
}

vector<string> outLines;

void DisplayRegister() {
  for (const auto &mod : config.getModules()) {

    if (mod == "os") {
      outLines.push_back("OS: " + getOsName());

    } else if (mod == "host") {
      outLines.push_back("Host: " + getHost());

    } else if (mod == "kernel") {
      outLines.push_back("Kernel: " + getKernelVersion());

    } else if (mod == "uptime") {
      outLines.push_back("Uptime: " + getUptime());

    } else if (mod == "cpu_model") {
      outLines.push_back("CPU: " + getCpuModel());

    } else if (mod == "cpu_cores") {
      outLines.push_back("CPU Cores: " + getCpuCoreCount());

    } else if (mod == "gpu") {
      for (const auto &gpu : getGpuModels())
        outLines.push_back("GPU: " + gpu);

    } else if (mod == "ram_total") {
      outLines.push_back("Total RAM: " + getTotalRam());

    } else if (mod == "ram_used") {
      outLines.push_back("Used RAM: " + getUsedRam());

    } else if (mod == "ram_available") {
      outLines.push_back("Available RAM: " + getAvailableRam());

    } else if (mod == "swap_total") {
      outLines.push_back("Total Swap: " + getTotalSwap());

    } else if (mod == "swap_free") {
      outLines.push_back("Swap Free: " + getSwapFree());

    } else if (mod == "disk_total") {
      outLines.push_back("Disk Size: " + getDiskSize());

    } else if (mod == "disk_used") {
      outLines.push_back("Disk Used: " + getDiskUsed());

    } else if (mod == "resolution") {
      outLines.push_back("Resolution: " + getResolution());

    } else if (mod == "shell") {
      outLines.push_back("Shell: " + getShell());

    } else if (mod == "terminal") {
      outLines.push_back("Terminal: " + getTerminal());

    } else if (mod == "wm") {
      outLines.push_back("WM: " + GetWM());

    } else if (mod == "packages") {
      outLines.push_back("Packages: " + getPackageCount());

    } else if (mod == "flatpaks") {
      outLines.push_back("Flatpaks: " + getFlatpakCount());

    } else if (mod == "arch") {
      outLines.push_back("Arch: " + getKernelArchiture());
    }
  }
}

void Display() {
  vector<string> logoRaw = getDistroLogo();
  vector<string> logoColors = {
      config.getString("theme.color_primary", "#1793D1"),
      config.getString("theme.color_secondary", "#0F3057"),
      config.getString("theme.color_tertiary", "#00AEEF"),
  };
  vector<string> logoLines = applyLogoColors(logoRaw, logoColors);
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