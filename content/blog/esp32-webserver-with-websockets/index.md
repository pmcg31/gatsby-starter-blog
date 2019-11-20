---
title: ESP32 Web Server with Web Sockets
date: "2019-11-20"
description: "Guide to setting up AsyncWebServer on ESP32 including web sockets. SPIFFS, mDNS and JSON are used along the way."
---

The ESP32 combined with AsyncWebServer and AsyncWebSocket create a powerful framework for applications that wish to display collected sensor data in real time on a web page. Of course this isn't limited to just sensor data.

This guide sets up a web server which reports the ADC values coming from GPIO 36. A potentiometer is used to provide a changing input. The page sports a range slider that controls how many measurements are averaged together to smooth out the reported value (mine was quite jumpy).

This arrangement creates a small, concise example that illustrates two-way communications with web sockets. It shows how to control settings on the server (the number of measurements averaged) from the client and how to display a changing data (the potentiometer value) on a web page in real time.

The project uses AsyncWebServer to serve up a static page stored in the ESP32's SPIFFS filesystem. On load, the page connects back to the server using a web socket. Data is exchanged in both directions over the web socket using JSON. Last, but not least, mDNS is used to allow the web server to be found on the local network at <code>http://esp32.local</code>.

Almost forgot! There is a JSON config file also stored in SPIFFS. This is read to get the network SSID to connect to and its key. Which also means this project connects to an existing WiFi network rather than creating its own.

To follow everything in this guide you will need an ESP32 development board and a potentiometer. I used a NodeMCU-32s and 100K pot, but any board and pot should be fine (it's on you to use ohm's law to make sure your pot doesn't draw too much current!). You will also need an internet connection to download libraries, and a WiFi network to connect to.