---
title: ESP32 Set Up on Arduino IDE
date: "2019-11-17"
description: "Actions required to install board definitions and SPIFFS uploader for ESP32 to Arduino IDE"
---

ESP32 based development boards offer a very nice microcontroller along with WiFi and Bluetooth connectivity. Even better, these boards can be programmed directly from the Arduino IDE, making prototyping super easy, quick, and fun!

These boards have a flash based filesystem connected via Serial Peripheral Interface (SPI) imaginatively named SPIFFS (SPI Flash FileSystem). The Arduino IDE can upload files directly to SPIFFS for later access via sketch by installing the data uploader.

This guide will show how to add both the board definitions and the SPIFFS uploader to the Arduino IDE...let's go!

### Bored? Here's the definition

Two steps are needed to get the board definitions loaded.

First, we need to tell the Arduino IDE about a new place to find definitions from. That place is:

<blockquote><code>https://dl.espressif.com/dl/package_esp32_index.json</code></blockquote>

Open preferences for the Arduino IDE, go to the _Settings_ tab, and add that URL to _Additional Boards Manager URLs_.

Close the preferences (in the affirmative, of course!) and then proceed to the _Tools_ menu. Under _Tools_, go to _Board_-->_Boards Manager..._ and select it. A dialog will appear, type "esp32" into the search bar at the top. Install "esp32 by Espressif Systems" and close the dialog.

When you return to the _Tools_-->_Board_ menu, an ESP32 section is now available. Select your board from the list and you're all set!

### What about that spiffy file system?

To enable uploading data to the SPIFFS on your board, go to:

<blockquote><code><a href="https://github.com/me-no-dev/arduino-esp32fs-plugin/releases/">https://github.com/me-no-dev/arduino-esp32fs-plugin/releases/</a></code></blockquote>

Download the latest "ESP32FS-_ver_.zip" file. At the time of this writing, this was "ESP32FS-1.0.zip."

Unzip the file. This will give you a folder called _ESP32FS_. Add this folder to the _tools_ folder (which you may also need to create) under your Arduino folder. On MacOS, the Arduino folder is at _~/Documents/Arduino_. The final path should be _~/Documents/Arduino/tools/ESP32FS_. For other platforms [click here](https://www.google.com/search?q=arduino+directory).

After restarting the IDE, you should now have an option on the _Tools_ menu called _ESP32 Sketch Data Upload_. More to come on how to use this!

### Conclusion

That's it! Your Arduino IDE is all set up for ESP32 boards. Now...what will you do with it?
