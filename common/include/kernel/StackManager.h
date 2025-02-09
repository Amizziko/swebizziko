//
// Created by amizz on 08.02.25.
//

#ifndef SWEB_STACKMANAGER_H
#define SWEB_STACKMANAGER_H

class UserProcess;

class StackManager {
public:

    UserProcess *parent_;

    StackManager(UserProcess *parent);
};


#endif //SWEB_STACKMANAGER_H
