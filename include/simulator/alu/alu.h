//
// Created by jm1417 on 05/05/2021.
//

#ifndef SIMULATOR_ALU_H
#define SIMULATOR_ALU_H

#include "simulator/base/component.h"
#include "simulator/base/config.h"

class ALU : public Component {
    RTTR_ENABLE(Component);
   public:
    enum OPCODE {
        ADD,
        SUB,
        INC,
        DEC,
        CMP,
        MUL,
        DIV,
        AND,
        OR,
        XOR
    };

   private:
    int cycle_count_;
    double time_;
    int bits_;

#ifdef TRACK_STATISTICS
    int calc_transistor_count() override;
    double calc_static() override;
    double calc_dynamic() override;
#endif

   public:
    // Flags
    bool N = false; // Set on negative

   public:
    ALU() = default;
    void init();

    void set_bits(int bits);

    int execute(int a, int b, OPCODE opcode);

    void update_dynamic(int count);

#ifdef TRACK_STATISTICS
    void update_static(double time) override;
    int get_cycle_count() override;
    void print_stats(const CycleCounter &counter) override;
#endif
};

#endif  //SIMULATOR_ALU_H
