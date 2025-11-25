import plotly.graph_objects as go
from plotly.subplots import make_subplots
import numpy as np
import plotly.express as px
from model.thermal_network import ThermalNetwork

def simulate(
    model: ThermalNetwork,
    sensor_node,
    heater_node,
    heater_power,
    control,
    samples,
    dt,
    plot_heater=True,
    temp_target=30.0,
    temp_ambient=20.0,
    temp_target_list=None,
    temp_ambient_list=None
):
    
    model.reset()

    temp_idxs = np.arange(0, samples, dtype=np.float64)
    temp_time = temp_idxs * dt / 60.0

    temp_sensor = np.ones_like(temp_idxs) * model.get_temperature(sensor_node)
    temp_heater  = np.ones_like(temp_idxs) * model.get_temperature(sensor_node)
    if(temp_target_list is None):
        temp_target_list  = np.ones_like(temp_idxs) * temp_target
    if(temp_ambient_list is None):
        temp_ambient_list = np.ones_like(temp_idxs) * temp_ambient

    output_vals = np.zeros_like(temp_idxs)

    for i in range(samples):

        # get sensor temperature
        temp_sensor[i] = model.get_temperature(sensor_node)
        temp_heater[i] = model.get_temperature(heater_node)

        # do control step
        output = control.step(temp_sensor[i], temp_target_list[i], dt)
        output_vals[i] = output * 100.0  # percentage

        # do model step
        model.inject_power(heater_node, output * heater_power)
        model.step(dt)

    fig = make_subplots(
        rows=2, cols=1,
        shared_xaxes=True,
        vertical_spacing=0.10,
        subplot_titles=("output Cycle", "Temperature"),
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
        x=temp_time,
        y=output_vals,
        mode="lines",
        name="Duty (%)",
        line=dict(color="black", width=2),
        fill="tozeroy", # fills down to y=0
        fillcolor="rgba(0,0,0,0.15)"  # light gray fill
    ), row=1, col=1)

    # temperatures
    fig.add_trace(go.Scatter(
        x=temp_time,
        y=temp_target_list,
        line=dict(color=px.colors.qualitative.Plotly[2]),
        name="Target Temperature"),
        row=2, col=1)

    fig.add_trace(go.Scatter(
        x=temp_time,
        y=temp_ambient_list,
        line=dict(color=px.colors.qualitative.Plotly[0]),
        name="Ambient Temperature"),
        row=2, col=1)

    if plot_heater:
        fig.add_trace(go.Scatter(
            x=temp_time,
            y=temp_heater,
            line=dict(color=px.colors.qualitative.Plotly[1]),
            name="Heater Temperature"),
            row=2, col=1)

    fig.add_trace(go.Scatter(
        x=temp_time,
        y=temp_sensor,
        line=dict(color=px.colors.qualitative.Plotly[3]),
        name="Sensor Temperature"),
        row=2, col=1)

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
