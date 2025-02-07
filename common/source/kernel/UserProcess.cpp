#include "ProcessRegistry.h"
#include "UserProcess.h"
#include "kprintf.h"
#include "Console.h"
#include "Loader.h"
#include "VfsSyscall.h"
#include "File.h"
#include "PageManager.h"
#include "ArchThreads.h"
#include "offsets.h"
#include "Scheduler.h"
#include "UserThread.h"

UserProcess::UserProcess(ustl::string filename, FileSystemInfo *fs_info, uint32 terminal_number) :
fd_(VfsSyscall::open(filename, O_RDONLY)), pid_(ProcessRegistry::instance()->getNewPID()),

//locks
threads_lock_("UserProcess::threads_lock_"),
loader_lock_("UserProcess::loader_lock_")
{
  ProcessRegistry::instance()->processStart(); //should also be called if you fork a process
  debug(USERPROCESS, "UserProcess constructor start for PID %zu, path %s\n", pid_, filename.c_str());

  if(!loaderValid(filename))
    return;

  auto u_thread = new UserThread(this, filename, fs_info);

  if (main_console->getTerminal(terminal_number))
    u_thread->setTerminal(main_console->getTerminal(terminal_number));

  u_thread->switch_to_userspace_ = 1;

  debug(USERPROCESS, "UserProcess constructor end for PID %zu, path %s\n", pid_, filename.c_str());

  Scheduler::instance()->addNewThread(u_thread);
}

bool UserProcess::loaderValid(const ustl::string &filename) {
  if (fd_ < 0) {
    debug(USERPROCESS, "Error: loading %s failed!\n", filename.c_str());
    kill();
    delete this;
    return false;
  }

  loader_ = new Loader(fd_);

  if (!loader_->loadExecutableAndInitProcess())
  {
    debug(USERPROCESS, "Error: loading %s failed!\n", filename.c_str());
    kill();
    delete this;
    return false;
  }
  return true;
}

void UserProcess::kill() {
  assert(threads_.empty() && "you probably shouldnt be calling process.kill()");

  debug(USERPROCESS, "kill called on Process %zu\n", pid_);
  ProcessRegistry::instance()->processExit();//holds locks
  debug(USERPROCESS, "Process kill complete!\n");
}

//do not hold locks here
UserProcess::~UserProcess()
{
  assert(Scheduler::instance()->isCurrentlyCleaningUp());
  assert(currentThread->holding_lock_list_ == nullptr && "do NOT hold locks in the destructor - do locked operations in kill()");

  debug(USERPROCESS, "Destructor called on Process %zu\n", pid_);
  delete loader_;

  if (fd_ > 0)
    VfsSyscall::close(fd_);
  delete fs_info_;
  debug(USERPROCESS, "Destructor complete!\n");
}

void UserProcess::addThread(UserThread *t) {
  threads_lock_.acquire();
  assert(!threads_.count(t->getTID()) && "Tried to add a duplicate TID!");
  threads_.emplace(t->getTID(), t);
  threads_lock_.release();
}
