❤️ Lovebox ❤️
===
DIY project to lasercut a lovebox inspired from https://en.lovebox.love/. It was a homemade Christmas present for my mom. So I can send her messages or pictures at any time to start the day motivated. When a new message is received, the heart moves in front of the box. It stops as soon as the message has been read (more specifically the built-in brightness sensor reacts when the box is opened). Have fun building your own!

![](https://i.imgur.com/5MPBxsc.jpg)
![](https://github.com/julisa99/Lovebox/blob/master/docs/images/demo_lovebox.gif)

## Materials
1. [3mm](https://www.amazon.de/dp/B0002FQKX4) or [4mm wood](https://www.amazon.de/dp/B06Y48L765)(to cut the case + heart)
2. [WeMos D1 Mini](https://www.amazon.de/dp/B01N9RXGHY)
3. [Servo motor](https://www.amazon.de/dp/B07236KYVC)
4. [Light sensor](https://www.amazon.de/dp/B01LX0ZQHL)
5. [10kΩ resistor](https://www.amazon.de/dp/B072BHDBDG)
6. [OLED display](https://www.amazon.de/dp/B074NJMPYJ)
7. [Perfboard](https://www.amazon.de/dp/B00VL1KHJQ)
8. [Breakaway PCB Connector](https://www.amazon.de/dp/B01M69EA9O)
9. [1mm acryl](https://www.amazon.de/dp/B07T4JSXYR) (to cut the display)
10. [Darkening foil/dazzle strip](https://www.amazon.de/dp/B001CC6VYO)

## Composition 
Below you can see where the OLED display, the light sensor with resistor and the servo motor are connected to the WeMos D1 Mini. The second photo is the soldered assembly. As you can see the micro-USB connector of the microcotroller board is at the ground. On top of the microcontroller a perfboard is soldered. Now the servo motor is added and also soldered to the perfboard. Notice the resistor and the light sensor that are also soldered to the board, with the light sensor pointing up. The display is soldered to another perfboard. The board provides a flat surface to hold the display in place. It is put on the motor and is connected to the other perfboard by using a Breakaway PCB Connector. It serves as a spacer to hold the other board above the motor.
![](https://i.imgur.com/6L2zcLs.png)
![](https://i.imgur.com/Y3Lg5tn.jpg)

After finishing the electronics, you have to cut the case, cap and hearts out of the wood and the display out of the 1mm acryl with a lasercutter. For that [four SVGs are provided](https://github.com/julisa99/Lovebox/tree/master/lasercut_files) containing the cutting plan. Then you have to glue the little heart on the larger heart and paint it red. After that, you have to cut with scissors the size of the display out of the darkening foil and put it on the acryl display. Finally, stuck the white servo horn into the back of the heart.

![](https://i.imgur.com/y2joo0B.jpg)

The last step is to assemble the case and the cap. Now you can put the hardware inside the case and program the microcontroller.

## How it works

For programming I used the [Arduino IDE](https://www.arduino.cc/en/Main/Software). The cool thing about the Wemos D1 Mini is the built in ESP8266, so you can connect to the internet. Messages like pictures and texts can be sent to the Lovebox via the internet. In order to control the ESP8266, the following address must be added in the Arduino IDE under "File->Preferences->Additional Board Manager URLs": http://arduino.esp8266.com/stable/package_esp8266com_index.json. Then you can type "esp8266" in the search field under "Tools->Board->Boards Manager" and click on "Install". Now the WeMos D1 Mini appears under the boards and can be selected.

Then you can open the ["message.ino"](https://github.com/julisa99/Lovebox/blob/master/message/message.ino) and upload it to the board, just connect it to your computer with a micro usb cable. You should first enter your WLAN, password and the path to your gist file (without the part "https://gist.github.com" and add "/raw/yourFilename" behind) in the ["credentials.h"](https://github.com/julisa99/Lovebox/blob/master/message/credentials.h). The gist file is a text document, which you create online as "private" and then almost always leave your message there, which the microcontroller can then retrieve.

The microcontroller will then always call your text document on gist.github and see if the first number in the first line is a different one. This serves as an identifier to determine if a new message is retrievable.
If this is different from the previous one, then the algorithm looks into the second line, if it contains a "t", then the microcontroller expects a text and shows the message on the display. If there is another character, e.g. a "b", then an image is expected. For the picture I use the website: https://www.dcode.fr/binary-image. There you can upload your black and white image and set the target width to 128 pixels. The image will then be converted to "0" and "1" only, so each black pixel becomes a 0 and each white pixel a 1. You can then simply enter this string in the third line of your text document on gist.github. So your text document could look like this:

![](https://i.imgur.com/07qDPxb.png)

###### tags: `Lovebox` `Instruction`
