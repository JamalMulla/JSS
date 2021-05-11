//
// Created by jm1417 on 11/05/2021.
//
#include <utility>

#include "simulator/metrics/pack_node.h"

PackNode::PackNode(int x, int y, int width, int height, std::shared_ptr<PackNode> down, std::shared_ptr<PackNode> right, bool used) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->down = std::move(down);
    this->right = std::move(right);
    this->used = used;
    this->empty = false;
}

PackNode::PackNode(int x, int y, int width, int height) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->empty = false;
}

PackNode::PackNode() {
    this->empty = true;
}
