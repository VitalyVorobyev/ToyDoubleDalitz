""" Plots for C and S parameters for the B0 -> D0 pi+ pi- decay """

import numpy as np
import matplotlib.pyplot as plt
import log_parser as lp

def plot_cs():
    """ Fit result for parameters C and S """
    label_size = 28
    bound = 1.2
    lbl_offset = (0.11, 0.07)
    max_diff = 0.5

    plt.rc('text', usetex=True)
    plt.rc('font', family='serif', size=22)
    plt.style.use('seaborn-white')
    plt.figure(num=None, figsize=(4.4, 4), dpi=130)

    data, pars = lp.get_cs()
    cgen, cfit, cerr = data[:, 0], data[:, 1], data[:, 2]
    sgen, sfit, serr = data[:, 3], data[:, 4], data[:, 5]

    angles = np.linspace(-np.pi, np.pi)
    plt.plot(np.cos(angles), np.sin(angles), 'r--', linewidth=1.5)
    plt.plot(cgen, sgen, 'ko', markersize=10, markerfacecolor='None',\
    markeredgewidth=1)
    plt.errorbar(cfit, sfit, xerr=cerr, yerr=serr, fmt='bp', markersize=10,\
    markeredgewidth=1, markeredgecolor='k', capsize=4, ecolor='k')
    plt.tight_layout(pad=1.5)

    main_color = 'black'
    if max(cgen - cfit) > max_diff or max(sgen - sfit) > max_diff:
        main_color = 'red'
        lblx = cfit - lbl_offset[0] * np.sign(cgen - cfit) - lbl_offset[1]
        lbly = sfit - lbl_offset[0] * np.sign(sgen - sfit) - lbl_offset[1]
        for xpf, ypf, ipf in zip(lblx, lbly, range(len(lblx))):
            plt.text(xpf, ypf, str(ipf + 1), color='blue')

    lblx = cgen + lbl_offset[0] * np.sign(cgen - cfit) - lbl_offset[1]
    lbly = sgen + lbl_offset[0] * np.sign(sgen - sfit) - lbl_offset[1]
    for xpg, ypg, ipg in zip(lblx, lbly, range(len(lblx))):
        plt.text(xpg, ypg, r'$' + str(ipg + 1) + r'$', color=main_color)

    plt.axis([-bound, bound, -bound, bound])

    # ticks and grid
    plt.gca().grid(which='minor', linestyle=':')
    plt.gca().grid(which='major')
    major_ticks = np.arange(-1, 1.01, 0.5)
    minor_ticks = np.arange(-1.25, bound+0.01, 0.125)
    plt.gca().tick_params(axis='both', which='major', direction='in', width=2, size=6)
    plt.gca().tick_params(axis='both', which='minor', direction='in', width=1, size=4)
    plt.gca().set_xticks(minor_ticks, minor=True)
    plt.gca().set_xticks(major_ticks)
    plt.gca().set_yticks(minor_ticks, minor=True)
    plt.gca().set_yticks(major_ticks)

    plt.ylabel(r'$s_j$', fontsize=label_size)
    plt.xlabel(r'$c_j$', fontsize=label_size)
    plt.tight_layout(pad=.1)

    plt.savefig('pics/csfit_' + str(pars[0]) + str(pars[1]) + '_' + \
     str(pars[2]) + '_' + str(pars[3]) + '.eps', format='eps', dpi=150)
    plt.savefig('pics/pdf/csfit_' + str(pars[0]) + str(pars[1]) + '_' + \
     str(pars[2]) + '_' + str(pars[3]) + '.pdf', format='pdf', dpi=150)

    plt.show()

if __name__ == "__main__":
    plot_cs()
