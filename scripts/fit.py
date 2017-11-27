""" Copyright 2016 Vitaly Vorobyev """

import ddcfg

FTYPE = ["--sim", "--cp", "--flv", "--dd"]
DIL = "--dil"

def fpars(beta, ftype, exp, epoch, dilut=False):
    """ Parameter list for fitter """
    args = ddcfg.common_pars(beta, ftype, exp, epoch) + ["-f"]
    if dilut is True:
        args += [DIL]
    return args

def fpars_d0h0(beta, ftype, exp, epoch, dilut=False):
    """ Parameter list for fitter (D0h0) """
    return fpars(beta, ftype, exp, epoch, dilut) + ["--d0h0"]

def flog(beta, ftype, exp, epoch, dilut=False):
    """ log file name for fitter """
    logname = ddcfg.common_log(beta, ftype, exp, epoch) + '_fit'
    if dilut is True:
        logname += '_dil'
    return logname + '.txt'

def flog_d0h0(beta, ftype, exp, epoch, dilut=False):
    """ log file name for fitter (d0h0) """
    logname = ddcfg.common_log(beta, ftype, exp, epoch) + '_fit'
    if dilut is True:
        logname += '_dil'
    return logname + '_d0h0.txt'

def fit(beta, ftype=None, epod=None, dil=False):
    """ Perform fit """
    output_list = []
    if ftype is None:
        ftype = FTYPE
    if epod is None:
        epod = ddcfg.EPODICT
    for exp in epod:
        for epoch in epod[exp]:
            for typ in ftype:
                log_file = flog(beta, typ, exp, epoch, dil)
                output_text = ddcfg.run(fpars(beta, typ, exp, epoch, dil))
                print "log file:", log_file
                with open(log_file, 'w') as logfile:
                    logfile.write(output_text)
                output_list.append(output_text)
    return output_list

def fit_d0h0(beta, ftype=None, epod=None):
    """ Perform fit """
    output_list = []
    if ftype is None:
        ftype = FTYPE
    if epod is None:
        epod = ddcfg.EPODICT
    for exp in epod:
        for epoch in epod[exp]:
            for typ in ftype:
                log_file = flog_d0h0(beta, typ, exp, epoch)
                output_text = ddcfg.run(fpars_d0h0(beta, typ, exp, epoch))
                print "log file:", log_file
                with open(log_file, 'w') as logfile:
                    logfile.write(output_text)
                output_list.append(output_text)
    return output_list

def fit_belle(beta, ftype=None, epochs=None, dil=False):
    """ Fit Belle data """
    if epochs is None:
        epochs = ddcfg.BELLE_EPOCHS
    fit(beta, ftype, {ddcfg.BELLE: epochs}, dil)

def fit_belle_d0h0(beta, ftype=None, epochs=None):
    """ Fit Belle data """
    if epochs is None:
        epochs = ddcfg.BELLE_EPOCHS
    fit_d0h0(beta, ftype, {ddcfg.BELLE: epochs})

def fit_lhcb(beta, ftype=None, epochs=None, dil=False):
    """ Fit LHCb data """
    if epochs is None:
        epochs = ddcfg.LHCB_EPOCHS
    fit(beta, ftype, {ddcfg.LHCB: epochs}, dil)

if __name__ == "__main__":
    ddcfg.go_home()
    print "Type the beta value:"
    fit(int(raw_input()))
