/*
 * Scamp5d M0 Program 2
 * This program runs the legacy AnalogNet architecture on the SCAMP5 device,
 * with a new output bining process (12 bins instead of 9, and a better
 * differentiation of central pixels, that now fall in different bins).
 */

/*
 * AnalogNet: 15 fps network
 * Author: Benoit Guillard, MSc Thesis, Imperial College London
 * Weights and AnalogNet architecture from Matthew Wong
 */

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

#include <simulator/ui/ui.h>

#include <cstdint>
#include <random>
#include <vector>

#include "conv_instructions.h"
#include "fc_weights.h"

// Given SCAN_SIZE coordinates,
// group them into 12 bins, filling the count array
// central division, with overlaping bins
template<int SCAN_SIZE>
inline void sum_pooling_events(const uint8_t coordinates[2 * SCAN_SIZE],
                               int count[12]) {
    uint8_t x, y;
    for(int i = 0; i < 12; i++) { count[i] = 0; }
    for(int i = 0; i < SCAN_SIZE; i++) {
        x = coordinates[2 * i];
        y = coordinates[2 * i + 1];
        if(x == 0 && y == 0)
            return;
        if(x >= 114 && x <= 141 && y >= 114 && y <= 141) {
            x = x - 114;
            y = y - 114;

            if(x >= 5 && x <= 13 && y >= 0 && y <= 8)
                count[0]++;
            if(x >= 14 && x <= 22 && y >= 0 && y <= 8)
                count[1]++;

            if(x >= 0 && x <= 8 && y >= 5 && y <= 13)
                count[2]++;
            if(x >= 5 && x <= 13 && y >= 5 && y <= 13)
                count[3]++;
            if(x >= 14 && x <= 22 && y >= 5 && y <= 13)
                count[4]++;
            if(x >= 19 && x <= 27 && y >= 5 && y <= 13)
                count[5]++;

            if(x >= 0 && x <= 8 && y >= 14 && y <= 22)
                count[6]++;
            if(x >= 5 && x <= 13 && y >= 14 && y <= 22)
                count[7]++;
            if(x >= 14 && x <= 22 && y >= 14 && y <= 22)
                count[8]++;
            if(x >= 19 && x <= 27 && y >= 14 && y <= 22)
                count[9]++;

            if(x >= 5 && x <= 13 && y >= 19 && y <= 27)
                count[10]++;
            if(x >= 14 && x <= 22 && y >= 19 && y <= 27)
                count[11]++;
        }
    }
}

// Apply ReLU to a fixed size array
template<int SIZE>
inline void relu(int vec[SIZE]) {
    for(int i = 0; i < SIZE; i++) {
        if(vec[i] < 0) {
            vec[i] = 0;
        }
    }
}

void update(UI& ui, const std::vector<Register*>& reg) {
    for(auto& r: reg) { ui.display_reg(r); }
}

