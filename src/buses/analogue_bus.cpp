//
// Created by jm1417 on 28/01/2021.
//

#include "analogue_bus.h"

void AnalogueBus::bus(AnalogueRegister &a, const DigitalRegister &FLAG) {
    //a = 0 + error
    a.value().setTo(0, FLAG.value());
}

void AnalogueBus::bus(AnalogueRegister &a, const AnalogueRegister &a0, DigitalRegister &FLAG) {
    //a = -a0 + error
    cv::bitwise_not(a0.value(), a.value(), FLAG.value());
}

void AnalogueBus::bus(AnalogueRegister &a, const AnalogueRegister &a0, const AnalogueRegister &a1, DigitalRegister &FLAG) {
    //a = -(a0 + a1) + error
    Data intermediate;
    cv::add(a0.value(), a1.value(), intermediate, FLAG.value());
    cv::bitwise_not(intermediate, a.value(), FLAG.value());
}

void AnalogueBus::bus(AnalogueRegister &a, const AnalogueRegister &a0, const AnalogueRegister &a1, const AnalogueRegister &a2, DigitalRegister &FLAG) {
    //a = -(a0 + a1 + a2) + error
    Data intermediate;
    cv::add(a0.value(), a1.value(), intermediate, FLAG.value());
    cv::add(intermediate, a2.value(), intermediate, FLAG.value());
    cv::bitwise_not(intermediate, a.value(), FLAG.value());
}

void AnalogueBus::bus(AnalogueRegister &a, const AnalogueRegister &a0, const AnalogueRegister &a1, const AnalogueRegister &a2, const AnalogueRegister &a3, DigitalRegister &FLAG) {
    //a = -(a0 + a1 + a2 + a3) + error
    Data intermediate;
    cv::add(a0.value(), a1.value(), intermediate, FLAG.value());
    cv::add(intermediate, a2.value(), intermediate, FLAG.value());
    cv::add(intermediate, a3.value(), intermediate, FLAG.value());
    cv::bitwise_not(intermediate, a.value(), FLAG.value());
}

void AnalogueBus::bus2(AnalogueRegister &a, AnalogueRegister &b, DigitalRegister &FLAG) {
    //a,b = 0 + error
    a.value().setTo(0, FLAG.value());
    b.value().setTo(0, FLAG.value());
}

void AnalogueBus::bus2(AnalogueRegister &a, AnalogueRegister &b, const AnalogueRegister &a0, DigitalRegister &FLAG) {
    //a,b = -0.5*a0 + error + noise
    Data intermediate;
    cv::multiply(0.5, a0.value(), intermediate);
    cv::bitwise_not(intermediate, intermediate, FLAG.value());
    intermediate.copyTo(a.value(), FLAG.value());
    intermediate.copyTo(b.value(), FLAG.value());
}

void AnalogueBus::bus2(AnalogueRegister &a, AnalogueRegister &b, const AnalogueRegister &a0, const AnalogueRegister &a1, DigitalRegister &FLAG) {
    //a,b = -0.5*(a0 + a1) + error + noise
    Data intermediate;
    cv::add(a0.value(), a1.value(), intermediate, FLAG.value());
    cv::multiply(0.5, intermediate, intermediate);
    cv::bitwise_not(intermediate, intermediate, FLAG.value());
    intermediate.copyTo(a.value(), FLAG.value());
    intermediate.copyTo(b.value(), FLAG.value());
}

void AnalogueBus::bus3(AnalogueRegister &a, AnalogueRegister &b, AnalogueRegister &c, const AnalogueRegister &a0, DigitalRegister &FLAG) {
    //a,b,c = -0.33*a0 + error + noise
    Data intermediate;
    cv::multiply(0.333, a0.value(), intermediate);
    cv::bitwise_not(intermediate, intermediate, FLAG.value());
    intermediate.copyTo(a.value(), FLAG.value());
    intermediate.copyTo(b.value(), FLAG.value());
    intermediate.copyTo(c.value(), FLAG.value());
}

