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

//using json = nlohmann::json;
class Component : public StatsOutputter {
    RTTR_ENABLE(StatsOutputter)
#ifdef TRACK_STATISTICS
   protected:
    int rows_; // rows of the whole plane
    int cols_; // cols of the whole plane
    int row_stride_ = 1;
    int col_stride_ = 1;
    std::shared_ptr<Config> config_;
    int cycle_count_;
    int transistor_count_;
    double static_power_;  // in Watts
    double dynamic_power_;  // in Watts
    cv::Mat internal_mask;  // Used to keep track of components in array when stride is not 1, i.e. spaces between components
    cv::Mat array_transistor_count_;
    cv::Mat array_static_energy_;
    cv::Mat array_dynamic_energy_;
   public:
    /* Update component with how much time has passed for operation. Used for updating static power. */
    virtual void update_static(double time) = 0;
    /* Returns the total amount of static energy use */
    virtual cv::Mat get_static_energy();
    /* Returns the total amount of dynamic energy use */
    virtual cv::Mat get_dynamic_energy();
    /* Returns the transistor count of the component
     * This should be a constant after the component is configured. */
    virtual cv::Mat get_transistor_count();
    /* The number of cycles needed for the operation. */
//    /* width of component in micrometres */
//    virtual cv::Mat get_width() = 0;
//    /* width of component in micrometres */
//    virtual cv::Mat get_height() = 0;
    virtual int get_cycle_count() = 0;
    void print_stats(const CycleCounter& counter) override = 0;
//    virtual void write_stats(const CycleCounter& counter, json& j) = 0;

    // caclulates the values for each individual component in the array
    virtual int calc_transistor_count();
    virtual double calc_static();
    virtual double calc_dynamic();

    void calc_internal_mask();

    /*Setters*/
    void set_rows(int rows);
    void set_cols(int cols);
    void set_row_stride(int row_stride);
    void set_col_stride(int col_stride);
    void set_config(std::shared_ptr<Config> config);


#endif
    virtual ~Component() = default;

};

#endif  // SIMULATOR_COMPONENT_H
