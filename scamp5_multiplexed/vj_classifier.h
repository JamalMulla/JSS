//
// Created by jm1417 on 18/05/2021.
//

#ifndef SIMULATOR_VJ_CLASSIFIER_H
#define SIMULATOR_VJ_CLASSIFIER_H

#include <memory>
#include <vector>

#include "image.h"

class VjClassifier {
   public:
    std::shared_ptr<Image> sum_img;
    std::shared_ptr<Image> sqsum_img;

    int stages = 7;
    int total_nodes = 2913;
    int original_width = 24;
    int original_height = 24;

    int sp0, sp1, sp2, sp3; // left top, right top, left bottom, right bottom indices of integral image
    int sqp0, sqp1, sqp2, sqp3; // left top, right top, left bottom, right bottom indices of squared integral image

    int inv_window_area;


    VjClassifier() = default;

    std::shared_ptr<std::vector<int>> stages_array_;
    std::shared_ptr<std::vector<int>> rectangles_array_;
    std::shared_ptr<std::vector<int>> scaled_rectangles_array_;
    std::shared_ptr<std::vector<int>> weights_array_;
    std::shared_ptr<std::vector<int>> alpha1_array_;
    std::shared_ptr<std::vector<int>> alpha2_array_;
    std::shared_ptr<std::vector<int>> tree_thresh_array_;
    std::shared_ptr<std::vector<int>> stages_thresh_array_;

    void set_stages_array(std::shared_ptr<std::vector<int>> stages_array);
    void set_rectangles_array(std::shared_ptr<std::vector<int>> rectangles_array);
    void set_scaled_rectangles_array(std::shared_ptr<std::vector<int>> scaled_rectangles_array);
    void set_weights_array(std::shared_ptr<std::vector<int>> weights_array);
    void set_alpha1_array(std::shared_ptr<std::vector<int>> alpha1_array);
    void set_alpha2_array(std::shared_ptr<std::vector<int>> alpha2_array);
    void set_tree_thresh_array(std::shared_ptr<std::vector<int>> tree_thresh_array);
    void set_stages_thresh_array(std::shared_ptr<std::vector<int>> stages_thresh_array);

};

#endif  //SIMULATOR_VJ_CLASSIFIER_H
