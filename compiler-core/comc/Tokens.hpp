#pragma once

#include <ostream>
#include "Framework.hpp"
#include "Reporter.hpp"

namespace comc
{
  template <class TokenType, class Info>
  struct Token
  {
    TokenType type;
    Info info;
  
    Token(TokenType type, Info info) : type(type), info(info) {}
    Token(TokenType type) : type(type) {}
    
    bool operator==(const Token& rhs) const {
      return type == rhs.type &&
             info == rhs.info;
    }
    bool operator!=(const Token& rhs) const {
      return !(rhs == *this);
    }
  };
}
