//
// Created by Jamal on 21/02/2021.
//

#ifndef SIMULATOR_INSTRUCTION_FACTORY_H
#define SIMULATOR_INSTRUCTION_FACTORY_H

#include <string>
#include <unordered_map>
#include <map>
#include <typeinfo>
#include <typeindex>
#include <iostream>
#include <fstream>
#include <sstream>
#include <variant>
#include <any>
#include <algorithm>
#include <functional>
#include <simulator/registers/analogue_register.h>
#include <simulator/registers/digital_register.h>


template <class T, class... Args>
struct InstructionHolder{
    static std::unordered_map<std::string, void(T::*)(Args&...)>instructionMap;
};

template <class T, class... Args>
std::unordered_map<std::string, void(T::*)(Args&...)>InstructionHolder<T, Args...>::instructionMap;


struct RegisterHolder{
    std::unordered_map<std::string, std::any>registerMap;
};


//template <class T>
//class InstructionFactory {
//
//public:
////    InstructionHolder<T>* i;
//
//    InstructionFactory() {
////        i = new InstructionHolder<T>();
//    }
//
//    template<class...Args>
//    void register_instruction(const std::string& name, void(T::*function)(Args&...)) {
//        std::vector<std::type_index> vec;
//        vec.insert(vec.end(), {typeid(Args)...});
//        for (auto i: vec)
//            std::cout << i.name() << ' ';
//
//        std::cout << "Instruction " << name << " registered " << std::endl;
//        InstructionHolder<T, Args&...>::instructionMap[name] = function;
//    }
//
//    template<class...Args>
//    void execute(T& s, const std::string& name, Args&&... args) {
//        std::vector<std::type_index> vec;
//        vec.insert(vec.end(), {typeid(Args)...});
//        for (auto i: vec)
//            std::cout << i.name() << ' ';
//
//        if (InstructionHolder<T, Args...>::instructionMap.find(name) == InstructionHolder<T, Args...>::instructionMap.end()) {
//            std::cerr << "Instruction " << name << " not found" << std::endl;
//            exit(1);
//        }
//        (s.*InstructionHolder<T, Args&...>::instructionMap[name])(std::forward<Args>(args)...);
//    }
//
//
//
//};

template <class T>
class InstructionFactory {

public:

    template <class... Args>
    static void register_instruction(const std::string& name, void(T::*function)(Args&...)) {
        std::cout << "Instruction " << name << " registered " << std::endl;
        std::vector<std::type_index> vec;
        vec.insert(vec.end(), {typeid(Args)...});
        for (auto i: vec)
            std::cout << i.name() << ' ';
        std::cout << std::endl;
        InstructionHolder<T, Args&...>::instructionMap[name] = function;
    }

    template <class... Args>
    static auto get_instruction(const std::string& name) {
        #ifndef NDEBUG
        std::vector<std::type_index> vec;
        vec.insert(vec.end(), {typeid(Args)...});
        for (auto i: vec)
            std::cout << i.name() << ' ';
        std::cout << std::endl;

        if (InstructionHolder<T, Args&...>::instructionMap.find(name) == InstructionHolder<T, Args&...>::instructionMap.end()) {
            std::cerr << "Instruction " << name << " not found" << std::endl;
            exit(1);
        }
        #endif
        return InstructionHolder<T, Args&...>::instructionMap[name];
    }