int analog_main() {
    SCAMP5 s = SCAMP5::builder{}.build();
    UI ui;
    ui.start();

    int threshold_value;
    int t1_value, t2_value, t3_value;
    int recording_value;
    int output_videos_value;

    // VAR DECLARATIONS
    // To read the events (=coordinates of 1s)
    uint8_t coordinates[200];
    // Number of events in the input frame
    uint8_t input_count;
    // Number of consecutively empty input frames
    int empty_input_frames = 0;
    // Flattened output of the convolution layers
    int conv_outputs[36];
    // First fully connected layer output
    int fc1_result[50];
    // Second fully connected layer output
    int fc2_result[10];
    // Output label (index of max. in results)
    uint8_t max_index;

    s.PIX->set_ui_handler(&ui);
    std::vector<Register*> regs;
    regs.push_back(s.PIX);
    regs.push_back(s.A);
    regs.push_back(s.B);
    regs.push_back(s.C);
    regs.push_back(s.D);
    regs.push_back(s.R5);
    regs.push_back(s.R6);
    regs.push_back(s.R7);
    regs.push_back(s.R8);
    regs.push_back(s.R9);
    regs.push_back(s.R10);
    regs.push_back(s.NEWS);
    regs.push_back(s.FLAG);
    regs.push_back(s.E);

    std::random_device rd;   // obtain a random number from hardware
    std::mt19937 gen(rd());  // seed the generator
    std::uniform_int_distribution<> distr(0, 85);  // define the range

    // 67, 74, 71 works for 0
    // 75, 47, 41 works for 1
    // 23, 13, 80 works for 2
    // 79, 36, 46 works for 3
    //  works for 4
    // 29, 29, 12 works for 5
    // 72, 10, 69 works for 6
    //  works for 7
    // 40, 50, 68 works for 8
    // 40, 21, 67 works for 9

    // 20, 80, 10 works for 1, 2, 4 with new scan

    // 40, 60, 35 works for 5, 8, 9
    // 12, 48, 30 works for 1, 5, 9
    // 17, 60, 40 works for 1, 2, 5, 9
    // 30, 50, 40 works for 1, 5, 8, 9
    // 12, 90, 45 works for 1, 2, 4, 8
    // 12, 60, 35 works for 1, 2, 5, 9

    // 17, 75, 10  works for 1, 2, 3, 5, 9 using default scan

    t1_value = 13;  // 30 // 50
    t2_value = 70;  // 15 // 25
    t3_value = 5;   // 50 // 75

    int index = 0;
    // Frame Loop
    while(1) {
        //        vs_process_message();
        std::fill(std::begin(coordinates), std::end(coordinates), 0);
        std::fill(std::begin(conv_outputs), std::end(conv_outputs), 0);
        std::fill(std::begin(fc1_result), std::end(fc1_result), 0);
        std::fill(std::begin(fc2_result), std::end(fc2_result), 0);

        //        t1_value = distr(gen);
        //        t2_value = distr(gen);
        //        t3_value = distr(gen);

        // Get GUI values at start of frame
        threshold_value = 50;

        recording_value = 0;
        output_videos_value = 0;

        // Perform Binarisation Procedure, store
        // binarised input image in R6
        s.scamp5_in(s.D, threshold_value);

        //        s.scamp5_get_image(s.A, s.B, 1);
        s.get_image(s.A, s.B);

        s.add(s.A, s.A, s.D);
        s.CLR(s.R6);
        s.where(s.A);
        s.OR(s.R5, s.FLAG, s.R6);
        s.ALL();
        s.NOT(s.R6, s.R5);
        s.CLR(s.R5);

        // Only preserve data in 28 x 28 square, everything else marked as 0
        // R7 will contain binary image of input within 28 x 28 square
        s.scamp5_draw_begin(s.R5);
        s.scamp5_draw_rect(114, 114, 141, 141);
        s.scamp5_draw_end();

        s.CLR(s.R7);
        s.AND(s.R7, s.R6, s.R5);

        // Convert binary image into analog image with uniform analog value
        // (120) Binary uniform analog images now stored in Registers A, B, C
        s.scamp5_in(s.D, 120);
        s.res(s.A);
        s.res(s.B);
        s.res(s.C);

        s.WHERE(s.R7);
        s.mov(s.A, s.D);
        s.ALL();

        s.mov(s.B, s.A);
        s.mov(s.C, s.A);

        conv_A(s);
        conv_B(s);
        conv_C(s);

        /*
         * Output Thresholding
         */
        // AREG A
        s.scamp5_in(s.F, t1_value);
        s.CLR(s.R8);
        s.sub(s.E, s.A, s.F);
        s.where(s.E);
        s.MOV(s.R8, s.FLAG);
        s.ALL();

        // AREG B
        s.scamp5_in(s.F, t2_value);
        s.CLR(s.R9);
        s.sub(s.E, s.B, s.F);
        s.where(s.E);
        s.MOV(s.R9, s.FLAG);
        s.ALL();

        // AREG C
        s.scamp5_in(s.F, t3_value);
        s.CLR(s.R10);
        s.sub(s.E, s.C, s.F);
        s.where(s.E);
        s.MOV(s.R10, s.FLAG);
        s.ALL();
        update(ui, regs);

        /*
         * COUNT 1s in Convolution Filter Results
         */
        // Process Register A
        s.scamp5_scan_events(s.R8, coordinates, 100);
        sum_pooling_events<100>(coordinates, &conv_outputs[0]);

        // Process Register B
        s.scamp5_scan_events(s.R9, coordinates, 100);
        sum_pooling_events<100>(coordinates, &conv_outputs[12]);

        // Process Register C
        s.scamp5_scan_events(s.R10, coordinates, 100);
        sum_pooling_events<100>(coordinates, &conv_outputs[24]);

        /*
         * Dense Layer Computation
         */
        // Compute neuron results
        fc_1(conv_outputs, fc1_result);
        relu<50>(fc1_result);
        fc_2(fc1_result, fc2_result);

        // Find max index in results
        max_index = 0;
        for(int i = 1; i < 10; i++) {
            //            std::cout << "i: " << i << " - " << fc2_result[i] <<
            //            std::endl;
            if(fc2_result[i] > fc2_result[max_index])
                max_index = i;
        }
        std::cout << "Predicted: " << (int)max_index << std::endl;

        //        if ((int) max_index == 9) {
        //            std::cout << "Matched value: " << (int) max_index <<
        //            std::endl; std::cout << "Found config" << std::endl;
        //            std::cout << "t1_value " << t1_value << std::endl;
        //            std::cout << "t2_value " << t2_value << std::endl;
        //            std::cout << "t3_value " << t3_value << std::endl;
        //            exit(0);
        //        }

        //        if (max_index == index) {
        //            std::cout << "Matched value: " << (int) max_index <<
        //            std::endl;
        //        } else {
        //            std::cout << "No match. Was " << (int) max_index << " but
        //            should be " << index << std::endl;
        //        }
        //        index++;
        //        if (index == 10) {
        //            index = 0;
        ////            exit(0);
        //        }

        // increase loop_counter by 1
        //        vs_loop_counter_inc();
    }
    return 0;
}

#pragma clang diagnostic pop
