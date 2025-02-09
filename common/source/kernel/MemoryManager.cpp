//
// Created by amizz on 08.02.25.
//

#include "MemoryManager.h"
#include "debug.h"

MemoryManager::MemoryManager(UserProcess *parent) : parent_(parent), stack(parent){
  debug(MEMORYMANAGER, "MemoryManager Constructed\n");
}
