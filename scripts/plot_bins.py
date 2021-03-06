""" Plot for Dalitz plot binning """

import sys
import matplotlib.pyplot as plt
from matplotlib import colors
import numpy as np
import binning_parser as bp

CMAP = colors.ListedColormap(['white', 'deepskyblue', 'black', 'thistle', 'red',\
                              'limegreen', 'cornsilk', 'blue', 'yellow'])

def mbc(mab, mac, masses):
    """ The third Dalitz variable """
    return - mab - mac + (masses**2).sum()

def val_to_idx(value, grid_size, lolim, hilim):
    """ Value into bin index """
    return (value - lolim) / (hilim - lolim) * grid_size

def color_map(binning, grid_size, limits, masses):
    """ Binning color map """
    bcgrig_size = int(0.5 * grid_size)
    ablo, abhi = limits[0]
    aclo, achi = limits[1]
    bclo, bchi = limits[2]
    abbins = np.array(val_to_idx(binning['mAB'], grid_size, ablo, abhi), dtype=int)
    acbins = np.array(val_to_idx(binning['mAC'], grid_size, aclo, achi), dtype=int)
    bcbinsl = np.array(val_to_idx(binning['mBC'], bcgrig_size, bclo, bchi), dtype=int)
    rhalf = mbc(binning['mAC'], binning['mAB'], masses)
    bcbinsr = np.array(val_to_idx(rhalf, bcgrig_size, bclo, bchi), dtype=int)

    bccmap = np.zeros((grid_size, grid_size), dtype=int)
    accmap = np.zeros((bcgrig_size, grid_size), dtype=int)
    for abidx, acidx, bcl, bcr, bmap in zip(abbins, acbins, bcbinsl, bcbinsr, binning['bin']):
        bccmap[abidx, acidx] = bmap
        bccmap[acidx, abidx] = bmap
        accmap[bcl, abidx] = bmap
        accmap[bcr, acidx] = bmap
        if abs(abidx - acidx) < 1.1:
            accmap[int(bcr-1), int(acidx)] = bmap
    for idx in xrange(grid_size-1):
        bccmap[idx, idx] = bccmap[idx+1, idx]
    return bccmap, accmap

def plot_bc():
    """ mAB vs mAC color map """
    binning, grid_size, masses, limits, _ = bp.parse_bins(sys.argv[1])
    bccmap, _ = color_map(binning, int(0.95*grid_size), limits, masses)
    ablo, abhi = limits[0]
    aclo, achi = limits[1]
    ticks = np.arange(10)-0.5
    norm = colors.BoundaryNorm(ticks, CMAP.N)
    img = plt.imshow(bccmap, cmap=CMAP, norm=norm, vmin=0, vmax=8, extent=[ablo, abhi, aclo, achi],\
               interpolation='nearest', origin='lower')
    plt.colorbar(img, cmap=CMAP, norm=norm, boundaries=ticks, ticks=np.arange(9))
    plt.show()

def plot_ac():
    """ mAB vs mBC color map """
    label_size = 32
    plt.rc('text', usetex=True)
    plt.rc('font', family='serif')
    plt.rc('font', size=26)
    plt.style.use('seaborn-white')

    fname = "d0pipi_kuz_binning.txt" if len(sys.argv) == 1 else sys.argv[1]
    binning, grid_size, masses, limits, labels = bp.parse_bins(fname)
    _, accmap = color_map(binning, int(0.95*grid_size), limits, masses)
    ablo, abhi = limits[2]
    bclo, bchi = limits[0]
    ticks = np.arange(10)-0.5
    norm = colors.BoundaryNorm(ticks, CMAP.N)
    plt.figure(num=None, figsize=(6, 5), dpi=130)
    img = plt.imshow(accmap.T, cmap=CMAP, norm=norm, vmin=0, vmax=8,\
               extent=[ablo, abhi, bclo, bchi],\
               interpolation='nearest', origin='lower', aspect='auto')
    colbar = plt.colorbar(img, cmap=CMAP, norm=norm, boundaries=ticks,\
                          ticks=np.arange(9), pad=0.01)
    colbar.ax.tick_params(labelsize=20)

    plt.tight_layout(pad=2.)
    lbly = '$' + labels[0].replace('#pi', r'\pi')[:-1] + '$'
    lblx = '$' + labels[2].replace('#pi', r'\pi')[:-1] + '$'
    plt.ylabel(lbly, fontsize=label_size)
    plt.xlabel(lblx, fontsize=label_size)
    plt.subplots_adjust(right=0.999, top=0.95)

    plt.show()

plot_ac()
