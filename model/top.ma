[top]
components : sun@sun solar_panel
out : out_port1 out_port2 
in : start stop
link : radiation@sun radiation@solar_panel
link : degree@sun degree@solar_panel
link : obtained_energy@solar_panel out_port1
link : consumed_energy@solar_panel out_port2

[solar_panel]
components : controller@controller solar_cell@solarcell engine@engine
in : radiation degree
out : obtained_energy consumed_energy

link : radiation radiation@controller
link : degree degree@controller

link : rotation_val@controller rotation_val@engine
link : rays_val@controller rays_val@solar_cell

link : obtained_energy@solar_cell obtained_energy
link : consumed_energy@engine consumed_energy


[solar_cell]
conversion_factor : 0.7


[engine]
consumption_factor : 0.2