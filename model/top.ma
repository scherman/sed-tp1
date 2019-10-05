[top]
components : sun@sun solar_panel
out : out_port1 out_port2 
in : start stop
link : radiation@sun radiation@solar_panel
link : degree@sun degree@solar_panel
link : obtained_energy@solar_panel out_port1
link : rays_val@solar_panel out_port2

[solar_panel]
components : controller@controller solar_cell@solarcell
in : radiation degree
out : obtained_energy rays_val rotation_val

link : radiation radiation@controller
link : degree degree@controller

link : rotation_val@controller rotation_val
link : rays_val@controller rays_val@solar_cell

link : obtained_energy@solar_cell obtained_energy
