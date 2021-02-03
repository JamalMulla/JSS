//
// Created by jm1417 on 31/01/2021.
//

#include "register.h"

Register::Register(int rows, int columns, int type)
    : value_(rows, columns, type) {}
