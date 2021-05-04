//
// Created by jm1417 on 05/03/2021.
//

#include "simulator/ui/file_watcher.h"

#include <chrono>
#include <filesystem>
#include <functional>
#include <string>
#include <thread>
#include <unordered_map>

// Monitor "path_to_watch" for changes and in case of a change execute_instructions the user
// supplied "action" function
void FileWatcher::start(
    const std::function<void(std::string, FileStatus)> &action) {
    while(running_) {
        // Wait for "delay" milliseconds
        std::this_thread::sleep_for(delay);

        auto it = paths_.begin();
        while(it != paths_.end()) {
            if(!std::filesystem::exists(it->first)) {
                action(it->first, FileStatus::erased);
                it = paths_.erase(it);
            } else {
                it++;
            }
        }

        // Check if a file was created or modified
        for(auto &file:
            std::filesystem::recursive_directory_iterator(path_to_watch)) {
            auto current_file_last_write_time =
                std::filesystem::last_write_time(file);

            // File creation
            if(!contains(file.path().string())) {
                paths_[file.path().string()] = current_file_last_write_time;
                action(file.path().string(), FileStatus::created);
                // File modification
            } else {
                if(paths_[file.path().string()] !=
                   current_file_last_write_time) {
                    paths_[file.path().string()] = current_file_last_write_time;
                    action(file.path().string(), FileStatus::modified);
                }
            }
        }
    }
}

bool FileWatcher::contains(const std::string &key) {
    // Check if "paths_" contains a given key
    // If your compiler supports C++20 use paths_.contains(key) instead of this
    // function
    auto el = paths_.find(key);
    return el != paths_.end();
}
