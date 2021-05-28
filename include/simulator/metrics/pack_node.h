//
// Created by jm1417 on 11/05/2021.
//

#ifndef SIMULATOR_PACK_NODE_H
#define SIMULATOR_PACK_NODE_H

#include <memory>

class PackNode {
   public:
    double x = -1;
    double  y = -1;
    double width = -1;
    double height = -1;
    std::shared_ptr<PackNode> right;
    std::shared_ptr<PackNode> down;
    bool used = false;
    bool empty = true;

    PackNode(double x, double y, double width, double height, std::shared_ptr<PackNode> down, std::shared_ptr<PackNode> right, bool used);

    PackNode(double x, double y, double width, double height);

    PackNode();
};

#endif  //SIMULATOR_PACK_NODE_H
