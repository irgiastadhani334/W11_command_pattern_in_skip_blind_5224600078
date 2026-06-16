#ifndef REWARD_COMMAND_MANAGER_H
#define REWARD_COMMAND_MANAGER_H

#include "ICommand.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <memory>

class RewardCommandManager {
private:
    std::vector<std::unique_ptr<ICommand>> activeRewards;

public:
    void addReward(std::unique_ptr<ICommand> reward) {
        if (reward != nullptr) {
            activeRewards.push_back(std::move(reward));
            std::cout << "[Manager] Reward baru berhasil disimpan ke antrean.\n";
        }
    }

    void handleEvent(TriggerEvent event) {
        for (auto& reward : activeRewards) {
            if (reward->getTrigger() == event) {
                reward->execute();
            }
        }
        cleanupRewards();
    }

private:
    void cleanupRewards() {
        activeRewards.erase(
            std::remove_if(activeRewards.begin(), activeRewards.end(),
                [](const std::unique_ptr<ICommand>& reward) {
                    return reward->isFinished();
                }), 
            activeRewards.end()
        );
    }
};

#endif
