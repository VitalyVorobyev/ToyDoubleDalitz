""" Copyright 2017 Vitaly Vorobyev """

import ddcfg
import gen

ddcfg.LOGDIR += "belle/"
ddcfg.set_config(ddcfg.TOPDIR + "params/params_cur.cfg")

print "Type angle beta values"
for beta in [int(value) for value in raw_input().strip().split()]:
    gen.gen_belle(beta, None, ["1"])
