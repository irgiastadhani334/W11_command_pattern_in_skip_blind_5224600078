#ifndef ICOMMAND_H
#define ICOMMAND_H

#include "TriggerEvent.h"

class ICommand {
public:
    virtual ~ICommand() = default;
    
    virtual void execute() = 0;
    virtual TriggerEvent getTrigger() const = 0;
    virtual bool isFinished() const = 0;
};

#endif
