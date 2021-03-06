""" Copyright 2017 Vitaly Vorobyev """

import ddcfg
import gen

ddcfg.LOGDIR += "lhcbOpt/"
ddcfg.set_config(ddcfg.TOPDIR + "params/params_opt.cfg")

print "Type angle beta values"
for beta in [int(value) for value in raw_input().strip().split()]:
    gen.gen_lhcb(beta, None, ["2"])
