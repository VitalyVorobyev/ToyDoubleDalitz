""" Copyright 2017 Vitaly Vorobyev """

import ddcfg
import gen

ddcfg.LOGDIR += "lhcbOpt/"
ddcfg.set_config(ddcfg.TOPDIR + "params/params_opt.cfg")

gen.gen_lhcb(int(raw_input()))
