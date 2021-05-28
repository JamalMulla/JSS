# Focal Plane Sensor Processor Simulator

A parameterisable simulation library to model and emulate different FPSP architectures. Currently only the SCAMP-5
architecture has been (mostly) implemented. The SCAMP-5 API documentation can be
found [here](https://personalpages.manchester.ac.uk/staff/jianing.chen/scamp5d_lib_doc_html/_p_a_g_e__d_e_v_i_c_e__a_p_i__c_a_t_e_g_o_r_y.html)
.

Requirements:

* C++
* Cmake
* Conan

Composed of two parts: the simulation library and the architecture you're simulating. The existing CMakeLists.txt will build the library first and then the SCAMP-5 architecture.

# Compile time options

There are various compile time options which conditionally compile code. The options can be toggled in the
CMakeLists.txt file or by passing in the option to CMake,
e.g. `cmake -DCMAKE_BUILD_TYPE=release .. -DTRACK_STATISTICS=1`.

Tracking statistics is expensive and will drastically slow down execution.

| Name | Description | Default |
| ------------- | ------------- | ------------- |
| USE_RUNTIME_CHECKS  | Checks arguments at runtime and emits warnings. | OFF |
| TRACK_STATISTICS | Tracks statistics such as register reads and energy use| OFF |

# Build - (release|debug)
(remmeber to remove the brackets around the choice before running)

* Clone repo
* `mkdir cmake-build-(release|debug) && cd cmake-build-(release|debug)`
* `conan profile update settings.compiler.libcxx=libstdc++11 default`
* `conan create ../conanfile.py -s build_type=(Release|Debug)`
* `conan install ../conanfile.txt -s build_type=(Release|Debug) --build missing`
* `cmake -DCMAKE_BUILD_TYPE=(release|debug) ..`
* `make -j`

# Runtime configuration
The simulator can be configured with a json file passed in as the first argument. There is an example `config.json` file in `examples/`. 

The format is hierarchical and relates directly to classes in the simulator.

The main fields are explained here:

| Field name | Required | Default | Description |
| ------------- | ------------- | ------------- | ------------- |
| use_gpu | false | false | Whether to use GPU for processing or CPU. If set and no GPU is available will default to CPU |
| architecture  | true | N/A | The name of the architecture that is being simulated. The architecture must be registered and should be a subclass of the `Architecture` class. See `SCAMP5.cpp` for an example architecture |
| frames | false | 1000 | The number of frames to run the program for. A negative number is interpreted as infinite, i.e. run forever|
| frame_time | false | true | Prints the amount of time in ms to process each frame from beginning to end to stdout|
| with_stats | false | false | If this is enabled the statistics of the run will be printed at the end. Must be compiled with statistics support for this to do anything|
| ui_enabled | false | false | Should the web UI server be enabled|
| ui_registers_to_display | true if ui_enabled is set | false | The registers to display on the web UI. Must be defined in the config somewhere and added to the cache (or a property of the architecture)|
| _inherit | false | false | Special field. Sometimes the child component needs to inherit from the parent. This is achieved by use of this special field. The field must be a list of all fields to inherit which have been defined previously. Inheritance is not strictly limited to parents. In fact any property defined previously at any level can be used. This means that if you define the same property in different components only the latest will be preserved.|
| _name | false | false | Special field. Defines the identifier of the component|
| _component | false | false | Specifal field. The component class. Must match exactly so the correct class can be found and constructed|


The architecture set in the `architecture` field must be defined as an object. This part is hierarchical. You can add define each component that is necessary in the definition of the architecture. 

Each architecture or component must have an empty constructor which will be called first to construct the object. The constructor must be registered.

Each field in the definition of the component must have a matching registered setter in the architecture or component class. For example the field `rows` must have a setter called `set_rows(int)` in the architecture class. The setter must always be named `set_+FIELD`.

More complicated fields will likely also need a converter method. This method must have the signature `rttr::variant FIELD_converter(json& j)` or`rttr::variant FIELD_converter(json& j, std::unordered_map<std::string, rttr::variant>& cache)` if you want to be able to add to the cache. 

If you want to do something after all the fields have been set (basically a pseudo constructor), then register a `void init()` method which will be called last if it exists.

# UI
There is a browser based UI which can be used to display the outputs of the registers in realtime. You can use it like this:
```
UI& ui = UI::get_instance();
```
Then in the main loop, you can display a register by calling
```
ui.display_reg(REGISTER_HERE);
```
which will send the image to the web client. 

By default the client can be accessed at `localhost:3000`



# Tests

The test suite can be run with `make test` after the above build steps.

Alternative you can run the `tests` binary in the `bin` folder which has many more options. Run `./tests -?` to see
available options.

# Usage

Once you have a simulator binary built you can run it with a config and a program like this:
`./simulator config.json program.txt`
