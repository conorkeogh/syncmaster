# AUTOGENERATED! DO NOT EDIT! File to edit: ../nbs/04_events.ipynb.

# %% auto 0
__all__ = ['getEvents']

# %% ../nbs/04_events.ipynb 4
# Process data
def getEvents(data, samplerate):
    '''
    Recover event timings
    Args:
        data: 1d array containing event signals recorded at each timepoint
        samplerate: samplerate of recording (Hz)
    Returns:
        events: 4 x t array with event onset IDs; 1 at timepoint where event starts
    '''
    
    # Define pulse lengths (ms)
    STARTPULSE = 50
    ENDPULSE = 100
    EVENT1PULSE = 150
    EVENT2PULSE = 200
    
    # Create empty array - 4 x t
    events = np.zeros((4, len(data)))
    
    # Define threshold - 0.75x max value (digital signal)
    threshold = 0.75 * np.max(data)
    
    # Define cutoffs - number of samples, set at midpoint between event types
    event2_cutoff = (175 / 1000) * samplerate
    event1_cutoff = (125 / 1000) * samplerate
    end_cutoff = (75 / 1000) * samplerate
    start_cutoff = (25 / 1000) * samplerate
    
    # Loop over points in data array
    idx = 0
    while idx < len(data):
        # Get current datapoint
        d = data[idx]
        
        # If point > threshold: find next point below threshold
        if d > threshold:
            # Reset state
            window = 1
            end_found = False
            
            # Continue until value drops below threshold or data ends
            while(idx + window <= len(data) and end_found == False):
                # Check if dropped below threshold
                if data[idx + window] < threshold:
                    end_found = True
                
                window += 1
                
            # If end found: assign to event type
            if end_found:
                if window > event2_cutoff:
                    events[3, idx] = 1
                elif window > event1_cutoff:
                    events[2, idx] = 1
                elif window > end_cutoff:
                    events[1, idx] = 1
                elif window > start_cutoff:
                    events[0, idx] = 1
                    
                # Move current point to end of window
                idx = idx + window
            
        # Move to next datapoint
        idx += 1
        
    # Return events list
    return events
