#import matplotlib
#matplotlib.use('Agg')
#import matplotlib.pyplot as plt
import numpy as np

def evaluate(rewards):
    # TODO implement your own code here if you want to
    # or alternatively you can modify the existing code


    np.set_printoptions(precision=5)

    # print("Rewards:")
    # print(rewards)
    # print("Simple moving average:")
    # print(simple_moving_average(rewards, 40))
    # print("Exponential moving average")
    # print(exponential_moving_average(rewards, 0.2))
    print("Average")
    print(np.sum(rewards) / len(rewards))

    # to use this plot function you have to install matplotlib
    # use conda install matplotlib
    #plot_series(simple_moving_average(rewards, 40))



# check Wikipedia: https://en.wikipedia.org/wiki/Moving_average
def simple_moving_average(x, N):
    mean = np.zeros(len(x) - N + 1)
    sum = np.sum(x[0:N])
    for i in range(len(mean) - 1):
        mean[i] = sum
        sum -= x[i]
        sum += x[i + N]
    mean[len(mean)-1] = sum
    return mean / N

# check Wikipedia: https://en.wikipedia.org/wiki/Moving_average
def exponential_moving_average(x, alpha):
    mean = np.zeros(len(x))
    mean[0] = x[0]
    for i in range(1, len(x)):
        mean[i] = alpha * x[i] + (1.0 - alpha) * mean[i - 1]
    return mean

# you can use this function to get a plot
# you need first to install matplotlib (conda install matplotlib)
# and then uncomment this function and lines 1-3
#def plot_series(arr):
#    plt.plot(arr)
#    plt.savefig('moving_average.pdf')