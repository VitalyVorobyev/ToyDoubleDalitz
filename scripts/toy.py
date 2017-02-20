""" Copyright 2016 Vitaly Vorobyev """

import glob
import ddcfg

ddcfg.go_home()

TTYPE = ["--cp", "--dd", "--sim"]
DIL = "--dil"

EXPNAMES = {"belle": ["belle", "belleII"],
            "lhcb" : ["lhcb", "lhcbRII", "lhcbUpgr"]}

def data_regex(exp, gen, beta, dtype):
    """ Regexpt for toy MC data """
    return ddcfg.TOPDIR + "data/" + ddcfg.DIRDICT[exp] + dtype + '_' + \
           EXPNAMES[exp][gen-1] + "_beta" + str(beta) + "_*.root"

def get_data_index(fname):
    """ Index of toy data file """
    return int(fname[:-5].split('_')[-1])

def find_max_generated(exp, gen, beta):
    """ Max index of generated toy datasets """
    ddlist = glob.glob(data_regex(exp, gen, beta, 'dd'))
    cplist = glob.glob(data_regex(exp, gen, beta, 'cp'))
    ddmaxval = get_data_index(sorted(ddlist)[-1]) if len(ddlist) > 0 else 0
    cpmaxval = get_data_index(sorted(cplist)[-1]) if len(cplist) > 0 else 0
    return min(ddmaxval, cpmaxval)

def tpars(beta, nexp, fexp, typ, exp, epoch):
    """ Parameter list for generator """
    return ddcfg.common_pars(beta, typ, exp, epoch) + ["--toy", str(nexp)] + \
    ["--exp", str(fexp)]

def tpars_d0h0(beta, nexp, fexp, typ, exp, epoch):
    """ Parameter list for generator D0h0 """
    return tpars(beta, nexp, typ, exp, fexp, epoch) + ["--d0h0"]

def tlog(beta, nexp, fexp, ttype, exp, epoch):
    """ log file name for generator """
    return ddcfg.common_log(beta, ttype, exp, epoch) + '_toy' + str(nexp+fexp) + '.txt'

def tlog_tplt(beta, ttype, exp, epoch):
    """ log file name for generator """
    return ddcfg.common_log(beta, ttype, exp, epoch) + '_toy*.txt'

def tlog_d0h0(beta, nexp, fexp, ttype, exp, epoch):
    """ log file name for generator """
    return ddcfg.common_log(beta, ttype, exp, epoch) + '_d0h0_toy' + str(nexp+fexp) + '.txt'

def toy(beta, nexp, fexp, ttype=None, epod=None):
    """ Perform fit """
    output_list = []
    if ttype is None:
        ttype = TTYPE
    if epod is None:
        epod = ddcfg.EPODICT
    for exp in epod:
        for epoch in epod[exp]:
            for typ in ttype:
                log_file = tlog(beta, nexp, fexp, typ, exp, epoch)
                output_text = ddcfg.run(tpars(beta, nexp, fexp, typ, exp, epoch))
                print "log file:", log_file
                with open(log_file, 'w') as logfile:
                    logfile.write(output_text)
                output_list.append(output_text)
    return output_list

def toy_d0h0(beta, nexp, fexp, ttype=None, epod=None):
    """ Perform fit """
    output_list = []
    if ttype is None:
        ttype = TTYPE
    if epod is None:
        epod = ddcfg.EPODICT
    for exp in epod:
        for epoch in epod[exp]:
            for typ in ttype:
                log_file = tlog_d0h0(beta, nexp, fexp, typ, exp, epoch)
                output_text = ddcfg.run(tpars(beta, nexp, typ, exp, fexp, epoch))
                print "log file:", log_file
                with open(log_file, 'w') as logfile:
                    logfile.write(output_text)
                output_list.append(output_text)
    return output_list

def toy_belle(beta, nexp, fexp, ftype=None, epochs=None):
    """ Generate Belle data """
    if epochs is None:
        epochs = ddcfg.BELLE_EPOCHS
    toy(beta, nexp, fexp, ftype, {ddcfg.BELLE: epochs})

def toy_belle_d0h0(beta, nexp, fexp, ftype=None, epochs=None):
    """ Generate Belle data """
    if epochs is None:
        epochs = ddcfg.BELLE_EPOCHS
    toy_d0h0(beta, nexp, fexp, ftype, {ddcfg.BELLE: epochs})

def toy_lhcb(beta, nexp, fexp, ftype=None, epochs=None):
    """ Generate LHCb data """
    if epochs is None:
        epochs = ddcfg.LHCB_EPOCHS
    toy(beta, nexp, fexp, ftype, {ddcfg.LHCB: epochs})

def get_params():
    """ Toy experiment parameters """
    print "Type [beta=23] [nexp=100] [ftype=sim]"
    params = raw_input().strip().split()
    beta = 23 if len(params) < 1 else int(params[0])
    nexp = 100 if len(params) < 2 else int(params[1])
    ftype = '--sim' if len(params) < 3 else "--" + params[2]
    return beta, nexp, ftype

if __name__ == "__main__":
    ddcfg.go_home()
    print "Type [beta] [nexp] [fexp=0]"
    BETA, NEXP, FEXP = [int(param) for param in raw_input().strip().split()]
    toy(BETA, NEXP, FEXP)
