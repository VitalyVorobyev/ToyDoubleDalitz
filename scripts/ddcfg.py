""" Copyright 2016 Vitaly Vorobyev """

import subprocess as spr
import os

TOPDIR = "/home/vitaly/B0toD0pipi/DDTatami/"
CFGFILE = TOPDIR + "params/params.cfg"
LOGDIR = TOPDIR + "logs/"
DRIVER = "./bin/fsbl"

LHCB_EPOCHS = ["1", "2", "3"]
BELLE_EPOCHS = ["1", "2"]

BELLE = "--belle"
LHCB = "--lhcb"

EXPR = [BELLE, LHCB]

EPODICT = {EXPR[0] : BELLE_EPOCHS,
           EXPR[1] : LHCB_EPOCHS}

DIRDICT = {"belle": "belle/",
           "lhcb": "lhcbOpt/"}

def go_home():
    """ Go to the top dir """
    os.chdir(TOPDIR)

def remove_cfg():
    """ Remove excisting (or not) symbolic link """
    try:
        os.remove(CFGFILE)
    except OSError:
        pass

def set_config(filename):
    """ Make symbolic link for a config file """
    remove_cfg()
    os.symlink(filename, CFGFILE)

def common_pars(beta, dtype, exp, epoch):
    """ Parameters for DDTatami """
    return [DRIVER, "--beta", str(beta), dtype, exp, str(epoch)]

def common_log(beta, dtype, exp, epoch):
    """ log file name """
    print beta, dtype, exp, epoch
    return LOGDIR + '_'.join(['log', str(beta), dtype[2:], exp[2:], str(epoch)])

def run_call(args):
    """ Execute DDTatami using subprocess.call """
    spr.call(args)

def run_check_output(args, killflg=False):
    """ Execute DDTatami using subprocess.check_output """
    print args
    stdoutput = spr.check_output(args)
    if killflg:
        spr.call(['killall', '-9', DRIVER])
    return stdoutput

def run_popen(args):
    """ Execute DDTatami using subprocess.Popen """
    # return spr.Popen(args, stdout=spr.PIPE, stderr=spr.STDOUT)
    return spr.Popen(args)

def write_logs(pstack):
    """ Save log files """
    print len(pstack), "processes"
    for log in pstack:
        proc = pstack[log]
        stdout_value, stderr_value = proc.communicate('through stdin to stdout\n')
        fout = open(LOGDIR + log + '.txt', 'w')
        fout.write(repr(stdout_value))
        fout.close()
        if stderr_value is not None:
            ferr = open(LOGDIR + log + '_err.txt', 'w')
            ferr.write(repr(stderr_value))
            ferr.close()

def run(args):
    """ Execute DDTatami """
    print args
    return run_check_output(args)
