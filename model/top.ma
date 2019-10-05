[top]
components : sun@sun solar_panel
out : out_port1 out_port2 
in : start stop
link : radiation@sun radiation@solar_panel
link : degree@sun degree@solar_panel
link : rotation_val@solar_panel out_port1
link : rays_val@solar_panel out_port2

[solar_panel]
components : controller@controller
in : radiation degree
out : rotation_val rays_val
link : radiation radiation@controller
link : degree degree@controller
link : rotation_val@controller rotation_val
link : rays_val@controller rays_val
