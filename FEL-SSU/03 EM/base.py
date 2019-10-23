#!/usr/local/bin/python3

import numpy as np
import matplotlib.pyplot as plt
from sklearn.cluster import KMeans
from sklearn.mixture import GaussianMixture
from sklearn.utils import shuffle
from PIL import Image
from snippets import compute_mean
from snippets import load_image
from snippets import compute_error

def recreate_image(codebook, labels, w, h):
    """Recreate the (compressed) image from the code book & labels"""
    d = codebook.shape[1]
    image = np.zeros((w, h, d))
    label_idx = 0
    for i in range(w):
        for j in range(h):
            # image[i][j] = codebook[labels[label_idx]]
            image[i][j] = 1 - labels[label_idx]
            label_idx += 1
    return image

def plot_image(image, title=''):
    plt.clf()
    plt.axis('off')
    plt.title(title)
    plt.imshow(image)
    plt.show()

def compute_error(image, ground_truth, width, height):
    diff = np.sum(abs(ground_truth - image))
    result = diff / (3 * width * height)
    if result > 0.5:
        result = 1 - result
    
    return result * 100

def process_image(image_name, sample_size=0.1, show_images=False, verbose=False):

    # Number of clusters
    n_colors = 2

    # Ground truth
    ground_truth = load_image(f"{image_name}_seg.png")

    # Load the image
    image = load_image(f"{image_name}.png")

    # Transform the image to a 2D numpy array.
    w, h, d = tuple(image.shape)
    assert d == 3
    image_array = np.reshape(image, (w * h, d))

    # Load fitting sample
    # sample_size = 0.1
    number_of_samples = int(np.ceil(w * h * sample_size))
    if verbose:
        print(f"Sample size {number_of_samples}")
    image_array_sample = shuffle(image_array, random_state=0)[:number_of_samples]

    # Compute means
    mean_f = compute_mean(image, is_foreground=True)
    mean_b = compute_mean(image, is_foreground=False)
    centers = np.array([mean_f, mean_b])

    # Fit k-Means on sample
    kmeans = KMeans(n_clusters=n_colors, init=centers, random_state=0, n_init=1).fit(image_array_sample)
    # kmeans = KMeans(n_clusters=n_colors, random_state=0).fit(image_array_sample)
    labels_kmeans = kmeans.predict(image_array)

    # Fit GMM on sample
    gmm = GaussianMixture(n_components=n_colors, means_init=centers).fit(image_array_sample)
    # gmm = GaussianMixture(n_components=n_colors).fit(image_array_sample)
    labels_gmm = gmm.predict(image_array)

    # Display k-Means result and print the error
    kmeans_image = recreate_image(kmeans.cluster_centers_, labels_kmeans, w, h)
    kmeans_error = compute_error(kmeans_image, ground_truth, w, h)
    if verbose:
        print(f'K-Means error: {kmeans_error}')
    if show_images:
        plot_image(kmeans_image, 'K-Means')

    # Display GMM result and print the error
    gmm_image = recreate_image(gmm.means_, labels_gmm, w, h)
    gmm_error = compute_error(gmm_image, ground_truth, w, h)
    if verbose:
        print(f'GMM error: {gmm_error}')
    if show_images:
        plot_image(gmm_image, 'GMM')

    return (kmeans_error, gmm_error)

number_of_iterations = 1
number_of_images = 50

kmeans_total_error = 0
gmm_total_error = 0

for i in range(number_of_images):
    image_name = "hand_{:0>2d}".format(i)
    for it in range(number_of_iterations):
        print(f"{image_name} iteration {it}")
        kmeans_error, gmm_error = process_image(image_name, verbose=True)
        kmeans_total_error += kmeans_error
        gmm_total_error += gmm_error

total_number_of_iterations = number_of_images * number_of_iterations

print("########################################")
print(f"K-Means error: {kmeans_total_error / total_number_of_iterations}")
print(f"GMM error: {gmm_total_error / total_number_of_iterations}")
