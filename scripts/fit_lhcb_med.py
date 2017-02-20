""" Copyright 2017 Vitaly Vorobyev """

import ddcfg
import fit

ddcfg.LOGDIR += "lhcbMed/"
ddcfg.set_config(ddcfg.TOPDIR + "params/params_med.cfg")

print "Type the angle beta value"
fit.fit_lhcb(int(raw_input()), ["--sim"])
