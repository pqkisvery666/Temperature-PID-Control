# Temperature-PID-control

The project uses the DS18B20 as a temperature sensor. Based on the current temperature and the target temperature, the duty cycle of the PWM wave is calculated using a traditional PID algorithm. The PWM wave is output from port A2, then connected to a buck circuit controlled by the PWM duty cycle. The buck circuit is connected to a thermoelectric cooler (TEC), thereby achieving PID control of the temperature.
Tip: Since the temperature change has a relatively large lag, the derivative coefficient should be set larger.
