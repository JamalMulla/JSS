//
// Created by jm1417 on 11/05/2021.
//
#include "simulator/metrics/packer.h"

std::shared_ptr<PackNode> Packer::find_node(std::shared_ptr<PackNode> root, double width, double height) {
    if (root->used) {
        std::shared_ptr<PackNode> node = find_node(root->right, width, height);
        if (!node->empty) {
            return node;
        }
        node = find_node(root->down, width, height);
        if (!node->empty) {
            return node;
        }
    } else if (width <= root->width && height <= root->height) {
        return root;
    }
    return std::make_shared<PackNode>();
}

std::shared_ptr<PackNode> Packer::grow_node(double width, double height) {
    bool can_grow_down = width <= this->root_->width;
    bool can_grow_right = height <= this->root_->height;

    bool should_grow_right = can_grow_right && (this->root_->height >= (this->root_->width + width));
    bool should_grow_down = can_grow_down && (this->root_->width >= (this->root_->height + height));

    if (should_grow_right) {
        return grow_right(width, height);
    } else if (should_grow_down) {
        return grow_down(width, height);
    } else if (can_grow_right) {
        return grow_right(width, height);
    } else if (can_grow_down) {
        return grow_down(width, height);
    } else {
        // should never happen
        std::cerr << "Could not pack " << std::endl;
        exit(EXIT_FAILURE);
    }
}

std::shared_ptr<PackNode> Packer::grow_right(double width, double height) {
    this->root_ = std::make_shared<PackNode>(0, 0, root_->width + width, root_->height, this->root_, std::make_shared<PackNode>(root_->width, 0, width, root_->height), true);
    std::shared_ptr<PackNode> node = find_node(this->root_, width, height);
    return split_node(node, width, height);
}

std::shared_ptr<PackNode> Packer::grow_down(double width, double height) {
    this->root_ = std::make_shared<PackNode>(0, 0, root_->width, root_->height + height, std::make_shared<PackNode>(0, root_->height, root_->width, height), this->root_, true);
    std::shared_ptr<PackNode> node = find_node(this->root_, width, height);
    return split_node(node, width, height);
}

std::shared_ptr<PackNode> Packer::split_node(std::shared_ptr<PackNode> node, double width, double height) {
    node->used = true;
    node->down = std::make_shared<PackNode>(node->x, node->y + height, node->width, node->height - height);
    node->right = std::make_shared<PackNode>(node->x + width, node->y, node->width - width, height);
    return node;
}

std::shared_ptr<PackNode> Packer::pack(std::vector<std::shared_ptr<Component>>& components) {
    if (components.empty()) {
        return std::make_shared<PackNode>();
    }

    std::sort(components.begin(), components.end(), [](const auto& lhs, const auto& rhs) {
        return std::max(lhs->get_width(), lhs->get_height()) > std::max(rhs->get_width(), rhs->get_height());
    });

    double w = components[0]->get_width();
    double h = components[0]->get_height();

    this->root_ = std::make_shared<PackNode>(0, 0, w, h);

    for (auto& c: components) {
        std::shared_ptr<PackNode> node = find_node(this->root_, c->get_width(), c->get_height());
        if (!node->empty) {
            c->fit = this->split_node(node, c->get_width(), c->get_height());
        } else {
            c->fit = this->grow_node(c->get_width(), c->get_height());
        }
    }

    return this->root_;
}
