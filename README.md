# Focal Plane Sensor Processor Simulator


A parameterisable simulator to model and emulate different FPSP architectures.


Requirements:

  * OpenCV  

  * Cmake  
  
  * Conan 

Works on both Linux and Windows as long as OpenCV is installed in the right place. Conan makes it much easier to deal with package management

# Installation
 * Clone repo
 * `mkdir build && cd build`
 * `conan install ..`
 * `cmake -DCMAKE_BUILD_TYPE=debug ..`
 * `make`

Or just use Clion

