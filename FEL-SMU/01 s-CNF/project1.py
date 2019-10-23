#!/usr/bin/env python

import numpy as np
from agent import Agent
from oracle import OracleSession

def run_project():
    np.random.seed(0)
    min_n_sessions = 100
    convergence_threshold = 1e-3
    epsilon = np.random.uniform(0.05, 0.1)
    delta = np.random.uniform(0.05, 0.10)

    agent = Agent(epsilon, delta)
    n_sessions = successes = 0
    
    while n_sessions < min_n_sessions or \
            abs((successes / n_sessions) - last_success_rate) >= convergence_threshold:
        print("-" * 64)
        oracle_session = OracleSession()
        n_sessions += 1
        last_success_rate = successes / n_sessions

        agent.interact_with_oracle(oracle_session)

        m = oracle_session.get_requested_dataset_size()
        if oracle_session.was_successful(epsilon):
            successes += 1
            print("%d\tSession was successful. Requested %d samples." % (n_sessions, m)) 
        else:
            print("%d\tSession was unsuccessful. Requested %d samples." % (n_sessions, m)) 
        print("Success rate: %.2f%%" % ((successes / n_sessions) * 100))
        

    s = (successes / n_sessions) 
    print("Converged to success rate: %.2f%%" % (s * 100))
    if s >= 1.0 - delta:
        print("Project was successful.")
    else:
        print("Project was not successful.")
        
if __name__ == "__main__":
    run_project()
