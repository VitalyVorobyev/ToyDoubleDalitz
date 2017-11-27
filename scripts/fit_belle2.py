#! /usr/bin/python
""" Copyright 2017 Vitaly Vorobyev """

import sys
import ddcfg
import fit

ddcfg.LOGDIR += "belle/"
ddcfg.set_config(ddcfg.TOPDIR + "params/params_cur.cfg")
if len(sys.argv) > 1:
    fittype = '--' + sys.argv[1]
else:
    fittype = '--sim'

print "Type angle beta values"
for beta in [int(value) for value in raw_input().strip().split()]:
    fit.fit_belle(beta, [fittype], ['2'])
