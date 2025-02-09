//
// Created by amizz on 08.02.25.
//

#ifndef SWEB_MEMORYMANAGER_H
#define SWEB_MEMORYMANAGER_H

#include "StackManager.h"

class UserProcess;

class MemoryManager {
public:
    UserProcess *parent_;


    StackManager stack;

//    HeapManager heap;
//    MapManager mappings;
    MemoryManager(UserProcess *parent);
};


#endif //SWEB_MEMORYMANAGER_H
