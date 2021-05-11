//
// Created by jm1417 on 11/05/2021.
//

#ifndef SIMULATOR_PACKER_H
#define SIMULATOR_PACKER_H

#include "simulator/base/component.h"
#include "simulator/metrics/pack_node.h"

class Packer {
    PackNode* root_ = nullptr;


    PackNode* find_node(PackNode* root, int width, int height);
    PackNode* grow_node(int width, int height);
    PackNode* grow_right(int width, int height);
    PackNode* grow_down(int width, int height);
    PackNode* split_node(PackNode* node, int width, int height);
    PackNode* pack(std::vector<Component>& components);
};

#endif  //SIMULATOR_PACKER_H
