//
// Created by amizz on 07.02.25.
//

#ifndef SWEB_USERTHREAD_H
#define SWEB_USERTHREAD_H

#include "Thread.h"
#include "helpers.h"
#define PTHREAD_CANCELED ((void*)-1)

class UserProcess;

class UserThread : public Thread {
    friend class UserProcess;

public:
    UserThread(UserProcess *parent, const ustl::string& filename, FileSystemInfo *fs_info);
    ~UserThread();
    void Run() override; // not used
    void kill() override;
    bool schedulable() override;
    bool asnychCancelRequested();
    bool deffCancelRequested();
    void tryCancel() override;

    UserProcess *parent_;


    //resources
    bool final = false; //make atomic if appropriate todo
    ustl::atomic<bool> cancel_requested_ = false;
    ustl::atomic<bool> cancel_enabled_ = true;
    ustl::atomic<bool> cancel_asynch_ = true;
    ustl::atomic<bool> force_cancel_requested_ = false;

    //locks


    //functions
    void prologue();
    void configureRegistersStart(thread_create::data& data);
    void configureRegistersPthread(thread_create::data& data);
    void epilogue();
};

#define currentUserThread ((UserThread*)currentThread)
#define currentUserProcess ((UserProcess*)currentUserThread->parent_)

#endif //SWEB_USERTHREAD_H
