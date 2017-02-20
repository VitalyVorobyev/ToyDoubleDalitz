""" Tools for getting info from log files """

import glob
import re
import numpy as np
import ddcfg
import fit

RAD_TO_DEG = 180. / np.pi

FITRESREX = re.compile(r'.*: -?\d{1}\.\d* -> -?\d{1}\.\d* \+- \d{1}\.\d*')

def fit_regex(exp, gen, ftype):
    """ Regexpt for log files """
    return ddcfg.TOPDIR + "logs/" + ddcfg.DIRDICT[exp] + \
           "log_*_" + ftype + "_" + exp + "_" + gen + "_fit.txt"

def get_params():
    """ Read command line input """
    print "Type: [Experiment: belle*, lhcb] [generation: 1*, 2, 3] [fittype: sim*, dd]"
    print "( * <- default value )"
    params = raw_input().strip().split()
    if len(params) == 3:
        exp, gen, ftype = params
    elif len(params) == 2:
        exp, gen = params
        ftype = "sim"
    elif len(params) == 1:
        exp = params[0]
        gen, ftype = "1", "sim"
    else:
        exp, gen, ftype = "belle", "1", "sim"
    return exp, gen, ftype

def parse_fit_log(fname):
    """ Extract info from a fit log file """
    fit_results = {}
    for line in reversed(open(fname).readlines()):
        if FITRESREX.match(line):
            info = line.split()
            fit_results[info[0][:-1]] = [float(info[1]), float(info[3]), float(info[5])]
        else:
            break
    return fit_results

def parse_toy_logs(fnames):
    """ Extract info from toy experimenta """
    fit_dict, pul_dict = {}, {}
    print fnames
    for fname in glob.glob(fnames):
        for line in open(fname).readlines():
            if FITRESREX.match(line):
                info = line.split()
                varn, genv, fitv, errv = \
                 info[0][:-1], float(info[1]), float(info[3]), float(info[5])
                if errv == 0:
                    print "Zero error:", varn, genv, fitv, errv
                    continue
                if varn in fit_dict:
                    fit_dict[varn].append(fitv)
                    pul_dict[varn].append((fitv - genv) / errv)
                else:
                    fit_dict[varn] = [fitv]
                    pul_dict[varn] = [(fitv - genv) / errv]
    print len(fit_dict), "variables found"
    print len(fit_dict["S1"]), "toy experiments found"
    return fit_dict, pul_dict

def rad_to_deg(val, err):
    """ Get fit result in deg """
    while val < -0.1 or val > np.pi+0.1:
        if val < 0:
            val += np.pi
        if val > np.pi:
            val -= np.pi
    return val*RAD_TO_DEG, err*RAD_TO_DEG

def scan_beta_fit(rexp):
    """ Find and extract info from logs """
    results = []
    for fname in glob.glob(rexp):
        gen_value = float(fname.split("_")[1])
        fitdict = parse_fit_log(fname)
        if 'beta' not in fitdict:
            print "problem with beta", gen_value
            continue
        _, val, err = fitdict["beta"]
        val, err = rad_to_deg(val, err)
        results.append([gen_value, val, err])
    results = np.array(results)
    return results[np.argsort(results[:, 0])]

def get_cs_fit(fname):
    """ Gen and fit values for the parameters C and S """
    fitdict = parse_fit_log(fname)
    result = []
    for idx in range(1, 9):
        result.append([])
        for value in fitdict["C" + str(idx)]:
            result[-1].append(value)
        for value in fitdict["S" + str(idx)]:
            result[-1].append(value)
    return np.array(result)

def adjust_beta_fit(data):
    """ Elimnate wrong pi distance """
    diff = data[:, 0] - data[:, 1]
    shift = [0, 180, 0]
    set1 = abs(diff - 180) < abs(diff)
    set2 = abs(diff + 180) < abs(diff)
    data[set1] = data[set1] + shift
    data[set2] = data[set2] - shift

def get_beta_fit():
    """ np array with beta scan fit results """
    params = get_params()
    data = scan_beta_fit(fit_regex(*params))
    adjust_beta_fit(data)
    return params, data

def input_beta():
    """ Ask user to type gen beta value """
    print "Type gen value of beta"
    return int(raw_input().strip())

def get_cs():
    """ np array """
    exp, gen, ftype = get_params()
    if exp == "belle":
        ddcfg.LOGDIR += "belle/"
    else:
        ddcfg.LOGDIR += "lhcbOpt/"
    beta = input_beta()
    fname = fit.flog(beta, "--"+ftype, "--"+exp, gen)
    return get_cs_fit(fname), (exp, gen, ftype, beta)
