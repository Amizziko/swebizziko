//
// Created by amizz on 07.02.25.
//

#ifndef SWEB_USERTHREAD_H
#define SWEB_USERTHREAD_H

#include "Thread.h"

class UserProcess;

class UserThread : public Thread {
    friend class UserProcess;

public:
    UserThread(UserProcess *parent, ustl::string filename, FileSystemInfo *fs_info);
    ~UserThread();
    void Run() override; // not used
    void kill() override;
    UserProcess *parent_;


    //resources
    bool final = false; //make atomic if appropriate todo

    //locks


    //functions
};


#endif //SWEB_USERTHREAD_H
