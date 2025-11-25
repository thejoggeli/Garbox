import numpy as np

class ThermalModelSequential:
    """
    General sequential N-node 1D thermal RC network.

    Topology:
        [Heater] - R₀ - [Node 1] - R₁ - [Node 2] - R₂ - … - [Node N-1] - Rₙ - Ambient

    Meaning:
        - Each node has its own heat capacity: C[i]
        - Each R[i] is the thermal resistance between node i and node i+1
        - R[N-1] is the resistance from the last node to ambient
        - Heater power is injected into node 0
        - Temperatures are integrated forward in time using explicit Euler
    """

    def __init__(self, C, R, sensor_node, names=None, P_heater=50.0, T_init=20.0):
        self.C = np.array(C, dtype=float)
        self.R = np.array(R, dtype=float)
        self.P = float(P_heater)
        self.T = np.ones(len(C), dtype=float) * float(T_init)
        self.sensor_node = sensor_node

        if names is None:
            self.names = [f"node{i}" for i in range(len(C))]
        else:
            if len(names) != len(C):
                raise ValueError("names must have same length as C")
            self.names = list(names)

        self.name_to_index = {name: i for i, name in enumerate(self.names)}

    def step(self, duty, ambient_temperature, dt):
        """
        Advance the N-node system by one timestep.
        
        The update uses explicit Euler integration:
            T_new = T_old + (Q_in / C) * dt

        Each node exchanges heat with:
            - its left neighbor (i-1)
            - its right neighbor (i+1), or ambient if it is the last node
        """
        duty = np.clip(duty, 0.0, 1.0)

        N = len(self.T)
        T_new = self.T.copy()

        # Heater injects heat into node 0
        Q_in = duty * self.P

        for i in range(N):
            Q = 0.0

            # Heater => node 0
            if i == 0:
                Q += Q_in

            # Heat flow from left neighbor (i-1 => i)
            if i > 0:
                Q += (self.T[i-1] - self.T[i]) / self.R[i-1]

            # Heat flow from right neighbor (i+1 => i)
            # or from ambient if this is the last node
            if i < N - 1:
                Q += (self.T[i+1] - self.T[i]) / self.R[i]
            else:
                # Last node exchanges heat with ambient
                Q += (ambient_temperature - self.T[i]) / self.R[i]

            # Temperature update for node i
            T_new[i] += (Q / self.C[i]) * dt

        # Commit all updates simultaneously
        self.T[:] = T_new
        return self.T[0]

    def get_temperature(self, which):
        if isinstance(which, str):
            return self.T[self.name_to_index[which]]
        return self.T[int(which)]

    def get_heater_temperature(self):
        return self.get_temperature(0)
    
    def get_sensor_temperature(self):
        return self.get_temperature(self.sensor_node)