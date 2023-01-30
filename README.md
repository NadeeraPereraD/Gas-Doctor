LP GAS Level Detector with Data Analysis
========================================
*by Nadeera Perera (18sea018)*

Introduction
++++++++++++

Now a days, LP GAS usage is very high. it not only home usage, hotel field and restaurant also. 
But the big issue is anyone cannot moniter the GAS level due to coming with iron cylinder. 
Hence if there is a method to moniter the LP GAS level, it is very essential. 
In this LP GAS Level Detector system comes with the Data Analysis part. So it will predict the date that GAS cylinder finish.


Method
------

Basically, this system working on a Arduino and Loadcell. Using Loadcell, can get the GAS Levelof the cylinder. Then data will store daily. Afterthat using that data, system will predict the date of the GAS over.



Features
--------
	1. LP GAS Level Detection
	2. Predict the date of GAS Cylinder finish
	
How to use
----------

There is a mounting base. GAS cylinder must put on that mounting base. Aferthat there will be a Display to show the GAS Level and the date of GAS over.
    
Software
--------
* Arduino IDE (C based Programming)
* Orcad 9.2

Hardware
--------
* Arduino Mega2560
* Load Cell and HX711 Driver Module
* DS1307 RTC Module
* SD Memory Card Module
* 16x2 LCD 

To do
-----
Need to add WiFi module and connected via Android Mobile and get display and control using Android App.