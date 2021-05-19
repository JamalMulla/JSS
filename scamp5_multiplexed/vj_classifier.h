//
// Created by jm1417 on 18/05/2021.
//

#ifndef SIMULATOR_VJ_CLASSIFIER_H
#define SIMULATOR_VJ_CLASSIFIER_H

#include <vector>
#include <memory>

class VjClassifier {
   public:
    VjClassifier() = default;

    std::shared_ptr<std::vector<int>> rectangles_array_;
    std::shared_ptr<std::vector<int>> scaled_rectangles_array_;
    std::shared_ptr<std::vector<int>> weights_array_;
    std::shared_ptr<std::vector<int>> alpha1_array_;
    std::shared_ptr<std::vector<int>> alpha2_array_;
    std::shared_ptr<std::vector<int>> tree_thresh_array_;
    std::shared_ptr<std::vector<int>> stages_thresh_array_;

    void set_rectangles_array(std::shared_ptr<std::vector<int>> rectangles_array);
    void set_scaled_rectangles_array(std::shared_ptr<std::vector<int>> scaled_rectangles_array);
    void set_weights_array(std::shared_ptr<std::vector<int>> weights_array);
    void set_alpha1_array(std::shared_ptr<std::vector<int>> alpha1_array);
    void set_alpha2_array(std::shared_ptr<std::vector<int>> alpha2_array);
    void set_tree_thresh_array(std::shared_ptr<std::vector<int>> tree_thresh_array);
    void set_stages_thresh_array(std::shared_ptr<std::vector<int>> stages_thresh_array);

};

#endif  //SIMULATOR_VJ_CLASSIFIER_H
