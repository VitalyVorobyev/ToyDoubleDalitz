#! /usr/bin/python
""" Plot toy MC experiments result for B0 -> D0h0 """

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.mlab as mlab
from scipy.stats import chi2
# from scipy import optimize
import log_parser as lp
import toy
import ddcfg

def get_toy_data():
    """ Toy MC fit results """
    print "Type [gen=1], [beta=22], [ftype=sim]"
    ddcfg.LOGDIR += "belle/"
    items = raw_input().strip().split()
    epoch = int(items[0]) if len(items) > 0 else 1
    beta = int(items[1]) if len(items) > 1 else 22
    ftype = items[2] if len(items) > 2 else 'sim'
    logexpr = toy.tlog_d0h0_tplt(beta, "--"+ftype, "--belle", epoch)
    return [lp.parse_toy_logs(logexpr), beta, (epoch, ftype)]

def get_toy_beta():
    """ Beta toy fit results """
    (fit_dict, pul_dict), beta, pars = get_toy_data()
    betaf = np.array(fit_dict['beta']) * lp.RAD_TO_DEG
    betap = np.array(pul_dict['beta'])
    diff = betaf - beta
    set1 = abs(diff - 180) < abs(diff)
    set2 = abs(diff + 180) < abs(diff)
    set3 = set1 + set2
    errf = np.zeros(len(betaf))
    errf[set3] = diff[set3] / betap[set3]
    betaf[set1] = betaf[set1] - 180
    betaf[set2] = betaf[set2] + 180
    betap[set3] = (betaf[set3] - beta) / errf[set3]
    return [betaf, betap, beta, pars]

def make_hist(data, binn):
    """ Returns histogram with asymmetric Poisson error bars """
    hist, edges = np.histogram(data, bins=binn)
    norm = (edges[1] - edges[0]) * len(data)
    centers = 0.5*(edges[1:] + edges[:-1])
    sqerr = np.sqrt(hist + 0.25)
    hist, centers, sqerr = hist[hist != 0], centers[hist != 0], sqerr[hist != 0]
    errp = sqerr + 0.5
    errn = sqerr - 0.5
    return centers, hist, [errn, errp], norm

def stat_box_str(mean, err, std):
    """ Make stat box string for a hist """
    return r'$\mathrm{mean =\ }' + r'{:.2f} \pm {:.2f}$'.format(mean, err) + '\n' + \
           r'$\mathrm{std\ \ \  =\ }' + r'{:.2f}$'.format(std)

def variance_conf_interval(std, nexp, pval=0.67):
    """ Confidence interval for variance assuming normal distribution """
    alpha = 1. - pval
    lolim = std * np.sqrt((nexp - 1) / chi2.pdf(0.5*alpha, nexp -1))
    hilim = std * np.sqrt((nexp - 1) / chi2.pdf(1. - 0.5*alpha, nexp - 1))
    return [lolim, hilim]

def save_fig(fig, figtitle, dpi=150):
    """ pdf, eps, png """
    fig.savefig('pics/' + figtitle + '.eps', format='eps', dpi=dpi)
    fig.savefig('pics/pdf/' + figtitle + '.pdf', format='pdf', dpi=dpi)
    fig.savefig('pics/png/' + figtitle + '.png', format='png', dpi=dpi)

def plot_toy():
    """ Plot toy MC """
    betaf, betap, beta, pars = get_toy_beta()
    epoch, ftype = pars
    nevent = len(betaf)
    nbins = min(50, nevent / 8)
    bmin, bmax = min(betaf), max(betaf)
    betapad = 0.1*(bmax - bmin)
    betalo = min(18., bmin - betapad)
    betahi = max(28., bmax + betapad)
    meanf, stdf = betaf.mean(), betaf.std()
    meanp, stdp = betap.mean(), betap.std()
    errmf = stdf / np.sqrt(len(betaf))
    errmp = stdp / np.sqrt(len(betap))
    print 'Fit  mean: {:.2f} +- {:.2f}, std: {:.2f}'.format(meanf, errmf, stdf)
    print 'Pull mean: {:.2f} +- {:.2f}, std: {:.2f}'.format(meanp, errmp, stdp)
#    fvar_range = variance_conf_interval(stdf, nevent)
#    pvar_range = variance_conf_interval(stdp, nevent)
#    print 'Fit  var in ({:.2f}, {:.2f})'.format(fvar_range[0], fvar_range[1])
#    print 'Pull var in ({:.2f}, {:.2f})'.format(pvar_range[0], pvar_range[1])

    binf, histf, errf, _ = make_hist(betaf, nbins)
    binp, histp, errp, normp = make_hist(betap, nbins)
    if max(abs(betap)) > 5:
        pullmin, pullmax = -10, 10
    else:
        pullmin, pullmax = -5, 5

    dpi = 150
    label_size = 26
    plt.rc('text', usetex=True)
    plt.rc('font', family='serif', size=20)
    plt.style.use('seaborn-white')

    fig1 = plt.figure(num=1, figsize=(4, 4), dpi=dpi)
    _, caps, _ = plt.errorbar(binf, histf, yerr=errf, fmt='o',\
                 markersize=5, ecolor='k', elinewidth=0.5, capsize=2)
    for cap in caps:
        cap.set_markeredgewidth(1.25)
    plt.plot([beta, beta], [0, max(histf)], 'r-', lw=1.5)
    # plt.text(max(betaf) - 2*stdf, 1.1*max(histf), stat_box_str(meanf, errmf, stdf), fontsize=14)
    plt.xlabel(r'$\beta\ \mathrm{(deg)}$', fontsize=label_size)
    plt.xlim([betalo, betahi])
    plt.tight_layout(pad=0.7, w_pad=2.2)
    plt.grid(True)

    save_fig(fig1, '_'.join(['toy', 'd0h0', str(epoch), ftype, 'beta', str(beta)]))
    fig2 = plt.figure(num=2, figsize=(4, 4), dpi=dpi)
    plt.grid(True)
    _, caps, _ = plt.errorbar(binp, histp, yerr=errp, fmt='o', markersize=5, ecolor='k', \
     elinewidth=0.5, capsize=2)
    for cap in caps:
        cap.set_markeredgewidth(1.25)
    bins = np.linspace(-5., 5., 100)
    gaus = mlab.normpdf(bins, 0., 1.) * normp
    plt.plot(bins, gaus, 'r--', linewidth=1)
    # plt.text(max(betap) - 2*stdp, 1.1*max(histp), stat_box_str(meanp, errmp, stdp), fontsize=14)
    plt.xlabel(r'$\beta\ \mathrm{pull}$', fontsize=label_size)
    plt.xlim([pullmin, pullmax])
    plt.tight_layout(pad=0.7, w_pad=2.2)

    save_fig(fig2, '_'.join(['toy_pull', 'd0h0', str(epoch), ftype, 'beta', str(beta)]))
    plt.show()

if __name__ == '__main__':
    plot_toy()
