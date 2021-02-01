//
// Created by jm1417 on 22/12/2020.
//

#include <opencv2/imgproc/types_c.h>
#include "PE.h"


PE::PE() {
    IN = cv::Mat(SCAMP_HEIGHT, SCAMP_WIDTH, MAT_TYPE);
    PIX = cv::Mat(SCAMP_HEIGHT, SCAMP_WIDTH, MAT_TYPE);
    A = cv::Mat(SCAMP_HEIGHT, SCAMP_WIDTH, MAT_TYPE);
    B = cv::Mat(SCAMP_HEIGHT, SCAMP_WIDTH, MAT_TYPE);
    C = cv::Mat(SCAMP_HEIGHT, SCAMP_WIDTH, MAT_TYPE);
    D = cv::Mat(SCAMP_HEIGHT, SCAMP_WIDTH, MAT_TYPE);
    E = cv::Mat(SCAMP_HEIGHT, SCAMP_WIDTH, MAT_TYPE);
    F = cv::Mat(SCAMP_HEIGHT, SCAMP_WIDTH, MAT_TYPE);
    NEWS = cv::Mat(SCAMP_HEIGHT, SCAMP_WIDTH, MAT_TYPE);
    XN = cv::Mat(SCAMP_HEIGHT, SCAMP_WIDTH, MAT_TYPE);
    XE = cv::Mat(SCAMP_HEIGHT, SCAMP_WIDTH, MAT_TYPE);
    XS = cv::Mat(SCAMP_HEIGHT, SCAMP_WIDTH, MAT_TYPE);
    XW = cv::Mat(SCAMP_HEIGHT, SCAMP_WIDTH, MAT_TYPE);
    FLAG = cv::Mat(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8U, 255);
}


bool matIsEqual(const cv::Mat mat1, const cv::Mat mat2){
    // treat two empty mat as identical as well
    if (mat1.empty() && mat2.empty()) {
        return true;
    }
    // if dimensionality of two mat is not identical, these two mat is not identical
    if (mat1.cols != mat2.cols || mat1.rows != mat2.rows || mat1.dims != mat2.dims) {
        return false;
    }
    cv::Mat diff;
    cv::compare(mat1, mat2, diff, cv::CMP_NE);
    int nz = cv::countNonZero(diff);
    return nz==0;
}

void printMatrix(const cv::Mat& mat, std::string m) {
    std::cout << mat.size << std::endl;
    std::cout << m << " " << mat << std::endl;
}

void PE::pushToNews(AREG& src, news_t dir) {
    switch (dir) {
        case east: {
            auto chunk = cv::Rect(1, 0, SCAMP_WIDTH - 1, SCAMP_HEIGHT);
            src(cv::Rect(0, 0, SCAMP_WIDTH - 1, SCAMP_HEIGHT)).copyTo(NEWS(chunk), FLAG(chunk));
            NEWS(cv::Rect(0, 0, 1, SCAMP_HEIGHT)).setTo(cv::Scalar(-1), FLAG(cv::Rect(0, 0, 1, SCAMP_HEIGHT)));
            break;
        }
        case west: {
            auto chunk = cv::Rect(0, 0, SCAMP_WIDTH - 1, SCAMP_HEIGHT);
            src(cv::Rect(1, 0, SCAMP_WIDTH - 1, SCAMP_HEIGHT)).copyTo(NEWS(chunk), FLAG(chunk));
            NEWS(cv::Rect(SCAMP_WIDTH - 1, 0, 1, SCAMP_HEIGHT)).setTo(cv::Scalar(-1),FLAG(cv::Rect(SCAMP_WIDTH - 1, 0, 1, SCAMP_HEIGHT)));
            break;
        }
        case north: {
            auto chunk = cv::Rect(0, 0, SCAMP_WIDTH, SCAMP_HEIGHT - 1);
            src(cv::Rect(0, 1, SCAMP_WIDTH, SCAMP_HEIGHT - 1)).copyTo(NEWS(chunk), FLAG(chunk));
            NEWS(cv::Rect(0, SCAMP_HEIGHT - 1, SCAMP_WIDTH, 1)).setTo(cv::Scalar(-1),FLAG(cv::Rect(0, SCAMP_HEIGHT - 1, SCAMP_WIDTH,1)));
            break;
        }
        case south: {
            auto chunk = cv::Rect(0, 1, SCAMP_WIDTH, SCAMP_HEIGHT - 1);
            src(cv::Rect(0, 0, SCAMP_WIDTH, SCAMP_HEIGHT - 1)).copyTo(NEWS(chunk), FLAG(chunk));
            NEWS(cv::Rect(0, 0, SCAMP_WIDTH, 1)).setTo(cv::Scalar(-1), FLAG(cv::Rect(0, 0, SCAMP_WIDTH, 1)));
            break;
        }
        default:
            break;
    }
}


