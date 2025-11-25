import numpy as np

class ThermalNetwork:
    """
    General thermal RC network with arbitrary connections.

    - Each node has: heat capacity C and temperature T
    - Each link has a thermal resistance R between two nodes
    - Heat flows both ways according to (T1 - T2) / R
    - Any node can receive heater power input
    - Ambient can be modeled as a fixed-temperature node
    """

    def __init__(self, T_init=20.0):
        self.T_init = float(T_init)

        self.nodes = {} # name -> index
        self.C = []     # capacity for each node
        self.T = []     # temperature for each node
        self.edges = [] # (i, j, R)
        self.power = {} # node_index -> injected power (W)

        # ambient node is optional; created when used
        self.ambient_index = None
        self.ambient_temperature = None

    def reset(self):
        for i in range(len(self.T)):
            self.T[i] = self.T_init
        if(self.ambient_index is not None):
            self.T[self.ambient_index] = self.ambient_temperature
        self.power = {}

    def add_node(self, name, C):
        if name in self.nodes:
            raise ValueError(f"Node {name} already exists")

        idx = len(self.C)
        self.nodes[name] = idx
        self.C.append(float(C))
        self.T.append(self.T_init)

        return idx

    def add_link(self, name1, name2, R):
        if name1 not in self.nodes or name2 not in self.nodes:
            raise ValueError("Both nodes must exist before linking")

        i = self.nodes[name1]
        j = self.nodes[name2]
        self.edges.append((i, j, float(R)))

    def inject_power(self, name, P):
        if name not in self.nodes:
            raise ValueError(f"Node {name} does not exist")
        self.power[self.nodes[name]] = float(P)

    def add_ambient_node(self, name, temperature):
        """
        Creates or identifies an ambient node with fixed temperature.
        """
        if name not in self.nodes:
            self.add_node(C=1.0, name=name) # C for ambient doesn't matter
        self.ambient_index = self.nodes[name]
        self.set_ambient_temperature(temperature)

    def set_ambient_temperature(self, temperature):
        if(self.ambient_index is None):
            raise KeyError("ambient node must be added before setting ambient temperature")
        self.T[self.ambient_index] = temperature
        self.ambient_temperature = temperature

    def step(self, dt):
        """
        Update all node temperatures by dt seconds.
        Ambient node (if set) is clamped to ambient_temperature.
        """

        N = len(self.C)
        T_new = np.array(self.T, dtype=float)

        # apply heater power
        Q_in = np.zeros(N)
        for idx, power in self.power.items():
            Q_in[idx] += power

        # compute heat flows through every link
        Q = np.zeros(N)

        for i, j, R in self.edges:
            dT = (self.T[i] - self.T[j]) / R

            # heat leaving i and entering j
            Q[i] -= dT
            Q[j] += dT

        # update temperatures
        for i in range(N):
            T_new[i] += (Q[i] + Q_in[i]) / self.C[i] * dt

        # Ambient node override (fixed temperature)
        if self.ambient_index is not None:
            T_new[self.ambient_index] = self.ambient_temperature

        self.T = T_new


    def get_temperature(self, name):
        return self.T[self.nodes[name]]
