class Hyst21Control:
    def __init__(self, 
        hyst_low_1, 
        hyst_low_2,
        hyst_high, 
        output_low_1 = 0.0, 
        output_low_2 = 0.0, 
        output_high = 1.0
    ):
        
        self.hyst_low_1 = hyst_low_1
        self.hyst_low_2 = hyst_low_2
        self.hyst_high = hyst_high
        self.output_low_1 = output_low_1
        self.output_low_2 = output_low_2
        self.output_high = output_high
        
        self.output = 0
        self.min_output = min(self.output_low_1, self.output_low_2, self.output_high)  
        self.max_output = max(self.output_low_1, self.output_low_2, self.output_high)

    def step(self, measured, target, dt):
        low_1 = target + self.hyst_low_1
        low_2 = target + self.hyst_low_2
        high = target + self.hyst_high

        if measured < low_1:
            self.output = self.output_low_1
        elif measured < low_2:
            self.output = self.output_low_2
        elif measured > high:
            self.output = self.output_high

        return self.output
