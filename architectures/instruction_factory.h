//
// Created by Jamal on 21/02/2021.
//

#ifndef SIMULATOR_INSTRUCTION_FACTORY_H
#define SIMULATOR_INSTRUCTION_FACTORY_H

#include <string>
#include <unordered_map>
#include <map>

template <class T, class... Args>
struct MapHolder{
    static std::unordered_map<std::string, void(T::*)(Args&...)>instructionMap;
};

template <class T, class... Args>
std::unordered_map<std::string, void(T::*)(Args&...)>MapHolder<T, Args...>::instructionMap;

template <class T>
class InstructionFactory {

public:

    template <class... Args>
    static void register_instruction(std::string name, void(T::*function)(Args&...)) {
        MapHolder<T, Args&...>::instructionMap[name] = function;
    }

    template <class... Args>
    static auto get_instruction(const std::string &name) {
        if (MapHolder<T, Args&...>::instructionMap.find(name) == MapHolder<T, Args&...>::instructionMap.end()) {
            std::cerr << "Instruction " << name << " not found" << std::endl;
            exit(0);
        }
        return MapHolder<T, Args&...>::instructionMap[name];
    }

    template <class... Args>
    static void execute(T& s, const std::string &name, Args &&... args) {
        if (MapHolder<T, Args&...>::instructionMap.find(name) == MapHolder<T, Args&...>::instructionMap.end()) {
            std::cerr << "Instruction " << name << " not found" << std::endl;
            exit(0);
        }
        (s.*MapHolder<T, Args&...>::instructionMap[name])(std::forward<Args>(args)...);
    }

};




#endif //SIMULATOR_INSTRUCTION_FACTORY_H
