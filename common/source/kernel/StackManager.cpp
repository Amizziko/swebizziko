//
// Created by amizz on 08.02.25.
//

#include "StackManager.h"
#include "UserProcess.h"
#include "debug.h"
#include "PageManager.h"
#include "Loader.h"

#define TOP_STACK_VPN (USER_BREAK / PAGE_SIZE - 1)
#define STACK_SIZE 1

StackManager::StackManager(UserProcess *parent) : parent_{parent} {
  debug(STACKMANAGER, "StackManager Constructor start, PID %zu\n", parent_->pid_);
  debug(STACKMANAGER, "StackManager Constructor end\n");
}

void StackManager::createStack(size_t tid) {
  debug(STACKMANAGER, "creating stack for thread %zu\n", tid);

  size_t stack_ppn = PageManager::instance()->allocPPN();
  size_t stack_vpn = TOP_STACK_VPN - tid;

  assert(parent_->loader_->arch_memory_.mapPage(stack_vpn, stack_ppn, 1)
         && "Virtual page for stack was already mapped - this should never happen");
}

void StackManager::freeStack(size_t tid) {
  debug(STACKMANAGER, "freeing stack for thread %zu\n", tid);

}

size_t StackManager::getStackOfThread(size_t tid) {
  return (TOP_STACK_VPN - tid +1)*PAGE_SIZE;
}
