
class ThermalModel_1Node:
    """
    Simple 1-node thermal model for a fermentation chamber.
    
    This model treats the entire interior (air + walls + contents)
    as a single “lumped” thermal mass with heat capacity C.

    Physics behind the model:
        dT/dt = (Q_in - Q_loss) / C

    Where:
        Q_in   = duty * P_heater
        Q_loss = (T - Tambient) / R

    Meaning:
        - Heater adds heat proportional to its duty cycle.
        - Heat is lost to the ambient through thermal resistance R.
        - Temperature is updated using explicit Euler integration.

    Limitations:
        - No internal gradients, instant mixing assumed.
        - No explicit time delay from heater location to sensor.
        - Good for rough behavior, not for spatial effects.
    """

    def __init__(self, 
                 C=500.0,        # heat capacity (J/°C)
                 R=2.0,          # thermal resistance to ambient (°C/W)
                 P_heater=50.0,  # heater max power (W)
                 T_init=20.0):   # initial temperature (°C)

        self.C = C
        self.R = R
        self.P_heater = P_heater
        self.T = float(T_init)

    def step(self, duty: float, ambient_temperature: float, dt: float):
        """
        Advance the model by one timestep and return new temperature.

        Parameters:
          duty (0.0 to 1.0): fraction of heater power
          ambient_temperature (°C)

        Returns:
          Updated internal temperature (°C)
        """

        # Clamp duty for safety
        duty = max(0.0, min(1.0, duty))

        # Heater power in watts
        Q_in = duty * self.P_heater

        # Heat lost to ambient (W)
        # Larger (T - Tambient) → more heat loss
        Q_loss = (self.T - ambient_temperature) / self.R

        # Net energy flow (W)
        Q_net = Q_in - Q_loss

        # Temperature slope (°C/s)
        dTdt = Q_net / self.C

        # Integrate temperature
        self.T += dTdt * dt

        return self.T
    
    def get_heater_temperature(self):
        return self.T

    def get_sensor_temperature(self):
        return self.T
