""" Copyright 2016 Vitaly Vorobyev """

import glob
import ddcfg

ddcfg.go_home()

TTYPE = ["--cp", "--dd", "--sim"]
DIL = "--dil"

EXPNAMES = {"belle": ["belle", "belleII"],
            "lhcb" : ["lhcb", "lhcbRII", "lhcbUpgr"]}

def data_regex(exp, gen, beta, dtype, d0h0=False):
    """ Regexpt for toy MC data """
    prefix = ddcfg.TOPDIR + 'data/' + ddcfg.DIRDICT[exp]
    strparts = [dtype, EXPNAMES[exp][gen-1]]
    if d0h0:
        strparts.append('d0h0')
    strparts += ['beta' + str(beta), '*.root']
    return prefix + '_'.join(strparts)

def get_data_index(fname):
    """ Index of toy data file """
    return int(fname[:-5].split('_')[-1])

def find_max_generated(exp, gen, beta, d0h0=False):
    """ Max index of generated toy datasets """
    ddlist = glob.glob(data_regex(exp, gen, beta, 'dd', d0h0))
    cplist = glob.glob(data_regex(exp, gen, beta, 'cp', d0h0))
    if len(ddlist) == 0 or len(cplist) == 0:
        return 0
    return min(max(map(get_data_index, ddlist)),
               max(map(get_data_index, cplist)))

def tpars(beta, nexp, fexp, typ, exp, epoch, dil):
    """ Parameter list for generator """
    pars = ddcfg.common_pars(beta, typ, exp, epoch) +\
           ["--toy", str(nexp)] + ["--exp", str(fexp)]
    return pars + [DIL] if dil else pars

def tpars_d0h0(beta, nexp, fexp, typ, exp, epoch):
    """ Parameter list for generator D0h0 """
    return tpars(beta, nexp, fexp, typ, exp, epoch, False) + ["--d0h0"]

def tlog(beta, nexp, fexp, ttype, exp, epoch, dil=False):
    """ log file name for generator """
    postfix = 'dil.txt' if dil else '.txt'
    return ddcfg.common_log(beta, ttype, exp, epoch) + '_toy' + str(nexp+fexp) + postfix

def tlog_tplt(beta, ttype, exp, epoch, dil=False):
    """ log file name for generator """
    postfix = '_toy*dil.txt' if dil else '_toy*[0-9].txt'
    return ddcfg.common_log(beta, ttype, exp, epoch) + postfix

def tlog_d0h0(beta, nexp, fexp, ttype, exp, epoch):
    """ log file name for generator """
    return ddcfg.common_log(beta, ttype, exp, epoch) + '_d0h0_toy' + str(nexp+fexp) + '.txt'

def tlog_d0h0_tplt(beta, ttype, exp, epoch):
    """ log file name for generator """
    return ddcfg.common_log(beta, ttype, exp, epoch) + '_d0h0_toy*.txt'

def toy(beta, nexp, fexp, ttype=None, epod=None, dil=False):
    """ Perform fit """
    output_list = []
    if ttype is None:
        ttype = TTYPE
    if epod is None:
        epod = ddcfg.EPODICT
    for exp in epod:
        for epoch in epod[exp]:
            for typ in ttype:
                log_file = tlog(beta, nexp, fexp, typ, exp, epoch, dil)
                output_text = ddcfg.run(tpars(beta, nexp, fexp, typ, exp, epoch, dil))
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
                output_text = ddcfg.run(tpars_d0h0(beta, nexp, fexp, typ, exp, epoch))
                print "log file:", log_file
                with open(log_file, 'w') as logfile:
                    logfile.write(output_text)
                output_list.append(output_text)
    return output_list

def toy_belle(beta, nexp, fexp, ftype=None, epochs=None, dil=False):
    """ Generate Belle data """
    if epochs is None:
        epochs = ddcfg.BELLE_EPOCHS
    toy(beta, nexp, fexp, ftype, {ddcfg.BELLE: epochs}, dil)

def toy_belle_d0h0(beta, nexp, fexp, ftype=None, epochs=None):
    """ Generate Belle data """
    if epochs is None:
        epochs = ddcfg.BELLE_EPOCHS
    toy_d0h0(beta, nexp, fexp, ftype, {ddcfg.BELLE: epochs})

def toy_lhcb(beta, nexp, fexp, ftype=None, epochs=None, dil=False):
    """ Generate LHCb data """
    if epochs is None:
        epochs = ddcfg.LHCB_EPOCHS
    toy(beta, nexp, fexp, ftype, {ddcfg.LHCB: epochs}, dil)

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
