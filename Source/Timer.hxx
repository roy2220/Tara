#pragma once

#include "libuv/heap-inl.h"

namespace Tara {

struct TimerItem;

class Timer final
{
  Timer(const Timer &other) = delete;
  void operator=(const Timer &other) = delete;

public:
  Timer();

  void addItem(TimerItem *item, int duration);
  void removeItem(TimerItem *item);
  unsigned int removeDueItems(TimerItem **buffer, unsigned int bufferLength);
  int calculateTimeout();

private:
  heap itemHeap_;
};

} // namespace Tara
