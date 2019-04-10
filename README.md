# BasicEPaperReader

## What you need
- WaveShare EPaper Display (ex 1.54in 200x200 https://smile.amazon.com/gp/product/B07DH6K1VQ/)
- ESP32 Development Board (ex https://www.adafruit.com/product/3405 or https://smile.amazon.com/gp/product/B0718T232Z/)
- 4 SPST switches
- Breadboard
- Optional: Basic USB logic analyzer (https://smile.amazon.com/gp/product/B077LSG5P2/)

## Wiring Instructions
The specific wiring is going to be different depending on what board you're using. These instructions are for the Adafruit HUZZAH32

Connect the following:
- SDI/SCK -> MOSI
- SCK/SCLK -> SCK
- CS -> 15
- DC -> 33
- RST/Reset -> 27
- Busy -> 32

If you want to use different pins, you can change the pin configurations for `GxIO_Class` and `GxEPD_Class` in BasicEPaperReader.ino

You might also try switching the `VSPI` option on the `SPIClass` object to `HSPI`. `VSPI` and `HSPI` are the two SPI interfaces exposed by the ESP32.

For each of the SPST switches, connect one side to ground and one side to pins: 14, 21, 22, 23

We'll use the ESP32's internal pullup resistors, so no external resistors are needed.

## Software Setup
1. Install Arduino IDE https://www.arduino.cc/en/Main/Software
2. Install Arduino Core for the ESP32 https://github.com/espressif/arduino-esp32
3. In the Arduino IDE, select Tools -> Manage Libraries and install the following libraries: ArduinoHttpClient, Adafruit GFX Library, ArdunoJSON
4. Install GxEPD manually
   1. Download the library as a zip file https://github.com/ZinggJM/GxEPD/archive/master.zip
   2. In Arduino IDE, select Sketch -> Include Library -> Add .ZIP Library -> Select the GxEPD zip file
5. Get a Pocket API key
   1. Create a new Pocket API app: https://getpocket.com/developer/
   2. At the end up the app creation, copy the consumer key and access key into `POCKET_CONSUMER_KEY` and `POCKET_ACCESS_KEY` in Keys.h
6. Get an Aylien Text Analysis API key
   1. Create an account here: https://developer.aylien.com/signup
   2. Create an app and get an application ID and an application key. Copy those into `AYLIEN_APPLICATION_ID` and `AYLIEN_APPLICATION_KEY` in Keys.h
7. Set up your WiFi
   1. Put your WiFi hotspot's name in `WIFI_AP_NAME` in Keys.h
   2. Put your WiFi hotspot's password in `WIFI_PASSWORD` in Keys.h
8. Click Run. Watch the EPaper display update with the 3 most recent articles saved to your Pocket account.
   

## Using a Digital Logic Analyzer to see what's happening
1. Download the supported software for your logic analyzer. Ex: https://www.saleae.com/downloads/
2. Connect your logic analyzer's ground lead to the circuit's ground
3. Connect other analyzer channels to the other SPI connections to the EPaper display. The important ones are CS, CLK, and DIN.
4. Add an Analyzer configuration for SPI. Set MOSI to the channel connected to the EPaper display's DIN pin; set MISO to None; set Clock to the CLK pin; set Enable to the CS pin. Other options should remain at their defaults.
5. Select the "Setup Trigger" button on the channel connected to the CS pin. Set it to Trigger on Falling Edge.
6. Start the logic analyzer. Click a button on the BasicEPaperReader setup and let the logic analyzer capture the SPI communication sent to the EPaper display to change what is being displayed.
