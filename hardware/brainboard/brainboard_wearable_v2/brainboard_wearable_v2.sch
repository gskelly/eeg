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
LIBS:SparkFun-Passives
LIBS:brainboard_wearable_v2-cache
EELAYER 27 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 7
Title "BrainBoard: Open-Source Hardware for Wearable BCI"
Date "25 jul 2014"
Rev "0"
Comp "Virginia Commonwealth University"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 1050 4600 1150 600 
U 53AA192D
F0 "Brainboard IMU" 50
F1 "brainboard_wearable_v2_imu.sch" 50
$EndSheet
$Sheet
S 1050 3650 1200 600 
U 53AA1930
F0 "Brainboard AFE" 50
F1 "brainboard_wearable_v2_afe.sch" 50
$EndSheet
$Sheet
S 1100 2750 1150 650 
U 53AA1933
F0 "Brainboard Power" 50
F1 "brainboard_wearable_v2_pwr.sch" 50
$EndSheet
$Sheet
S 1100 1800 1150 650 
U 53AA1936
F0 "Brainboard MCU" 50
F1 "brainboard_wearable_v2_mcu.sch" 50
$EndSheet
$Sheet
S 1100 1050 1100 500 
U 53AA1939
F0 "Brainboard Bluetooth" 50
F1 "brainboard_wearable_v2_bt.sch" 50
$EndSheet
$Sheet
S 1050 5450 1150 750 
U 53AB1AA4
F0 "Brainboard Analog In" 50
F1 "brainboard_wearable_v2_ain.sch" 50
$EndSheet
$Comp
L MHOLE MTG1
U 1 1 53AB1D68
P 3400 2000
F 0 "MTG1" H 3400 2200 60  0000 C CNN
F 1 "MHOLE" H 3700 2000 60  0000 C CNN
F 2 "~" H 3400 2000 60  0000 C CNN
F 3 "~" H 3400 2000 60  0000 C CNN
	1    3400 2000
	1    0    0    -1  
$EndComp
$Comp
L MHOLE MTG2
U 1 1 53AB1D6E
P 3400 2500
F 0 "MTG2" H 3400 2700 60  0000 C CNN
F 1 "MHOLE" H 3700 2500 60  0000 C CNN
F 2 "~" H 3400 2500 60  0000 C CNN
F 3 "~" H 3400 2500 60  0000 C CNN
	1    3400 2500
	1    0    0    -1  
$EndComp
$Comp
L MHOLE MTG3
U 1 1 53AB1D74
P 3400 3000
F 0 "MTG3" H 3400 3200 60  0000 C CNN
F 1 "MHOLE" H 3700 3000 60  0000 C CNN
F 2 "~" H 3400 3000 60  0000 C CNN
F 3 "~" H 3400 3000 60  0000 C CNN
	1    3400 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3400 2150 3400 2200
Wire Wire Line
	3400 2200 3950 2200
Wire Wire Line
	3400 2650 3400 2700
Wire Wire Line
	3400 2700 3950 2700
Connection ~ 3950 2700
Wire Wire Line
	3400 3150 3400 3200
Wire Wire Line
	3400 3200 3950 3200
Connection ~ 3950 3200
$Comp
L DGND #PWR01
U 1 1 53AB1D97
P 3950 3250
F 0 "#PWR01" H 3950 3250 40  0001 C CNN
F 1 "DGND" H 3950 3180 40  0000 C CNN
F 2 "~" H 3950 3250 60  0000 C CNN
F 3 "~" H 3950 3250 60  0000 C CNN
	1    3950 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	3950 2200 3950 3250
$EndSCHEMATC
