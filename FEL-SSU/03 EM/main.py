#!/usr/local/bin/python3

from GaussMVD import GaussMVD
from PIL import Image
from snippets import *
import matplotlib.pyplot as plt
import numpy as np
from scipy.optimize import minimize

np.set_printoptions(threshold=np.nan)

FOREGROUND = 1
BACKGROUND = 0

def shape_model(u, s):
    return np.exp(u * s) / (1 + np.exp(u))

def compute_shape_model(u, s):
    return np.array([shape_model(x, s) for x in u])

def load_image_with_style(image_name, style):
    image = Image.open(image_name).convert(style)
    return np.array(image, dtype=np.float64) / 255.0

class Hand:

    def __init__(self, number):
        self.foreground = GaussMVD()
        self.background = GaussMVD()
        self.name = "hand_{:0>2d}".format(number)
        
        # Load image

        image_name = f"{self.name}.png"
        self.image = load_image_with_style(image_name, "RGB")

        # Load ground truth
        segmentation_name = f"{self.name}_seg.png"
        self.ground_truth_image = load_image_with_style(segmentation_name, "L")
        
        # Initialize alpha with image size
        self.alpha = np.zeros(self.image.size)

        # Load default means
        means_foreground, means_background = [compute_mean(self.image, is_foreground=x) for x in [True, False]]
        self.foreground.means = means_foreground
        self.background.means = means_background
    
    def update_alpha(self, u):
        # Compute probabilities for appearance model
        probs_f = self.foreground.compute_probs(self.image)
        probs_b = self.background.compute_probs(self.image)

        # Compute probabilities for shape model
        smf = compute_shape_model(u, FOREGROUND)
        smb = compute_shape_model(u, BACKGROUND)

        # Compute joint probabilities
        multiply_f = np.multiply(probs_f, smf)
        multiply_b = np.multiply(probs_b, smb)

        # Update alpha using joint probabilities
        self.alpha = np.divide(multiply_f, multiply_f + multiply_b)

    def maximize(self):
        # Update appearance model
        self.foreground.estimate(self.image, self.alpha)
        self.background.estimate(self.image, 1 - self.alpha)
    
    def compute_distance(self):
        return self.background.compute_distance(self.foreground)

    def compute_segmentation(self):
        result = np.array(self.alpha, copy=True)
        result[result >= 0.5] = 1
        result[result < 0.5] = 0
        return result

    def compute_error(self):
        result = self.compute_segmentation()
        return compute_error(result, self.ground_truth_image)

    def save_segmentation(self):
        result = np.array(self.alpha, copy=True)
        image = to_image(result)
        image.save(f"em/{self.name}.png")

# Initialize u_init
image = Image.open('model_init.png').convert('L')
u_init = np.array(image) / 255

# Initialize EM algorithm test data
number_of_images = 50
hands = [Hand(i) for i in range(number_of_images)]

# E-step
def e_step(u):
    for hand in hands:
        hand.update_alpha(u)

# M-step
def m_step():
    # Weighted sum of alphas
    alphas = np.zeros(hands[0].alpha.shape)
    for hand in hands:
        alphas += hand.alpha
    alphas /= len(hands)

    for hand in hands:
        hand.maximize()

    # Fix runtime crash "division by zero"
    diff = 1 - alphas
    diff[diff == 0] = 0.001
    
    # Compute update function
    return np.log(np.divide(alphas, diff))

u_current = u_init
distance = -1

while (True):
    e_step(u_current)
    u_current = m_step()

    total_error = 0
    total_distance = 0
    for hand in hands:
        total_error += hand.compute_error()
        total_distance += hand.compute_distance()
    
    total_error /= len(hands)
    total_distance /= len(hands)

    print("Error", total_error, "Distance", total_distance)

    # End of iterations
    if (distance != -1 and abs(distance - total_distance) < pow(10, -3)):
        break
    
    distance = total_distance

for hand in hands:
    print(hand.name)
    hand.save_segmentation()
    print("Error", hand.compute_error())

image = to_image(compute_shape_model(u_current, FOREGROUND))
image.save("result.png")
