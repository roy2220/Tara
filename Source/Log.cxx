#include "Log.hxx"

#include <stdio.h>
#
#include "Atomic.hxx"

namespace Tara {

Log::Level Log::Level_(Level::Debugging);

Log::Level Log::GetLevel()
{
  auto level = static_cast<Level>(0);
  ExchangeAdd(Level_, level);
  return level;
}

void Log::SetLevel(Level level)
{
  Exchange(Level_, level);
}

Log::Log()
  : outputStream_("Tara: ", std::ostringstream::ate)
{}

Log::~Log()
{
  outputStream_.put('\n');
  fputs(outputStream_.str().c_str(), stderr);
}

} // namespace Tara
