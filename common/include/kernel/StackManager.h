//
// Created by amizz on 08.02.25.
//

#ifndef SWEB_STACKMANAGER_H
#define SWEB_STACKMANAGER_H

#include "types.h"

class UserProcess;

class UserThread;

class StackManager {
public:

    UserProcess *parent_;

    StackManager(UserProcess *parent);

    void createStack(size_t tid);
    size_t getStackOfThread(size_t tid);

    void freeStack(size_t tid);
};

/*
 * how should the stack work?
 * i will make life easy and just statically map it and just mark the pages cow
 * each thread will get X pages for the stack, they will be marked as cow, when a thread terminates its pages will be unmapped
 * todo allocnppn
 * */

#endif //SWEB_STACKMANAGER_H
