//
// Created by amizz on 07.02.25.
//

#include "UserThread.h"
#include "UserProcess.h"
#include "debug.h"
#include "ArchThreads.h"
#include "PageManager.h"
#include "Loader.h"

void UserThread::Run() {
  debug(USERTHREAD, "Run: Fail-safe kernel panic - you probably have forgotten to set switch_to_userspace_ = 1\n");
  assert(false);
}

UserThread::UserThread(UserProcess *parent, const ustl::string& filename, FileSystemInfo *fs_info) :
        Thread(fs_info, filename, Thread::USER_THREAD), parent_(parent)
{
  loader_ = parent->loader_;
}

void UserThread::prologue(){
  //todo?
}

void UserThread::configureStack() {
  size_t page_for_stack = PageManager::instance()->allocPPN();
  bool vpn_mapped = loader_->arch_memory_.mapPage(USER_BREAK / PAGE_SIZE - 1, page_for_stack, 1);
  assert(vpn_mapped && "Virtual page for stack was already mapped - this should never happen");
}

void UserThread::configureRegistersStartPthread(void *start_function) {
  ArchThreads::createUserRegisters(user_registers_, start_function,
                                   (void*) (USER_BREAK - sizeof(pointer)),
                                   getKernelStackStartPointer());
}

void UserThread::epilogue() {
  ArchThreads::setAddressSpace(this, loader_->arch_memory_);

  debug(USERTHREAD, "ctor: Done loading %s\n", parent_->path_.c_str());
  parent_->addThread(this);
}

void UserThread::kill() {
  debug(USERTHREAD, "kill called on userthread %zu\n", getTID());

  parent_->threads_lock_.acquire();
  assert(parent_->threads_.count(getTID()) && "Tried to remove a nonexistent TID!");
  parent_->threads_.erase(getTID());
  if (parent_->threads_.empty()) {
    debug(USERTHREAD, "kill called on final thread\n");
    final = true;
  }
  parent_->threads_lock_.release();

  if (final)
    parent_->kill();

  debug(USERTHREAD, "userthread kill complete!\n");

  Thread::kill();
}

//do not hold locks here
UserThread::~UserThread() {
  assert(currentThread->holding_lock_list_ == nullptr && "do NOT hold locks in the destructor - do locked operations in kill()");
  debug(USERTHREAD, "destructor called on thread %zu\n", getTID());
//  loader_ = nullptr;
//  working_dir_ = nullptr;
  if (final)
    delete parent_;
  debug(USERTHREAD, "thread destructor complete!\n");
}
