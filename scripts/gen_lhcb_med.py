""" Copyright 2017 Vitaly Vorobyev """

import ddcfg
import gen

ddcfg.LOGDIR += "lhcbMed/"
ddcfg.set_config(ddcfg.TOPDIR + "params/params_med.cfg")

print "Type the angle beta value"
gen.gen_lhcb(int(raw_input()))
