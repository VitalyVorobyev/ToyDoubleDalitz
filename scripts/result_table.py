""" Print latex table summarizing numerical experiments """

import os
import numpy as np
import log_parser as lp

RAD_TO_DEG = 180./np.pi

RES_LIST = \
  [
      ['belle', 1, 'sim', False],
      ['belle', 2, 'sim', False],
      ['lhcb', 1, 'sim', False],
      ['lhcb', 2, 'sim', False],
      ['lhcb', 3, 'sim', False],
      ['belle', 1, 'sim', True],
      ['belle', 2, 'sim', True],
      ['lhcb', 1, 'sim', True],
      ['lhcb', 2, 'sim', True],
      ['lhcb', 3, 'sim', True],
  ]

for exp, epoch, ftype, dil in RES_LIST:
    fname = lp.log_path(exp, epoch, ftype, 23, dil)
    if not os.path.isfile(fname):
        print fname, "does not exists"
        continue
    print exp, epoch, ftype, dil, lp.parse_fit_log(fname)['beta'][2]*RAD_TO_DEG
