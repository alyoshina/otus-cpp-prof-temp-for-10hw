#include "commands.h"

void ICommands::execInAnyCase() {
    if (!cmds.empty()) {
        auto it = cmds.begin();
        std::shared_ptr data = std::make_shared<Data>(
                                    std::format("bulk{}", (*it)->getCreateTimeSec())
                                    , "bulk: " + (*it)->getName());
        std::for_each(std::next(cmds.begin()), cmds.end(), [this, &data] (auto c) {
            data->data += ", " + c->getName();
        });
        data->data += '\n';

        std::for_each(oList.begin(), oList.end(), [&data] (auto l) { l->addData(data); });
        cmds.clear();
    }
}