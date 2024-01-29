#include "commands.h"

void ICommands::execInAnyCase() {
    auto& cmdsRef = getCmdList();
    if (!cmdsRef.empty()) {
        auto it = cmdsRef.begin();
        std::shared_ptr data = std::make_shared<Data>(
                                    std::format("bulk{}", (*it)->getCreateTimeSec())
                                    , "bulk: " + (*it)->getName());
        std::for_each(std::next(cmdsRef.begin()), cmdsRef.end(), [this, &data] (auto c) {
            data->data += ", " + c->getName();
        });
        data->data += '\n';

        std::for_each(oList.begin(), oList.end(), [&data] (auto l) { l->addData(data); });
        cmdsRef.clear();
    }
}