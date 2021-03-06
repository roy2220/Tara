#pragma once

#include <assert.h>
#include <setjmp.h>
#
#include "libuv/queue.h"
#
#include "Async.hxx"
#include "Coroutine.hxx"
#include "IOPoll.hxx"
#include "Timer.hxx"

namespace Tara {

struct Fiber;
enum class IOEvent;

class Scheduler final
{
  Scheduler(const Scheduler &other) = delete;
  void operator=(const Scheduler &other) = delete;

public:
  Scheduler();

  Fiber *getCurrentFiber() const { assert(runningFiber_ != nullptr);
                                   return runningFiber_; }
  bool ioIsWatched(int fd) const { return ioPoll_.watcherExists(fd); }
  void watchIO(int fd) { ioPoll_.createWatcher(fd); }
  void awaitTask(const Task *task) { async_.awaitTask(task); }

  void callCoroutine(const Coroutine &coroutine);
  void callCoroutine(Coroutine &&coroutine);
  void run();
  void yieldCurrentFiber();
  void sleepCurrentFiber(int duration);
  [[noreturn]] void exitCurrentFiber() const;
  [[noreturn]] void killCurrentFiber();
  void unwatchIO(int fd);
  int awaitIOEvent(int fd, IOEvent ioEvent, int timeout);
  void suspendCurrentFiber();
  void resumeFiber(Fiber *fiber);

private:
  unsigned int fiberCount_;
  jmp_buf *context_;
  int status_;
  Fiber *runningFiber_;
  QUEUE readyFiberQueue_;
  QUEUE deadFiberQueue_;
  IOPoll ioPoll_;
  Timer timer_;
  Async async_;

  [[noreturn]] void execute();
  [[noreturn]] void executeFiber(Fiber *fiber);
};

} // namespace Tara
