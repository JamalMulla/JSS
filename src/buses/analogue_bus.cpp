//
// Created by jm1417 on 28/01/2021.
//

#include "analogue_bus.h"

void AnalogueBus::bus(AREG &a, const DREG &FLAG) {
    //a = 0 + error
    a.value().setTo(0, FLAG.value());
}

void AnalogueBus::bus(AREG &a, const AREG &a0, DREG &FLAG) {
    //a = -a0 + error
    cv::bitwise_not(a0.value(), a.value(), FLAG.value());
}

void AnalogueBus::bus(AREG &a, const AREG &a0, const AREG &a1, DREG &FLAG) {
    //a = -(a0 + a1) + error
    Data intermediate;
    cv::add(a0.value(), a1.value(), intermediate, FLAG.value());
    cv::bitwise_not(intermediate, a.value(), FLAG.value());
}

void AnalogueBus::bus(AREG &a, const AREG &a0, const AREG &a1, const AREG &a2, DREG &FLAG) {
    //a = -(a0 + a1 + a2) + error
    Data intermediate;
    cv::add(a0.value(), a1.value(), intermediate, FLAG.value());
    cv::add(intermediate, a2.value(), intermediate, FLAG.value());
    cv::bitwise_not(intermediate, a.value(), FLAG.value());
}

void AnalogueBus::bus(AREG &a, const AREG &a0, const AREG &a1, const AREG &a2, const AREG &a3, DREG &FLAG) {
    //a = -(a0 + a1 + a2 + a3) + error
    Data intermediate;
    cv::add(a0.value(), a1.value(), intermediate, FLAG.value());
    cv::add(intermediate, a2.value(), intermediate, FLAG.value());
    cv::add(intermediate, a3.value(), intermediate, FLAG.value());
    cv::bitwise_not(intermediate, a.value(), FLAG.value());
}

void AnalogueBus::bus2(AREG &a, AREG &b, DREG &FLAG) {
    //a,b = 0 + error
    a.value().setTo(0, FLAG.value());
    b.value().setTo(0, FLAG.value());
}

void AnalogueBus::bus2(AREG &a, AREG &b, const AREG &a0, DREG &FLAG) {
    //a,b = -0.5*a0 + error + noise
    Data intermediate;
    cv::multiply(0.5, a0.value(), intermediate);
    cv::bitwise_not(intermediate, intermediate, FLAG.value());
    intermediate.copyTo(a.value(), FLAG.value());
    intermediate.copyTo(b.value(), FLAG.value());
}

void AnalogueBus::bus2(AREG &a, AREG &b, const AREG &a0, const AREG &a1, DREG &FLAG) {
    //a,b = -0.5*(a0 + a1) + error + noise
    Data intermediate;
    cv::add(a0.value(), a1.value(), intermediate, FLAG.value());
    cv::multiply(0.5, intermediate, intermediate);
    cv::bitwise_not(intermediate, intermediate, FLAG.value());
    intermediate.copyTo(a.value(), FLAG.value());
    intermediate.copyTo(b.value(), FLAG.value());
}

void AnalogueBus::bus3(AREG &a, AREG &b, AREG &c, const AREG &a0, DREG &FLAG) {
    //a,b,c = -0.33*a0 + error + noise
    Data intermediate;
    cv::multiply(0.333, a0.value(), intermediate);
    cv::bitwise_not(intermediate, intermediate, FLAG.value());
    intermediate.copyTo(a.value(), FLAG.value());
    intermediate.copyTo(b.value(), FLAG.value());
    intermediate.copyTo(c.value(), FLAG.value());
}

void AnalogueBus::conditional_positive_set(const AREG &a, DREG &b){
    //value := 1 if a > 0
    cv::threshold(a.value(), b.value(), 0, 1, cv::THRESH_BINARY);
}

void AnalogueBus::conditional_positive_set(const AREG &a0, const AREG &a1, DREG &b) {
    //value := 1 if (a0 + a1) > 0.
    Data intermediate;
    cv::add(a0.value(), a1.value(), intermediate);
    threshold(intermediate, b.value(), 0, 1, cv::THRESH_BINARY);
}

