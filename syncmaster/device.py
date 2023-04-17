# AUTOGENERATED! DO NOT EDIT! File to edit: ../nbs/02_initialisation.ipynb.

# %% auto 0
__all__ = ['SyncMaster']

# %% ../nbs/02_initialisation.ipynb 10
'''
    SyncMaster device drivers

    Oxford Neural Interfacing
    Written by Conor Keogh
    conor.keogh@nds.ox.ac.uk
    16/04/2023

    Provides functions for interacting with SyncMaster device
'''

'''
SyncMaster device class
Provide functions to interact with device
Finds device and confirms presence
Sends messages to trigger device
'''
import serial
import serial.tools.list_ports
import time

class SyncMaster:

    def __init__(self):
        '''
        Device object for controlling synchronisation
        '''

        '''
        Define constants for device configuration
        '''
        # Define messages
        self.STARTMARKER = b'1'
        self.ENDMARKER = b'2'
        self.EVENT1 = b'3'
        self.EVENT2 = b'4'
    
        # Define trigger pulse widths (milliseconds)
        self.STARTPULSE = 50
        self.ENDPULSE = 100
        self.EVENT1PULSE = 150
        self.EVENT2PULSE = 200
    
        # Define host acknowledge messages
        self.HOST_MESSAGE = b'5'
        self.ACKNOWLEDGE = 42
    
        # Define COM port settings
        self.BAUDRATE = 115200
        
        # Get all serial ports
        ports = serial.tools.list_ports.comports()

        # For each port: try accessing and checking for acknowledge message
        port_found = False
        for port in ports:
            try:
                # Connect to serial port
                self.ser = serial.Serial(port.device, self.BAUDRATE, timeout=1)

                # Send test message and read response; repeat 3 times and keep third
                for _ in range(3):
                    self.sendMessage(self.HOST_MESSAGE)
                    response = self.ser.readline()

                # Check if response is appropriate
                response = int(response)
                if response == self.ACKNOWLEDGE:
                    self.target_port = port.device
                    port_found = True

                # Close port
                self.ser.close()

            except Exception as e:
                # Do nothing - just ignore failed ports
                pass

        # If port found: connect to port
        if port_found:
            self.ser = serial.Serial(self.target_port, self.BAUDRATE, timeout=5)

        # If port not found: raise error
        else:
            raise Exception("Device not found")
#            print("Device not found")

    ''' Send required messages over serial '''

    def start(self):
        ''' Send start signal '''
        self.sendMessage(self.STARTMARKER)

    def end(self):
        ''' Send end signal '''
        self.sendMessage(self.ENDMARKER)

    def event1(self):
        ''' Send event 1 signal '''
        self.sendMessage(self.EVENT1)

    def event2(self):
        ''' Send event 2 signal '''
        self.sendMessage(self.EVENT2)

    # Send message via serial port
    def sendMessage(self, message):
        '''
        Send message over serial port
        Takes message to send
        '''
        self.ser.write(message)

    # Close channel
    def close(self):
        '''
        Closes device connection
        '''
        self.ser.close()
        
    # Send test signal
    def testSignal(self):
        '''
        Sends test pulses over output port once per second for five seconds
        '''
        for _ in range(5):
            self.start()
            time.sleep(1)
