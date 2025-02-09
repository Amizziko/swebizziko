//
// Created by amizz on 08.02.25.
//

#include "StackManager.h"
#include "UserProcess.h"
#include "debug.h"

StackManager::StackManager(UserProcess *parent) : parent_{parent} {
  debug(STACKMANAGER, "StackManager Constructor start, PID %zu\n", parent_->pid_);
  debug(STACKMANAGER, "StackManager Constructor end\n");
}
