class PidControl:
    def __init__(self, 
        Kp, 
        Ki, 
        Kd, 
        min_output=0.0, 
        max_output=1.0
    ):
        self.Kp = Kp
        self.Ki = Ki
        self.Kd = Kd

        self.min_output = min_output
        self.max_output = max_output

        self.integral = 0.0
        self.prev_error = 0.0
        self.output = 0.0

        self._first = True

        
    def reset(self):
        self.integral = 0
        self._first = True

    
    def step(self, measured, target, dt):

        error = target - measured

        # Integral
        old_integral = self.integral
        self.integral += error * dt

        # Derivative (simple backward difference)
        if self._first:
            derivative = 0.0
            self._first = False
        else:
            derivative = (error - self.prev_error) / dt

        self.prev_error = error

        # Raw PID output
        u = (
            self.Kp * error +
            self.Ki * self.integral +
            self.Kd * derivative
        )

        # Anti-windup: if saturated, freeze integrator
        if u > self.max_output:
            u = self.max_output
            # prevent further windup
            if error > 0:
                self.integral = old_integral

        elif u < self.min_output:
            u = self.min_output
            # prevent wind-down
            if error < 0:
                self.integral = old_integral

        self.output = u
        return self.output
