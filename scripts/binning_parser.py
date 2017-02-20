""" Dalitz plot text files binning """

import sys
import re
import numpy as np

BINS_PATH = "/home/vitaly/B0toD0pipi/B0toD0pipiFeas/params/"
FLOATS = re.compile(r"[-+]?\d+\.\d+")

def get_path(file_name):
    """ Add prefix if needed """
    if file_name[0] != '/':
        return BINS_PATH + file_name
    return file_name

def parse_bins(file_name):
    """ Parse text file """
    with open(get_path(file_name)) as binf:
        grid_size = int(binf.readline().split()[1])
        massstr = binf.readline().split()
        masses = np.array([float(val) for val in [massstr[0]] + massstr[2:]])
        lims_ab = [float(val) for val in binf.readline().split()[1:]]
        lims_ac = [float(val) for val in binf.readline().split()[1:]]
        lims_bc = [float(val) for val in binf.readline().split()[1:]]
        limits = np.array([lims_ab, lims_ac, lims_bc])
        titles = [binf.readline() for _ in xrange(3)]
        binning = []
        for line in binf.readlines():
            data = line.split()
            binning.append(tuple([float(data[0]), float(data[1]), float(data[2]), int(data[3])]))
        binning = np.array(binning,\
        dtype=[("mAB", float), ("mAC", float), ("mBC", float), ("bin", int)])
        return binning, grid_size, masses, limits, titles

def parse_csk(file_name):
    """ Read parameters of binned Dalitz distribution """
    dppar = []
    for line in open(get_path(file_name)).readlines():
        dppar.append([float(val) for val in FLOATS.findall(line)][:-1])
    return np.array(dppar)

def print_bin_info(binning, grid_size, masses, limits, titles):
    """ Testing function """
    print "Grid size:", grid_size
    print masses[0], "->", masses[1], masses[2], masses[3]
    print "Limits"
    print limits
    print "Titles:", titles
    print "Binning:"
    print binning['mAB'][binning['bin'] == 4]

def main(file_name):
    """ Unit test """
    cskq = parse_csk(file_name)
    print cskq
    # binning, grid_size, masses, limits, titles = parse_bins(file_name)
    # print_bin_info(binning, grid_size, masses, limits, titles)

if __name__ == "__main__":
    if len(sys.argv) == 0:
        print "Usage: python binning_parser.py [file_name]"
    else:
        main(sys.argv[1])
