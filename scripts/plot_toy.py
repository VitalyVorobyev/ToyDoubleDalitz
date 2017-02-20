""" Plot toy MC experiments result """

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.mlab as mlab
# from scipy import optimize
import log_parser as lp
import toy
import ddcfg

def get_toy_data():
    """ Toy MC fit results """
    print "Type [exp=belle] [gen=1], [beta=23], [ftype=sim]"
    items = raw_input().strip().split()
    exp = items[0] if len(items) > 0 else 'belle'
    if exp == "belle":
        ddcfg.LOGDIR += "belle/"
    else:
        ddcfg.LOGDIR += "lhcbOpt/"
    epoch = int(items[1]) if len(items) > 1 else 1
    beta = int(items[2]) if len(items) > 2 else 23
    ftype = items[3] if len(items) > 2 else 'sim'
    logexpr = toy.tlog_tplt(beta, "--"+ftype, "--"+exp, epoch)
    return lp.parse_toy_logs(logexpr), beta, (exp, epoch, ftype)

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
    return betaf, betap, beta, pars

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
    return r"$\mathrm{mean =\ }" + r"{:.2f} \pm {:.2f}$".format(mean, err) + "\n" + \
    r"$\mathrm{std\ \ \  =\ }" + r"{:.2f}$".format(std)

def plot_toy():
    """ Plot toy MC """
    betaf, betap, beta, pars = get_toy_beta()
    nevent = len(betaf)
    nbins = min(50, nevent / 5)
    betalo = min(betaf) - 5
    betahi = max(betaf) + 5
    meanf = betaf.mean()
    meanp = betap.mean()
    stdf = np.sqrt((betaf**2).mean() - meanf**2)
    stdp = np.sqrt((betap**2).mean() - meanp**2)
    errmf = stdf / np.sqrt(len(betaf))
    errmp = stdp / np.sqrt(len(betap))
    print "Fit  mean: {:.2f} +- {:.2f}, std: {:.2f}".format(meanf, errmf, stdf)
    print "Pull mean: {:.2f} +- {:.2f}, std: {:.2f}".format(meanp, errmp, stdp)

    binf, histf, errf, _ = make_hist(betaf, nbins)
    binp, histp, errp, normp = make_hist(betap, nbins)

    label_size = 20
    plt.rc('text', usetex=True)
    plt.rc('font', family='serif', size=18)
    plt.rc('font', size=18)
    plt.style.use('seaborn-white')
    plt.figure(num=None, figsize=(10, 5), dpi=150)

    plt.subplot(121)
    _, caps, _ = plt.errorbar(binf, histf, yerr=errf, fmt='o', markersize=4, ecolor='k', \
     elinewidth=0.75, capsize=3)
    for cap in caps:
        cap.set_markeredgewidth(1.25)
    plt.plot([beta, beta], [0, max(histf)], 'r-', lw=1.5)
    # plt.text(max(betaf) - 2*stdf, 1.1*max(histf), stat_box_str(meanf, errmf, stdf), fontsize=14)
    plt.xlabel(r'$\mathrm{Fit\ value\ of}\ \beta\ \mathrm{(deg)}$', fontsize=label_size)
    plt.xlim([betalo, betahi])
    plt.grid(True)

    plt.subplot(122)
    plt.grid(True)
    plt.errorbar(binp, histp, yerr=errp, fmt='o', markersize=4, ecolor='k')
    bins = np.linspace(-5., 5., 100)
    gaus = mlab.normpdf(bins, 0., 1.) * normp
    plt.plot(bins, gaus, 'r--', linewidth=1)
    # plt.text(max(betap) - 2*stdp, 1.1*max(histp), stat_box_str(meanp, errmp, stdp), fontsize=14)
    plt.xlabel(r'$\beta\ \mathrm{pull\ distribution}$', fontsize=label_size)
    plt.xlim([-10, 10])

    plt.tight_layout(pad=0.7, w_pad=2.2)
    plt.savefig('pics/toy_' + pars[0] + str(pars[1]) + '_' + \
     pars[2] + "beta" + str(beta) + '.eps', format='eps', dpi=150)
    plt.show()

plot_toy()

