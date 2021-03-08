//
// Created by jm1417 on 28/01/2021.
//

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <simulator/util/utility.h>
#include <opencv2/opencv.hpp>
#include "simulator/buses/analogue_bus.h"

void AnalogueBus::bus(AnalogueRegister &a, DigitalRegister &FLAG) {
    //a = 0 + error
    a.value().setTo(0, FLAG.value());
    a.inc_write(FLAG.value());
    FLAG.inc_write();
}

void AnalogueBus::bus(AnalogueRegister &a, AnalogueRegister &a0, DigitalRegister &FLAG) {
    //a = -a0 + error
    cv::bitwise_not(a0.value(), a.value(), FLAG.value());
    a.inc_write(FLAG.value());
    a0.inc_read();
    FLAG.inc_read();
}

void AnalogueBus::bus(AnalogueRegister &a, AnalogueRegister &a0, AnalogueRegister &a1, DigitalRegister &FLAG) {
    //a = -(a0 + a1) + error
    Data intermediate;
    cv::add(a0.value(), a1.value(), intermediate, FLAG.value());
    cv::bitwise_not(intermediate, a.value(), FLAG.value());
    a.inc_write(FLAG.value());
    a0.inc_read();
    a1.inc_read();
    FLAG.inc_read();
}

void AnalogueBus::bus(AnalogueRegister &a, AnalogueRegister &a0, AnalogueRegister &a1, AnalogueRegister &a2, DigitalRegister &FLAG) {
    //a = -(a0 + a1 + a2) + error
    Data intermediate;
    cv::add(a0.value(), a1.value(), intermediate, FLAG.value());
    cv::add(intermediate, a2.value(), intermediate, FLAG.value());
    cv::bitwise_not(intermediate, a.value(), FLAG.value());
    a.inc_write(FLAG.value());
    a0.inc_read();
    a1.inc_read();
    a2.inc_read();
    FLAG.inc_read();
}

void AnalogueBus::bus(AnalogueRegister &a,  AnalogueRegister &a0,  AnalogueRegister &a1,  AnalogueRegister &a2,  AnalogueRegister &a3, DigitalRegister &FLAG) {
    //a = -(a0 + a1 + a2 + a3) + error
    Data intermediate;
    cv::add(a0.value(), a1.value(), intermediate, FLAG.value());
    cv::add(intermediate, a2.value(), intermediate, FLAG.value());
    cv::add(intermediate, a3.value(), intermediate, FLAG.value());
    cv::bitwise_not(intermediate, a.value(), FLAG.value());
    a.inc_write(FLAG.value());
    a0.inc_read();
    a1.inc_read();
    a2.inc_read();
    a3.inc_read();
    FLAG.inc_read();
}

void AnalogueBus::bus2(AnalogueRegister &a, AnalogueRegister &b, DigitalRegister &FLAG) {
    //a,b = 0 + error
    a.value().setTo(0, FLAG.value());
    b.value().setTo(0, FLAG.value());
    a.inc_write(FLAG.value());
    b.inc_write(FLAG.value());
    FLAG.inc_read();
}

void AnalogueBus::bus2(AnalogueRegister &a, AnalogueRegister &b, AnalogueRegister &a0, DigitalRegister &FLAG) {
    //a,b = -0.5*a0 + error + noise
    Data intermediate;
    cv::multiply(a0.value(), 0.5, intermediate);
    cv::bitwise_not(intermediate, intermediate, FLAG.value());
    intermediate.copyTo(a.value(), FLAG.value());
    intermediate.copyTo(b.value(), FLAG.value());
    a.inc_write(FLAG.value());
    b.inc_write(FLAG.value());
    a0.inc_read();
    FLAG.inc_read();
}