void AnalogueBus::conditional_positive_set(const AnalogueRegister &a, DigitalRegister &b){
    //value := 1 if a > 0
    cv::threshold(a.value(), b.value(), 0, 1, cv::THRESH_BINARY);
}

void AnalogueBus::conditional_positive_set(const AnalogueRegister &a0, const AnalogueRegister &a1, DigitalRegister &b) {
    //value := 1 if (a0 + a1) > 0.
    Data intermediate;
    cv::add(a0.value(), a1.value(), intermediate);
    threshold(intermediate, b.value(), 0, 1, cv::THRESH_BINARY);
}

void AnalogueBus::conditional_positive_set(const AnalogueRegister &a0, const AnalogueRegister &a1, const AnalogueRegister &a2, DigitalRegister &b) {
    //value := 1 if (a0 + a1 + a2) > 0.
    Data intermediate;
    cv::add(a0.value(), a1.value(), intermediate);
    cv::add(intermediate, a2.value(), intermediate);
    threshold(intermediate, b.value(), 0, 1, cv::THRESH_BINARY);
}

void AnalogueBus::mov(AnalogueRegister &y, const AnalogueRegister &x0, AnalogueRegister &intermediate, DigitalRegister &FLAG) {
    //y = x0
    AnalogueBus::bus(intermediate, x0, FLAG);
    AnalogueBus::bus(y, intermediate, FLAG);
}

void AnalogueBus::add(AnalogueRegister &y, const AnalogueRegister &x0, const AnalogueRegister &x1, AnalogueRegister &intermediate, DigitalRegister &FLAG) {
    // y = x0 + x1
    AnalogueBus::bus(intermediate, x0, x1, FLAG);
    AnalogueBus::bus(y, intermediate, FLAG);
}

void AnalogueBus::add(AnalogueRegister &y, const AnalogueRegister &x0, const AnalogueRegister &x1, const AnalogueRegister &x2, AnalogueRegister &intermediate, DigitalRegister &FLAG) {
    // y = x0 + x1 + x2
    AnalogueBus::bus(intermediate, x0, x1, x2, FLAG);
    AnalogueBus::bus(y, intermediate, FLAG);
}

void AnalogueBus::sub(AnalogueRegister &y, const AnalogueRegister &x0, const AnalogueRegister &x1, AnalogueRegister &intermediate, DigitalRegister &FLAG) {
    // y = x0 - x1
    AnalogueBus::bus(intermediate, x0, FLAG);
    AnalogueBus::bus(y, intermediate, x1, FLAG);
}

void AnalogueBus::neg(AnalogueRegister &y, const AnalogueRegister &x0, AnalogueRegister &intermediate, DigitalRegister &FLAG) {
    // y = -x0
    AnalogueBus::bus(intermediate, FLAG);
    AnalogueBus::bus(y, intermediate, x0, FLAG);
}

void AnalogueBus::abs(AnalogueRegister &y, const AnalogueRegister &x0, AnalogueRegister &intermediate, DigitalRegister &FLAG) {
    // y = |x0|
    AnalogueBus::bus(intermediate, FLAG);
    AnalogueBus::bus(y, intermediate, x0, FLAG);
    AnalogueBus::bus(intermediate, x0, FLAG);
    AnalogueBus::conditional_positive_set(x0, FLAG);
    AnalogueBus::bus(y, intermediate, FLAG);
    FLAG.set();
}

void AnalogueBus::div(AnalogueRegister &y0, AnalogueRegister &y1, AnalogueRegister &y2, AnalogueRegister &intermediate, DigitalRegister &FLAG) {
    // y0 := 0.5*y2; y1 := -0.5*y2 + error, y2 := y2 + error
    AnalogueBus::bus2(y0, y1, y2, FLAG);
    AnalogueBus::bus(intermediate, y2, y1, FLAG);
    AnalogueBus::bus(y2, intermediate, y0, FLAG);
    AnalogueBus::bus2(y0, y1, y2, FLAG);
    AnalogueBus::bus(y0, y1, FLAG);
}

