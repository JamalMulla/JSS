# Focal Plane Sensor Processor Simulator


A parameterisable simulation library to model and emulate different FPSP architectures. Currently only the SCAMP-5 architecture has been (mostly) implemented. The SCAMP-5 API documentation can be found [here](https://personalpages.manchester.ac.uk/staff/jianing.chen/scamp5d_lib_doc_html/_p_a_g_e__d_e_v_i_c_e__a_p_i__c_a_t_e_g_o_r_y.html).


Requirements: 
  * Cmake  
  * Conan 

Works on both Linux and Windows as long as Conan is installed.

Composed of two parts: the simulation library and the architecture you're simulating. The existing CMakeLists.txt will build the library first and then the SCAMP-5 architecture.

# Build - release (debug)
 * Clone repo
 * `mkdir cmake-build-release && cd cmake-build-release` (`mkdir cmake-build-debug && cd cmake-build-debug`)
 * `conan install ..`
 * `cmake -DCMAKE_BUILD_TYPE=release ..` (`cmake -DCMAKE_BUILD_TYPE=debug ..`)
 * `make`

Or just use Clion.

# Usage
The architectures/ directory and main.cpp shows how an architecture can be defined and programs can be run on that architecture.
