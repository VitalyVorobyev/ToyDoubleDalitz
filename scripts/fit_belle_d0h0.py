""" Copyright 2017 Vitaly Vorobyev """

import ddcfg
import fit

ddcfg.LOGDIR += "belle/"
ddcfg.set_config(ddcfg.TOPDIR + "params/params_cur.cfg")

print "Type the angle beta value"
fit.fit_belle_d0h0(int(raw_input()), ["--cp"])