void AnalogueBus::bus2(AnalogueRegister &a, AnalogueRegister &b, AnalogueRegister &a0, AnalogueRegister &a1, DigitalRegister &FLAG) {
    //a,b = -0.5*(a0 + a1) + error + noise
    Data intermediate;
    cv::add(a0.value(), a1.value(), intermediate, FLAG.value());
    cv::multiply(intermediate, 0.5, intermediate);
    cv::bitwise_not(intermediate, intermediate, FLAG.value());
    intermediate.copyTo(a.value(), FLAG.value());
    intermediate.copyTo(b.value(), FLAG.value());
    a.inc_write(FLAG.value());
    b.inc_write(FLAG.value());
    a0.inc_read();
    a1.inc_read();
    FLAG.inc_read();
}

void AnalogueBus::bus3(AnalogueRegister &a, AnalogueRegister &b, AnalogueRegister &c, AnalogueRegister &a0, DigitalRegister &FLAG) {
    //a,b,c = -0.33*a0 + error + noise
    Data intermediate;
    cv::multiply(0.333, a0.value(), intermediate);
    cv::bitwise_not(intermediate, intermediate, FLAG.value());
    intermediate.copyTo(a.value(), FLAG.value());
    intermediate.copyTo(b.value(), FLAG.value());
    intermediate.copyTo(c.value(), FLAG.value());
    a.inc_write(FLAG.value());
    b.inc_write(FLAG.value());
    c.inc_write(FLAG.value());
    a0.inc_read();
    FLAG.inc_read();
}

void AnalogueBus::conditional_positive_set(AnalogueRegister &a, DigitalRegister &b){
    //b := 1 if a > 0
    cv::threshold(a.value(), b.value(), 0, 1, cv::THRESH_BINARY);
    b.value().convertTo(b.value(), CV_8U);
    a.inc_read();
    b.inc_write();
}

void AnalogueBus::conditional_positive_set(AnalogueRegister &a0, AnalogueRegister &a1, DigitalRegister &b) {
    //b := 1 if (a0 + a1) > 0.
    Data intermediate;
    cv::add(a0.value(), a1.value(), intermediate);
    cv::threshold(intermediate, b.value(), 0, 1, cv::THRESH_BINARY);
    b.value().convertTo(b.value(), CV_8U);
    a0.inc_read();
    a1.inc_read();
    b.inc_write();
}

void AnalogueBus::conditional_positive_set(AnalogueRegister &a0, AnalogueRegister &a1, AnalogueRegister &a2, DigitalRegister &b) {
    //b := 1 if (a0 + a1 + a2) > 0.
    Data intermediate;
    cv::add(a0.value(), a1.value(), intermediate);
    cv::add(intermediate, a2.value(), intermediate);
    threshold(intermediate, b.value(), 0, 1, cv::THRESH_BINARY);
    b.value().convertTo(b.value(), CV_8U);
    a0.inc_read();
    a1.inc_read();
    a2.inc_read();
    b.inc_write();
}

void AnalogueBus::mov(AnalogueRegister &y, AnalogueRegister &x0, AnalogueRegister &intermediate, DigitalRegister &FLAG) {
    //y = x0
    AnalogueBus::bus(intermediate, x0, FLAG);
    AnalogueBus::bus(y, intermediate, FLAG);
}

void AnalogueBus::add(AnalogueRegister &y, AnalogueRegister &x0, AnalogueRegister &x1, AnalogueRegister &intermediate, DigitalRegister &FLAG) {
    // y = x0 + x1
    AnalogueBus::bus(intermediate, x0, x1, FLAG);
    AnalogueBus::bus(y, intermediate, FLAG);
}

void AnalogueBus::add(AnalogueRegister &y, AnalogueRegister &x0, AnalogueRegister &x1, AnalogueRegister &x2, AnalogueRegister &intermediate, DigitalRegister &FLAG) {
    // y = x0 + x1 + x2
    AnalogueBus::bus(intermediate, x0, x1, x2, FLAG);
    AnalogueBus::bus(y, intermediate, FLAG);
}

