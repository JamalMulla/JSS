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

#include <cstdint>
#include <simulator/util/utility.h>
#include <simulator/ui/ui.h>
#include "conv_instructions.h"
#include "fc_weights.h"


// Given SCAN_SIZE coordinates,
// group them into 12 bins, filling the count array
// central division, with overlaping bins
template <int SCAN_SIZE>
inline void sum_pooling_events(const uint8_t coordinates[2*SCAN_SIZE], int count[12]){
    uint8_t x,y;
    for(int i = 0; i < 12; i++){
        count[i] = 0;
    }
    for(int i = 0; i < SCAN_SIZE; i++)
    {
        x = coordinates[2*i];
        y = coordinates[2*i+1];
        if(x == 0 && y == 0)
            return;
        if(x >= 114 && x <= 141 && y >= 114 && y <= 141){
            x = x-114;
            y = y-114;

            if(x >= 5 && x <= 13 && y >= 0 && y <= 8)
                count[0] ++;
            if(x >= 14 && x <= 22 && y >= 0 && y <= 8)
                count[1] ++;

            if(x >= 0 && x <= 8 && y >= 5 && y <= 13)
                count[2] ++;
            if(x >= 5 && x <= 13 && y >= 5 && y <= 13)
                count[3] ++;
            if(x >= 14 && x <= 22 && y >= 5 && y <= 13)
                count[4] ++;
            if(x >= 19 && x <= 27 && y >= 5 && y <= 13)
                count[5] ++;

            if(x >= 0 && x <= 8 && y >= 14 && y <= 22)
                count[6] ++;
            if(x >= 5 && x <= 13 && y >= 14 && y <= 22)
                count[7] ++;
            if(x >= 14 && x <= 22 && y >= 14 && y <= 22)
                count[8] ++;
            if(x >= 19 && x <= 27 && y >= 14 && y <= 22)
                count[9] ++;

            if(x >= 5 && x <= 13 && y >= 19 && y <= 27)
                count[10] ++;
            if(x >= 14 && x <= 22 && y >= 19 && y <= 27)
                count[11] ++;
        }
    }
}


// Apply ReLU to a fixed size array
template <int SIZE>
inline void relu(int vec[SIZE]){
    for (int i=0; i< SIZE; i++){
        if (vec[i] < 0){
            vec[i] = 0;
        }
    }
}

void update(UI& ui, const std::vector<Register*>& reg) {
    for (auto& r : reg) {
        ui.display_reg(*r);
    }
}