void AnalogueBus::div(AnalogueRegister &y0, AnalogueRegister &y1, AnalogueRegister &y2, const AnalogueRegister &x0, AnalogueRegister &intermediate, DigitalRegister &FLAG) {
    // y0 := 0.5*x0; y1 := -0.5*x0 + error, y2 := x0 + error
    AnalogueBus::bus2(y0,y1, x0, FLAG);
    AnalogueBus::bus(intermediate, x0, y1, FLAG);
    AnalogueBus::bus(y2, intermediate, y0, FLAG);
    AnalogueBus::bus2(y0, y1, y2, FLAG);
    AnalogueBus::bus(y0, y1, FLAG);
}

void AnalogueBus::diva(AnalogueRegister &y0, AnalogueRegister &y1, AnalogueRegister &y2, AnalogueRegister &intermediate, DigitalRegister &FLAG) {
    // y0 := 0.5*y0; y1 := -0.5*y0 + error, y2 := -0.5*y0 + error
    AnalogueBus::bus2(y1, y2, y0, FLAG);
    AnalogueBus::bus(intermediate, y1, y0, FLAG);
    AnalogueBus::bus(y0, intermediate, y2, FLAG);
    AnalogueBus::bus2(y1, y2, y0, FLAG);
    AnalogueBus::bus(y0, y1, FLAG);
}

void AnalogueBus::divq(AnalogueRegister &y0, const AnalogueRegister &x0, AnalogueRegister &intermediate, DigitalRegister &FLAG) {
    // y0 := 0.5*x0 + error
    AnalogueBus::bus2(y0, intermediate, x0, FLAG);
    AnalogueBus::bus(y0, intermediate, FLAG);
}

void AnalogueBus::push_north(const AnalogueRegister &a, AnalogueRegister& b, int offset, DigitalRegister &FLAG) {
    // Push value of register a north by offset into register b
    auto chunk = cv::Rect(0, 0, a.value().cols, a.value().rows - offset);
    a.value()(cv::Rect(0, offset, a.value().cols, a.value().rows - offset)).copyTo(b.value()(chunk), FLAG.value()(chunk));
    auto fill = cv::Rect(0, a.value().rows - offset, a.value().cols, offset);
    b.value()(fill).setTo(cv::Scalar(0), FLAG.value()(fill));
}

void AnalogueBus::push_east(const AnalogueRegister &a, AnalogueRegister &b, int offset, DigitalRegister &FLAG) {
    // Push value of register a east by offset into register b
    auto chunk = cv::Rect(offset, 0, a.value().cols - offset, a.value().rows);
    a.value()(cv::Rect(0, 0, a.value().cols - offset, a.value().rows)).copyTo(b.value()(chunk), FLAG.value()(chunk));
    auto fill = cv::Rect(0, 0, offset, a.value().rows - offset);
    b.value()(fill).setTo(cv::Scalar(0), FLAG.value()(fill));
}

void AnalogueBus::push_south(const AnalogueRegister &a, AnalogueRegister &b, int offset, DigitalRegister &FLAG) {
    // Push value of register a south by offset into register b
    auto chunk = cv::Rect(0, offset, a.value().cols, a.value().rows - offset);
    a.value()(cv::Rect(0, 0, a.value().cols, a.value().rows - offset)).copyTo(b.value()(chunk), FLAG.value()(chunk));
    auto fill = cv::Rect(0, 0, a.value().cols, offset);
    b.value()(fill).setTo(cv::Scalar(0), FLAG.value()(fill));
}

void AnalogueBus::push_west(const AnalogueRegister &a, AnalogueRegister &b, int offset, DigitalRegister &FLAG) {
    // Push value of register a west by offset into register b
    auto chunk = cv::Rect(0, 0, a.value().cols - offset, a.value().rows);
    a.value()(cv::Rect(1, 0, a.value().cols - offset, a.value().rows)).copyTo(b.value()(chunk), FLAG.value()(chunk));
    auto fill = cv::Rect(a.value().cols - offset, 0, offset, a.value().rows);
    b.value()(fill).setTo(cv::Scalar(0), FLAG.value()(fill));
}










