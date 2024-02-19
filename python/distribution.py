import sys
import numpy as np
from scipy import interpolate
import matplotlib.pyplot as plt
from numba import njit
import pathlib


@njit
def _get_mid_point(arr: np.ndarray):
    res = np.empty(len(arr) - 1)

    for i in range(len(arr) - 1):
        res[i] = arr[i] + (arr[i + 1] - arr[i]) / 2

    return res


def get_distribution(size: int):
    size = int(size)

    # distribution
    data = np.random.normal(0, 1, 10_000)

    # get cdf
    values, bins, _ = plt.hist(
        data, bins="auto", density=True, cumulative=True, histtype="stepfilled"
    )
    bins = _get_mid_point(bins)

    # get inverse cdf
    inverse_cdf = interpolate.interp1d(values, bins)

    # get sample
    e = 1e-5
    while True:
        try:
            u = np.random.uniform(e, 1 - e, size)
            sample = inverse_cdf(u)
            break
        except ValueError as _:
            continue

    # save to file
    np.savetxt(pathlib.Path("../build/spectrum.txt"), sample)


if __name__ == "__main__":
    args = sys.argv
    globals()[args[1]](*args[2:])
