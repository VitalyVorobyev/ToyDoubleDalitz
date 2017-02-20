""" Copyright 2017 Vitaly Vorobyev """

import ddcfg
import gen

ddcfg.LOGDIR += "belle/"
ddcfg.set_config(ddcfg.TOPDIR + "params/params_cur.cfg")

print "Type the angle beta value"
gen.gen_belle(int(raw_input()))
