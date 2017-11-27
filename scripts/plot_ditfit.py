""" Plots for fitted dilution factors for the B0 -> D0 pi+ pi- decay """

import numpy as np
import matplotlib.pyplot as plt
import log_parser as lp

def plot_dil():
    """ Fit result for dilution factors """
    label_size = 28

    plt.rc('text', usetex=True)
    plt.rc('font', family='serif', size=22)
    # plt.style.use('seaborn-white')
    plt.figure(num=None, figsize=(4.4, 4), dpi=130)

    data, pars = lp.get_dil()
    dgen, dfit, derr = data[:, 0], data[:, 1], data[:, 2]
    nbins = np.arange(1, 9, 1, dtype=int)

    plt.plot(nbins, dgen, 'ko', markersize=10, markerfacecolor='None')
    plt.errorbar(nbins, dfit, yerr=derr, fmt='bp', markersize=10,\
    markeredgewidth=1, markeredgecolor='k', capsize=4, ecolor='k')
    plt.tight_layout(pad=1.5)
    plt.axis([0.5, 8.5, -1., 1.])

    # ticks and grid
    plt.gca().grid(which='minor', linestyle=':')
    plt.gca().grid(which='major')
    major_ticks = np.arange(1, 9, 1)
    # minor_ticks = np.arange(-1.25, bound+0.01, 0.125)
    plt.gca().tick_params(axis='both', which='major', direction='in', width=2, size=6)
    plt.gca().tick_params(axis='both', which='minor', direction='in', width=1, size=4)
    # plt.gca().set_xticks(minor_ticks, minor=True)
    plt.gca().set_xticks(major_ticks)
    # plt.gca().set_yticks(minor_ticks, minor=True)
    # plt.gca().set_yticks(major_ticks)

    plt.ylabel(r'$d_j$', fontsize=label_size)
    plt.xlabel(r'$\mathrm{Bin\ index}$', fontsize=label_size)
    plt.tight_layout(pad=.15)

    plt.savefig('pics/dilfit_' + str(pars[0]) + str(pars[1]) + '_' + \
     str(pars[2]) + '_' + str(pars[3]) + '.eps', format='eps', dpi=150)
    plt.savefig('pics/pdf/dilfit_' + str(pars[0]) + str(pars[1]) + '_' + \
     str(pars[2]) + '_' + str(pars[3]) + '.pdf', format='pdf', dpi=150)

    plt.show()

if __name__ == "__main__":
    plot_dil()
