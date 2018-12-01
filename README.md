# PU_BIKE_CARFRAME
The firmware of PU_BIKE carframe.

## Introduce 
This is just a part of PU_Bike, the main purpose is a note for recording code sections of the firmware when I worked on the design of the 
carframe. Fetching informations from Database is main function, it can analysis by the information of database table and provide the 
feature of reservation, renting the car and returning the car, besides, there is a led light on the device, then user can read the information 
from specific color of the light. Additionally, we provice UI interface on our APP, it can allow user to enroll the system, login the system, 
store cash into the system, billing the payment and renting the car, our system server is on Raspberry pi worked on raspbian system, use 
mysql as main system for database. The carframe contain a development board named Wemos d1 based on ESP8266EX chip, a micro switch, a servo
motor, relay module, led light module, and power supply module. Finally, the lock on the carframe is made by 3D Printer.

## The main structure of this project
<img src="https://github.com/GuanYu914/PU_BIKE_CARFRAME/blob/master/photos/PU_Bike_structure.JPG" width="50%" height="50%">

## The exterior appearance of carframe

### The main circuit 
<img src="https://github.com/GuanYu914/PU_BIKE_CARFRAME/blob/master/photos/PU_Bike_circuit.JPG" width="50%" height="50%">

### The main state 
STATE "RESERVATION"<br>
<img src="https://github.com/GuanYu914/PU_BIKE_CARFRAME/blob/master/photos/PU_Bike_Reservation.JPG" width="50%" height="50%">

STATE "RETURN"<br>
<img src="https://github.com/GuanYu914/PU_BIKE_CARFRAME/blob/master/photos/PU_Bike_Return.JPG" width="50%" height="50%">

STATE "RENT"<br>
<img src="https://github.com/GuanYu914/PU_BIKE_CARFRAME/blob/master/photos/PU_Bike_Rent.JPG" width="50%" height="50%">

STATE "WAITING"<br>
<img src="https://github.com/GuanYu914/PU_BIKE_CARFRAME/blob/master/photos/PU_Bike_Waiting.JPG" width="50%" height="50%">

### The lock of carframe
STATE "LOCK"<br>
<img src="https://github.com/GuanYu914/PU_BIKE_CARFRAME/blob/master/photos/PU_Bike_Lock.JPG" width="50%" height="50%">

STATER "UNLOCK"<br>
<img src="https://github.com/GuanYu914/PU_BIKE_CARFRAME/blob/master/photos/PU_Bike_Unlock.JPG" width="50%" height="50%">

## The video for demonstration
[![http://img.youtube.com/vi/U7sT7Vtu6R8/0.jpg](http://img.youtube.com/vi/U7sT7Vtu6R8/0.jpg)](http://www.youtube.com/watch?v=U7sT7Vtu6R8 "PU_Bike ")
