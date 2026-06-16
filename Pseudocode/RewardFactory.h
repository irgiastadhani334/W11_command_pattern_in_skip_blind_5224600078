#ifndef REWARD_FACTORY_H
#define REWARD_FACTORY_H

#include "ICommand.h"
#include "ConcreteCommands.h"
#include <memory>

class RewardFactory {
public:
    static std::unique_ptr<ICommand> createReward(TagType tag) {
        switch (tag) {
            case TagType::FreeJoker:
                return std::make_unique<FreeJokerCommand>();
            case TagType::DoubleMoney:
                return std::make_unique<DoubleMoneyCommand>();
            case TagType::MegaBlindBuff:
                return std::make_unique<MegaBlindBuffCommand>();
            default:
                return nullptr;
        }
    }
};

#endif
