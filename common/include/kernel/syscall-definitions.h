#pragma once

#define fd_stdin 0
#define fd_stdout 1
#define fd_stderr 2

#define sc_exit 1
#define sc_fork 2
#define sc_read 3
#define sc_write 4
#define sc_open 5
#define sc_close 6
#define sc_lseek 19
#define sc_pseudols 43
#define sc_outline 105
#define sc_sched_yield 158
#define sc_createprocess 191
#define sc_trace 252

#define sc_pthread_create 256
#define sc_pthread_exit 257
#define sc_pthread_join 258
#define sc_pthread_detach 259
#define sc_pthread_cancel 260
#define sc_pthread_set_cancel_state 261
#define sc_pthread_set_cancel_type 262
