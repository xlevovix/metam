#pragma once

#include "Tokens.hpp"

namespace comc
{
  struct StreamLexer
  {
    std::wistream& stream;
    struct {
      uint64 line = 0;
      uint64 pos = 0;
      wchar c = '\0';
    } current;
    
    StreamLexer(std::wistream& stream);
  };
  
  
  StreamLexer::StreamLexer(std::wistream& stream) : stream(stream) {}
}