void PE::pullFromNews(AREG& dst, news_t dir) {
    switch (dir) {
        case west: {
            auto chunk = cv::Rect(0, 0, SCAMP_WIDTH - 1, SCAMP_HEIGHT);
            auto n = NEWS(cv::Rect(1, 0, SCAMP_WIDTH - 1, SCAMP_HEIGHT));
            n.copyTo(dst(chunk), FLAG(chunk));
            dst(cv::Rect(SCAMP_WIDTH - 1, 0, 1, SCAMP_HEIGHT)).setTo(cv::Scalar(-1), FLAG(cv::Rect(SCAMP_WIDTH - 1, 0, 1, SCAMP_HEIGHT)));
            break;
        }
        case east: {
            auto chunk = cv::Rect(1, 0, SCAMP_WIDTH - 1, SCAMP_HEIGHT);
            NEWS(cv::Rect(0, 0, SCAMP_WIDTH - 1, SCAMP_HEIGHT)).copyTo(dst(chunk), FLAG(chunk));
            dst(cv::Rect(0, 0, 1, SCAMP_HEIGHT)).setTo(cv::Scalar(-1), FLAG(cv::Rect(0, 0, 1, SCAMP_HEIGHT)));
            break;
        }
        case south: {
            auto chunk = cv::Rect(0, 1, SCAMP_WIDTH, SCAMP_HEIGHT - 1);
            NEWS(cv::Rect(0, 0, SCAMP_WIDTH, SCAMP_HEIGHT - 1)).copyTo(dst(chunk), FLAG(chunk));
            dst(cv::Rect(0, 0, SCAMP_WIDTH, 1)).setTo(cv::Scalar(-1), FLAG(cv::Rect(0, 0, SCAMP_WIDTH, 1)));
            break;
        }
        case north: {
            auto chunk = cv::Rect(0, 0, SCAMP_WIDTH - 1, SCAMP_HEIGHT);
            NEWS(cv::Rect(1, 0, SCAMP_WIDTH - 1, SCAMP_HEIGHT)).copyTo(dst(chunk), FLAG(chunk));
            dst(cv::Rect(SCAMP_WIDTH - 1, 0, 1, SCAMP_HEIGHT)).setTo(cv::Scalar(-1), FLAG(cv::Rect(SCAMP_WIDTH - 1, 0, 1, SCAMP_HEIGHT)));
            break;
        }
        default:
            break;
    }
}

// Misc

void PE::nop() {}

// Image Capturing
void PE::rpix() {
    //reset PIX
    this->PIX.setTo(0);
}

void PE::get_image(AREG &y) {
    //y := half-range image, and reset PIX
    this->bus(NEWS, PIX);
    this->rpix();
    this->rpix();
    this->nop();
    this->bus(y, NEWS, PIX);
}

void PE::get_image(AREG &y, AREG &h) {
    //y := full-range image, h := negative half-range image, and reset PIX
    this->bus(NEWS, PIX);
    this->bus(h, PIX);
    this->rpix();
    this->rpix();
    this->nop();
    this->bus(y, h, NEWS, PIX);
}

void PE::respix() {
    //reset PIX
    this->rpix();
    this->rpix();
    this->nop();
}

void PE::respix(AREG &y) {
    //reset PIX, keep its reset level in y
    this->rpix();
    this->rpix();
    this->nop();
    this->bus(NEWS, PIX);
    this->bus(y, NEWS);
}

void PE::getpix(AREG &y, AREG &pix_res) {
    //y := half-range image, supplying the reset level of PIX
    this->bus(NEWS,PIX);
    this->bus(y,NEWS, pix_res);
}

void PE::getpix(AREG &y, AREG &h, AREG &pix_res) {
    //y := full-range, h := half-range image, supplying the reset level of PIX
    this->bus(h,PIX);
    this->bus(NEWS, PIX);
    this->bus(y, h, NEWS, pix_res);
}

// Analog Register Transfer

//void PE::all() {
//    //FLAG := 1.
//    this->FLAG.setTo(1);
//}

//void PE::mov(AREG& y, const AREG& x0) {
//    //y = x0
//    this->bus(NEWS, x0);
//    this->bus(y, NEWS);
//}

