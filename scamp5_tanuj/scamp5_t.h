//
// Created by td1518 on 20/05/2021.
//

#ifndef SIMULATOR_SCAMP5_T_H
#define SIMULATOR_SCAMP5_T_H

#include "../scamp5/scamp5.h"

/*Tanuj SCAMP5*/

class SCAMP5T : public SCAMP5 {
   RTTR_ENABLE(SCAMP5);

   public:
    SCAMP5T() = default;
    void init();

    void hard_sigmoid(const std::shared_ptr<AnalogueRegister>&dst, const std::shared_ptr<AnalogueRegister>&src);
};


#endif  //SIMULATOR_SCAMP5_T_H
