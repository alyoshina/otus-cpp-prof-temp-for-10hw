
#include "shared_commands.h"

std::mutex SharedCommands::sharedCmdsMutex;
std::list<std::shared_ptr<ICmd>> SharedCommands::sharedCmds;
std::mutex SharedCommands::objCountMutex;
std::size_t SharedCommands::objCount = 0;