# CASA0016-Multi-purpose-Security-Box
This is a connected security system prototype designed in CASA0016 by Zixu Cheng.

Author: Zixu Cheng

Department: University College London, London WC1E 6BT, UK

email: zixu.cheng.21@ucl.ac.uk

## Introduction

The security system is one of the most significant sections in intelligent Connected Sensor Systems. After sunset, the security system plays a crucial role in an innovative home system. It would be more convenient if the security system could activate to work automatically when the circumstance is getting darker. The primary function of a security system is warm body detection or human motion detection to prevent someone from breaking into your room. For this purpose, I designed a Multi-purpose Security Box in CASA0016 courses to create a connected security system.

The core sensors I used in my design are the LDR (Light-Dependent Resistor) and PIR (Passive Infrared) sensors. The LDR changes its resistor value with the light environment variation, automatically starting the system. On the other hand, PIR (Passive Infrared) Sensor is a good choice for human motion detection and no response for abiotic movements. It allows you to detect when someone is in your room when they shouldn't be to ensure room security.

As for "Multi-purpose", I utilise some peripheral sensors and actuators, like potentiometers, buzzers and LCD, to consummate and enhance my design and make my prototype could be applied in a broader range of uses. With the reinforcement of those components, this box could start working manually or automatically according to your demand, making a due response to detection and visualising the system operating status and detection data.

## Circuit design

I completed the circuit design in two steps. First, I realised the primary function of the security system with the core sensors. 

The LDR is a resistance semiconductor whose resistance value varies with the brightness changes in the environment, usually increasing while light intensity decreases[1]. So it is a good choice for activating the security system spontaneously after sunset. All we need to do is that setting a threshold of current value in the LDR branch. The resistance of LDR will increase, and the current will decrease when getting darker. If the current value is lower than the threshold, the system will be activated to operate automatically. As we all know, All things in the world emit heat energy in the form of infrared radiation, including human bodies. The hotter an object is, the more radiation it emits[2]. Based on this theory, the PIR sensor could detect the infrared radiation from a warm body then stop intruders from breaking into your room.

The original circuit of the prototype is shown in Fig 1. I use an Arduino UNO WIFI board with an LDR, a PIR sensor, a buzzer and a red LED light as the alarm responders.

<img src=".\imgs\Original circuit.png" alt="Original circuit design" style="zoom:27%;" />

Fig 1. Original circuit design.

However, the limitations of this circuit design cannot be ignored. Firstly, it is hard to adjust the threshold, so we cannot start the system for emergency uses whenever needed. What's more, it isn't easy to use and maintain without a visualisation component like LCD to display the operation status of the system[3].

To fix those problems, I add some auxiliary sensors and actuators for the circuit in the next step. I add a potentiometer in the LDR circuit branch to adjust the current manually so that you can activate the system via decreasing the current below the threshold at any time. Moreover, I use an LCD to show the current value in the LDR branch and the status of the system working or not, helping activate the system when using the potentiometer. The number of detected motions is also shown on the LCD. The final circuit is shown in Fig 2.

<img src=".\imgs\Final circuit.png" style="zoom:20%;" /> <img src=".\imgs\LCD.jpg" style="zoom:20%;" />

Fig 2. Final circuit design and the LCD.

## Physical design

For the physical prototyping, I designed a box to put everything together. I created my box with the help of Fusion 360 and materialised it with the 3D printer in the CE Lab. I use transparent material to print the box because I hope the LDR will not be blocked by light in the box. I also design two suitable windows, one in the front for the PIR sensor to detect warm body motion and the other in the back for the LCD to display information. I didn't design a cover for the box because I wouldn't like the cover to block the light to LDR. It would also be easier for me to control the potentiometer without a cover at that time.

<img src=".\imgs\original box.jpg" style="zoom:5%;" /><img src=".\imgs\3D model.gif" style="zoom:100%;" />

Fig 3. Box and its 3D Model

However, after the final presentation, I paid more attention to aesthetics, and I found it far from an exquisite appearance, although all systems could work well in the box. Then I refurbished the box – using delicate paper to wrap the box. I still made a cover for the box, considering the beauty of the prototype. To solve the problems of light occlusion and the difficulty in adjusting the potentiometer, I made two small holes on the top of the cover, one for the LDR to expose under the light and the other for adjusting the potentiometer. I used long enough wires to connect the LDR and potentiometer to the breadboard inside the box.

<img src=".\imgs\final box.jpg" style="zoom:10%;" />

Fig 4 Final design of my box after the presentation.

For the enclosure, due to the constraints of 3D printing materials and my imprecise design, the prototype has not yet been waterproof, so we had probably better deploy it indoors now. I believe the materials need to be hard enough to destroy because of the importance of the security system. More enclosure issues would be considered in further work. As for power, it needs an external power supply of 5V. If we are going to deploy it outdoors, we may need a battery.

## Workflow

The workflow of the Multi-purpose Security Box is quite simple. An LDR and a potentiometer work together to activate the system. I manually set the threshold to 400mA. The LDR could activate the system after dark, and the potentiometer could activate the system whenever you need. And the LCD screen helps you control the operating status of the security system. After the system works, it detects human movements and accumulates the number in an hour. Whenever motion is detected, the buzzer will alarm, and the red LED light will turn on. The data would also display on the LCD.

## Communication and Data consumer

I chose the Arduino UNO WIFI board as my control system to communicate with the Internet through a WIFI connection. First, I built an internet connection to the CE-hub WIFI and Used MQTT to publish the number of detected motions. Subsequently, I created a FluxDB database in Raspberry Pi to preserve the data published from MQTT and visualise them through the Grafana.

<img src=".\imgs\MQTT.png" style="zoom:100%;" />

Fig 5. Data on the MQTT server.

However, I found it meaningless if I only broadcasted the number of the detected motions and stored them in the database. That's because the number would always ascend, and it would be hard to analyse the data and meaningless for visualisation. To figure out this problem, I added a reset function for the number of detected motions to reset the number to 0 per hour. By doing this, We can quickly know when the highest number of motions will be detected and when there will be almost no intruders. Thinking more about it, this can also help optimise the configuration of security deployment because we know when we will need more or less security according to the data regularity. I believe this is the charm of statistical analysis.

<img src=".\imgs\Grafana.gif" style="zoom:100%;" />

Fig 6. The visualisation chart shows the peak time of the number of motions after reset per hour.

## Power Consumption

To calculate the power consumption, I applied a 5000mAh battery for my prototype working at 5V. It is a rough method to estimate the power my device use. It took nearly 25h to run out of the battery. According to the formula, the average working current of the device is 200mA, so that the average power when the device is running is approximate 1W.

I had to calculate the power consumption in this way because I didn't have time to measure the power consumption in the CE Lab with the professional equipment before the Christmas break. I should do more experiments about the power in future.

## Reference 

[1] En.wikipedia.org. 2022. *Photoresistor - Wikipedia*. [online] Available at: <https://en.wikipedia.org/wiki/Photoresistor> [Accessed 10 January 2022].

[2] Last Minute Engineers. 2022. How HC-SR501 PIR Sensor Works & How To Interface It With Arduino. [online] Available at: <https://lastminuteengineers.com/pir-sensor-arduino-tutorial/> [Accessed 10 January 2022].

[3] Arduino.cc. 2022. [online] Available at: <https://www.arduino.cc/en/Tutorial/LibraryExamples/LiquidCrystalDisplay> [Accessed 10 January 2022].
