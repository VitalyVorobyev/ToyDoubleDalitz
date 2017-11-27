""" Plot parameters of binned Dalitz distribution """

import sys
import matplotlib.pyplot as plt
import numpy as np
import binning_parser as bp

def plot_csk(show_bins=False):
    """ Make plot """
    plt.rc('text', usetex=True)
    plt.rc('font', family='serif', size=22)
    plt.style.use('seaborn-white')
    label_size = 28

    csk = bp.parse_csk(sys.argv[1])
    bins = np.arange(1, 9, dtype=int)
    clist, slist, kplist, knlist = csk[:, 0], csk[:, 1], csk[:, 2], csk[:, 3]
    title = sys.argv[2] if len(sys.argv) > 2 else ""

    plt.figure(num=1, figsize=(6, 4), dpi=130)
    plt.plot(bins, kplist, 'bo', markersize=12, markeredgewidth=1, markeredgecolor='k')
    plt.plot(bins, knlist, 'rp', markersize=12, markeredgewidth=1, markeredgecolor='k')
    plt.xlim(0.5, 8.5)
    plt.ylim(0., .2)
    plt.ylabel(r'$k_j$', fontsize=label_size)
    plt.xlabel(r'$\mathrm{Absolute\ value\ of\ bin\ index}$', fontsize=label_size)
    plt.tight_layout(pad=.2)

    # ticks and grid
    plt.axes().yaxis.grid(which='minor', linestyle=':')
    plt.axes().yaxis.grid(which='major')
    kmax = kplist.max() + 0.04
    major_ticks = np.arange(0., kmax, 0.04)
    minor_ticks = np.arange(0., kmax, 0.02)
    xticks = np.arange(1, 9, dtype=int)
    plt.gca().tick_params(axis='y', which='major', direction='in', width=1, size=4)
    plt.gca().tick_params(axis='y', which='minor', direction='in', width=1, size=4)
    plt.gca().set_yticks(minor_ticks, minor=True)
    plt.gca().set_yticks(major_ticks)
    plt.gca().set_xticks(xticks)
    plt.savefig('pics/k' + title + '.eps', format='eps', dpi=130)
    plt.savefig('pics/pdf/k' + title + '.pdf', format='pdf', dpi=130)
    plt.savefig('pics/png/k' + title + '.png', format='png', dpi=130)

    plt.figure(num=2, figsize=(4.4, 4), dpi=130)
    plt.axis('equal')
    bound = 1.2
    plt.plot(clist, slist, 'bo', markersize=10, markeredgewidth=1, markeredgecolor='k')
    angles = np.linspace(-np.pi, np.pi)
    plt.plot(np.cos(angles), np.sin(angles), 'r--', linewidth=1.5)

    if show_bins:
        lbl_offset = (0.14, 0.06)
        lblx = clist + lbl_offset[0] * np.sign(clist) - lbl_offset[1]
        lbly = slist + lbl_offset[0] * np.sign(slist) - lbl_offset[1]
        for xpg, ypg, ipg in zip(lblx, lbly, range(len(lblx))):
            plt.text(xpg, ypg, r'$' + str(ipg + 1) + r'$', color='k')

    plt.ylabel(r'$s_j$', fontsize=label_size)
    plt.xlabel(r'$c_j$', fontsize=label_size)
    plt.axis([-bound, bound, -bound, bound])
    plt.tight_layout(pad=0.2)

    # ticks and grid
    plt.gca().grid(which='minor', linestyle=':')
    plt.gca().grid(which='major')
    major_ticks = np.arange(-1, 1.01, 0.5)
    minor_ticks = np.arange(-1.25, bound+0.1, 0.125)
    plt.gca().tick_params(axis='both', which='major', direction='in', width=2, size=6)
    plt.gca().tick_params(axis='both', which='minor', direction='in', width=1, size=4)
    plt.gca().set_xticks(minor_ticks, minor=True)
    plt.gca().set_xticks(major_ticks)
    plt.gca().set_yticks(minor_ticks, minor=True)
    plt.gca().set_yticks(major_ticks)
    plt.savefig('pics/cs' + title + '.eps', format='eps', dpi=130)
    plt.savefig('pics/pdf/cs' + title + '.pdf', format='pdf', dpi=130)
    plt.savefig('pics/png/cs' + title + '.png', format='png', dpi=130)

    plt.show()

plot_csk(True)
