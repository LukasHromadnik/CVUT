#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
import hexapod_sim.vrep_api.vrep as vrep
import math
import time
import numpy as np

class RobotHAL:
    #class for interfacing the hexapod robot in V-REP simulator

    def __init__(self, simulation_step=36):
        self.simulation_step = simulation_step

        self.clientID = self.connect_simulator()
        self.hexapod = None
        self.ref_frame = None
        self.hexa_base = None
        self.collision = None
        self.collision_first = True
        self.laser_init = True

        self.servos = []
        self.foot_tips = []
        
        self.pos_first = True
        self.orientation_first = True
        self.on_startup()

    def __del__(self):
        self.stop_simulation()
        self.disconnect_simulator()

    #############################################################
    # helper functions for simulator interfacing
    #############################################################
    
    def connect_simulator(self):
        """
        Establish connection to the simulator on localhost
        """
        vrep.simxFinish(-1) # just in case, close all opened connections
        IP_address = "127.0.0.1"
        port = 19997 # port on which runs the continuous remote API
        waitUntilConnected = True
        doNotReconnectOnceDisconnected = True
        timeOutInMs = 5000
        commThreadCycleInMs = 20
        new_clientID = vrep.simxStart(IP_address, port, waitUntilConnected, doNotReconnectOnceDisconnected, timeOutInMs, commThreadCycleInMs)
        if new_clientID!=-1:
            print("Connected to remote API server")
        else:
            print("Connection to remote API server failed")
            sys.exit()
        return new_clientID

    def start_simulation(self):
        """
        start the simulation
        """
        errorCode = vrep.simxStartSimulation(self.clientID, vrep.simx_opmode_blocking)
        assert errorCode == vrep.simx_return_ok, "Simulation could not be started"
        return

    def stop_simulation(self):
        """
        stop the simulation
        """
        errorCode = vrep.simxStopSimulation(self.clientID, vrep.simx_opmode_blocking)
        assert errorCode == vrep.simx_return_ok, "Simulation could not be stopped"
        return

    def disconnect_simulator(self):
        """
        disconnect from the simulator
        """
        vrep.simxFinish(self.clientID)
        return

    def get_object_handle(self, string):
        """
        provides object handle for V-REP object
        """
        errorCode, handle = vrep.simxGetObjectHandle(self.clientID, string, vrep.simx_opmode_oneshot_wait)
        assert errorCode == vrep.simx_return_ok, "Conection to " + string + "failed"
        return handle

    def get_collision_handle(self, string):
        """
        provides handle to the collision object in V-REP
        """
        errorCode, handle = vrep.simxGetCollisionHandle(self.clientID, string, vrep.simx_opmode_oneshot_wait)
        assert errorCode == vrep.simx_return_ok, "Getting " + string + " handle failed"
        return handle

    def get_collision_state(self, c_handle):
        """
        getting collision status of object
        
        Parameters
        ----------
        c_handle: int
            vrep simulation object handle

        Returns
        -------
        bool
            True if the object is in collision state, False otherwise
        """
        if self.collision_first:
            errorCode, collisionState=vrep.simxReadCollision(self.clientID, c_handle, vrep.simx_opmode_streaming)
            self.collision_first = False
        else:
            errorCode, collisionState=vrep.simxReadCollision(self.clientID, c_handle, vrep.simx_opmode_buffer)
            
        if errorCode == vrep.simx_return_novalue_flag:
            collisionState = False
        else:
            assert errorCode == vrep.simx_return_ok, "Cannot read collision"
        
        return collisionState

    def get_servos_handles(self):
        """
        retrieve servo handles
        
        Returns
        -------
        list (int)
            list of vrep servos object handles         
        """
        servos=[]
        for i in range(1,19):
            servo = self.get_object_handle("hexa_joint" + str(i))
            servos.append(servo)
        return servos

    def get_hexa_base_handle(self):
        """
        retrieve handle of the robot base
        
        Returns
        -------
        ïnt
            hexapod robot base vrep handle
        """
        hexa_base = self.get_object_handle("hexa_base")
        return hexa_base

    def get_foot_tips_handles(self):
        """
        retrieve handles of the foot tips
        
        Returns
        -------
        list (int)
            list of vrep hexapod foottip handles
        """
        foot_tips = []
        for i in range(1,7):
            foot_tip = self.get_object_handle("hexa_footTip" + str(i))
            foot_tips.append(foot_tip)
        return foot_tips

    def get_hexapod_handle(self):
        """
        retrieve handle for the hexapod object
        
        Returns
        -------
        int
            hexapod robot handle
        """
        hexapod=self.get_object_handle("hexapod")
        return hexapod
        
    def on_startup(self):
        """
        startup routine
        """
        self.hexapod = self.get_hexapod_handle()
        self.servos = self.get_servos_handles()
        self.hexa_base = self.get_hexa_base_handle()
        self.collision = self.get_collision_handle('hexa_c')

        self.foot_tips = self.get_foot_tips_handles()
        
        #enable synchronous operation of the vrep
        #self.enable_sync()
        #self.set_sim_step(self.simulation_step)

        #start the simulation
        self.start_simulation()
        print("Robot ready")
        return

    #############################################################
    # locomotion helper functions
    #############################################################

    def get_servo_position(self, servoID):
        """
        getting position of a single servo
        
        Parameters
        ----------
        servoID: int
            ID of the servo to be read
        
        Returns
        -------
        float
            current joint angle
        """
        assert servoID > 0 and servoID <= 18, "Commanding unexisting servo"
        errorCode, value = vrep.simxGetJointPosition(self.clientID, self.servos[servoID-1], vrep.simx_opmode_blocking)
        assert errorCode == vrep.simx_return_ok, "Failed to read servo position"
        #self.trigger()
        #self.ping_time()
        time.sleep(self.simulation_step/1000.0)
        return value

    def get_all_servo_position(self):
        """
        getting position of all servos

        Returns
        -------
        list (float)
            joint angles of all servos
        """
        angles = np.zeros(18)
        for i in range(0,18):
            angles[i] = self.get_servo_position(i+1)
        return angles

    def set_servo_position(self, servoID, angle):
        """
        setting position of a single servo

        Parameters
        ----------
        servoID: int
            ID of the currently commanded servo

        """
        assert servoID > 0 and servoID <= 18, "Commanding unexisting servo"
        errorCode = vrep.simxSetJointTargetPosition(self.clientID, self.servos[servoID-1], angle, vrep.simx_opmode_streaming)
        assert errorCode == vrep.simx_return_ok or errorCode == vrep.simx_return_novalue_flag, "Failed to set servo position"
        #self.trigger()
        #self.ping_time()
        time.sleep(self.simulation_step/1000.0)

    def set_all_servo_position(self, angles):
        #setting position of servos
        assert len(angles) == 18, "wrong number of operated servos"
        for i in range(0,len(angles)):
            errorCode = vrep.simxSetJointTargetPosition(self.clientID, self.servos[i], angles[i], vrep.simx_opmode_streaming)
            assert errorCode == vrep.simx_return_ok or errorCode == vrep.simx_return_novalue_flag, "Failed to set servo position"
        #self.trigger()
        #self.ping_time()
        time.sleep(self.simulation_step/1000.0)

    def get_robot_position(self):
        #get the position of the robot
        if self.pos_first:
            self.pos_first = False
            errorCode, position = vrep.simxGetObjectPosition(self.clientID, self.hexapod, -1, vrep.simx_opmode_streaming) #start streaming
        else:
            errorCode, position = vrep.simxGetObjectPosition(self.clientID, self.hexapod, -1, vrep.simx_opmode_buffer) #fetch new data from stream
        
        if errorCode == vrep.simx_return_novalue_flag:
            position = None
        else:
            assert errorCode == vrep.simx_return_ok, "Cannot get object position" 
       
        if not position == None:
            #extract only 2D position
            position = position[0:2]

        return position

    def get_robot_orientation(self): 
        #get the orientation of the robot
        if self.orientation_first:
            self.orientation_first = False
            errorCode, orientation = vrep.simxGetObjectOrientation(self.clientID, self.hexapod, -1, vrep.simx_opmode_streaming) #start streaming
        else:
            errorCode, orientation = vrep.simxGetObjectOrientation(self.clientID, self.hexapod, -1, vrep.simx_opmode_buffer) #fetch new data from stream
        
        if errorCode == vrep.simx_return_novalue_flag:
            return None
        else:
            assert errorCode == vrep.simx_return_ok, "Cannot get object orientation"
        
        #extract orientation from Euler angles to one angle between 0 and 2pi
        phi = 0
        if orientation[1] < 0:
            if orientation[2] < 0:
                phi = math.pi/2 - orientation[1] 
            else:
                phi = 3*math.pi/2 + orientation[1]
        else:
            if orientation[2] < 0:
                phi = math.pi/2 - orientation[1]
            else:
                phi = 3*math.pi/2 + orientation[1]
        
        return phi

    def get_robot_collision(self):
        #return if any part of the body is in collision with objects
        state = self.get_collision_state(self.collision)
        return state
    
    def get_joint_force(self, servoID):
            errorCode, force = vrep.simxGetJointForce(self.clientID, self.servos[servoID-1], vrep.simx_opmode_blocking)
            return force

    #############################################################
    # laser scanner sensor interface
    #############################################################

    def get_laser_scan(self):
            ret = None
            #fetch the data from the laser scanner
            if self.laser_init:
                    errorCode, signalVal = vrep.simxGetStringSignal(self.clientID, "MySignal", vrep.simx_opmode_streaming) #start streaming
            else:
                    errorCode, signalVal = vrep.simxGetStringSignal(self.clientID, "MySignal", vrep.simx_opmode_buffer) #fetch new data from stream
            
            if errorCode == vrep.simx_return_novalue_flag:
                    ret = None
            else:
                    assert errorCode == vrep.simx_return_ok, "Cannot grab laser data"
            #clear the message queue
            vrep.simxClearStringSignal(self.clientID, "MySignal", vrep.simx_opmode_oneshot)
            
            data = vrep.simxUnpackFloats(signalVal)
            
            scan_x = data[0::2]
            scan_y = data[1::2]
            if len(scan_x) != len(scan_y):
                    ret = None
            else:
                    ret = (scan_x, scan_y)
            return ret

    #############################################################
    # synchronous simulation
    #############################################################

    def enable_sync(self):
            # enable synchronous mode
            errorCode = vrep.simxSynchronous(self.clientID, True)
            assert errorCode == vrep.simx_return_ok, "Cannot enable synchronous mode"
            self.sync = True
            
    def disable_sync(self):
            errorCode = vrep.simxSynchronous(self.clientID, False)
            assert errorCode == vrep.simx_return_ok, "Cannot disable synchronous mode"
            self.sync = False
            
    def trigger(self):
            # trigger simulation step
            errorCode = vrep.simxSynchronousTrigger(self.clientID)
            assert errorCode == vrep.simx_return_ok, "Cannot trigger simulation"

    def ping_time(self):
            # get ping time
            # after this function returns, values are up-to-date
            errorCode, pingTime = vrep.simxGetPingTime(self.clientID)
            assert errorCode == vrep.simx_return_ok, "Problem occured during simulation step"
            return pingTime

    def set_sim_step(self, step_time):
            # set simulation step time in seconds
            errorCode = vrep.simxSetFloatingParameter(self.clientID, vrep.sim_floatparam_simulation_time_step, step_time, vrep.simx_opmode_oneshot)
            #assert errorCode == vrep.simx_return_ok, "Could not set simulation step time.'"
            return errorCode

    #############################################################
    # call script functions
    #############################################################
    def execute_code(self, code):
            emptyBuff = bytearray()
            res,retInts,retFloats,retStrings,retBuffer=vrep.simxCallScriptFunction(self.clientID,"hexa_base",vrep.sim_scripttype_childscript,'executeCode_function',[],[],[code],emptyBuff,vrep.simx_opmode_blocking)
            assert res == vrep.simx_return_ok, "Remote function call failed"
            return retStrings[0]

################################################
# Main function for testing
################################################
if __name__ == "__main__":
    robot_hal = RobotHAL()
    time.sleep(2)
