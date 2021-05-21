//
// Created by jm1417 on 20/05/2021.
//

#ifndef SIMULATOR_IMAGE_H
#define SIMULATOR_IMAGE_H

struct Image
{
    int width;
    int height;
    int reg;
    int flag;
    Image() = default;
    Image(int width, int height, int reg, int flag) {
        this->width = width;
        this->height = height;
        this->reg = reg;
        this->flag = flag;
    }

};

typedef struct
{
    int width;
    int height;
} Size;

typedef struct
{
    int x;
    int y;
} Point;

#endif  //SIMULATOR_IMAGE_H
