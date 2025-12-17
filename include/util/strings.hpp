#pragma once
#include <string>

std::string trim(const std::string &s);
bool invalid(const std::string &s);
std::string readTrim(const std::string &path);
std::string stripAnsi(const std::string &s);
size_t visibleLength(const std::string &s);
std::string ansiFgHex(const std::string &hex);