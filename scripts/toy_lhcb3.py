""" Copyright 2017 Vitaly Vorobyev """

import ddcfg
import toy

ddcfg.LOGDIR += "lhcbOpt/"
ddcfg.set_config(ddcfg.TOPDIR + "params/params_opt.cfg")

def run_toy():
    """ Toy MC experiment """
    beta, nexp, ftype = toy.get_params()
    fexp = toy.find_max_generated('lhcb', 3, beta)
    toy.toy_lhcb(beta, nexp, fexp, [ftype], ["3"])

run_toy()
