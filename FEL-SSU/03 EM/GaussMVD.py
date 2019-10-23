import numpy as np

# --------------------------------------------------
# Example 1
# load a colour image and put it into a 3D numpy array

# -------------------------------------------------
# Example 2
# assume that arr is an n-dimensional numpy array

# apply a function to all elements of the array
# parr = np.log(arr / (1.0 - arr))

# truncate the values in arr by some maxval
# maxval = 1
# arr[arr > maxval] = maxval

# ----------------------------------------------------
# Example 3
# class definition of a multivariate Gaussian (fixed dimension = 3)

class GaussMVD:
    """ Multivariate normal distribution """
    dim = 3

    # ===============================================
    def __init__(self):
        self.mean = np.zeros(GaussMVD.dim, dtype=np.float64)
        self.cov = np.eye(GaussMVD.dim, dtype=np.float64)
        self.det = np.linalg.det(np.matrix(self.cov))
        self.normc = 1.0 / np.sqrt(np.power((2.0 * np.pi), GaussMVD.dim) * self.det)

    # ===============================================
    def modify(self, mean, cov):
        if not ((mean.shape == (GaussMVD.dim,)) and (cov.shape == (GaussMVD.dim, GaussMVD.dim))):
            raise Exception("Gaussian: shape mismatch!")

        self.mean = np.array(mean, dtype=np.float64)
        self.cov = np.array(cov, dtype=np.float64)

        self.det = np.linalg.det(np.matrix(self.cov))
        self.normc = 1.0 / np.sqrt(np.power((2.0 * np.pi), GaussMVD.dim) * self.det)

        return None

    # ===============================================
    def compute_probs(self, arr):
        """ compute probabilities for an array of values """

        inv_cov = np.asarray(np.linalg.inv(np.matrix(self.cov)))
        darr = arr - self.mean
        varr = np.sum(darr * np.inner(darr, inv_cov), axis=-1)
        varr = - varr * 0.5
        varr = np.exp(varr) * self.normc

        return varr

    # ===============================================
    def estimate(self, arr, weight):
        """ estimate parameters from data (array of values & array of weights) """
        eweight = weight[..., np.newaxis]
        wsum = np.sum(weight)

        dimlist = list(range(len(arr.shape) - 1))
        dimtup = tuple(dimlist)

        # estimate mean
        mean = np.sum(eweight * arr, axis=dimtup) / wsum

        # estimate covariance
        darr = arr - mean
        cov = np.tensordot(darr, darr * eweight, axes=(dimlist, dimlist)) / wsum

        self.modify(mean, cov)

        return None

    # ===============================================
    def compute_distance(self, mvgd):
        """ Bhattacharyya distance """

        ccov = (self.cov + mvgd.cov) / 2.0
        inv_ccov = np.asarray(np.linalg.inv(np.matrix(ccov)))
        d_ccov = np.linalg.det(np.matrix(ccov))
        cmean = self.mean - mvgd.mean
        v1 = np.dot(cmean, np.tensordot(inv_ccov, cmean, 1)) / 8.0
        v2 = np.log(d_ccov / np.sqrt(self.det * mvgd.det)) / 2.0

        return v1 + v2

    # ===============================================
    def write(self):
        print("Mean, covariance:")
        print(repr(self.mean))
        print(repr(self.cov))