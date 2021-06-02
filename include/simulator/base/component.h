//
// Created by jm1417 on 08/02/2021.
//

#ifndef SIMULATOR_COMPONENT_H
#define SIMULATOR_COMPONENT_H

#include <nlohmann/json.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/cuda.hpp>
#include <rttr/registration>
#include <memory>

#include "config.h"
#include "simulator/metrics/cycle_counter.h"
#include "simulator/metrics/stats_outputter.h"
#include "simulator/metrics/pack_node.h"

//using json = nlohmann::json;
class Component : public StatsOutputter {
    RTTR_ENABLE(StatsOutputter)
   public:
    std::shared_ptr<PackNode> fit;

   protected:
    int process_node_ = -1; // process node in nm that the other metrics are defined in terms of.
    int rows_; // rows of the whole plane
    int cols_; // cols of the whole plane
    int row_stride_ = 1;
    int col_stride_ = 1;
    std::shared_ptr<Config> config_;
#ifdef USE_CUDA
    cv::cuda::GpuMat internal_mask;  // Used to keep track of components in array when stride is not 1, i.e. spaces between components
#else
    cv::UMat internal_mask;  // Used to keep track of components in array when stride is not 1, i.e. spaces between components
#endif

   public:
    void init();
    void calc_internal_mask();

    /*Setters*/
    void set_process_node(int process_node);
    void set_rows(int rows);
    void set_cols(int cols);
    void set_row_stride(int row_stride);
    void set_col_stride(int col_stride);
    void set_config(std::shared_ptr<Config> config);


#ifdef TRACK_STATISTICS
   protected:
    int cycle_count_ = 1;
    int transistor_count_;
    double static_power_;  // in Watts
    double dynamic_power_;  // in Watts
    double width_; // in Micrometres
    double height_; // in Mircometres
    cv::UMat array_static_energy_;
    cv::UMat array_dynamic_energy_; // No GPUMat for these two as float ops are not always supported on GPUs but int ops usually are
#ifdef USE_CUDA
    cv::cuda::GpuMat array_transistor_count_;
#else
    cv::UMat array_transistor_count_;
#endif
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
    virtual double get_width();
    /* width of component in micrometres */
    virtual double get_height();

    virtual int get_cycle_count() = 0;
    void print_stats(const CycleCounter& counter) override = 0;
//    virtual void write_stats(const CycleCounter& counter, json& j) = 0;
    // calculates the values for each individual component in the array
    virtual int calc_transistor_count();
    virtual double calc_static();
    virtual double calc_dynamic();
    virtual double calc_width();
    virtual double calc_height();

    /* Scales base speed with the config*/
    double scale_speed(double base);

    /* Scales the width with the config*/
    double scale_width(double base);

    /* Scales the height with the config*/
    double scale_height(double base);
#endif
    virtual ~Component() = default;

};


#endif  // SIMULATOR_COMPONENT_H
