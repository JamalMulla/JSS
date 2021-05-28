//
// Created by jm1417 on 18/05/2021.
//

#include "vj_classifier.h"

#include <utility>

void VjClassifier::set_stages_array(std::shared_ptr<std::vector<int>> stages_array) {
    this->stages_array_ = std::move(stages_array);
}

void VjClassifier::set_rectangles_array(std::shared_ptr<std::vector<int> > rectangles_array) {
    this->rectangles_array_ = std::move(rectangles_array);
}

void VjClassifier::set_scaled_rectangles_array(std::shared_ptr<std::vector<int> > scaled_rectangles_array) {
    this->scaled_rectangles_array_ = std::move(scaled_rectangles_array);
}

void VjClassifier::set_weights_array(std::shared_ptr<std::vector<int> > weights_array) {
    this->weights_array_ = std::move(weights_array);
}

void VjClassifier::set_alpha1_array(std::shared_ptr<std::vector<int> > alpha1_array) {
    this->alpha1_array_ = std::move(alpha1_array);
}

void VjClassifier::set_alpha2_array(std::shared_ptr<std::vector<int> > alpha2_array) {
    this->alpha2_array_ = std::move(alpha2_array);
}

void VjClassifier::set_tree_thresh_array(std::shared_ptr<std::vector<int> > tree_thresh_array) {
    this->tree_thresh_array_ = std::move(tree_thresh_array);
}

void VjClassifier::set_stages_thresh_array(std::shared_ptr<std::vector<int> > stages_thresh_array) {
    this->stages_thresh_array_ = std::move(stages_thresh_array);
}
