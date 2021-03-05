//
// Created by jm1417 on 05/03/2021.
//

#ifndef SIMULATOR_FILE_WATCHER_H
#define SIMULATOR_FILE_WATCHER_H

#include <functional>
#include <filesystem>

enum class FileStatus {
    created, modified, erased
};

class FileWatcher {
private:
    std::unordered_map<std::string, std::filesystem::file_time_type> paths_;
    bool running_ = true;
    bool contains(const std::string &key);

public:
    std::string path_to_watch;
    // Time interval at which we check the base folder for changes
    std::chrono::duration<int, std::milli> delay;

    // Keep a record of files from the base directory and their last modification time
    FileWatcher(const std::string &path_to_watch, std::chrono::duration<int, std::milli> delay) : path_to_watch{
            path_to_watch}, delay{delay} {
        for (auto &file : std::filesystem::recursive_directory_iterator(path_to_watch)) {
            paths_[file.path().string()] = std::filesystem::last_write_time(file);
        }
    }

    void start(const std::function<void(std::string, FileStatus)> &action);
};


#endif //SIMULATOR_FILE_WATCHER_H
