//
// Created by jm1417 on 28/01/2021.
//

#include "processing_element.h"

ProcessingElement::ProcessingElement(int rows, int columns, int num_analogue_regs, int num_digital_regs) {
    for (int i = 0; i < num_analogue_regs; i++) {
        analogue_registers.emplace_back(AnalogueRegister());
    }
    for (int i = 0; i < num_digital_regs; i++) {
        digital_registers.emplace_back(DigitalRegister());
    }
    photodiode = Photodiode(rows, columns);
}
