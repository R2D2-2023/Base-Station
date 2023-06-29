# Base-Station

We are the team base-station.

> Our focus is on the charging of the robot and also maintaining the robot, by charging it and maybe updating it in times.
>  - Charging will be done with a contact pad and an arm that moves these pads to connect to the robot.
>  - Updating will be done by the same action, the pads all have a different function
>        The functions are;
>      - Charging, by using a ground and power pad.
>      - Data transfer.
>      - Powering on the Raspberry Pi for data transfers.

To see the [scrumboard](https://github.com/orgs/R2D2-2023/projects/4) of our team

To run the program for the charging pi navigate to the directory and enter the following line in a terminal: 
g++ main.cpp HX711.cpp HX711.hpp -lwiringPi -o charge_pi 

Then to run the program enter:
sudo ./charge_pi
