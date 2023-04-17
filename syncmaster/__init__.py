__version__ = "0.0.2"

# Import required libraries
import numpy as np
import serial
import serial.tools.list_ports
import time

# Import main functions
from syncmaster.device import SyncMaster
from syncmaster.analysis import getEvents
