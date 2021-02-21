//
// Created by Jamal on 21/02/2021.
//

#ifndef SIMULATOR_INSTRUCTION_FACTORY_H
#define SIMULATOR_INSTRUCTION_FACTORY_H

#include <string>
#include <unordered_map>
#include <map>
#include <typeinfo>

template <class T, class... Args>
struct InstructionHolder{
    static std::unordered_map<const char *, void(T::*)(Args&...)>instructionMap;
};

template <class T, class... Args>
std::unordered_map<const char *, void(T::*)(Args&...)>InstructionHolder<T, Args...>::instructionMap;

template <class U>
struct RegisterHolder{
    static std::unordered_map<const char *, U*>registerMap;
};

template <class U>
std::unordered_map<const char *, U*>RegisterHolder<U>::registerMap;


template <class T>
class InstructionFactory {

public:
    template <class... Args>
    static void register_instruction(const char *name, void(T::*function)(Args&...)) {
        InstructionHolder<T, Args&...>::instructionMap[name] = function;
    }

    template <class... Args>
    static auto get_instruction(const char * name) {
        if (InstructionHolder<T, Args&...>::instructionMap.find(name) == InstructionHolder<T, Args&...>::instructionMap.end()) {
            std::cerr << "Instruction " << name << " not found" << std::endl;
            exit(1);
        }
        return InstructionHolder<T, Args&...>::instructionMap[name];
    }

    template <class... Args>
    static void execute(T& s, const char * name, Args &&... args) {
        if (InstructionHolder<T, Args&...>::instructionMap.find(name) == InstructionHolder<T, Args&...>::instructionMap.end()) {
            std::cerr << "Instruction " << name << " not found" << std::endl;
            exit(1);
        }
        (s.*InstructionHolder<T, Args&...>::instructionMap[name])(std::forward<Args>(args)...);
    }

};

class RegisterFactory {
public:

    template <class U>
    static void register_register(const char * name, U* type) {
        RegisterHolder<U>::registerMap[name] = type;
    }

    template <class U>
    static auto get_register(const char * name) {
        if (RegisterHolder<U>::registerMap.find(name) == RegisterHolder<U>::registerMap.end()) {
            std::cerr << "Register " << name << " of type " << typeid(U).name() << " not found" << std::endl;
            exit(1);
        }
        return RegisterHolder<U>::registerMap[name];
    }

};




#endif //SIMULATOR_INSTRUCTION_FACTORY_H
