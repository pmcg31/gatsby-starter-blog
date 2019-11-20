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

### Lib Free or Die

We need three (free!) libraries that are not part of the default Arduino IDE environment. One can be grabbed using the library manager tool in the IDE; we'll need to brave the interwebs for the others.

Easy first...go to the _Tools_ menu in the IDE and choose _Manage Libraries..._ From there, type _json_ into the search bar. A list of matching libraries will be shown, find the one for _ArduinoJson_ by _Beniot Blanchon_. It was the third in my list, but that doesn't mean much. It is the self-proclaimed "most popular Arduino library on GitHub," which probably doesn't mean much either. But it works, so install it and close the manager.

Next is the ESPAsyncWebServer library. It depends on the ESPAsyncTCP library, so let's go round 'em both up on GitHub. Neither makes claims about GitHub popularity, but we'll use them anyway.

For ESPAsyncWebServer, go to:

<blockquote><a href="https://github.com/me-no-dev/ESPAsyncWebServer">https://github.com/me-no-dev/ESPAsyncWebServer</a></blockquote>

Click the _Clone or download_ button and download a zip file. Unzip it and remove _-master_ from the directory that results, leaving you with _ESPAsyncWebServer_. Drop that directory under the _libraries_ directory of your Arduino home directory (<code>~/Documents/Arduino</code> on MacOS).

For ESPAsyncTCP, go to:

<blockquote><a href="https://github.com/me-no-dev/ESPAsyncTCP">https://github.com/me-no-dev/ESPAsyncTCP</a></blockquote>

Same thing again, download a zip, rename the directory and plop it into the libaries directory.

That's it for libraries. Restart the Arduino IDE so it picks up the libraries we added manually.

### Get Your Web On

### Hang Your Shingle
