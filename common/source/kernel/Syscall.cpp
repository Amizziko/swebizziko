#include "offsets.h"
#include "Syscall.h"
#include "syscall-definitions.h"
#include "Terminal.h"
#include "debug_bochs.h"
#include "VfsSyscall.h"
#include "ProcessRegistry.h"
#include "File.h"
#include "Scheduler.h"
#include "UserThread.h"
#include "UserProcess.h"

size_t
Syscall::syscallException(size_t syscall_number, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5) {
  size_t return_value = 0;

  currentThread->tryCancel();

  if ((syscall_number != sc_sched_yield) &&
      (syscall_number != sc_outline)) // no debug print because these might occur very often
  {
    debug(SYSCALL, "Syscall %zd called with arguments %zd(=%zx) %zd(=%zx) %zd(=%zx) %zd(=%zx) %zd(=%zx)\n",
          syscall_number, arg1, arg1, arg2, arg2, arg3, arg3, arg4, arg4, arg5, arg5);
  }

  switch (syscall_number) {
    case sc_sched_yield:
      Scheduler::instance()->yield();
      break;
    case sc_createprocess:
      return_value = createprocess(arg1, arg2);
      break;
    case sc_exit:
      exit(arg1);
      break;
    case sc_write:
      return_value = write(arg1, arg2, arg3);
      break;
    case sc_read:
      return_value = read(arg1, arg2, arg3);
      break;
    case sc_open:
      return_value = open(arg1, arg2);
      break;
    case sc_close:
      return_value = close(arg1);
      break;
    case sc_outline:
      outline(arg1, arg2);
      break;
    case sc_trace:
      trace();
      break;
    case sc_pseudols:
      pseudols((const char *) arg1, (char *) arg2, arg3);
      break;
    case sc_pthread_create:
      return_value = createThread(arg1, arg2, arg3, arg4, arg5);
      break;
    case sc_pthread_exit:
      exitThread(arg1);
      break;
    case sc_pthread_cancel:
      return_value = cancelThread(arg1);
      break;
    default:
      return_value = -1;
      kprintf("Syscall::syscallException: Unimplemented Syscall Number %zd\n", syscall_number);
  }

  currentThread->tryCancel();

  return return_value;
}

void Syscall::pseudols(const char *pathname, char *buffer, size_t size) {
  if (buffer && ((size_t) buffer >= USER_BREAK || (size_t) buffer + size > USER_BREAK))
    return;
  if ((size_t) pathname >= USER_BREAK)
    return;
  VfsSyscall::readdir(pathname, buffer, size);
}

void Syscall::exit(size_t exit_code) {
  debug(SYSCALL, "Syscall::EXIT: called, exit_code: %zd\n", exit_code);

  currentUserProcess->killAllThreads();

  currentThread->kill();
  assert(false && "This should never happen");
}

size_t Syscall::write(size_t fd, pointer buffer, size_t size) {
  //WARNING: this might fail if Kernel PageFaults are not handled
  if ((buffer >= USER_BREAK) || (buffer + size > USER_BREAK)) {
    return -1U;
  }

  size_t num_written = 0;

  if (fd == fd_stdout) //stdout
  {
    debug(SYSCALL, "Syscall::write: %.*s\n", (int) size, (char *) buffer);
    kprintf("%.*s", (int) size, (char *) buffer);
    num_written = size;
  } else {
    num_written = VfsSyscall::write(fd, (char *) buffer, size);
  }
  return num_written;
}

size_t Syscall::read(size_t fd, pointer buffer, size_t count) {
  if ((buffer >= USER_BREAK) || (buffer + count > USER_BREAK)) {
    return -1U;
  }

  size_t num_read = 0;

  if (fd == fd_stdin) {
    //this doesn't! terminate a string with \0, gotta do that yourself
    num_read = currentThread->getTerminal()->readLine((char *) buffer, count);
    debug(SYSCALL, "Syscall::read: %.*s\n", (int) num_read, (char *) buffer);
  } else {
    num_read = VfsSyscall::read(fd, (char *) buffer, count);
  }
  return num_read;
}

size_t Syscall::close(size_t fd) {
  return VfsSyscall::close(fd);
}

size_t Syscall::open(size_t path, size_t flags) {
  if (path >= USER_BREAK) {
    return -1U;
  }
  return VfsSyscall::open((char *) path, flags);
}

void Syscall::outline(size_t port, pointer text) {
  //WARNING: this might fail if Kernel PageFaults are not handled
  if (text >= USER_BREAK) {
    return;
  }
  if (port == 0xe9) // debug port
  {
    writeLine2Bochs((const char *) text);
  }
}

size_t Syscall::createprocess(size_t path, size_t sleep) {
  // THIS METHOD IS FOR TESTING PURPOSES ONLY AND NOT MULTITHREADING SAFE!
  // AVOID USING IT AS SOON AS YOU HAVE AN ALTERNATIVE!

  // parameter check begin
  if (path >= USER_BREAK) {
    return -1U;
  }

  debug(SYSCALL, "Syscall::createprocess: path:%s sleep:%zd\n", (char *) path, sleep);
  ssize_t fd = VfsSyscall::open((const char *) path, O_RDONLY);
  if (fd == -1) {
    return -1U;
  }
  VfsSyscall::close(fd);
  // parameter check end

  size_t process_count = ProcessRegistry::instance()->processCount();
  ProcessRegistry::instance()->createProcess((const char *) path);
  if (sleep) {
    while (ProcessRegistry::instance()->processCount() > process_count) // please note that this will fail ;)
    {
      Scheduler::instance()->yield();
    }
  }
  return 0;
}

void Syscall::trace() {
  currentThread->printBacktrace();
}

size_t
Syscall::createThread(size_t tid_addr, size_t attr_addr, size_t start_routine, size_t arg, size_t entry_function) {
  if (currentThread->type_ != Thread::USER_THREAD) return -1;
  (void) tid_addr;
  (void) attr_addr;

  debug(PTHREAD_CREATE, "called by thread %zu, process %zu\n", currentThread->getTID(), currentUserProcess->pid_);

  auto data = thread_create::data((void *) entry_function, (void *) start_routine, (void *) arg);
  auto pthread = currentUserProcess->createThread(data);
  Scheduler::instance()->addNewThread(pthread);
  return 0;
}

void Syscall::suicideThread() {
  debug(SUICIDE, "asynch cancel received, exiting thread %zu ...\n", currentThread->getTID());
  Syscall::exitThread((size_t) PTHREAD_CANCELED);
}

void Syscall::exitThread(size_t retval) {
  if (currentThread->type_ != Thread::USER_THREAD) return;

  debug(PTHREAD_EXIT, "called on thread %zu with retval %zu\n", currentThread->getTID(), retval);
  currentThread->kill();
}

size_t Syscall::cancelThread(size_t tid) {
  currentUserProcess->threads_lock_.acquire();
  if(!currentUserProcess->threads_.count(tid)){
    currentUserProcess->threads_lock_.release();
    return -1;
  }
  auto thread =  currentUserProcess->threads_.find(tid)->second;
  thread->cancel_requested_ = true;
  currentUserProcess->threads_lock_.release();
  return 0;
}
