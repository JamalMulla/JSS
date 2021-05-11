//
// Created by jm1417 on 11/05/2021.
//
#include "simulator/metrics/packer.h"

PackNode* Packer::find_node(PackNode* root, int width, int height) {
    if (root_->used) {
        PackNode* node;
        node = find_node(root_->right, width, height);
        if (node != nullptr) {
            return node;
        }
        node = find_node(root_->down, width, height);
        if (node != nullptr) {
            return node;
        }
    } else if (width <= root_->width && height <= root_->height) {
        return root;
    }
    return nullptr;
}

PackNode* Packer::grow_node(int width, int height) {
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
        std::cerr << "Could not pack " << std::endl;
        exit(EXIT_FAILURE);
    }
}

PackNode* Packer::grow_right(int width, int height) {
    this->root_ = new PackNode(0, 0, root_->width + width, root_->height, this->root_, new PackNode(root_->width, 0, width, root_->height), true);
    PackNode* node = find_node(this->root_, width, height);
    return split_node(node, width, height);
}

PackNode* Packer::grow_down(int width, int height) {
    this->root_ = new PackNode(0, 0, root_->width, root_->height + height, new PackNode(0, root_->height, root_->width, height), this->root_, true);
    PackNode* node = find_node(this->root_, width, height);
    return split_node(node, width, height);
}

PackNode* Packer::split_node(PackNode* node, int width, int height) {
    node->used = true;
    node->down = new PackNode(node->x, node->y + height, node->width, node->height - height);
    node->right = new PackNode(node->x + width, node->y, node->width - width, height);
    return node;
}

PackNode* Packer::pack(std::vector<Component>& components) {
    if (components.empty()) {
        return nullptr;
    }

    std::sort(components.begin(), components.end(), [](const auto& lhs, const auto& rhs) {
        return std::max(lhs.get_width(), lhs.get_height()) > std::max(rhs.get_width(), rhs.get_height());
    });

    int w = components[0].get_width();
    int h = components[1].get_height();

    this->root_ = new PackNode(0, 0, w, h);

    for (auto& c : components) {
        PackNode* node = find_node(this->root_, c.get_width(), c.get_height());
        if (node != nullptr) {
            c.fit = this->split_node(node, c.get_width(), c.get_height());
        } else {
            c.fit = this->grow_node(c.get_width(), c.get_height());
        }
    }

    return this->root_;
}
