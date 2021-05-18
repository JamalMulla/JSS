import json
import os
import subprocess
import pathlib
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import itertools

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
    row_strides = [2, 8, 16, 256]
    col_strides = [2, 8, 16, 256]
    rows        = [128, 256]
    cols        = [128, 256]
    clock_rates = [10000000, 50000000, 100000000]

    possibilities = list(itertools.product(row_strides,col_strides,rows, cols, clock_rates))
    print("Combinations:", len(possibilities))

    count = 0
    for row_stride, col_stride, rows, cols, clock_rate in possibilities:
        if (row_stride > rows or col_stride > cols):
            continue
        config['SCAMP5M']['rows'] = rows
        config['SCAMP5M']['cols'] = cols
        config['SCAMP5M']['row_stride'] = row_stride
        config['SCAMP5M']['col_stride'] = col_stride
        config['SCAMP5M']['config']['clock_rate'] = clock_rate
        config['output_filename'] = str(count)
        print("Executing with row_stride=", row_stride, "col_stride=", col_stride, "rows=", rows, "cols=", cols, "clock=", clock_rate)
        execute(config)
        try:
            res = read_json(output_path + "/" + str(count) + ".json")
            results.append((res["Cycle count"], res["Architecture total power"]))
        except:
            print("Could not add")
        count+=1



# execute the simulator with a new config
def run():
    config = read_config_file()
    mutate(config)
    write_config_file(original_config)

    for r in results:
        print(r)


def is_outlier(points, thresh=6):
    """
    Returns a boolean array with True if points are outliers and False
    otherwise.

    Parameters:
    -----------
        points : An numobservations by numdimensions array of observations
        thresh : The modified z-score to use as a threshold. Observations with
            a modified z-score (based on the median absolute deviation) greater
            than this value will be classified as outliers.

    Returns:
    --------
        mask : A numobservations-length boolean array.

    References:
    ----------
        Boris Iglewicz and David Hoaglin (1993), "Volume 16: How to Detect and
        Handle Outliers", The ASQC Basic References in Quality Control:
        Statistical Techniques, Edward F. Mykytka, Ph.D., Editor.
    """
    if len(points.shape) == 1:
        points = points[:,None]
    median = np.median(points, axis=0)
    diff = np.sum((points - median)**2, axis=-1)
    diff = np.sqrt(diff)
    med_abs_deviation = np.median(diff)

    modified_z_score = 0.6745 * diff / med_abs_deviation

    return modified_z_score > thresh


'''
Method to take two equally-sized lists and return just the elements which lie 
on the Pareto frontier, sorted into order.
Default behaviour is to find the maximum for both X and Y, but the option is
available to specify maxX = False or maxY = False to find the minimum for either
or both of the parameters.
'''
def pareto_frontier(Xs, Ys, maxX = True, maxY = True):
    # Sort the list in either ascending or descending order of X
    myList = sorted([[Xs[i], Ys[i]] for i in range(len(Xs))], reverse=maxX)
    # Start the Pareto frontier with the first value in the sorted list
    p_front = [myList[0]]
    # Loop through the sorted list
    for pair in myList[1:]:
        if maxY:
            if pair[1] >= p_front[-1][1]: # Look for higher values of Y…
                p_front.append(pair) # … and add them to the Pareto frontier
        else:
            if pair[1] <= p_front[-1][1]: # Look for lower values of Y…
                p_front.append(pair) # … and add them to the Pareto frontier
    # Turn resulting pairs back into a list of Xs and Ys
    p_frontX = [pair[0] for pair in p_front]
    p_frontY = [pair[1] for pair in p_front]
    return p_frontX, p_frontY

def plot_results():
    res = np.array(results)
    cycles = res[:, 0]
    powers = res[:, 1]
    pareto_front = pareto_frontier(powers, cycles, False, False)

    plt.scatter(powers, cycles)
    plt.plot(pareto_front[0], pareto_front[1], color='r')
    plt.xlim(0, 0.5)
    plt.ylim(0, 0.4e7)
    plt.xlabel('Total Power (W)')
    plt.ylabel('Total Cycles')
    plt.savefig("pareto.png")
    plt.show()



run()

plot_results()