// Analog Arithmetic
void PE::res(AREG& a) {
    // a = 0
    this->bus(NEWS);
    this->bus(a, NEWS);
}

void PE::res(AREG& a, AREG& b) {
    // a = 0, b := 0
    this->bus(NEWS);
    this->bus(a, NEWS);
    this->bus(b, NEWS);
}

//void PE::add(AREG& y, const AREG& x0, const AREG& x1) {
//    // y = x0 + x1
//    this->bus(NEWS, x0, x1);
//    this->bus(y, NEWS);
//}
//
//void PE::add(AREG& y, const AREG& x0, const AREG& x1, const AREG& x2) {
//    // y = x0 + x1 + x2
//    this->bus(NEWS, x0, x1, x2);
//    this->bus(y, NEWS);
//}
//
//void PE::sub(AREG& y, const AREG& x0, const AREG& x1) {
//    // y = x0 - x1
//    this->bus(NEWS, x0);
//    this->bus(y, NEWS, x1);
//}

//void PE::neg(AREG& y, const AREG& x0) {
//    // y = -x0
//    this->bus(NEWS);
//    this->bus(y, NEWS, x0);
//}

//void PE::abs(AREG& y, const AREG& x0) {
//    // y = |x0|
//    this->bus(NEWS);
//    this->bus(y, NEWS, x0);
//    this->bus(NEWS, x0);
//    this->where(x0);
//    this->bus(y, NEWS);
//    this->all();
//}

//void PE::div(AREG& y0, AREG& y1, AREG& y2) {
//    // y0 := 0.5*y2; y1 := -0.5*y2 + error, y2 := y2 + error
//    this->bus2(y0, y1, y2);
//    this->bus(NEWS, y2, y1);
//    this->bus(y2, NEWS, y0);
//    this->bus2(y0, y1, y2);
//    this->bus(y0, y1);
//}
//
//void PE::div(AREG& y0, AREG& y1, AREG& y2, const AREG& x0) {
//    // y0 := 0.5*x0; y1 := -0.5*x0 + error, y2 := x0 + error
//    this->bus2(y0, y1, x0);
//    this->bus(NEWS, x0, y1);
//    this->bus(y2,NEWS, y0);
//    this->bus2(y0, y1, y2);
//    this->bus(y0, y1);
//}
//
//void PE::diva(AREG& y0, AREG& y1, AREG& y2) {
//    // y0 := 0.5*y0; y1 := -0.5*y0 + error, y2 := -0.5*y0 + error
//    this->bus2(y1, y2, y0);
//    this->bus(NEWS, y1, y0);
//    this->bus(y0, NEWS, y2);
//    this->bus2(y1, y2, y0);
//    this->bus(y0, y1);
//}
//
//void PE::divq(AREG& y0, const AREG& x0) {
//    // y0 := 0.5*x0 + error
//    this->bus2(y0, NEWS, x0);
//    this->bus(y0, NEWS);
//}

// Analog Neighbour Access
void PE::movx(AREG& y, const AREG& x0, const news_t dir) {
    // y = x0_dir

    //TODO What is write on read??
    AREG intermediate;
    this->bus(intermediate, x0);
    this->pushToNews(intermediate, dir);
    this->bus(y, NEWS);
}

void PE::mov2x(AREG& y, const AREG& x0, const news_t dir, const news_t dir2) {
    // y = x0_dir_dir (note: this only works when FLAG is "all")
    AREG intermediate;
    AREG intermediate2(SCAMP_HEIGHT, SCAMP_WIDTH, MAT_TYPE);
    this->bus(intermediate, x0);
    this->pushToNews(intermediate, dir);
    this->pullFromNews(intermediate2, dir2);
    this->bus(y, intermediate2);
}

void PE::addx(AREG& y, const AREG& x0, const AREG& x1, const news_t dir) {
    // y = x0_dir + x1_dir
    AREG intermediate;
    this->bus(intermediate, x0, x1);
    this->pushToNews(intermediate, dir);
    this->bus(y, NEWS);
}

void PE::add2x(AREG& y, const AREG& x0, const AREG& x1, const news_t dir, const news_t dir2) {
    // y = x0_dir_dir2 + x1_dir_dir2

    AREG intermediate;
    AREG intermediate2(SCAMP_HEIGHT, SCAMP_WIDTH, MAT_TYPE);

    this->bus(intermediate, x0, x1);
    this->pushToNews(intermediate, dir);
    this->pullFromNews(intermediate2, dir2);
    this->bus(y, intermediate2);
}

