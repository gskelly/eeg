EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:brainboard_customparts
LIBS:resistor-ruf
LIBS:resistor-bourns
LIBS:SparkFun-Passives
LIBS:beaglebone
LIBS:SparkFun-RF
LIBS:brainboardv0-cache
EELAYER 27 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 6 6
Title "BrainBoard: Open-Source Hardware for Wearable BCI"
Date "24 jul 2013"
Rev "0"
Comp "Virginia Commonwealth University"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L TPS73150 U10
U 1 1 51DE0EFA
P 5650 3750
F 0 "U10" H 5800 4050 60  0000 C CNN
F 1 "TPS73150" H 5950 3550 60  0000 C CNN
F 2 "~" H 5650 3750 60  0000 C CNN
F 3 "~" H 5650 3750 60  0000 C CNN
F 4 "Texas Instruments" H 5650 3750 60  0001 C CNN "Manufacturer 1"
F 5 "TPS73150DBVT" H 5650 3750 60  0001 C CNN "PN 1"
F 6 "DigiKey" H 5650 3750 60  0001 C CNN "Vendor 1"
F 7 "$1.23/1 $1.091/10 $0.9848/25" H 5650 3750 60  0001 C CNN "Pricing 1"
	1    5650 3750
	1    0    0    -1  
$EndComp
$Comp
L +5VD #PWR064
U 1 1 51DE0F09
P 5100 3400
F 0 "#PWR064" H 5100 3350 20  0001 C CNN
F 1 "+5VD" H 5100 3500 30  0000 C CNN
F 2 "" H 5100 3400 60  0000 C CNN
F 3 "" H 5100 3400 60  0000 C CNN
	1    5100 3400
	1    0    0    -1  
$EndComp
$Comp
L +5VA #PWR065
U 1 1 51DE129C
P 6350 3400
F 0 "#PWR065" H 6350 3530 20  0001 C CNN
F 1 "+5VA" H 6350 3500 30  0000 C CNN
F 2 "" H 6350 3400 60  0000 C CNN
F 3 "" H 6350 3400 60  0000 C CNN
	1    6350 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5100 3750 5200 3750
Wire Wire Line
	5100 3400 5100 3750
Connection ~ 5100 3600
$Comp
L GNDPWR #PWR066
U 1 1 51DE130A
P 5650 4200
F 0 "#PWR066" H 5650 4250 40  0001 C CNN
F 1 "GNDPWR" H 5650 4120 40  0000 C CNN
F 2 "" H 5650 4200 60  0000 C CNN
F 3 "" H 5650 4200 60  0000 C CNN
	1    5650 4200
	1    0    0    -1  
$EndComp
$Comp
L C C36
U 1 1 51DE1328
P 6350 3950
F 0 "C36" H 6350 4050 40  0000 L CNN
F 1 "0.1uF" H 6356 3865 40  0000 L CNN
F 2 "~" H 6388 3800 30  0000 C CNN
F 3 "~" H 6350 3950 60  0000 C CNN
F 4 "Kemet" H 6350 3950 60  0001 C CNN "Manufacturer 1"
F 5 "C0603C104K4RACTU" H 6350 3950 60  0001 C CNN "PN 1"
F 6 "DigiKey" H 6350 3950 60  0001 C CNN "Vendor 1"
F 7 "$0.10/1 $0.022/10 $0.0118/50" H 6350 3950 60  0001 C CNN "Pricing 1"
	1    6350 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	5650 4050 5650 4200
Wire Wire Line
	6350 4150 5650 4150
Connection ~ 5650 4150
Wire Wire Line
	6100 3750 6350 3750
Wire Wire Line
	6100 3600 6650 3600
Wire Wire Line
	6350 3600 6350 3400
Wire Wire Line
	5100 3600 5200 3600
Text Notes 5050 4500 0    51   ~ 0
Rev. 0 is meant for demonstration of basic signal acquisition capabilities\nand does not use battery power. +5VD is the VCC_5V supply from the\nBeagleBone. The TPS73150 is used for post-regulation.
Wire Wire Line
	6650 3600 6650 3500
Connection ~ 6350 3600
$Comp
L TST P8
U 1 1 51E98283
P 6650 3500
F 0 "P8" H 6650 3800 40  0000 C CNN
F 1 "5VA" H 6650 3750 30  0000 C CNN
F 2 "~" H 6650 3500 60  0000 C CNN
F 3 "~" H 6650 3500 60  0000 C CNN
	1    6650 3500
	1    0    0    -1  
$EndComp
$EndSCHEMATC
