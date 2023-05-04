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
        self.GREETING = b'<best wishes>'
        self.RESPONSE = 'warmest regards'
        self.startMarker = '<'
        self.endMarker = '>'
    
        # Define trigger pulse widths (val*10, milliseconds)
        self.STARTPULSE = 5 # 50ms
        self.ENDPULSE = 10 # 100ms
        self.EVENT1PULSE = 15 # 150ms
        self.EVENT2PULSE = 20 # 200ms
    
        # Define COM port settings
        self.BAUDRATE = 115200
        
        # Get all serial ports
        ports = serial.tools.list_ports.comports()

        # For each port: try accessing and checking for acknowledge message
        port_found = False
        for port in ports:
            try:
                # Connect to serial port
                self.ser = serial.Serial(port.device, self.BAUDRATE, timeout=1, write_timeout=1)

                # Send test message and read response; repeat 3 times and keep third
                for _ in range(3):
                    self.ser.write(self.GREETING)
                    response = self.ser.readline()

                # Check if response is appropriate
                if response.decode().strip() == self.RESPONSE:
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
            #print("Device not found")

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
        
    def event(self, eventID):
        ''' Create event marker '''
        self.sendMessage(eventID)

    # Send message via serial port
    def sendMessage(self, message):
        '''
        Send message over serial port
        Takes message to send
        '''
        message_prepared = self.startMarker + str(message) + self.endMarker
        self.ser.write(message_prepared.encode())

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
