//
// Created by jm1417 on 28/01/2021.
//

#ifndef SIMULATOR_PROCESSING_ELEMENT_H
#define SIMULATOR_PROCESSING_ELEMENT_H


#include "photodiode.h"
#include "../registers/analogue_register.h"
#include "../registers/digital_register.h"
#include <vector>
#include <iostream>

class ProcessingElement {
private:
    Photodiode photodiode;
    std::vector<AnalogueRegister> analogue_registers;
    std::vector<DigitalRegister> digital_registers;
    

};


#endif //SIMULATOR_PROCESSING_ELEMENT_H
