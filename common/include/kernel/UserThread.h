//
// Created by amizz on 07.02.25.
//

#ifndef SWEB_USERTHREAD_H
#define SWEB_USERTHREAD_H

#include "Thread.h"
#include "helpers.h"

class UserProcess;

class UserThread : public Thread {
    friend class UserProcess;

public:
    UserThread(UserProcess *parent, const ustl::string& filename, FileSystemInfo *fs_info);
    ~UserThread();
    void Run() override; // not used
    void kill() override;
    UserProcess *parent_;


    //resources
    bool final = false; //make atomic if appropriate todo

    //locks


    //functions
    void prologue();
    void configureStack();
    void configureRegistersStart(thread_create::data& data);
    void configureRegistersPthread(thread_create::data& data);
    void epilogue();
};

#define currentUserThread ((UserThread*)currentThread)
#define currentUserProcess ((UserProcess*)currentUserThread->parent_)

#endif //SWEB_USERTHREAD_H
