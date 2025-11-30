#include <fstream>
#include <iostream>

using namespace std;

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

int main() {
  string osName = getOsName();
  cout << "Operating System: " << osName << endl;
  return 0;
}
