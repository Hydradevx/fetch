#include "core/enviroment.hpp"
#include "util/exec.hpp"
#include "util/strings.hpp"
#include <string>
#include <unistd.h>

using namespace std;

string getShell() {
  string shell = getenv("SHELL");
  return shell.empty() ? "Unknown Shell" : shell;
}

string getTerminal() {
  string term = getenv("TERMINAL");
  return term.empty() ? "Unknown Terminal" : term;
}

string GetWM() {
  string wm = getenv("XDG_CURRENT_DESKTOP");
  return trim(wm).empty() ? "Unknown WM" : trim(wm);
}

string getPackageCount() {
  // Detect distro via package managers
  if (access("/bin/pacman", F_OK) == 0 ||
      access("/usr/bin/pacman", F_OK) == 0) {
    return trim(execCmd("pacman -Qq | wc -l"));
  }

  if (access("/usr/bin/apt", F_OK) == 0) {
    return trim(execCmd("dpkg -l | grep '^ii' | wc -l"));
  }

  if (access("/usr/bin/dnf", F_OK) == 0) {
    return trim(execCmd("dnf list installed | wc -l"));
  }

  if (access("/usr/bin/rpm", F_OK) == 0) {
    return trim(execCmd("rpm -qa | wc -l"));
  }

  if (access("/usr/bin/xbps-query", F_OK) == 0) {
    return trim(execCmd("xbps-query -l | wc -l"));
  }

  if (access("/usr/bin/emerge", F_OK) == 0) { // Gentoo
    return trim(execCmd("qlist -I | wc -l"));
  }

  if (access("/sbin/apk", F_OK) == 0) { // Alpine
    return trim(execCmd("apk info | wc -l"));
  }

  if (access("/usr/bin/eopkg", F_OK) == 0) { // Solus
    return trim(execCmd("eopkg list-installed | wc -l"));
  }

  return "Unknown";
}

string getFlatpakCount() {
  if (access("/usr/bin/flatpak", F_OK) == 0) {
    return trim(execCmd("flatpak list | wc -l"));
  }
  return "0";
}
