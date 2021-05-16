import json
import os
import subprocess
import pathlib
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt


simulator_path = "/home/jm1417/Simulator/cmake-build-release/bin/simulator"
config_path = "/home/jm1417/Simulator/examples/multiplex/config.json"
program_path = "/home/jm1417/Simulator/examples/scamp5/motion.txt"
output_path = str(pathlib.Path(__file__).parent.absolute())
original_config = None

results = []

def read_json(file):
    with open(file, 'r') as f:
        data = json.load(f)
    return data

def read_config_file():
    data = read_json(config_path)
    global original_config
    original_config = data
    return data

def write_config_file(config):
    with open(config_path, 'w') as f:
        json.dump(config, f, indent=4)

def execute(config):
    write_config_file(config)
    process = subprocess.Popen([simulator_path, config_path, program_path], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out, err = process.communicate()

def mutate(config):
    row_strides = [4, 8, 16, 32, 64, 128, 256]
    col_strides = [4, 8, 16, 32, 64, 128, 256]
    count = 0
    for row_stride in row_strides:
        for col_stride in col_strides:
            config['SCAMP5M']['row_stride'] = row_stride
            config['SCAMP5M']['col_stride'] = col_stride
            config['output_filename'] = str(count)
            print("Executing with row_stride=", row_stride, "col_stride=", col_stride)
            execute(config)
            res = read_json(output_path + "/" + str(count) + ".json")
            results.append((res["Cycle count"], res["Architecture total power"]))
            count+=1


# execute the simulator with a new config
def run():
    config = read_config_file()
    mutate(config)
    write_config_file(original_config)

    for r in results:
        print(r)



def identify_pareto(scores):
    # Count number of items
    population_size = scores.shape[0]
    # Create a NumPy index for scores on the pareto front (zero indexed)
    population_ids = np.arange(population_size)
    # Create a starting list of items on the Pareto front
    # All items start off as being labelled as on the Parteo front
    pareto_front = np.ones(population_size, dtype=bool)
    # Loop through each item. This will then be compared with all other items
    for i in range(population_size):
        # Loop through all other items
        for j in range(population_size):
            # Check if our 'i' pint is dominated by out 'j' point
            if all(scores[j] >= scores[i]) and any(scores[j] > scores[i]):
                # j dominates i. Label 'i' point as not on Pareto front
                pareto_front[i] = 0
                # Stop further comparisons with 'i' (no more comparisons needed)
                break
    # Return ids of scenarios on pareto front
    return population_ids[pareto_front]

def plot_results():
    res = np.array(results)
    pareto = identify_pareto(res)
    pareto_front = res[pareto]
    pareto_front_df = pd.DataFrame(pareto_front)
    pareto_front_df.sort_values(0, inplace=True)
    pareto_front = pareto_front_df.values

    y_all = res[:, 0]
    x_all = res[:, 1]
    y_pareto = pareto_front[:, 0]
    x_pareto = pareto_front[:, 1]

    plt.scatter(x_all, y_all)
    plt.plot(x_pareto, y_pareto, color='r')
    plt.xlabel('Total Power (W)')
    plt.ylabel('Total Cycles')
    plt.savefig("pareto.png")
    plt.show()



run()


plot_results()
