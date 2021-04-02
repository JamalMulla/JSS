//
// Created by jm1417 on 05/03/2021.
//

#ifndef SIMULATOR_BASE64_ENCODER_H
#define SIMULATOR_BASE64_ENCODER_H

#include <cstdint>
#include <string>

namespace base64 {
std::string encode(const unsigned char* src, size_t len);
}

#endif  // SIMULATOR_BASE64_ENCODER_H