int analog_main(){
    SCAMP5 s;
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

    s.PIX.set_ui_handler(&ui);
    std::vector<Register*> regs;
    regs.push_back(&s.PIX);
    regs.push_back(&s.A);
    regs.push_back(&s.B);
    regs.push_back(&s.C);
    regs.push_back(&s.D);
    regs.push_back(&s.R5);
    regs.push_back(&s.R6);
    regs.push_back(&s.R7);
    regs.push_back(&s.R8);
    regs.push_back(&s.R9);
    regs.push_back(&s.R10);
    regs.push_back(&s.NEWS);
    regs.push_back(&s.FLAG);
    regs.push_back(&s.E);
    // Frame Loop
    while(1){
//        vs_process_message();

        // Get GUI values at start of frame
        threshold_value = 120;
        t1_value = 40; //30
        t2_value = 50; //15
        t3_value = 50; //50
        recording_value = 0;
        output_videos_value = 0;

        // Perform Binarisation Procedure, store
        // binarised input image in R6
        s.scamp5_in(s.D, threshold_value);

//        s.scamp5_get_image(s.A, s.B, 1);
        s.get_image(s.A, s.B);
        update(ui, regs);

        s.add(s.A, s.A, s.D);
        s.CLR(s.R6);
        s.where(s.A);
        update(ui, regs);
        s.OR(s.R5, s.FLAG, s.R6);
        s.ALL();
        s.NOT(s.R6, s.R5);
        s.CLR(s.R5);
        update(ui, regs);


        // Only preserve data in 28 x 28 square, everything else marked as 0
        // R7 will contain binary image of input within 28 x 28 square
        s.scamp5_draw_begin(s.R5);
        s.scamp5_draw_rect(114, 114, 141, 141);
        s.scamp5_draw_end();
        update(ui, regs);


//        scamp5_kernel_begin();
        s.CLR(s.R7);
        s.AND(s.R7, s.R6, s.R5);
        update(ui, regs);

//        scamp5_kernel_end();

        // Convert binary image into analog image with uniform analog value (120)
        // Binary uniform analog images now stored in Registers A, B, C
        s.scamp5_in(s.D, 120);
//        scamp5_kernel_begin();
        s.res(s.A);
        s.res(s.B);
        s.res(s.C);

        s.WHERE(s.R7);
        s.mov(s.A, s.D);
        s.ALL();
        update(ui, regs);


        s.mov(s.B, s.A);
        s.mov(s.C, s.A);
        update(ui, regs);


//        scamp5_kernel_end();

        conv_A(s);
        update(ui, regs);
        conv_B(s);
        update(ui, regs);
        conv_C(s);
        update(ui, regs);

        /*
         * Output Thresholding
         */
        // AREG A
        s.scamp5_in(s.F, t1_value);
//        scamp5_kernel_begin();
        s.CLR(s.R8);
        s.sub(s.E, s.A, s.F);
        s.where(s.E);
//        utility::display_register<uint8_t>("FLAG", s.FLAG);
//        utility::display_register<int16_t>("E", s.E);
//        utility::display_register<int16_t>("A", s.A);
//        utility::display_register<int16_t>("F", s.F);
//        cv::waitKey();
        s.MOV(s.R8, s.FLAG);
        s.ALL();
        update(ui, regs);

//        scamp5_kernel_end();

        // AREG B
        s.scamp5_in(s.F, t2_value);
//        scamp5_kernel_begin();
        s.CLR(s.R9);
        s.sub(s.E, s.B, s.F);
        s.where(s.E);
        s.MOV(s.R9, s.FLAG);
        s.ALL();
        update(ui, regs);

//        scamp5_kernel_end();

        // AREG C
        s.scamp5_in(s.F, t3_value);
//        scamp5_kernel_begin();
        s.CLR(s.R10);
        s.sub(s.E, s.C, s.F);
        s.where(s.E);
        s.MOV(s.R10, s.FLAG);
        s.ALL();
        update(ui, regs);

//        scamp5_kernel_end();

        ///////////////////////////////////////////////////
        //Processing from here is on microcontroller
        ///////////////////////////////////////////////////

        /*
         * COUNT 1s in Convolution Filter Results
         */
        // Process Register A
//        s.scamp5_scan_events(s.R8, coordinates, 100, 114, 114, 140, 141, 1, 1);
        s.scamp5_scan_events(s.R8, coordinates, 100);
        sum_pooling_events<100>(coordinates, &conv_outputs[0]);

        // Process Register B
        s.scamp5_scan_events(s.R9, coordinates, 100);
//        s.scamp5_scan_events(s.R9, coordinates, 100, 114, 114, 141, 141, 1, 1);
        sum_pooling_events<100>(coordinates, &conv_outputs[12]);

        // Process Register C
        s.scamp5_scan_events(s.R10, coordinates, 100);
//        s.scamp5_scan_events(s.R10, coordinates, 100, 114, 114, 141, 141, 1, 1);
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
        for (int i=1; i<10; i++){
            std::cout << "i: " << i << " - " << fc2_result[i] << std::endl;
            if (fc2_result[i] > fc2_result[max_index])
                max_index = i;
        }

        std::cout << "Max index: " << (int) max_index << std::endl;

        // increase loop_counter by 1
//        vs_loop_counter_inc();
    }
    return 0;
}


#pragma clang diagnostic pop