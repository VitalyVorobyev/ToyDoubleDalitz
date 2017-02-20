""" Copyright 2016 Vitaly Vorobyev """

import numpy as np
import ddcfg

ddcfg.go_home()

DDTY = ["--cp", "--dd"]
ALTY = ["--flv"] + DDTY

def gpars(beta, typ, exp, epoch):
    """ Parameter list for generator """
    return ddcfg.common_pars(beta, typ, exp, epoch) + ["-g"]

def gpars_d0h0(beta, typ, exp, epoch):
    """ Parameter list for generator D0h0 """
    return gpars(beta, typ, exp, epoch) + ["--d0h0"]

def glog(beta, ftype, exp, epoch):
    """ log file name for generator """
    return ddcfg.common_log(beta, ftype, exp, epoch) + '_gen.txt'

def glog_d0h0(beta, ftype, exp, epoch):
    """ log file name for generator """
    return ddcfg.common_log(beta, ftype, exp, epoch) + '_d0h0_gen.txt'

def gen(beta, dtype=None, epod=None):
    """ Generate any data """
    output_list = []
    if dtype is None:
        dtype = ALTY
    if epod is None:
        epod = ddcfg.EPODICT
    for exp in epod:
        for epoch in epod[exp]:
            for typ in dtype:
                output_text = ddcfg.run(gpars(beta, typ, exp, epoch))
                log_file = glog(beta, typ, exp, epoch)
                print "log file:", log_file
                with open(log_file, 'w') as logfile:
                    logfile.write(output_text)
                output_list.append(output_text)
    return output_list

def gen_d0h0(beta, dtype=None, epod=None):
    """ Generate any data """
    output_list = []
    if dtype is None:
        dtype = ALTY
    if epod is None:
        epod = ddcfg.EPODICT
    for exp in epod:
        for epoch in epod[exp]:
            for typ in dtype:
                output_text = ddcfg.run(gpars_d0h0(beta, typ, exp, epoch))
                log_file = glog_d0h0(beta, typ, exp, epoch)
                print "log file:", log_file
                with open(log_file, 'w') as logfile:
                    logfile.write(output_text)
                output_list.append(output_text)
    return output_list

def gen_belle(beta, dtype=None, epochs=None):
    """ Generate Belle data """
    if epochs is None:
        epochs = ddcfg.BELLE_EPOCHS
    gen(beta, dtype, {ddcfg.BELLE: epochs})

def gen_belle_d0h0(beta, dtype=None, epochs=None):
    """ Generate Belle data for D0h0 """
    if epochs is None:
        epochs = ddcfg.BELLE_EPOCHS
    gen_d0h0(beta, dtype, {ddcfg.BELLE: epochs})

def gen_lhcb(beta, dtype=None, epochs=None):
    """ Generate LHCb data """
    if epochs is None:
        epochs = ddcfg.LHCB_EPOCHS
    gen(beta, dtype, {ddcfg.LHCB: epochs})

if __name__ == "__main__":
    # BETA = int(raw_input())
    ANGLES = [23, 67] + [int(angle) for angle in np.linspace(0., 180., 13)]
    for BETA in ANGLES:
        gen(BETA)
