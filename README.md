# Focal Plane Sensor Processor Simulator


A parameterisable simulator to model and emulate different FPSP architectures. Currently only the SCAMP-5 architecture has been (mostly) implementedsupports. The SCAMP-5 API documentation can be found [here](https://personalpages.manchester.ac.uk/staff/jianing.chen/scamp5d_lib_doc_html/_p_a_g_e__d_e_v_i_c_e__a_p_i__c_a_t_e_g_o_r_y.html).


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

Or just use Clion.

# Usage
The architectures/ directory and main.cpp shows how programs can be run on the simulator.
