# Smart-AC-System
A smart Air Conditioner system able to adjust the power based on the enviornment's temperature and air pressure.

The project is an intelligient Air Conditioner adjuster using Arduino as its hardward base.
It makes the AC adjust the power of its motors according to the environment's temperature and atmosphere pressure.

Sensor types:
Temperature ----------- DS18B20
Atmosphere pressure --- analog sensor
Output types:
Digital Potentiometer-- X9C102P, X9C104P
Capacitive Touchable Screen - RA8875(800 x 480) from Buydisplay.com
State Machine fields:
STATE: STDB --- to turn off the power
       AUTO --- to run automatically
       MANM --- to allow manual adjust of SP and DP3
       VENT --- to allow manual adjust of DP3
       TEST --- to allow manual adjust of all sensors and outputs, meanwhile suspend the auto update function
SP: a set_point as the formula's coefficient

Display System
One button for each state. Press the button to swtitch to the state.
Several groups of toggles for certain states. One group of toggles show only when the state they belong to is on.
Input Field at the left bottom corner keeps showing the update sensor value
Warning Field at the right bottom corner shows the abnormal evaluation only when there is such a condition.
It saves the data to SD card every second.

TODOs:
1.More OOP about the STATE system?

P.S. It takes two months and 6 versions to get the project to this stage, althoug I know the code still looks redundant.
Please don't hesitate to let me know how I could optimize it. Thank you!
