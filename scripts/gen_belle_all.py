""" Copyright 2017 Vitaly Vorobyev """

import numpy as np
import ddcfg
import gen

ddcfg.LOGDIR += "belle/"
ddcfg.set_config(ddcfg.TOPDIR + "params/params_cur.cfg")

print "Type the angle beta value"

ANGLES = [23, 67] + [int(angle) for angle in np.linspace(0., 180., 13)]
for BETA in ANGLES:
    gen.gen_belle(BETA)
