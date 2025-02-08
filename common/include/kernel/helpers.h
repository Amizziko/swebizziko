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
            struct {
                void *entry_function;
                void *start_routine;
                void *arg;
            };
        };

        explicit data(void* entry) : mode(START), entry_function(entry){};
        data(void *entry, void *start, void *a) : mode(PTHREAD), entry_function(entry), start_routine(start), arg(a) {};
        explicit data(UserThread *forker) : mode(FORK), src(forker) {};//todo
    };
}


#endif //SWEB_HELPERS_H
