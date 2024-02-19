import numpy as np
from scipy import interpolate
import matplotlib.pyplot as plt
from numba import njit


@njit
def get_mid_point(arr: np.ndarray):
    res = np.empty(len(arr) - 1)

    for i in range(len(arr) - 1):
        res[i] = arr[i] + (arr[i + 1] - arr[i]) / 2

    return res


# distribution
data = np.random.normal(0, 1, 10_000)

# get cdf
values, bins, _ = plt.hist(
    data, bins="auto", density=True, cumulative=True, histtype="stepfilled"
)
bins = get_mid_point(bins)

# get inverse cdf
inverse_cdf = interpolate.interp1d(values, bins)

# get sample
e = 1e-4
u = np.random.uniform(e, 1 - e, 10_000)
sample = inverse_cdf(u)

# save to file
np.savetxt("spectrum.txt", sample)
