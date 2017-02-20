""" Copyright 2017 Vitaly Vorobyev """

import numpy as np
import matplotlib.pyplot as plt
import log_parser as lp

def plot_linearity(data):
    """ Linearity plot """
    label_size = 20
    plt.rc('text', usetex=True)
    plt.rc('font', family='serif', size=18)
    plt.rc('font', size=18)
    plt.style.use('seaborn-white')
    plt.figure()
    xgen, xfit, xfiterr = data[:, 0], data[:, 1], data[:, 2]
    plt.plot(xgen, np.zeros(len(xgen)), 'r-', linewidth=2.5)
    plt.errorbar(xgen, xfit - xgen, yerr=xfiterr, fmt='o', markersize=8)
    plt.axis([-10, 190, -30, 30])
    plt.tight_layout(pad=1.9)
    plt.grid(True)
    plt.ylabel(r'Fit offset of $\beta$ (deg)', fontsize=label_size)
    plt.xlabel(r'Gen value of $\beta$ (deg)', fontsize=label_size)
    plt.show()

def plot_uncertainty(data):
    """ Errors plot """
    label_size = 20
    plt.rc('text', usetex=True)
    plt.rc('font', family='serif', size=18)
    plt.style.use('seaborn-white')
    plt.figure()
    xgen, xfiterr = data[:, 0], data[:, 2]
    plt.plot(xgen, xfiterr, 'o', markersize=8)
    plt.axis([-10, 190, 0, 1.2*max(xfiterr)])
    plt.tight_layout(pad=1.9)
    plt.grid(True)
    plt.ylabel(r'Fit uncertainty $\delta\beta$ (deg)', fontsize=label_size)
    plt.xlabel(r'Gen value of $\beta$ (deg)', fontsize=label_size)
    plt.show()

def plot_all(data, params):
    """ Linearity and errors plot """
    label_size = 20
    xgen, xfit, xfiterr = data[:, 0], data[:, 1], data[:, 2]
    xoffset = xfit - xgen
    plt.rc('text', usetex=True)
    plt.rc('font', family='serif', size=18)
    plt.rc('font', size=18)
    plt.style.use('seaborn-white')
    plt.figure(num=None, figsize=(10, 5), dpi=150)

    plt.subplot(121)
    plt.plot(xgen, np.zeros(len(xgen)), 'r-', linewidth=2.5)
    plt.errorbar(xgen, xoffset, yerr=xfiterr, fmt='o', markersize=8)
    ylo = 1.2*min(xoffset - xfiterr)
    yhi = 1.2*max(xoffset + xfiterr)
    plt.axis([-10, 190, ylo, yhi])
    plt.grid(True)
    plt.ylabel(r'$\mathrm{Fit\ offset\ of}\ \beta\ \mathrm{(deg)}$', fontsize=label_size)
    plt.xlabel(r'$\mathrm{Gen\ value\ of}\ \beta\ \mathrm{(deg)}$', fontsize=label_size)

    plt.subplot(122)
    plt.plot(xgen, xfiterr, 'o', markersize=8)
    plt.axis([-10, 190, 0, 1.2*max(xfiterr)])
    plt.grid(True)
    plt.ylabel(r'$\mathrm{Fit\ uncertainty}\ \delta\beta\ \mathrm{(deg)}$', fontsize=label_size)
    plt.xlabel(r'$\mathrm{Gen\ value\ of}\ \beta\ \mathrm{(deg)}$', fontsize=label_size)

    plt.tight_layout(pad=1.9, w_pad=2.)
    plt.subplots_adjust(left=0.1, bottom=0.15, right=0.95, top=0.95)
    plt.savefig('pics/lin_' + params[0] + params[1] + '_' + \
     params[2] + '.eps', format='eps', dpi=150)
    plt.show()

def plot_beta_scan():
    """ Fit offset and statistical error """
    params, data = lp.get_beta_fit()
    plot_all(data, params)

if __name__ == "__main__":
    plot_beta_scan()
