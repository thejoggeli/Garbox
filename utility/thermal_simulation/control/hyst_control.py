class HystControl:
    def __init__(self, 
        hyst_low, 
        hyst_high, 
        output_low = 1.0, 
        output_high = 0.0
    ):
        
        self.hyst_low = hyst_low
        self.hyst_high = hyst_high
        self.output_low = output_low
        self.output_high = output_high
        
        self.output = 0
        self.min_output = min(self.output_low, self.output_high)  
        self.max_output = max(self.output_low, self.output_high)

    def step(self, measured, target, dt):
        low = target + self.hyst_low
        high = target + self.hyst_high

        if measured < low:
            self.output = self.output_low
        elif measured > high:
            self.output = self.output_high

        return self.output
