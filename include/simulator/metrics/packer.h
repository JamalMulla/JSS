//
// Created by jm1417 on 11/05/2021.
//

#ifndef SIMULATOR_PACKER_H
#define SIMULATOR_PACKER_H

#include "simulator/base/component.h"
#include "simulator/metrics/pack_node.h"

class Packer {
    std::shared_ptr<PackNode> root_ = std::make_shared<PackNode>();

    std::shared_ptr<PackNode> find_node(std::shared_ptr<PackNode> root, int width, int height);
    std::shared_ptr<PackNode> grow_node(int width, int height);
    std::shared_ptr<PackNode> grow_right(int width, int height);
    std::shared_ptr<PackNode> grow_down(int width, int height);
    std::shared_ptr<PackNode> split_node(std::shared_ptr<PackNode> node, int width, int height);

   public:
    std::shared_ptr<PackNode> pack(std::vector<std::shared_ptr<Component> >& components);
};

#endif  //SIMULATOR_PACKER_H