void PE::subx(AREG& y, const AREG& x0, const news_t dir, const AREG& x1) {
    // y = x0_dir - x1
    AREG intermediate;
    this->bus(intermediate, x0);
    this->pushToNews(intermediate, dir);
    this->bus(y, NEWS, x1);
}

void PE::sub2x(AREG& y, const AREG& x0, const news_t dir, const news_t dir2, const AREG& x1) {
    // y = x0_dir_dir2 - x1
    AREG intermediate;
    AREG intermediate2(SCAMP_HEIGHT, SCAMP_WIDTH, MAT_TYPE);
    this->bus(intermediate, x0);
    this->pushToNews(intermediate, dir);
    this->pullFromNews(intermediate2, dir2);
    this->bus(y, intermediate2, x1);
}

// Asynchronized Blur

// Digital Logic Operations
//void PE::OR(DREG& d, DREG& d0, DREG& d1) {
//    // d := d0 OR d1
//    cv::bitwise_or(d0, d1, d, FLAG);
//}
//
//void PE::OR(DREG& d, DREG& d0, DREG& d1, DREG& d2) {
//    // d := d0 OR d1 OR d2
//    cv::bitwise_or(d0, d1, d, FLAG);
//    cv::bitwise_or(d, d2, d, FLAG);
//}
//
//void PE::OR(DREG& d, DREG& d0, DREG& d1, DREG& d2, DREG& d3) {
//    // d := d0 OR d1 OR d2 OR d3
//    cv::bitwise_or(d0, d1, d, FLAG);
//    cv::bitwise_or(d, d2, d, FLAG);
//    cv::bitwise_or(d, d3, d, FLAG);
//}
//
//void PE::NOT(DREG& d, DREG& d0) {
//    // d := NOT d0
//    cv::bitwise_not(d0, d);
//}
//
//void PE::NOR(DREG& d, DREG& d0, DREG& d1) {
//    // d := NOT(d0 OR d1)
//    this->OR(d, d0, d1);
//    this->NOT(d);
//}
//
//void PE::NOR(DREG& d, DREG& d0, DREG& d1, DREG& d2) {
//
//}
//
//void PE::NOR(DREG& d, DREG& d0, DREG& d1, DREG& d2, DREG& d3) {
//
//}
//
//void PE::NOT(DREG& Rl) {
//    // logic operation Rl := NOT Rl
//    cv::bitwise_not(Rl, Rl);
//}
//
//void PE::OR(DREG& Rl, DREG& Rx) {
//    // logic operation Rl := Rl OR Rx
//    this->OR(Rl, Rl, Rx);
//}
//
//void PE::NOR(DREG& Rl, DREG& Rx) {
//    // logic operation Rl := Rl NOR Rx
//    this->NOR(Rl, Rl, Rx);
//}
//
//void PE::AND(DREG& Ra, DREG& Rx, DREG& Ry) {
//    //  Ra := Rx AND Ry; R0 = NOT Ry; R12 = NOT RX
//    this->SET(RF);
//    this->NOT(RP, Rx);
//    this->NOT(RF, Ry);
//    this->NOR(Ra, RF, RP);
//}
//
//void PE::NAND(DREG& Ra, DREG& Rx, DREG& Ry) {
//    // Ra := Rx NAND Ry; R0 = NOT Ry; R12 = NOT RX
//    this->SET(RF);
//    this->NOT(RP, Rx);
//    this->NOT(RF, Ry);
//    this->OR(Ra, RF, RP);
//}
//
//void PE::ANDX(DREG& Ra, DREG& Rb, DREG& Rx) {
//    // Ra := Rb AND Rx; Rb := NOT Rx; R0 = NOT Rb
//    this->NOT(RF, Rb);
//    this->NOT(Rb, Rx);
//    this->NOR(Ra, RF, Rb);
//}
//
//void PE::NANDX(DREG& Ra, DREG& Rb, DREG& Rx) {
//    // Ra := Rx NAND Ry; Rb := NOT Rx; R0 = NOT Rb
//    this->NOT(RF, Rb);
//    this->NOT(Rb, Rx);
//    this->OR(Ra, RF, Rb);
//}
//
//void PE::IMP(DREG& Rl, DREG& Rx, DREG& Ry) {
//    // Rl := Rx IMP Ry (logical implication)
//    //    Truth Table:
//    //    Rx  Ry    Rl
//    //    0   0     1
//    //    0   1     0
//    //    1   0     1
//    //    1   1     1
//    this->NOT(RF, Ry);
//    this->OR(RS, Rx, RF);
//}
//
//void PE::NIMP(DREG& Rl, DREG& Rx, DREG& Ry) {
//    // Rl := Rx NIMP Ry
//    this->NOT(RF, Ry);
//    this->NOR(RS, Rx, RF);
//}
//
//void PE::XOR(DREG& Rl, DREG& Rx, DREG& Ry) {
//    // Rl := Rx XOR Ry, Rx := *
//    this->NOT(RF, Ry);
//    this->NOR(Rl, Rx, RF);
//    this->NOT(RF, Rx);
//    this->NOR(Rx, Ry, RF);
//    this->OR(Rl, Rx);
//}

