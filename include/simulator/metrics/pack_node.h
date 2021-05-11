//
// Created by jm1417 on 11/05/2021.
//

#ifndef SIMULATOR_PACK_NODE_H
#define SIMULATOR_PACK_NODE_H

class PackNode {
   public:
    int x;
    int y;
    int width;
    int height;
    PackNode* right;
    PackNode* down;
    bool used = false;

    PackNode(int x, int y, int width, int height, PackNode* down, PackNode* right, bool used);

    PackNode(int x, int y, int width, int height);
};

#endif  //SIMULATOR_PACK_NODE_H
