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

### Base Sketch

To get the basic sketch up and running, connect the potentiometer wiper to GPIO 36. Connect the other two legs to 3.3V and GND pins, respectively. Start a new sketch in the IDE, and load the following code. If you need to use a different GPIO pin, change <code>POT_PIN</code> to the pin you're using. 

<blockquote><a href="code_v1.ino">First code version</a></blockquote>

Upload the sketch, play with the pot and watch the pretty numbers on your serial monitor. When you get bored, move on to adding the web server.

### Get Your Web On

OK, now you have the pot hooked up and tested. Let's get this web server happening.

Here's the new code (edits required!):

<blockquote><a href="code_v2.ino">Second code version</a></blockquote>

We've added some includes for WiFi and the web server. Then a new variable for the web server object and a handler function.

Further down you'll find two new variables in <code>setup</code> called <code>ssid</code> and <code>key</code>. You will need to edit these variables for your WiFi network.

After that comes the call to <code>WiFi.begin</code>. If you entered the network credentials properly, it will connect to WiFi fairly quickly and print its IP address to the serial monitor. If not, it will loop endlessly, dutifully reporting its futile attempt to connect once a second until the end of time.

Finally, we hook up that handler function to be called whenever someone asks for the root page and start the server.

Watch your serial monitor, then enter the IP address it prints into a browser. If all is well you will be rewarded with the text "Hello from ESP32!"

Yep, life is pretty good. Except for that whole waiting to see what the IP address is in the serial monitor. Tedious. Clicking through windows and typing...it's so much work. Is there a better way?

### Hang Your Shingle

Wouldn't it be great if we could skip all that IP ugliness and just refer to our little board by name? It would be great, and it's called mDNS. The DNS stands for DNS, and the m stands for multicast. DNS is domain name system, of course.

Generally speaking, getting your server into a DNS requires an afternoon of configuration editing and google searches (and profanity). But that's like moving to a new neighborhood and waiting months for a new telephone book to be published. mDNS is more like showing up to a party with a sticker on your shirt that says "Hello, my name is..."

Let's add a name tag to our little server. Here's the code (don't forget to edit the ssid and key!):

<blockquote><a href="code_v3.ino">Third code version</a></blockquote>

Not much to this change. A new include file. In setup, a new line to start mDNS and some logging. After we start the web server, we add that to the list of things we're advertising via mDNS.

Now upload the code and don't even look at the serial monitor. Ok, I know you're going to check anyway. You can simply type "esp32.local" into your browser now, and it will find your board. Don't like the name esp32? No problem, set it to whatever you want in <code>mDNS.begin()</code>. You're stuck with the ".local" part though.

### Time to SPIFFS things up

I know what you're saying. No, besides the part about being stuck with ".local"...you're saying, fine, I don't have to wait for that silly IP address anymore, but why do I need to keep editing this code with my network creds??

It's a hard knock life.

But it doesn't have to be. Let's move our network stuff to a file. You'll still have to edit that file one more time. I can feel your eyes rolling...

Here's the new code (no edits required! HA!):

<blockquote><a href="code_v4.ino">Fourth code version</a></blockquote>

Once again, we've got some more includes. In addition to the SPIFFS header (which needs FS), I'm sneaking the JSON header in here, too. That's because our new config file is going to be in JSON format. 

The JSON lib will make loading and parsing our config file a snap, and that's exactly what happens next. The variables <code>ssid</code> and <code>key</code> are still here, but you don't have to edit them. Instead, the SPIFFS code is fired up, we open the config file and pass it to the <code>deserializeJson</code> function.

Finally, we "edit" the ssid and key by asking the JSON document for their values. From there things proceed as they have been all along.

You will need to add a directory called "data" next to your sketch. It has to be called "data." I don't care that you don't like it. It doesn't work unless it's called "data."

Add this file to the "data" directory (edits required!):

<blockquote><a href="config.json">Config file</a></blockquote>

Once you have the file edited and in the "data" directory, make sure the window with your sketch in it is active. Select the _ESP32 Sketch Data Upload_ item from the _Tools_ menu in the IDE. The config file will be uploaded. You may need to close the serial monitor for this to work. I do.

Now that your config file is edited and uploaded, upload the sketch. Refresh your browser confident in the knowledge that you will not have to enter your network ssid and key again!


