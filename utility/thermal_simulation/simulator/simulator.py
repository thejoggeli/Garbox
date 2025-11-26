import plotly.graph_objects as go
from plotly.subplots import make_subplots
import numpy as np
import plotly.express as px
from model.thermal_network import ThermalNetwork

class Simulator:

    def __init__(self, model: ThermalNetwork, sensor_node, heater_node, heater_power, node_color_indices = None):
        self.model = model
        self.sensor_node = sensor_node
        self.heater_node = heater_node
        self.heater_power = heater_power
        self.node_color_indices = node_color_indices if node_color_indices is not None else np.arange(0, len(model.nodes)+1) 

    def simulate(self, control, samples, dt,
        temp_target=30.0,
        temp_ambient=20.0,
        temp_target_list=None,
        temp_ambient_list=None,
        plot_slicing=1
    ):
        
        self.model.reset()

        temp_idxs = np.arange(0, samples, dtype=np.float64)
        temp_time = temp_idxs * dt / 60.0

        num_nodes = len(self.model.nodes)
        temp_nodes = np.ones((num_nodes, samples), dtype=np.float64) * self.model.T_init
        if(temp_target_list is None):
            temp_target_list  = np.ones_like(temp_idxs) * temp_target
        if(temp_ambient_list is None):
            temp_ambient_list = np.ones_like(temp_idxs) * temp_ambient

        output_vals = np.zeros_like(temp_idxs)

        for i in range(samples):

            temp_sensor = self.model.get_temperature(self.sensor_node)
            temp_target = temp_target_list[i]

            # get sensor temperature
            for n in range(num_nodes):
                temp_nodes[n, i] = self.model.get_temperature(n)

            # do control step
            output = control.step(temp_sensor, temp_target, dt)
            output_vals[i] = output * 100.0  # percentage

            # do model step
            self.model.inject_power(self.heater_node, output * self.heater_power)
            self.model.step(dt)

        fig = make_subplots(
            rows=2, cols=1,
            shared_xaxes=True,
            vertical_spacing=0.10,
            subplot_titles=("Duty Cycle", "Temperature"),
            row_heights=[0.25, 0.75]
        )

        # output
        fig.add_trace(go.Scatter(
            x=[0, temp_time[-1]],
            y=[0, 0],
            mode="lines",
            showlegend=False,
            line=dict(color="gray", width=2)))

        fig.add_trace(go.Scatter(
            x=temp_time[::plot_slicing],
            y=output_vals[::plot_slicing],
            mode="lines",
            name="Duty (%)",
            line=dict(color="black", width=2),
            fill="tozeroy", # fills down to y=0
            fillcolor="rgba(0,0,0,0.15)"  # light gray fill
        ), row=1, col=1)

        # temperatures
        fig.add_trace(go.Scatter(
            x=temp_time[::plot_slicing],
            y=temp_target_list[::plot_slicing],
            line=dict(color="black"),
            name="Target Temperature"
        ), row=2, col=1)
        
        for n in range(num_nodes):
            color_default = px.colors.qualitative.Plotly[self.node_color_indices[n%10]]
            color = self.model.meta[n].get("color", color_default)
            visible = self.model.meta[n].get("visible", True)
            fig.add_trace(go.Scatter(
                x=temp_time[::plot_slicing],
                y=temp_nodes[n][::plot_slicing],
                line=dict(color=color),
                visible=visible,
                name=f"{self.model.names[n]} Temperature"
            ), row=2, col=1)

        padding = control.max_output * 10
        fig.update_yaxes(title_text="Duty (%)", row=1, col=1, range=[-padding, control.max_output*100+padding])
        fig.update_yaxes(title_text="Temperature (°C)", row=2, col=1)

        fig.update_xaxes(title_text="Time (min)", row=2, col=1)

        fig.update_layout(
            height=600,
            title="Thermal Model",
            showlegend=True
        )

        fig.show()
