""" Copyright 2017 Vitaly Vorobyev """

import ddcfg
import fit

ddcfg.LOGDIR += "lhcbCur/"
ddcfg.set_config(ddcfg.TOPDIR + "params/params_cur.cfg")

fit.fit_lhcb(int(raw_input()), ["--sim"])
