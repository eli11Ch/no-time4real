# No Time4Real
*Logbook of **realtime computing** introduction*

## Purpose
*<In Progress>*
Make a lidar DIY for find obstacle and point the camera at it.
With realtime computing (*C*)

## Materials
- DC motor with encoder;
- IR distance sensor;
- Raspberry pi 3 B+;
- Sd card;
- x2 Servo-motor;
- USB camera.

## Installation and Setup

###  SD card Preparation via [rpi-imager](https://howtoraspberrypi.com/create-sd-card-windows-mac-linux-raspberry-pi-imager/)
For Ubuntu:
```
snap install rpi-imager
```
Once installed, launch `rpi-imager`

- Choose the OS to install (**If you have no preference, choose Recommended**) ;
- Choose the SD card (**It must be insert to your computer before**) ;
- Start writing on the card.

### Installation and Configuration
<*In Progress*>

## Do a Lidar

## Do project (*Minority report*) Camera + servo-motor

### Use of OpenCV (Camera)
[This link](https://pysource.com/2018/10/31/raspberry-pi-3-and-opencv-3-installation-tutorial/) + python3-opencv + libopencv-dev

### Grove base HAT
[Library](https://github.com/Seeed-Studio/grove.py)

### [C++ GPIO](https://www.aranacorp.com/fr/programmez-votre-raspberry-pi-en-c/)

## Related Documentations

- [Infrared Distance Measurement with the Raspberry Pi](https://tutorials-raspberrypi.com/infrared-distance-measurement-with-the-raspberry-pi-sharp-gp2y0a02yk0f/) ;
- [Code Test For Camera](https://stackoverrun.com/fr/q/7383231)
- [Grove Base HAT](https://wiki.seeedstudio.com/Grove_Base_Hat_for_Raspberry_Pi/)


## Problems (?)

- *Grove Base HAT* (**3.3V**) vs *Sharp IR Distance Sensor* + *Servo Moteur* (**5V**) => Seems to be working
