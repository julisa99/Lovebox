❤️ Lovebox ❤️
===
DIY project to lasercut a lovebox like https://en.lovebox.love/

![](https://i.imgur.com/5MPBxsc.jpg)
![](https://github.com/julisa99/Lovebox/blob/master/docs/images/demo_lovebox.gif)

## Materials
1. [3mm wood](https://www.amazon.de/dp/B0002FQKX4) (to cut the case + heart)
2. [WeMos D1 Mini](https://www.amazon.de/dp/B01N9RXGHY)
3. [Servo motor](https://www.amazon.de/dp/B07236KYVC)
4. [Light sensor](https://www.amazon.de/dp/B01LX0ZQHL)
5. [10kΩ resistor](https://www.amazon.de/dp/B072BHDBDG)
6. [OLED display](https://www.amazon.de/dp/B074NJMPYJ)
7. [PCB](https://www.amazon.de/dp/B00VL1KHJQ)
8. [1mm acryl](https://www.amazon.de/dp/B07T4JSXYR) (to cut the display)
9. [Darkening foil/dazzle strip](https://www.amazon.de/dp/B001CC6VYO)

## Composition 
Below you can see where the OLED display, the light sensor with resistor and the servo motor are connected to the WeMos D1 Mini.
The second photo is the soldered assembly, where I used two PCBs to have the power supply, which is plugged into the microcontroller board, on the ground and so that the OLED display can lie straight on top.
![](https://i.imgur.com/Y7jeOma.png)
![](https://i.imgur.com/9Fiw6kp.jpg)

After it, you have to cut the SVGs for the case, cap and hearts out of the 3mm wood with the lasercutter. Additionally, you have to cut the display out of the 1mm acryl with the lasercutter. Then I have stuck the little heart on the larger heart and painted it red. Then I have cut with a scissors the display out of the darkening foil and stuck it on the acryl display. Then I have stuck the white servo horn into the back of the heart.

![](https://i.imgur.com/y2joo0B.jpg)

The last step is to assemble the case and the cap. Now you can put the hardware inside the case and program the microcontroller.

## How it works

For programming I used the [Arduino IDE](https://www.arduino.cc/en/Main/Software). The cool thing about the Wemos D1 Mini is the built in ESP8266, so you can connect to the internet. Messages like pictures and texts can be sent to the Lovebox via the internet. In order to control the ESP8266, the following address must be added in the Arduino IDE under "File->Settings->Additional board administrator URLs": http://arduino.esp8266.com/stable/package_esp8266com_index.json. Then you can type "esp8266" in the search field under "Tools->Board->Boardverwalter" and click on "Install". Now the WeMos D1 Mini appears under the boards and can be selected.

Then you can open the "message.ino" and upload it to the board, just connect it to your computer with a micro usb cable. You should first enter your WLAN, password and the path to your gist file (without the part "https://gist.github.com/") in the "credentials.txt". The gist file is a text document, which you create online as "private" and then almost always leave your message there, which the microcontroller can then retrieve.

The microcontroller will then always call your text document on gist.github and see if the first number in the first line is a different one. This serves as an identifier to determine if a new message is retrievable.
If this is different from the previous one, then the algorithm looks into the second line, if it contains a "t", then the microcontroller expects a text and shows the message on the display. If there is another character, e.g. a "b", then an image is expected. For the picture I use the website: https://www.dcode.fr/binary-image. There you can upload your black and white image and set the target width to 128 pixels. The image will then be converted to "0" and "1" only, so each black pixel becomes a 0 and each white pixel a 1. You can then simply enter this string in the third line of your text document on gist.github. So your text document could look like this:

![](https://i.imgur.com/07qDPxb.png)

###### tags: `Lovebox` `Instruction`
