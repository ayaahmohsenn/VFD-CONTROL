VFD_ control:

This code is controlling the Variable Frequeny drive(VFD) in the Tiva C microcontroller, using IFOC method (indirect field oriented control). Kp and Ki are contstants
of the PI-controllers all over the code. segma, segma_r, Lm, and Rs are all constants of the motor. this method is based on creating a ficticious current(I_mr), and using
it to measure the flux to measure the rotor current (because in the squirrel cage motors the rotor current is unmeasurable)
this using this to get the d- and q- frames currents to get the md, and mq to control the inverter at the specified frequency. if you want me to clearify anything, please don't
hesitate to contact me.