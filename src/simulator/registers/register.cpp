//
// Created by jm1417 on 31/01/2021.
//

#include "simulator/registers/register.h"

#include <opencv2/core.hpp>

#include "simulator/metrics/stats.h"

Register::Register(int rows, int columns, int type, Memory& memoryType)
    : memory_(&memoryType),
      value_(rows, columns, type, cv::Scalar(0))
#ifdef TRACK_STATISTICS
      ,read_counter(rows, columns, CV_32S, cv::Scalar(0)),
      write_counter(rows, columns, CV_32S, cv::Scalar(0)),
      read_energy_counter(rows, columns, CV_64F, cv::Scalar(0)),
      write_energy_counter(rows, columns, CV_64F, cv::Scalar(0)),
      reads(0),
      writes(0)
#endif
       {}

cv::Mat &Register::read() {
#ifdef TRACK_STATISTICS
    this->inc_read();
#endif
    return this->value_;
}

void Register::write(cv::Mat& data) {
    data.copyTo(this->value_);
#ifdef TRACK_STATISTICS
    this->inc_write();
#endif
}

void Register::write(const cv::Mat &data) {
    data.copyTo(this->value_);
#ifdef TRACK_STATISTICS
    this->inc_write();
#endif
}

void Register::write(Register &data) {
    this->write(data.read());
}

void Register::write(cv::Mat &data, cv::Mat &mask) {
    data.copyTo(this->value_, mask);
#ifdef TRACK_STATISTICS
    this->inc_write();
#endif
}

void Register::write(Register &data, Register &mask) {
    this->write(data.read(), mask.read());
}

void Register::write(int data) {
    this->value_.setTo(data);
#ifdef TRACK_STATISTICS
    this->inc_write();
#endif
}

void Register::write(int data, cv::Mat &mask) {
    this->value_.setTo(data, mask);
#ifdef TRACK_STATISTICS
    this->inc_write();
#endif
}

void Register::write(int data, Register &mask) {
    this->write(data, mask.read());
}

#ifdef TRACK_STATISTICS
void Register::inc_read(const cv::_InputOutputArray &mask) {
    cv::add(this->read_counter, 1, this->read_counter, mask);
//    int number_of_cycle = 1;  // Should be parameterisable
//    double energy_usage = this->memory_.read_power_draw *
//                          stats::CYCLE_TIME * number_of_cycle;
//    cv::add(this->read_energy_counter, energy_usage, this->read_energy_counter,
//            mask);
    this->reads++;
}

void Register::inc_write(const cv::_InputOutputArray &mask) {
    cv::add(this->write_counter, 1, this->write_counter, mask);
//    int number_of_cycle = 1;
//    double energy_usage = this->memory_.write_power_draw *
//                          stats::CYCLE_TIME * number_of_cycle;
//    cv::add(this->write_energy_counter, energy_usage,
//            this->write_energy_counter, mask);
    this->writes++;
}

int Register::get_reads() { return this->reads; }

int Register::get_writes() { return this->writes; }

int Register::get_total_reads() { return cv::sum(this->read_counter)[0]; }

int Register::get_total_writes() { return cv::sum(this->write_counter)[0]; }

double Register::get_read_energy() {
    return cv::sum(this->read_energy_counter)[0];
}

double Register::get_write_energy() {
    return cv::sum(this->write_energy_counter)[0];
}

double Register::get_total_energy() {
    return get_read_energy() + get_write_energy();
}

#endif


//Data &Register::value() { return this->value_; }
//const Data &Register::value() const { return this->value_; }

void Register::change_memory_type(Memory &memory_type) {
    this->memory_ = &memory_type;
}

void Register::set_ui_handler(UI *ui_ptr) { this->ui = ui_ptr; }
