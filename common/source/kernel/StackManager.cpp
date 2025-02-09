//
// Created by amizz on 08.02.25.
//

#include "StackManager.h"
#include "UserProcess.h"
#include "debug.h"
#include "PageManager.h"
#include "Loader.h"

StackManager::StackManager(UserProcess *parent) : parent_{parent} {
  debug(STACKMANAGER, "StackManager Constructor start, PID %zu\n", parent_->pid_);
  debug(STACKMANAGER, "StackManager Constructor end\n");
}

void StackManager::createStack(size_t tid) {
  debug(STACKMANAGER, "creating stack for thread %zu\n", tid);
  size_t page_for_stack = PageManager::instance()->allocPPN();
  bool vpn_mapped = parent_->loader_->arch_memory_.mapPage(USER_BREAK / PAGE_SIZE - 1, page_for_stack, 1);
  assert(vpn_mapped && "Virtual page for stack was already mapped - this should never happen");
}

void StackManager::freeStack(size_t tid) {
  debug(STACKMANAGER, "freeing stack for thread %zu\n", tid);

}