void AnalogueBus::sub(AnalogueRegister &y, AnalogueRegister &x0, AnalogueRegister &x1, AnalogueRegister &intermediate, DigitalRegister &FLAG) {
    // y = x0 - x1
    AnalogueBus::bus(intermediate, x0, FLAG);
    AnalogueBus::bus(y, intermediate, x1, FLAG);
}

void AnalogueBus::neg(AnalogueRegister &y, AnalogueRegister &x0, AnalogueRegister &intermediate, DigitalRegister &FLAG) {
    // y = -x0
    AnalogueBus::bus(intermediate, FLAG);
    AnalogueBus::bus(y, intermediate, x0, FLAG);
}

void AnalogueBus::abs(AnalogueRegister &y, AnalogueRegister &x0, AnalogueRegister &intermediate, DigitalRegister &FLAG) {
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

void AnalogueBus::div(AnalogueRegister &y0, AnalogueRegister &y1, AnalogueRegister &y2, AnalogueRegister &x0, AnalogueRegister &intermediate, DigitalRegister &FLAG) {
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

void AnalogueBus::divq(AnalogueRegister &y0, AnalogueRegister &x0, AnalogueRegister &intermediate, DigitalRegister &FLAG) {
    // y0 := 0.5*x0 + error
    AnalogueBus::bus2(y0, intermediate, x0, FLAG);
    AnalogueBus::bus(y0, intermediate, FLAG);
}

void AnalogueBus::push_north(AnalogueRegister &src, AnalogueRegister& dst, int offset, DigitalRegister &FLAG) {
    // Push value of register src north by offset into register dst
    int width = src.value().cols;
    int height = src.value().rows;
    auto src_rect = cv::Rect(0, 0, width, height - offset);
    auto dst_rect = cv::Rect(0, offset, width, height - offset);
    auto fill_rect = cv::Rect(0, 0, src.value().cols, offset);
    src.value()(src_rect).copyTo(dst.value()(dst_rect), FLAG.value()(dst_rect));
    dst.value()(fill_rect).setTo(cv::Scalar(0), FLAG.value()(fill_rect));
    src.inc_read();
    dst.inc_write(FLAG.value());
    FLAG.inc_read();
}

void AnalogueBus::push_east(AnalogueRegister &src, AnalogueRegister &dst, int offset, DigitalRegister &FLAG) {
    // Push value of register src east by offset into register dst
    int width = src.value().cols;
    int height = src.value().rows;
    auto src_rect = cv::Rect(0, 0, width - offset, height);
    auto dst_rect = cv::Rect(offset, 0, width - offset, height);
    auto fill_rect = cv::Rect(0, 0, offset, height);
    auto a = src.value()(src_rect);
    a.copyTo(dst.value()(dst_rect), FLAG.value()(dst_rect));
    dst.value()(fill_rect).setTo(cv::Scalar(0), FLAG.value()(fill_rect));
    src.inc_read();
    dst.inc_write(FLAG.value());
    FLAG.inc_read();
}

void AnalogueBus::push_south(AnalogueRegister &src, AnalogueRegister &dst, int offset, DigitalRegister &FLAG) {
    // Push value of register src south by offset into register dst
    int width = src.value().cols;
    int height = src.value().rows;
    auto src_rect = cv::Rect(0, offset, width, height - offset);
    auto dst_rect = cv::Rect(0, 0, width, height - offset);
    auto fill_rect = cv::Rect(0, height - offset, width, offset);
    src.value()(src_rect).copyTo(dst.value()(dst_rect), FLAG.value()(dst_rect));
    dst.value()(fill_rect).setTo(cv::Scalar(0), FLAG.value()(fill_rect));
    src.inc_read();
    dst.inc_write(FLAG.value());
    FLAG.inc_read();
}

void AnalogueBus::push_west(AnalogueRegister &src, AnalogueRegister &dst, int offset, DigitalRegister &FLAG) {
    // Push value of register src west by offset into register dst
    int width = src.value().cols;
    int height = src.value().rows;
    auto src_rect = cv::Rect(offset, 0, width - offset, height);
    auto dst_rect = cv::Rect(0, 0, width - offset, height);
    auto fill_rect = cv::Rect(width - offset, 0, offset, height);
    src.value()(src_rect).copyTo(dst.value()(dst_rect), FLAG.value()(dst_rect));
    dst.value()(fill_rect).setTo(cv::Scalar(0), FLAG.value()(fill_rect));
    src.inc_read();
    dst.inc_write(FLAG.value());
    FLAG.inc_read();
}

void AnalogueBus::pull_north(AnalogueRegister &src, AnalogueRegister &dst, int offset, DigitalRegister &FLAG) {
    // Pull value from north src by offset into register dst
    this->push_south(src, dst, offset, FLAG);
}

void AnalogueBus::pull_east(AnalogueRegister &src, AnalogueRegister &dst, int offset, DigitalRegister &FLAG) {
    // Pull value from east src by offset into register dst
    this->push_west(src, dst, offset, FLAG);
}

void AnalogueBus::pull_south(AnalogueRegister &src, AnalogueRegister &dst, int offset, DigitalRegister &FLAG) {
    // Pull value from south src by offset into register dst
    this->push_north(src, dst, offset, FLAG);
}

void AnalogueBus::pull_west(AnalogueRegister &src, AnalogueRegister &dst, int offset, DigitalRegister &FLAG) {
    // Pull value from south src by offset into register dst
    this->push_east(src, dst, offset, FLAG);
}

void AnalogueBus::get_east(AnalogueRegister &src, AnalogueRegister& dst, int offset) {
    // x, y, width, height
    auto read_chunk = cv::Rect(offset, 0, src.value().cols - offset, src.value().rows);
    auto write_chunk = cv::Rect(0, 0, src.value().cols - offset, src.value().rows);
    src.value()(read_chunk).copyTo(dst.value()(write_chunk));
    auto fill = cv::Rect(0, 0, offset, src.value().rows);
    dst.value()(fill).setTo(cv::Scalar(0));
    src.inc_read();
    dst.inc_write();
}

void AnalogueBus::get_west(AnalogueRegister &src, AnalogueRegister& dst, int offset) {
    // x, y, width, height
    auto read_chunk = cv::Rect(0, 0, src.value().cols - offset, src.value().rows);
    auto write_chunk = cv::Rect(offset, 0, src.value().cols - offset, src.value().rows);
    src.value()(read_chunk).copyTo(dst.value()(write_chunk));
    auto fill = cv::Rect(src.value().cols - offset, 0, offset, src.value().rows);
    dst.value()(fill).setTo(cv::Scalar(0));
    src.inc_read();
    dst.inc_write();
}

void AnalogueBus::get_north(AnalogueRegister &src, AnalogueRegister& dst, int offset) {
    // x, y, width, height
    auto read_chunk = cv::Rect(0, offset, src.value().cols, src.value().rows - offset);
    auto write_chunk = cv::Rect(0, 0, src.value().cols, src.value().rows - offset);
    src.value()(read_chunk).copyTo(dst.value()(write_chunk));
    auto fill = cv::Rect(0, 0, src.value().cols, offset);
    dst.value()(fill).setTo(cv::Scalar(0));
    src.inc_read();
    dst.inc_write();
}

void AnalogueBus::get_south(AnalogueRegister &src, AnalogueRegister& dst, int offset) {
    // x, y, width, height
    auto read_chunk = cv::Rect(0, 0, src.value().cols, src.value().rows - offset);
    auto write_chunk = cv::Rect(0, offset, src.value().cols, src.value().rows - offset);
    src.value()(read_chunk).copyTo(dst.value()(write_chunk));
    auto fill = cv::Rect(0, src.value().rows - offset, src.value().cols, offset);
    dst.value()(fill).setTo(cv::Scalar(0));
    src.inc_read();
    dst.inc_write();
}










