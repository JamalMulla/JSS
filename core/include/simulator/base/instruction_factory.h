//
// Created by Jamal on 21/02/2021.
//

#ifndef SIMULATOR_INSTRUCTION_FACTORY_H
#define SIMULATOR_INSTRUCTION_FACTORY_H

#include <string>
#include <unordered_map>
#include <map>
#include <typeinfo>
#include <iostream>
#include <fstream>
#include <sstream>
#include <any>

template <class T, class... Args>
struct InstructionHolder{
    static std::unordered_map<std::string, void(T::*)(Args&...)>instructionMap;
};

template <class T, class... Args>
std::unordered_map<std::string, void(T::*)(Args&...)>InstructionHolder<T, Args...>::instructionMap;


struct RegisterHolder{
    std::unordered_map<std::string, std::any>registerMap;
};


template <class T>
class InstructionFactory {

public:
    template <class... Args>
    static void register_instruction(std::string name, void(T::*function)(Args&...)) {
        std::cout << "Instruction " << name << " registered " << std::endl;
        InstructionHolder<T, Args&...>::instructionMap[name] = function;
    }

    template <class... Args>
    static auto get_instruction(std::string name) {
        #ifndef NDEBUG
        if (InstructionHolder<T, Args&...>::instructionMap.find(name) == InstructionHolder<T, Args&...>::instructionMap.end()) {
            std::cerr << "Instruction " << name << " not found" << std::endl;
            exit(1);
        }
        #endif
        return InstructionHolder<T, Args&...>::instructionMap[name];
    }

    template <class... Args>
    static void execute(T& s, std::string name, Args &&... args) {
        #ifndef NDEBUG
        if (InstructionHolder<T, Args&...>::instructionMap.find(name) == InstructionHolder<T, Args&...>::instructionMap.end()) {
            std::cerr << "Instruction " << name << " not found" << std::endl;
            exit(1);
        }
        #endif
        (s.*InstructionHolder<T, Args&...>::instructionMap[name])(std::forward<Args>(args)...);
    }

};

class ArgumentFactory {
public:

    RegisterHolder* r;

    ArgumentFactory() {
        r = new RegisterHolder();
    }

    void register_arg(const std::string& name, const std::any& value) const {
        r->registerMap[name] = value;
        std::cout << "Added argument " << name  << std::endl;
    }

    [[nodiscard]] std::any& get_arg(const std::string& name) const {
        #ifndef NDEBUG
        if (r->registerMap.find(name) == r->registerMap.end()) {
            std::cerr << "Argument " << name <<  " not found" << std::endl;
            exit(1);
        }
        #endif
        return r->registerMap[name];
    }

    ~ArgumentFactory() {
        delete r;
    }

};

//
//class InstructionParser {
//
//    void parse_and_execute(std::string& program_file, int iterations);
//
//};
//
//void InstructionParser::parse_and_execute(std::string& program_file, int iterations) {
//
//    std::ifstream read(program_file);
//    std::vector<std::pair<std::string, std::vector<std::string>>> instructionArgs;
//
//    // read until you reach the end of the file
//    for (std::string line; std::getline(read, line); ) {
//
//        // removing punctuation like paranthesis, commas, etc.
//        std::replace_if(std::begin(line), std::end(line), [] (char x) { return std::ispunct(x); }, ' ');
//
//        // inserting the line into a stream that helps us parse the content
//        std::stringstream ss(line);
//
//        // read the node number
//        std::string instr;
//        ss >> instr;
//
//        // read the args until end of line
//        std::vector<std::string> args;
//        std::string arg;
//        while (ss >> arg) {
//            args.emplace_back(arg);
//        }
////        instructionArgs.emplace_back(std::make_pair<std::string, std::vector<std::string>>(instr, args));
//    }
//}


#endif //SIMULATOR_INSTRUCTION_FACTORY_H
