[top]
components : sun@sun solar_panel
out : sun_degree sun_radiation obtained_energy consumed_energy battery
link : radiation@sun radiation@solar_panel
link : degree@sun degree@solar_panel
link : radiation@sun sun_radiation
link : degree@sun sun_degree
link : obtained_energy@solar_panel obtained_energy
link : consumed_energy@solar_panel consumed_energy
link : battery@solar_panel battery

[solar_panel]
components : controller@controller solar_cell@solarcell engine@engine
in : radiation degree
out : obtained_energy consumed_energy battery

link : radiation radiation@controller
link : degree degree@controller

link : rotation@controller rotation@engine
link : received_energy@controller received_energy@solar_cell
link : battery@controller battery

link : obtained_energy@solar_cell obtained_energy
link : obtained_energy@solar_cell obtained_energy@controller
link : consumed_energy@engine consumed_energy
link : consumed_energy@engine consumed_energy@controller

[solar_cell]
conversion_factor : 0.4

[engine]
consumption_factor : 10

[controller]
tolerance : 1.5
min_energy_to_rotate: 10
