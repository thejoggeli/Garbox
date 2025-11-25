from control.pid_control import PidControl

class PidHystControl:
    def __init__(self, 
        Kp, 
        Ki, 
        Kd, 
        hyst_low, 
        hyst_high,
        min_output=0.0, 
        max_output=1.0
    ):
        
        self.pid = PidControl(Kp, Ki, Kd, min_output, max_output)

        self.hyst_low  = hyst_low
        self.hyst_high = hyst_high 

        self.min_output = min_output
        self.max_output = max_output

        self.output = 0
    
    def step(self, measured, target, dt):
        
        thresh_low = target + self.hyst_low
        thresh_high = target + self.hyst_high

        if measured < thresh_low:
            self.pid.reset()
            self.output = self.max_output
        elif measured > thresh_high:
            self.pid.reset()
            self.output = self.min_output
        else:
            self.output = self.pid.step(measured, target, dt)

        return self.output
