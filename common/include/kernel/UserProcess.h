#pragma once

#include "UserThread.h"
#include "umap.h"
#include "Mutex.h"

class UserProcess {
    friend class UserThread;

public:
    /**
     * Constructor
     * @param minixfs_filename filename of the file in minixfs to execute
     * @param fs_info filesysteminfo-object to be used
     * @param terminal_number the terminal to run in (default 0)
     *
     */
    UserProcess(ustl::string minixfs_filename, FileSystemInfo *fs_info, uint32 terminal_number = 0);
    ~UserProcess();

    Loader* loader_;
    FileSystemInfo *fs_info_;

//resources
    ustl::map<size_t, UserThread *> threads_; //locked by threads_lock_
    int32 fd_; //to be changed to a list of open fds maybe? todo
    bool construction_failed = true; //default to true, set to false when constructor finishes


//locks
    Mutex threads_lock_;
    Mutex loader_lock_;

    void kill();
};

