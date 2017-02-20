""" Copyright 2017 Vitaly Vorobyev """

import ddcfg
import fit

ddcfg.LOGDIR += "lhcbOpt/"
ddcfg.set_config(ddcfg.TOPDIR + "params/params_opt.cfg")

fit.fit_lhcb(int(raw_input()), ["--sim"])
