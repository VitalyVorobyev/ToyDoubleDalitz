""" Copyright 2017 Vitaly Vorobyev """

import ddcfg
import toy

ddcfg.LOGDIR += "belle/"
ddcfg.set_config(ddcfg.TOPDIR + "params/params_opt.cfg")

def run_toy():
    """ Toy MC experiment """
    beta, nexp, ftype = toy.get_params()
    fexp = toy.find_max_generated('belle', 2, beta)
    print beta, nexp, ftype
    print fexp
    toy.toy_belle(beta, nexp, fexp, [ftype], ["2"])

run_toy()
