""" Copyright 2017 Vitaly Vorobyev """

import ddcfg
import toy

ddcfg.LOGDIR += "lhcbOpt/"
ddcfg.set_config(ddcfg.TOPDIR + "params/params_opt.cfg")

def run_toy():
    """ Toy MC experiment """
    beta, nexp, ftype = toy.get_params()
    fexp = toy.find_max_generated('lhcb', 3, beta)
    print beta, nexp, ftype
    print fexp
    toy.toy_lhcb(beta, fexp, 0, [ftype], ["3"], True)

run_toy()
