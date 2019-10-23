#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import numpy as np
from cpg.oscilator_constants import * 
from random import uniform
from random import randint

from matplotlib import pyplot as plt

INIT_GAIT = QUADRUPED_GAIT_WEIGHTS

INIT_NEURON_STATE = 0.1
INIT_NEURON_RAND = 0.2

class Neuron:
    def __init__(self, init, unit_index):
        init = self.count_init(unit_index)
        self.last_state = init
        self.current_state = init

    def add_new_record(self,state):
        self.last_state = self.current_state
        self.current_state = state

    def count_init(self, unit_index):
        init = INIT_NEURON_STATE * randint(-1, 1) + uniform(-INIT_NEURON_RAND, INIT_NEURON_RAND)
        return init

    def reset_state(self, unit_index):
        init = self.count_init(unit_index)
        self.last_state = init
        self.current_state = init

    def get_last_state(self):
        return self.last_state

class Oscilator:
    def __init__(self, unit_index, num_neurons):
        self.unit_index = unit_index
        self.last_inhib = [0.9, 0.9]
        self.type_of_gait = INIT_GAIT
        init = self.count_init(unit_index)
        self.neurons = [Neuron(init, unit_index), Neuron(init, unit_index)]
        self.last_state = 0
        self.num_neurons = num_neurons

    def count_init(self, unit_index):
        neuron_base = 0.9
        init = (unit_index % 2) * (-2) * neuron_base + neuron_base
        init = INIT_NEURON_STATE + unit_index * uniform(0, 0.5)
        return init

    def sigmoid(self, x):
        res = np.arcsin(np.tanh(x))    
        return res

    def self_inhibit_delta(self, neuron_index):
        delta = - self.last_inhib[neuron_index]
        delta += max(self.neurons[neuron_index].get_last_state(), 0)
        delta /= T_a
        delta *= (1./frequency)
        return delta

    def self_inhibit(self, neuron_index):
        new_value = self.last_inhib[neuron_index] + self.self_inhibit_delta(neuron_index)
        self.last_inhib[neuron_index] = new_value
        return new_value

    def get_influenced_by_other_CPGs(self, neuron_index, last_state_values):
        value = 0
        for j in range(self.num_neurons):
            value += self.type_of_gait[self.unit_index][j]*max(0, last_state_values[j][neuron_index])
        return value

    def count_current_neuron_delta(self, neuron_index, last_state_values, feedback=0):
        other_neuron_index = (neuron_index + 1) % 2
        #########
        delta = -self.neurons[neuron_index].get_last_state()
        delta += w_fe * max(self.neurons[other_neuron_index].get_last_state(), 0)
        delta += -(beta[neuron_index] * self.self_inhibit(neuron_index))
        delta += s[neuron_index]
        delta += self.get_influenced_by_other_CPGs(neuron_index, last_state_values)
        delta += feedback
        #########
        delta /= T_r
        delta *= (1./frequency)
        return delta

    def count_current_neuron_state(self, neuron_index, last_state_values):
        delta = self.count_current_neuron_delta(neuron_index, last_state_values)
        new_state = self.neurons[neuron_index].get_last_state() + delta
        new_state = self.sigmoid(new_state)
        self.neurons[neuron_index].add_new_record(new_state)
        return(new_state)

    def oscilate(self, last_state_values):
        new_value = -self.count_current_neuron_state(0, last_state_values)
        new_value += self.count_current_neuron_state(1, last_state_values)
        new_value = self.sigmoid(new_value)
        self.last_state = new_value


    def change_gait(self, new_weights):
        for i in range(2):
            self.neurons[i].reset_state(self.unit_index)
        self.type_of_gait = new_weights


class OscilatorNetwork:
    def __init__(self, num_neurons):
        self.period = None
        self.amplitude = None
        self.num_neurons = num_neurons
        self.new_cycle_started = [False]*self.num_neurons
        self.CPGunits = list()
        for i in range(0, self.num_neurons):
            self.CPGunits.append(Oscilator(i, self.num_neurons))

    def oscilate_all_CPGs(self):
        last_values = self.get_last_ext_and_flex()
        last_CPG_values = []
        
        for index in range(0, self.num_neurons):
            last_CPG_values.append(self.CPGunits[index].last_state)
            self.CPGunits[index].oscilate(last_values)
        
        for i in range(0, self.num_neurons):
            if last_CPG_values[i] < 0 and self.CPGunits[i].last_state > 0:
                self.new_cycle_started[i] = True
            else:
                self.new_cycle_started[i] = False
        return self.new_cycle_started

    def get_last_values(self):
        values = list()
        for index in range(self.num_neurons):
            values.append(self.CPGunits[index].last_state)
        values = np.array(values)
        return values

    def get_last_ext_and_flex(self):
        values = list()
        for index in range(self.num_neurons):
            values.append([self.CPGunits[index].neurons[0].current_state, self.CPGunits[index].neurons[1].current_state])
        return values

    def set_period_and_amplitude(self):
        iterations = frequency * 100
        time_unit = 1./frequency
        period = 0
        max_value = 0
        last_state = self.CPGunits[0].last_state
        counting_started = False
        for i in range(iterations):
            self.oscilate_all_CPGs()
            new_state = self.CPGunits[0].last_state
            if max_value < np.abs(new_state):
                max_value = np.abs(new_state)
            if new_state * last_state < 0:
                if counting_started:
                    period += time_unit
                    break
                counting_started = True
            if counting_started:
                period += time_unit
            last_state = new_state
        self.period = period * 2
        self.amplitude = max_value

    def change_gait(self, new_weights):
        for i in range(self.num_neurons):
            self.CPGunits[i].change_gait(new_weights)


# main
if __name__ == "__main__":
    num_neurons = 6
    osc = OscilatorNetwork(num_neurons)
    data = list()
    for i in range(num_neurons):
        data.append(list())

    iterations = 100
    
    for i in range(iterations * frequency):
        osc.oscilate_all_CPGs()
        if i == 50 * frequency:
            osc.change_gait(TRIPOD_GAIT_WEIGHTS)
        for i in range(num_neurons):
            data[i].append(osc.get_last_values()[i])

    x = np.arange(0, iterations, step=1./frequency)
    #print(data[0])

    
    f = plt.figure(figsize=(7,4))

    plt.plot(x, data[0], color=[0.07, 0.55, 0.98], label='CPG$_1$')
    plt.plot(x, data[1], color=[1.0, 0.82, 0.0], label='CPG$_2$')
    plt.plot(x, data[2], color=[0, 0.39, 0.74], label='CPG$_3$')
    plt.plot(x, data[3], color=[0.83, 0.67, 0.0], label='CPG$_4$')
    plt.plot(x, data[4], color=[0, 0.23, 0.44], label='CPG$_5$')
    plt.plot(x, data[5], color=[0.65, 0.53, 0.0], label='CPG$_6$')
    
    plt.xlabel('Sample', fontsize=14)
    plt.ylabel('Amplitude', fontsize=14)
    
    ax = plt.gca()

    ax.spines['right'].set_visible(False)
    ax.spines['top'].set_visible(False)

    plt.legend(loc='upper center', ncol=3, bbox_to_anchor=(0.5,1.28), fontsize=13)
    plt.show()


