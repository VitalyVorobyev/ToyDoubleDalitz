#! /usr/bin/python
""" Copyright 2017 Vitaly Vorobyev """

import numpy as np
import matplotlib.pyplot as plt
import log_parser as lp

MIN_ANGLE = -10
MAX_ANGLE = 180
ANGLE_TICKS = np.arange(0, 181, 30)
OFFSET_TITLE = r'$\mathrm{Fit\ bias\ (deg)}$'
SIGMA_TITLE = r'$\mathrm{Fit\ uncertainty\ (deg)}$'
GEN_TITLE = r'$\mathrm{Input}\ \beta\ \mathrm{(deg)}$'
DPI = 150
LBL_SIZE = 22

def plot_linearity(data, num=None):
    """ Linearity plot """
    plt.rc('text', usetex=True)
    plt.rc('font', family='serif', size=18)
    plt.rc('font', size=18)
    plt.style.use('seaborn-white')
    fig = plt.figure(num=num, figsize=(4, 4), dpi=130)
    xgen, xfit, xfiterr = data[:, 0], data[:, 1], data[:, 2]
    plt.plot(xgen, np.zeros(len(xgen)), 'r-', linewidth=2.5)
    plt.errorbar(xgen, xfit - xgen, yerr=xfiterr, fmt='o', markersize=8)
    xoffset = xfit - xgen
    ylo = 1.2*min(xoffset - xfiterr)
    yhi = 1.2*max(xoffset + xfiterr)
    plt.axis([MIN_ANGLE, MAX_ANGLE, ylo, yhi])
    plt.tight_layout(pad=1.9)
    plt.grid(True)
    plt.ylabel(OFFSET_TITLE, fontsize=LBL_SIZE)
    plt.xlabel(GEN_TITLE, fontsize=LBL_SIZE)
    return fig

def plot_uncertainty(data, num=None):
    """ Errors plot """
    plt.rc('text', usetex=True)
    plt.rc('font', family='serif', size=18)
    plt.style.use('seaborn-white')
    fig = plt.figure(num=num, figsize=(4, 4), dpi=130)
    xgen, xfiterr = data[:, 0], data[:, 2]
    plt.plot(xgen, xfiterr, 'o', markersize=8)
    plt.axis([MIN_ANGLE, MAX_ANGLE, 0, 1.2*max(xfiterr)])
    plt.tight_layout(pad=1.9)
    plt.grid(True)
    plt.ylabel(SIGMA_TITLE, fontsize=LBL_SIZE)
    plt.xlabel(GEN_TITLE, fontsize=LBL_SIZE)
    return fig

def plot_dil_uncertainty_comparison(data, dil_Data, num=None):
    """ Error bars comparison """
    plt.rc('text', usetex=True)
    plt.rc('font', family='serif', size=18)
    plt.style.use('seaborn-white')
    fig = plt.figure(num=num, figsize=(4, 4), dpi=130)
    xgen, xfiterr, xdilfiterr = data[:, 0], data[:, 2], dil_Data[:, 2]
    plt.plot(xgen, xfiterr, 'o', markersize=8)
    plt.plot(xgen, xdilfiterr, 'rp', markersize=8)
    plt.axis([MIN_ANGLE, MAX_ANGLE, 0, 1.2*max(xdilfiterr)])
    plt.tight_layout(pad=1.9)
    plt.grid(True)
    plt.ylabel(SIGMA_TITLE, fontsize=LBL_SIZE)
    plt.xlabel(GEN_TITLE, fontsize=LBL_SIZE)
    return fig

def plot_all(data):
    """ Linearity and errors plot """
    xgen, xfit, xfiterr = data[:, 0], data[:, 1], data[:, 2]
    xoffset = xfit - xgen
    plt.rc('text', usetex=True)
    plt.rc('font', family='serif', size=18)
    plt.rc('font', size=18)
    plt.style.use('seaborn-white')
    fig = plt.figure(num=None, figsize=(8, 4), dpi=130)

    plt.subplot(121)
    plt.plot(xgen, np.zeros(len(xgen)), 'r-', linewidth=2.5)
    plt.errorbar(xgen, xoffset, yerr=xfiterr, fmt='o', markersize=9)
    ylo = 1.2*min(xoffset - xfiterr)
    yhi = 1.2*max(xoffset + xfiterr)
    plt.axis([MIN_ANGLE, MAX_ANGLE, ylo, yhi])
    plt.grid(True)
    plt.ylabel(OFFSET_TITLE, fontsize=LBL_SIZE)
    plt.xlabel(GEN_TITLE, fontsize=LBL_SIZE)
    plt.gca().set_xticks(ANGLE_TICKS)

    plt.subplot(122)
    plt.plot(xgen, xfiterr, 'o', markersize=9)
    plt.axis([MIN_ANGLE, MAX_ANGLE, 0, 1.2*max(xfiterr)])
    plt.grid(True)
    plt.ylabel(SIGMA_TITLE, fontsize=LBL_SIZE)
    plt.xlabel(GEN_TITLE, fontsize=LBL_SIZE)
    plt.gca().set_xticks(ANGLE_TICKS)

    plt.tight_layout(pad=1.9, w_pad=1.5)
    plt.subplots_adjust(left=0.1, bottom=0.2, right=0.98, top=0.98)
    return fig

def save_fig(fig, cmd, pars, dpi=DPI):
    """ eps, png and pdf """
    pict_name = '_'.join([cmd] + [str(par) for par in pars])
    fig.savefig('pics/' + pict_name + '.eps', format='eps', dpi=dpi)
    fig.savefig('pics/png/' + pict_name + '.png', format='png', dpi=dpi)
    fig.savefig('pics/pdf/' + pict_name + '.pdf', format='pdf', dpi=dpi)

def plot_beta_scan():
    """ Fit bias and statistical error """
    exp, gen, ftype = lp.ask_params()
    params = [exp, gen, ftype]

    data = lp.get_beta_fit(exp, gen, ftype)
#    figlin = plot_linearity(data, 1)
#    save_fig(figlin, 'slin', params)
#    figerr = plot_uncertainty(data, 2)
#    save_fig(figerr, 'serr', params)

    dil_data = lp.get_beta_fit(exp, gen, ftype, dil=True)
#    figlindil = plot_linearity(dil_data, 3)
#    save_fig(figlindil, 'slin_dil', params)
#    figerrdil = plot_uncertainty(dil_data, 4)
#    save_fig(figerrdil, 'serr_dil', params)
    fig_comp = plot_dil_uncertainty_comparison(data, dil_data, 5)
    save_fig(fig_comp, 'serr_dil_comp', params)

    plt.show()

if __name__ == "__main__":
    plot_beta_scan()
