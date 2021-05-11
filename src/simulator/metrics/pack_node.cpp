//
// Created by jm1417 on 11/05/2021.
//
#include "simulator/metrics/pack_node.h"

PackNode::PackNode(int x, int y, int width, int height, PackNode* down, PackNode* right, bool used) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->down = down;
    this->right = right;
    this->used = used;
}

PackNode::PackNode(int x, int y, int width, int height) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
}
