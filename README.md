# Air Quality
An embedded air quality monitoring application in C++
## Goals for this project
This project is intended to become an embedded application running an air quality sensor on the Raspberry Pi Pico, transmitting data which will be received by a Raspberry Pi 4. I will be writing a UDP listener on the Raspberry Pi which will receive the air quality data. The Pico will sense the data and transmit it to a NodeMCU, which will transmit the data over LAN. Both these devices will run in C. The Raspberry Pi server will then make the data available over HTTP on the LAN to be inspected through a browser.
Until all this is implemented, the repository will simply be a place for me to experiment in C++ networking.
