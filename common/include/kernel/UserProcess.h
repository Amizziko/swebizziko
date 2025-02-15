#pragma once

#include "UserThread.h"
#include "umap.h"
#include "Mutex.h"
#include "Condition.h"
#include "helpers.h"
#include "MemoryManager.h"


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
    void kill();
    ~UserProcess();

    MemoryManager memory_manager_;
    Loader* loader_;
    ustl::string path_;
    FileSystemInfo *fs_info_;

//resources
    ustl::map<size_t, UserThread *> threads_; //locked by threads_lock_
    int32 fd_; //to be changed to a list of open fds maybe? todo
    size_t pid_;

    ustl::atomic<size_t> tid_counter_;


//locks
    Mutex threads_lock_;
    Condition threads_condition_;
    Mutex loader_lock_;


    bool loaderValid(const ustl::string &filename);
    size_t getNewTID();
    void addThread(UserThread *t);
    UserThread *createThread(thread_create::data &data);

    void killAllThreads();
};

