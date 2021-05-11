//
// Created by jm1417 on 08/02/2021.
//

#ifndef SIMULATOR_COMPONENT_H
#define SIMULATOR_COMPONENT_H

#include <nlohmann/json.hpp>
#include <opencv4/opencv2/core.hpp>
#include <rttr/registration>

#include "config.h"
#include "simulator/metrics/cycle_counter.h"
#include "simulator/metrics/stats_outputter.h"
#include "simulator/metrics/pack_node.h"

//using json = nlohmann::json;
class Component : public StatsOutputter {
    RTTR_ENABLE(StatsOutputter)
   public:
    PackNode* fit;

   protected:
    int rows_; // rows of the whole plane
    int cols_; // cols of the whole plane
    int row_stride_ = 1;
    int col_stride_ = 1;
    std::shared_ptr<Config> config_;
    cv::Mat internal_mask;  // Used to keep track of components in array when stride is not 1, i.e. spaces between components

   public:
    void calc_internal_mask();

    /*Setters*/
    void set_rows(int rows);
    void set_cols(int cols);
    void set_row_stride(int row_stride);
    void set_col_stride(int col_stride);
    void set_config(std::shared_ptr<Config> config);


#ifdef TRACK_STATISTICS
   protected:
    int cycle_count_;
    int transistor_count_;
    double static_power_;  // in Watts
    double dynamic_power_;  // in Watts
    int width_; // in Micrometres
    int height_; // in Mircometres
    cv::Mat array_transistor_count_;
    cv::Mat array_static_energy_;
    cv::Mat array_dynamic_energy_;
   public:
    /* Update component with how much time has passed for operation. Used for updating static power. */
    virtual void update_static(double time) = 0;
    /* Returns the total amount of static energy use */
    virtual cv::Mat get_static_energy_array();
    /* Returns the total amount of dynamic energy use */
    virtual cv::Mat get_dynamic_energy_array();
    /* Returns the transistor count of the component
     * This should be a constant after the component is configured. */
    virtual cv::Mat get_transistor_count_array();
    virtual int get_transistor_count();
    /* The number of cycles needed for the operation. */
    /* width of component in micrometres */
    virtual int get_width();
    /* width of component in micrometres */
    virtual int get_height();

    virtual int get_cycle_count() = 0;
    void print_stats(const CycleCounter& counter) override = 0;
//    virtual void write_stats(const CycleCounter& counter, json& j) = 0;
    // calculates the values for each individual component in the array
    virtual int calc_transistor_count();
    virtual double calc_static();
    virtual double calc_dynamic();
    virtual int calc_width();
    virtual int calc_height();
#endif
    virtual ~Component() = default;

};


#endif  // SIMULATOR_COMPONENT_H
