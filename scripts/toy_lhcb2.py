""" Copyright 2017 Vitaly Vorobyev """

import ddcfg
import toy

ddcfg.LOGDIR += "lhcbOpt/"
ddcfg.set_config(ddcfg.TOPDIR + "params/params_opt.cfg")

def get_params():
    """ Toy experiment parameters """
    print "Type [beta=23] [nexp=100] [ftype=sim]"
    params = raw_input().strip().split()
    beta = 23 if len(params) < 1 else int(params[0])
    nexp = 100 if len(params) < 2 else int(params[1])
    ftype = '--sim' if len(params) < 3 else "--" + params[2]
    return beta, nexp, ftype

def run_toy():
    """ Toy MC experiment """
    beta, nexp, ftype = get_params()
    toy.toy_lhcb(beta, nexp, [ftype], ["2"])

run_toy()
