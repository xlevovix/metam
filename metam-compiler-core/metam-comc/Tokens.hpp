#pragma once

#include "Framework.hpp"

namespace mcomc
{
  enum TokenType {
#define X(x) x,
#include "Tokens-enum.hpp"
#undef X
  };
  static std::wstring _TokenTypeString[] {
#define X(x) L""#x,
#include "Tokens-enum.hpp"
#undef X
  };
  
  using Token = comc::Token<TokenType, std::any>;
  
  std::wostream& operator<<(std::wostream& os, const Token& t) {
    os << _TokenTypeString[t.type];
    if (auto s = any_cast<uint64>(&t.info)) {
      std::wcout << ": " << "\033[0;34m" << *s << "\033[0m";
    } else if (auto s = any_cast<double>(&t.info)) {
      std::wcout << ": " << "\033[0;34m" << *s << "\033[0m";
    } else if (auto s = any_cast<std::wstring>(&t.info)) {
      std::wcout << ": " << "\033[0;33m" << *s << "\033[0m";
    } else if (auto s = any_cast<wchar>(&t.info)) {
      std::wcout << ": " << "\033[0;33m" << *s << "\033[0m";
    };
    return os;
  }
}
