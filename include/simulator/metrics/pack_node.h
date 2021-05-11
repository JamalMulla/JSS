//
// Created by jm1417 on 11/05/2021.
//

#ifndef SIMULATOR_PACK_NODE_H
#define SIMULATOR_PACK_NODE_H

#include <memory>

class PackNode {
   public:
    int x = -1;
    int y = -1;
    int width = -1;
    int height = -1;
    std::shared_ptr<PackNode> right;
    std::shared_ptr<PackNode> down;
    bool used = false;
    bool empty = true;

    PackNode(int x, int y, int width, int height, std::shared_ptr<PackNode> down, std::shared_ptr<PackNode> right, bool used);

    PackNode(int x, int y, int width, int height);

    PackNode();
};

#endif  //SIMULATOR_PACK_NODE_H
