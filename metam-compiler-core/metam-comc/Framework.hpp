#pragma once

#include <any>

#include <comc.hpp>
#include <fstream>
#include <filesystem>

using uchar = unsigned char;

inline std::string WsToS(std::wstring_view wsl) {
  std::string s;
  for (auto c : wsl)
    s += char(c);
  return s;
}
inline std::string WsToS(std::wstring const& ws) {
  std::string s;
  for (auto c : ws)
    s += char(c);
  return s;
}
