//
// Created by amizz on 08.02.25.
//

#ifndef SWEB_HELPERS_H
#define SWEB_HELPERS_H

#include "types.h"

class UserThread;

namespace thread_create {
    enum create_mode : uint8 {
        START,
        PTHREAD,
        FORK
    };

    struct data {
        create_mode mode;
        union {
            UserThread *src;
            void *entry_function;
        };

        data() : mode(START){};
        data(enum create_mode mode, void* data) : mode(mode), entry_function(data) {};
    };
}



#endif //SWEB_HELPERS_H