void AnalogueBus::conditional_positive_set(const AREG &a0, const AREG &a1, const AREG &a2, DREG &b) {
    //value := 1 if (a0 + a1 + a2) > 0.
    Data intermediate;
    cv::add(a0.value(), a1.value(), intermediate);
    cv::add(intermediate, a2.value(), intermediate);
    threshold(intermediate, b.value(), 0, 1, cv::THRESH_BINARY);
}

void AnalogueBus::mov(AREG &y, const AREG &x0, AREG &intermediate, DREG &FLAG) {
    //y = x0
    AnalogueBus::bus(intermediate, x0, FLAG);
    AnalogueBus::bus(y, intermediate, FLAG);
}

void AnalogueBus::add(AREG &y, const AREG &x0, const AREG &x1, AREG &intermediate, DREG &FLAG) {
    // y = x0 + x1
    AnalogueBus::bus(intermediate, x0, x1, FLAG);
    AnalogueBus::bus(y, intermediate, FLAG);
}

void AnalogueBus::add(AREG &y, const AREG &x0, const AREG &x1, const AREG &x2, AREG &intermediate, DREG &FLAG) {
    // y = x0 + x1 + x2
    AnalogueBus::bus(intermediate, x0, x1, x2, FLAG);
    AnalogueBus::bus(y, intermediate, FLAG);
}

void AnalogueBus::sub(AREG &y, const AREG &x0, const AREG &x1, AREG &intermediate, DREG &FLAG) {
    // y = x0 - x1
    AnalogueBus::bus(intermediate, x0, FLAG);
    AnalogueBus::bus(y, intermediate, x1, FLAG);
}

void AnalogueBus::neg(AREG &y, const AREG &x0, AREG &intermediate, DREG &FLAG) {
    // y = -x0
    AnalogueBus::bus(intermediate, FLAG);
    AnalogueBus::bus(y, intermediate, x0, FLAG);
}

void AnalogueBus::abs(AREG &y, const AREG &x0, AREG &intermediate, DREG &FLAG) {
    // y = |x0|
    AnalogueBus::bus(intermediate, FLAG);
    AnalogueBus::bus(y, intermediate, x0, FLAG);
    AnalogueBus::bus(intermediate, x0, FLAG);
    AnalogueBus::conditional_positive_set(x0, FLAG);
    AnalogueBus::bus(y, intermediate, FLAG);
    FLAG.set();
}

void AnalogueBus::div(AREG &y0, AREG &y1, AREG &y2, AREG &intermediate, DREG &FLAG) {
    // y0 := 0.5*y2; y1 := -0.5*y2 + error, y2 := y2 + error
    AnalogueBus::bus2(y0, y1, y2, FLAG);
    AnalogueBus::bus(intermediate, y2, y1, FLAG);
    AnalogueBus::bus(y2, intermediate, y0, FLAG);
    AnalogueBus::bus2(y0, y1, y2, FLAG);
    AnalogueBus::bus(y0, y1, FLAG);
}

void AnalogueBus::div(AREG &y0, AREG &y1, AREG &y2, const AREG &x0, AREG &intermediate, DREG &FLAG) {
    // y0 := 0.5*x0; y1 := -0.5*x0 + error, y2 := x0 + error
    AnalogueBus::bus2(y0,y1, x0, FLAG);
    AnalogueBus::bus(intermediate, x0, y1, FLAG);
    AnalogueBus::bus(y2, intermediate, y0, FLAG);
    AnalogueBus::bus2(y0, y1, y2, FLAG);
    AnalogueBus::bus(y0, y1, FLAG);
}

void AnalogueBus::diva(AREG &y0, AREG &y1, AREG &y2, AREG &intermediate, DREG &FLAG) {
    // y0 := 0.5*y0; y1 := -0.5*y0 + error, y2 := -0.5*y0 + error
    AnalogueBus::bus2(y1, y2, y0, FLAG);
    AnalogueBus::bus(intermediate, y1, y0, FLAG);
    AnalogueBus::bus(y0, intermediate, y2, FLAG);
    AnalogueBus::bus2(y1, y2, y0, FLAG);
    AnalogueBus::bus(y0, y1, FLAG);
}

void AnalogueBus::divq(AREG &y0, const AREG &x0, AREG &intermediate, DREG &FLAG) {
    // y0 := 0.5*x0 + error
    AnalogueBus::bus2(y0, intermediate, x0, FLAG);
    AnalogueBus::bus(y0, intermediate, FLAG);
}





