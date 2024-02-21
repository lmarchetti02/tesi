import sys
import numpy as np
from scipy import interpolate, integrate
import pathlib


def _pdf(x):
    return (1 / np.sqrt(2 * np.pi)) * np.exp(-(x**2) / 2)


def _cdf(x):
    return integrate.quad(_pdf, -np.inf, x)[0]


def _get_xy():
    x = np.linspace(-6, 6, num=1_000)
    y = np.array([_cdf(i) for i in x])

    return x, y


def get_distribution(size: int):
    size = int(size)

    # get inverse cdf
    x, y = _get_xy()
    cdf_inverse = interpolate.interp1d(y, x)

    # get sample
    e = 1e-5
    while True:
        try:
            u = np.random.uniform(e, 1 - e, size)
            sample = cdf_inverse(u)
            break
        except ValueError as _:
            continue

    # save to file
    np.savetxt(pathlib.Path("../build/spectrum.txt"), sample)


if __name__ == "__main__":
    args = sys.argv
    globals()[args[1]](*args[2:])
