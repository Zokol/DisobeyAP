# DisobeyAP
Disobey WiFi Access Point

Portable captive portal project based on ESP8266.

Hardware: Wemos D1 Mini Pro and Wemos Battery Shield + Nokia BP-4L battery
Software:
  - Done using softAP-mode of ESPWifiServer-library and modified DNSServer-library to support captive portal on mobile platforms
  - ESP8266 Wemos library from https://github.com/wemos/Arduino_D1
  - DNSServer fixed for captive portal from: https://github.com/esp8266/Arduino/pull/396
  - ASCII image conversion by: http://www.text-image.com/convert/ascii.html

ToDo:
  - OSX captive portal does not work