// Digital Register Transfer
void PE::WHERE(DREG& d) {
    // FLAG := d.
    this->FLAG.setTo(d);
}

void PE::WHERE(DREG& d0, DREG& d1) {
    // FLAG := d0 OR d1.
    DREG intermediate;
    this->OR(intermediate, d0, d1);
    this->FLAG.setTo(intermediate);
}

void PE::WHERE(DREG& d0, DREG& d1, DREG& d2) {
    // FLAG := d0 OR d1 OR d2.
    DREG intermediate;
    this->OR(intermediate, d0, d1, d2);
    this->FLAG.setTo(intermediate);
}

void PE::ALL() {
    // FLAG := 1, same as all.
    this->FLAG.setTo(1);
}

void PE::SET(DREG& d0) {
    // d0 := 1
    d0.setTo(1);
}

void PE::SET(DREG& d0, DREG& d1) {
    // d0, d1 := 1
    d0.setTo(1);
    d1.setTo(1);
}

void PE::SET(DREG& d0, DREG& d1, DREG& d2) {
    // 	d0, d1, d2 := 1
    d0.setTo(1);
    d1.setTo(1);
    d2.setTo(1);
}

void PE::SET(DREG& d0, DREG& d1, DREG& d2, DREG& d3) {
    // d0, d1, d2, d3 := 1
    d0.setTo(1);
    d1.setTo(1);
    d2.setTo(1);
    d3.setTo(1);
}

void PE::CLR(DREG& d0) {
    // d0 := 0
    d0.setTo(0);
}

void PE::CLR(DREG& d0, DREG& d1) {
    // d0, d1 := 0
    d0.setTo(0);
    d1.setTo(0);
}

void PE::CLR(DREG& d0, DREG& d1, DREG& d2) {
    // d0, d1, d2 := 0
    d0.setTo(0);
    d1.setTo(0);
    d2.setTo(0);
}

void PE::CLR(DREG& d0, DREG& d1, DREG& d2, DREG& d3) {
    // 	d0, d1, d2, d3 := 0
    d0.setTo(0);
    d1.setTo(0);
    d2.setTo(0);
    d3.setTo(0);
}

void PE::MOV(DREG& d, DREG& d0) {
    // d := d0
    //TODO is this enough?
    cv::copyTo(d0, d, FLAG);
}

void PE::MUX(DREG& Rl, DREG& Rx, DREG& Ry, DREG& Rz) {
    // Rl := Ry IF Rx = 1, Rl := Rz IF Rx = 0.
    this->SET(RF);
    this->MOV(RP, Rz);
    this->MOV(RF, Rx);
    this->MOV(RP, Ry);
    this->MOV(Rl, RP);
}

void PE::CLR_IF(DREG& Rl, DREG& Rx) {
    // Rl := 0 IF Rx = 1, Rl := Rl IF Rx = 0
    this->NOT(RF, Rl);
    this->NOR(Rl, RF, Rx);
}

void PE::REFRESH(DREG& Rl) {
    // refresh a DREG& to prevent decay after a long time (e.g. > 1.5 seconds) without any operations
    //TODO do we need to do anything here? Maybe add a small delay?
}

// Digital Neighbour Access
void PE::DNEWS0(DREG& d, DREG& d0) {

}

void PE::DNEWS1(DREG& d, DREG& d0) {

}

void PE::DNEWS(DREG& Ra, DREG& Rx, const int dir, const bool boundary) {
    this->CLR(RS, RW, RN, RE);
    // set multiple DREG& (upto 4) can be done via one icw
    if(dir&south){
        this->SET(RS);
    }
    if(dir&west){
        this->SET(RW);
    }
    if(dir&north){
        this->SET(RN);
    }
    if(dir&east){
        this->SET(RE);
    }

    if(boundary){
        this->DNEWS1(Ra,Rx);
    }else{
        this->DNEWS0(Ra,Rx);
    }
}

// Digital Propagation
void PE::PROP0() {
    // async-propagation on RP, masked by RF
}

void PE::PROP1() {

}