    template <class... Args>
    static void execute(T& s, const std::string& name, Args &&... args) {
        #ifndef NDEBUG
        std::vector<std::type_index> vec;
        vec.insert(vec.end(), {typeid(Args)...});
        for (auto i: vec)
            std::cout << i.name() << ' ';
        std::cout << std::endl;

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

    [[nodiscard]] std::any get_arg(const std::string& name) const {
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

template <class T>
class InstructionParser {

private:
    ArgumentFactory& args_;
public:

    explicit InstructionParser(ArgumentFactory& args) :  args_(args) { }

    void parse(T& s, const std::string& program_file, int iterations);
    std::any get_arg(const std::string& name);
    std::variant<AnalogueRegister*, DigitalRegister*> cast_arg(std::any& arg);

};

using namespace std::placeholders;  // for _1, _2, _3...


template<class T>
void InstructionParser<T>::parse(T& s, const std::string& program_file, int iterations) {

    std::ifstream file(program_file);

    if(!file.is_open()) {
        std::cerr << "Error opening file " << program_file << std::endl;
        exit(EXIT_FAILURE);
    }

    std::vector<std::pair<std::string, std::vector<std::string>>> instructionArgs;

    // read until you reach the end of the file

    std::string line;
    while (std::getline(file, line)) {

        // inserting the line into a stream that helps us parse the content

        unsigned first = line.find('(');
        unsigned last = line.find(')');
        std::string arg_string = line.substr (first+1, last-first-1);

        //replace commas with space
        std::replace(arg_string.begin(), arg_string.end(), ',', ' ');
        std::stringstream arg_stream(arg_string);

        std::replace(line.begin(), line.end(), '(', ' ');
        std::replace(line.begin(), line.end(), ')', ' ');

        // read the node number
        std::stringstream ss(line);
        std::string instr;
        ss >> instr;

        std::cout << "Executing instruction " << instr << std::endl;

        // read the args until end of line
        std::vector<std::any> args;
        std::string arg;
        while (arg_stream >> arg) {
            std::cout << "argument: " << arg << std::endl;
            args.emplace_back(get_arg(arg));
        }

        switch (args.size()) {
            case 0: {
                InstructionFactory<T>::execute(s, instr);
                break;
            }
            case 1: {
                auto f1 = std::bind(&InstructionFactory<T>::template execute<AnalogueRegister>, s, instr, _1);

                auto arg1 = args.at(0);

                if (arg1.type() == typeid(AnalogueRegister*)) {
                    f1 = std::bind(f1, *(std::any_cast<AnalogueRegister*>(arg1)));
                } else if (arg1.type() == typeid(DigitalRegister*)) {
                    f1 = std::bind(f1, *(std::any_cast<DigitalRegister*>(arg1)));
                } else {
                    std::cerr << "Unable to cast arg of type " << arg1.type().name() << std::endl;
                    exit(1);
                }
                
                f1();
                break;
            }
            case 2: {
                auto f1 = std::bind(&InstructionFactory<T>::template execute<AnalogueRegister, AnalogueRegister>, s, instr, _1);

                auto arg1 = args.at(0);
                auto arg2 = args.at(1);

                if (arg1.type() == typeid(AnalogueRegister*)) {
                    f1 = std::bind(f1, *(std::any_cast<AnalogueRegister*>(arg1)), _1);
                } else if (arg1.type() == typeid(DigitalRegister*)) {
                    f1 = std::bind(f1, *(std::any_cast<DigitalRegister*>(arg1)), _1);
                } else {
                    std::cerr << "Unable to cast arg of type " << arg1.type().name() << std::endl;
                    exit(1);
                }

                if (arg2.type() == typeid(AnalogueRegister*)) {
                    f1 = std::bind(f1, *(std::any_cast<AnalogueRegister*>(arg2)));
                } else if (arg2.type() == typeid(DigitalRegister*)) {
                    f1 = std::bind(f1, *(std::any_cast<DigitalRegister*>(arg2)));
                } else {
                    std::cerr << "Unable to cast arg of type " << arg2.type().name() << std::endl;
                    exit(1);
                }

                f1();
                break;
            }
            default: {
                std::cerr << "Cannot support more than 2 arguments at this time" << std::endl;
                exit(1);
            }
        }



//        instructionArgs.emplace_back(std::make_pair<std::string, std::vector<std::string>>(instr, args));


    }

}

constexpr unsigned int hash(const char *s, int off = 0) {
    return !s[off] ? 5381 : (hash(s, off+1)*33) ^ s[off];
}

constexpr inline unsigned int operator "" _(char const * p, size_t) { return hash(p); }

template<class T>
std::any InstructionParser<T>::get_arg(const std::string& name) {
    return this->args_.get_arg(name);
}

template<class T>
std::variant<AnalogueRegister*, DigitalRegister*> InstructionParser<T>::cast_arg(std::any& arg) {
    if (arg.type() == typeid(AnalogueRegister*)) {
        return std::any_cast<AnalogueRegister*>(arg);
    } else if (arg.type() == typeid(DigitalRegister*)) {
        return std::any_cast<DigitalRegister*>(arg);
    } else {
        std::cerr << "Unable to cast arg of type " << arg.type().name() << std::endl;
        exit(1);
    }

}


#endif //SIMULATOR_INSTRUCTION_FACTORY_H
