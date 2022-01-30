import toml
import json
import subprocess
import sys
import time
import json
import os
import numpy as np
import scipy.stats as stats


def gauss(mu, sigma, number):
    '''
    Return `number`-length truncated Gaussian sequence.

    Range: [mu - 3 sigma, mu + 3 sigma]

    Args:
        mu (int/float): mean
        sigma (int/float): variance       
        number (int): the length of truncated Gaussian sequence

    Returns:
        np.ndarray: a number-length truncated Gaussian sequence
    '''
    lower, upper = -3, 3
    X = stats.truncnorm(lower, upper, loc=mu, scale=sigma)
    return X.rvs(size=number)

def Unitraceloader(prebdw_list,postbdw_list):
    for premu,presigma,prenum,preinterval in prebdw_list:
        for postmu,postsigma,postnum,postinterval in postbdw_list:
            raw_data=gauss(premu,presigma,prenum)
            raw_data=np.append(raw_data,gauss(postmu,postsigma,postnum))
            trace_file="uni"+"-"+str(premu)+"-"+str(postmu)+".txt"
            f = open(trace_file, 'w', encoding="utf-8")
            for bw in raw_data:
                f.write(str(bw)+"Mbps"+"\n")


def Countertraceloader(bdw_list):
    for premu,presigma,prenum,preinterval in bdw_list:
        raw_data=gauss(premu,presigma,prenum)
        trace_file="counter"+"-"+str(premu)+"-"+str(presigma)+".txt"
        f = open(trace_file, 'w', encoding="utf-8")
        for bw in raw_data:
            f.write(str(bw)+"Mbps"+"\n")
