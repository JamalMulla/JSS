//
// Created by td1518 on 20/05/2021.
//

#include "scamp5_t.h"

#include <rttr/registration>

void SCAMP5T::init() {
    SCAMP5::init();
}

void SCAMP5T::hard_sigmoid(
        const std::shared_ptr<AnalogueRegister>& dst,
        const std::shared_ptr<AnalogueRegister>& src) {
    hard_sigmoid(dst, src);
}

RTTR_REGISTRATION {
    using namespace rttr;

    registration::class_<SCAMP5T>("SCAMP5T")
        .constructor()
        .method("init", &SCAMP5T::init)
        .method("hard_sigmoid", &SCAMP5T::hard_sigmoid);
};
