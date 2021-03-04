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

#include <cstdint>
#include <simulator/util/utility.h>
#include "conv_instructions.h"
#include "fc_weights.h"


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
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
    return;
}

int analog_main(){
    SCAMP5 s;

    // Initialization
//    vs_init();
    //vs_stopwatch stopwatch;

    /////////////////
    // Setup Host GUI
    /////////////////
    // DISPLAYS:
    // First convolution result and binarised version
//    auto display_a = vs_gui_add_display("Register A",0,0);
//    auto display_d = vs_gui_add_display("R8",0,1);
    // Second convolution result and binarised version
//    auto display_b = vs_gui_add_display("Register B",1,0);
//    auto display_e = vs_gui_add_display("R9",1,1);
    // Third convolution result and binarised version
//    auto display_c = vs_gui_add_display("Register C",2,0);
//    auto display_f = vs_gui_add_display("R10",2,1);
    // Input image
//    auto original_input = vs_gui_add_display("Original Input",2,2);
    // Binarised input image
//    auto display_5 = vs_gui_add_display("R5",0,2);
    // Cropped (28*28 square) binarised input image
//    auto display_6 = vs_gui_add_display("R6",1,2);

    // SLIDERS:
    // Input binarisation threshold
//    auto threshold = vs_gui_add_slider("Input thresh: ",10,127,50);
    int threshold_value;
    // Output thresholds for each of the 3 convolutions
//    auto t1 = vs_gui_add_slider("Thresh conv1: ",10,100,30);
//    auto t2 = vs_gui_add_slider("Thresh conv2: ",10,100,15);
//    auto t3 = vs_gui_add_slider("Thresh conv3: ",10,100,50);
    int t1_value, t2_value, t3_value;
//    auto recording = vs_gui_add_slider("Recording: ",0,1,0);
    int recording_value;
//    auto output_videos = vs_gui_add_slider("Output videos: ",0,1,0);
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

//    vs_on_host_connect([&](){
//        vs_post_text("\nLegacy AnalogNet architecture\n");
//        vs_led_on(VS_LED_2);
//    });

//    vs_on_host_disconnect([&](){
//        vs_led_off(VS_LED_2);
//    });

    // Set framerate to 12 fps,
    // with constant loop time
//    vs_configure_frame_trigger(VS_FRAME_TRIGGER_CONST_LOOP, 12);

    // Frame Loop
    while(1){
//        vs_process_message();

        // Get GUI values at start of frame
        threshold_value = 50;
        t1_value = 30;
        t2_value = 15;
        t3_value = 50;
        recording_value = 0;
        output_videos_value = 0;

        //vs_post_text("%lu\n", stopwatch.get_usec());
        //stopwatch.reset();

        /*
         * Begin Input
         */
        // Perform Binarisation Procedure, store
        // binarised input image in R6
//        vs_wait_frame_trigger();
        s.scamp5_in(s.D, threshold_value);

        s.scamp5_get_image(s.A, s.B, 1);

//        scamp5_kernel_begin();
        s.add(s.A, s.A, s.D);
        s.CLR(s.R6);
        s.where(s.A);
        s.OR(s.R5, s.FLAG, s.R6);
        s.ALL();
        s.NOT(s.R6, s.R5);
        s.CLR(s.R5);
//        scamp5_kernel_end();

        // Only preserve data in 28 x 28 square, everything else marked as 0
        // R7 will contain binary image of input within 28 x 28 square
        s.scamp5_draw_begin(s.R5);
        s.scamp5_draw_rect(114, 114, 141, 141);
        s.scamp5_draw_end();

//        scamp5_kernel_begin();
        s.CLR(s.R7);
        s.AND(s.R7, s.R6, s.R5);
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

        s.mov(s.B, s.A);
        s.mov(s.C, s.A);
//        scamp5_kernel_end();

        // Display results of binarization:
        // original input; binarised; binarised and cropped
//        if (output_videos_value){
//            scamp5_output_image(A, original_input);
//            scamp5_output_image(R6,display_6);
//            scamp5_output_image(R7,display_5);
//        }

        /*
         * (binary Images are now in Registers A, B and C)
         * CONVOLUTIONS: filters applied and
         * results stored on registers A, B and C
         */
        conv_A(s);
        conv_B(s);
        conv_C(s);

        /*
         * Output Thresholding
         */
        // AREG A
        s.scamp5_in(s.F, t1_value);
//        scamp5_kernel_begin();
        s.CLR(s.R8);
        s.sub(s.E, s.A, s.F);
        s.where(s.E);
        s.MOV(s.R8, s.FLAG);
        s.ALL();
//        scamp5_kernel_end();

        // AREG B
        s.scamp5_in(s.F, t2_value);
//        scamp5_kernel_begin();
        s.CLR(s.R9);
        s.sub(s.E, s.B, s.F);
        s.where(s.E);
        s.MOV(s.R9, s.FLAG);
        s.ALL();
//        scamp5_kernel_end();

        // AREG C
        s.scamp5_in(s.F, t3_value);
//        scamp5_kernel_begin();
        s.CLR(s.R10);
        s.sub(s.E, s.C, s.F);
        s.where(s.E);
        s.MOV(s.R10, s.FLAG);
        s.ALL();
//        scamp5_kernel_end();

        ///////////////////////////////////////////////////
        //Processing from here is on microcontroller
        ///////////////////////////////////////////////////

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

//        if (recording_value){
//            // If recording the output of the conv layers,
//            // count the events in the binarised input. If no event is found,
//            // a white screen was shown to the camera. This is used
//            // to synchronise the acquisition process with the
//            // display process.
//            scamp5_scan_events(R7, coordinates, 10);
//            input_count = 0;
//            for(int i = 0; i < 10; i++){
//                if (coordinates[2*i] != 0 || coordinates[2*i+1] != 0)
//                    input_count ++;
//            }
//            if(input_count <= 3){
//                empty_input_frames ++;
//                // After 300 empty frames (25 s at 60 fps),
//                // save and clear current output
//                if(empty_input_frames == 300){
//                    vs_post_text("!save");
//                    vs_post_text("!clear");
//                }
//                // After 720 empty frames (1 min at 60 fps),
//                // forever loop (ie stop main processing loop)
//                if(empty_input_frames > 720){
//                    while(true){
//                    }
//                }
//            } else {
//                empty_input_frames = 0;
//            }
//
//            // Structure of one output line:
//            // an array of 37 elements: the first one is the count
//            // of 1s in the input binarised image, the 36 others
//            // is the output of the conv layer
//            vs_post_text("[%i, ", input_count);
//            for(int i=0; i<35; i++)
//                vs_post_text("%i, ", conv_outputs[i]);
//            vs_post_text("%i]\n", conv_outputs[35]);
//        }
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
            if (fc2_result[i] > fc2_result[max_index])
                max_index = i;
        }

        if(!recording_value){
            // Output value of each neuron
            /*
            for (int i=0; i<10; i++){
                  vs_post_text("%i,", fc2_result[i]);
            }
            vs_post_text("\n");
            */

            // Output value of max index (=output label)
//            vs_post_text("%i,\n\n", max_index);
            std::cout << "Max index: " << (int) max_index << std::endl;
        }


        utility::display_register("A", s.A);
        utility::display_register("B", s.B);
        utility::display_register("C", s.C);
        utility::display_register("R5", s.R5);
        utility::display_register("R8", s.R8);
        utility::display_register("R9", s.R9);
        utility::display_register("R10", s.R10);
        cv::waitKey(1);
        // Output images to GUI
//        if (output_videos_value){
//            scamp5_output_image(A,display_a);
//            scamp5_output_image(B,display_b);
//            scamp5_output_image(C,display_c);
//            scamp5_output_image(R8,display_d);
//            scamp5_output_image(R9,display_e);
//            scamp5_output_image(R10,display_f);
//        }

        // increase loop_counter by 1
//        vs_loop_counter_inc();
    }
    return 0;
}


#pragma clang diagnostic pop