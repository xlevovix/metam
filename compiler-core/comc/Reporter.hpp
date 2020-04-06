#pragma once

#include "Framework.hpp"

namespace comc
{
  struct Reporter
  {
    std::function<void(std::wstring const&)> error;
    uint64 errors = 0;
  };
}
