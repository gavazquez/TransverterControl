# TransverterControl

Repo for controlling the UR3LMZ Transverter. It also uses a custom ADF4351 library

Check the images in the [Doc](./Doc) folder of the repo to see how the default firmware configures the registers of the ADF4351.
You can also check the [datasheet](./Doc/adf4351.pdf) of the ADF4351

### This repo collaborates with the [Satcom radio](http://satcomradio.github.io) website!
Plase check it's [transverter](https://satcomradio.github.io/#/transverters/index) section for more info.

## Arduino

In the [Sketches](./Sketches) Folder you will see 3 sketches.

1. AttinyIndependent
This sketch allows you to upload the code to the attiny and configure the frequencies you want in the selector.  
Use the [ADFSoftware](./ADFSoftware) to calculate the registers and insert them in **reverse** order (from R5 to R0)

2. AttinySlave
This sketch makes the Attiny a slave of an arduino. It only tells the arduino in what position you have the selector and when it's on TX or RX.

3. ArduinoController
Upload this sketch to an arduino and you'll be able to control the transverter. It basically recieves the status from the AttinySlave and set the configured frequencies using an easy library.

In order to compile any of the Attiny sketches with an **Arduino UNO**:

1. Install the [microcore](https://github.com/MCUdude/MicroCore) board to be able to compile Attiny code.
2. Copy the [ADFLibrary](./Library) into your arduino libraries folder.
3. Upload the Arduino ISP sketch into your ardino board
4. With the transverter **OFF**. Connect the pins to the transverter board following this [picture](./Doc/adf_pinout.jpg)
	Mosi - 11, MISO - 12, CLK - 13, RESET - 10, 5V and GND
5. Upload the Attiny sketches with ´Sketch -> Upload using programmer´

You can use a spectrum analyzer to check the frequencies of the ADF on the first pin of the output pins but use an attenuator if needed!
