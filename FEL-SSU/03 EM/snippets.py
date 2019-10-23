#!/usr/local/bin/python3

import numpy as np
from PIL import Image
import matplotlib.pyplot as plt

def to_image(img):
    formatted = (img * 255).astype('uint8')
    return Image.fromarray(formatted)

def compute_error(image, ground_truth):
    diff = np.sum(abs(ground_truth - image))
    result = diff / image.size
    if result > 0.5:
        result = 1 - result
    
    return result * 100

# Print full array
# np.set_printoptions(threshold=np.nan)

def load_image(image_name):
    im = Image.open(image_name).convert("RGB")
    return np.array(im, dtype=np.float64) / 255.0

def plot_image(image, title=''):
    plt.clf()
    plt.axis('off')
    plt.title(title)
    plt.imshow(image)
    plt.show()

def compute_mean(hand, is_foreground):
    model = "model_init.png"
    image = Image.open(model).convert("RGB")
    arr = np.array(image, dtype=np.float64) / 255.0

    background = 0.11764706
    # middle = 0.50196078
    foreground = 0.88235294

    if is_foreground:
        arr[arr > foreground] = 1
        arr[arr < foreground] = 0
    else:
        arr[arr > background] = 2
        arr[arr < background] = 1
        arr[arr > 1] = 0

    hand_copy = np.array(hand, copy=True)

    counter = 0
    w, h, d = tuple(hand_copy.shape)
    for i in range(w):
        for j in range(h):
            if arr[i, j, 0] == 0:
                hand_copy[i, j, 0] = 0
                hand_copy[i, j, 1] = 0
                hand_copy[i, j, 2] = 0
            else:
                counter += 1

    mean = np.sum(hand_copy, axis=1)
    mean = np.sum(mean, axis=0) / counter

    return mean
