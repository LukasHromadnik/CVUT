import numpy as np
import math
import matplotlib.pyplot as plt

class Planner:
   
    def __init__(self, limits):
        """
        Parameters
        ----------
        limits: list((float, float))
            translation limits in individual axes 
        """
        self.limits = limits
        self.setup()
    
    def setup(self):
        pass

    def max_translation(self):
        x_lower = self.limits[0][0]
        x_upper = self.limits[0][1]
        y_lower = self.limits[1][0]
        y_upper = self.limits[1][1]
        z_lower = self.limits[2][0]
        z_upper = self.limits[2][1]

        ratio_limit = 1/250.0

        return ratio_limit * np.max([
            x_upper - x_lower,
            y_upper - y_lower,
            z_upper - z_lower
        ])
    
    def max_rotation(self):
        return np.pi / 6
    
    def construct_pose_matrix(self, state):
        """
        Constructs SE(3) matrix from given state.

        Parameters
        ----------
        state: numpy array (6x1)
            configuration of the robot (x, y, z, phi_x, phi_y, phi_z)
        
        Returns
        -------
        numpy array (4x4)
            pose matrix in SE(3) coordinates
        """

        R = self.rotation_matrix(state[5], state[4], state[3])
        T = state[0:3]

        pose = np.hstack((R, T.reshape((3, 1))))
        pose = np.vstack((pose, [0, 0, 0, 1]))

        return pose
    
    def rotation_matrix(self, yaw, pitch, roll):
        """
        Constructs rotation matrix given the euler angles
        yaw = rotation around z axis
        pitch = rotation around y axis
        roll = rotation around x axis

        Parameters
        ----------
        yaw: float
        pitch: float
        roll: float
            respective euler angles
        """
        R_x = np.array([[1, 0, 0],
                        [0, math.cos(roll), math.sin(roll)],
                        [0, -math.sin(roll), math.cos(roll)]])
        R_y = np.array([[math.cos(pitch), 0, -math.sin(pitch)],
                        [0, 1, 0],
                        [math.sin(pitch), 0, math.cos(pitch)]])
        R_z = np.array([[math.cos(yaw), math.sin(yaw), 0],
                        [-math.sin(yaw), math.cos(yaw), 0],
                        [0, 0, 1]])

        R = R_x.dot(R_y).dot(R_z)

        return R

    def plot_nodes(self, samples, pause=None, color='r'):
        xs = [samples[i][0] for i in range(len(samples))]
        ys = [samples[i][1] for i in range(len(samples))]
        zs = [samples[i][2] for i in range(len(samples))]
        plt.plot(xs, ys, zs, color + '.')
        plt.show()
        if pause != None:
            plt.pause(pause)
    
    def generate_samples(self, n_samples, size):
        # Random sampling from uniform distribution between 0 and 1
        samples = np.random.rand(size, n_samples)
    
        # Scale and shift the samples
        i = 0
        for limit in self.limits:
            scale = limit[1] - limit[0]
            samples[i, :] = samples[i, :] * scale + limit[0]
            i += 1
        
        return samples.T
    
    def compute_difference_vector(self, start, goal):
        start_coord = start[0:3]
        goal_coord = goal[0:3]
        direction = goal_coord - start_coord
        distance = np.linalg.norm(direction)
        n_samples = int(np.ceil(distance / self.max_translation()))
        
        return (n_samples, distance, direction)
    
    def compute_path_between(self, start, goal):
        n_translation, distance, direction = self.compute_difference_vector(start[0:3], goal[0:3])
        n_rotation, _, rotation = self.compute_difference_vector(start[3:6], goal[3:6])
        
        n_samples = max(n_translation, n_rotation)

        path = []
        if n_samples == 1:
            sample = np.array(start) + np.multiply(list(direction) + list(rotation), 1)
            path.append(sample)
        else:
            for i in range(1, n_samples):
                ratio = i / n_samples
                sample = np.array(start) + np.multiply(list(direction) + list(rotation), ratio)
                path.append(sample)

        return (path, distance)
    
    def is_path_valid(self, path, environment):
        for node in path:
            pose = self.construct_pose_matrix(node)
            if environment.check_robot_collision(pose):
                return False
        
        return True
    
    def reconstruct_path(self, nodes, predecessors, end_index):
        pred = end_index
        path = []

        if predecessors[pred] == -1:
            return path

        while pred != 0:
            computed_path, _ = self.compute_path_between(nodes[predecessors[pred]], nodes[pred])
            pred = predecessors[pred]
            path = computed_path + path
        
        return path
