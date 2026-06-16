#ifndef CONCRETE_COMMANDS_H
#define CONCRETE_COMMANDS_H

#include "ICommand.h"
#include <iostream>

// 1. Free Joker Reward
class FreeJokerCommand : public ICommand {
private:
    bool finished = false;
public:
    TriggerEvent getTrigger() const override { return TriggerEvent::EnterShop; }
    void execute() override {
        std::cout << "[REWARD] FreeJokerCommand: Joker berikutnya di Shop gratis!\n";
        finished = true;
    }
    bool isFinished() const override { return finished; }
};

// 2. Double Money Reward
class DoubleMoneyCommand : public ICommand {
private:
    bool finished = false;
public:
    TriggerEvent getTrigger() const override { return TriggerEvent::EnterShop; }
    void execute() override {
        std::cout << "[REWARD] DoubleMoneyCommand: Uang pemain digandakan (2x)!\n";
        finished = true;
    }
    bool isFinished() const override { return finished; }
};

// 3. Mega Blind Buff Reward
class MegaBlindBuffCommand : public ICommand {
private:
    bool finished = false;
public:
    TriggerEvent getTrigger() const override { return TriggerEvent::EnterBlind; }
    void execute() override {
        std::cout << "[REWARD] MegaBlindBuffCommand: Bonus Multiplier diterapkan pada Blind ini!\n";
        finished = true;
    }
    bool isFinished() const override { return finished; }
};

#endif
