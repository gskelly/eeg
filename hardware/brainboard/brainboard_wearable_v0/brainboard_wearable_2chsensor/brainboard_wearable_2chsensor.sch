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
LIBS:brainboard_wearable_2chsensor-cache
EELAYER 27 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date "25 apr 2014"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L GND #PWR25
U 1 1 5347375C
P 3900 -3200
F 0 "#PWR25" H 3900 -3200 30  0001 C CNN
F 1 "GND" H 3900 -3270 30  0001 C CNN
F 2 "~" H 3900 -3200 60  0000 C CNN
F 3 "~" H 3900 -3200 60  0000 C CNN
	1    3900 -3200
	1    0    0    -1  
$EndComp
$Comp
L +BATT #PWR26
U 1 1 5347375D
P 4000 -4250
F 0 "#PWR26" H 4000 -4300 20  0001 C CNN
F 1 "+BATT" H 4000 -4150 30  0000 C CNN
F 2 "~" H 4000 -4250 60  0000 C CNN
F 3 "~" H 4000 -4250 60  0000 C CNN
	1    4000 -4250
	-1   0    0    -1  
$EndComp
$Comp
L DVDD #PWR27
U 1 1 5347375E
P 4100 -4150
F 0 "#PWR27" H 4100 -4050 30  0001 C CNN
F 1 "DVDD" H 4100 -4040 30  0000 C CNN
F 2 "~" H 4100 -4150 60  0000 C CNN
F 3 "~" H 4100 -4150 60  0000 C CNN
	1    4100 -4150
	1    0    0    -1  
$EndComp
NoConn ~ 3050 -4300
$Comp
L CONN_7X2 P1
U 1 1 5340C7E8
P 800 -4200
F 0 "P1" H 800 -3800 60  0000 C CNN
F 1 "CONN_7X2" V 800 -4200 60  0000 C CNN
F 2 "~" H 800 -4200 60  0000 C CNN
F 3 "~" H 800 -4200 60  0000 C CNN
	1    800  -4200
	1    0    0    -1  
$EndComp
NoConn ~ 1200 -4300
NoConn ~ 1200 -4200
NoConn ~ 1200 -4100
NoConn ~ 400  -4300
NoConn ~ 400  -4200
NoConn ~ 400  -4100
NoConn ~ 400  -4000
Wire Wire Line
	3850 -4100 4100 -4100
Wire Wire Line
	4000 -4200 3850 -4200
Wire Wire Line
	3900 -3400 3850 -3400
Wire Wire Line
	3900 -3600 3900 -3200
Wire Wire Line
	3850 -3500 3900 -3500
Connection ~ 3900 -3400
Wire Wire Line
	2750 -4200 3050 -4200
Wire Wire Line
	2750 -4100 3050 -4100
Wire Wire Line
	2750 -4000 3050 -4000
Wire Wire Line
	2850 -3800 3050 -3800
Wire Wire Line
	2950 -3700 3050 -3700
Wire Wire Line
	2950 -3600 3050 -3600
Wire Wire Line
	400  -4500 0    -4500
Wire Wire Line
	1200 -4500 1550 -4500
Wire Wire Line
	1200 -4400 1550 -4400
Wire Wire Line
	1200 -4000 1600 -4000
Wire Wire Line
	400  -4400 0    -4400
Wire Wire Line
	50   -3900 400  -3900
Wire Wire Line
	350  -3900 350  -3800
Wire Wire Line
	350  -3800 1250 -3800
Wire Wire Line
	1250 -3800 1250 -3900
Wire Wire Line
	1250 -3900 1200 -3900
Connection ~ 350  -3900
NoConn ~ 3850 -4000
NoConn ~ 3850 -3900
NoConn ~ 3850 -3800
$Comp
L VEXT #PWR24
U 1 1 534723AB
P 3900 -4350
F 0 "#PWR24" H 3900 -4250 30  0001 C CNN
F 1 "VEXT" H 3900 -4240 30  0000 C CNN
F 2 "~" H 3900 -4350 60  0000 C CNN
F 3 "~" H 3900 -4350 60  0000 C CNN
	1    3900 -4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 -4200 4000 -4250
Wire Wire Line
	4100 -4100 4100 -4150
Wire Wire Line
	3900 -3600 3850 -3600
Connection ~ 3900 -3500
Wire Wire Line
	3850 -4300 3900 -4300
Wire Wire Line
	3900 -4300 3900 -4350
$Comp
L CONN_10X2 P2
U 1 1 53481745
P 3450 -3850
F 0 "P2" H 3450 -3300 60  0000 C CNN
F 1 "CONN_10X2" V 3450 -3950 50  0000 C CNN
F 2 "~" H 3450 -3850 60  0000 C CNN
F 3 "~" H 3450 -3850 60  0000 C CNN
	1    3450 -3850
	1    0    0    -1  
$EndComp
NoConn ~ 3050 -3500
NoConn ~ 3050 -3900
NoConn ~ 3850 -3700
NoConn ~ 3050 -3400
$Comp
L C C15
U 1 1 53474D24
P -100 -2550
F 0 "C15" H -100 -2450 40  0000 L CNN
F 1 "1uF" H -94 -2635 40  0000 L CNN
F 2 "~" H -62 -2700 30  0000 C CNN
F 3 "~" H -100 -2550 60  0000 C CNN
	1    -100 -2550
	1    0    0    -1  
$EndComp
$Comp
L C C16
U 1 1 53474D2A
P 200 -2550
F 0 "C16" H 200 -2450 40  0000 L CNN
F 1 "0.1uF" H 206 -2635 40  0000 L CNN
F 2 "~" H 238 -2700 30  0000 C CNN
F 3 "~" H 200 -2550 60  0000 C CNN
	1    200  -2550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR22
U 1 1 53474D30
P 50 -2250
F 0 "#PWR22" H 50  -2250 30  0001 C CNN
F 1 "GND" H 50  -2320 30  0001 C CNN
F 2 "~" H 50  -2250 60  0000 C CNN
F 3 "~" H 50  -2250 60  0000 C CNN
	1    50   -2250
	1    0    0    -1  
$EndComp
$Comp
L C C13
U 1 1 53474D36
P -750 -2550
F 0 "C13" H -750 -2450 40  0000 L CNN
F 1 "1uF" H -744 -2635 40  0000 L CNN
F 2 "~" H -712 -2700 30  0000 C CNN
F 3 "~" H -750 -2550 60  0000 C CNN
	1    -750 -2550
	1    0    0    -1  
$EndComp
$Comp
L C C14
U 1 1 53474D3C
P -450 -2550
F 0 "C14" H -450 -2450 40  0000 L CNN
F 1 "0.1uF" H -444 -2635 40  0000 L CNN
F 2 "~" H -412 -2700 30  0000 C CNN
F 3 "~" H -450 -2550 60  0000 C CNN
	1    -450 -2550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR20
U 1 1 53474D42
P -600 -2250
F 0 "#PWR20" H -600 -2250 30  0001 C CNN
F 1 "GND" H -600 -2320 30  0001 C CNN
F 2 "~" H -600 -2250 60  0000 C CNN
F 3 "~" H -600 -2250 60  0000 C CNN
	1    -600 -2250
	1    0    0    -1  
$EndComp
$Comp
L C C10
U 1 1 53474D48
P -1750 -850
F 0 "C10" H -1750 -750 40  0000 L CNN
F 1 "1uF" H -1744 -935 40  0000 L CNN
F 2 "~" H -1712 -1000 30  0000 C CNN
F 3 "~" H -1750 -850 60  0000 C CNN
	1    -1750 -850
	1    0    0    -1  
$EndComp
$Comp
L C C11
U 1 1 53474D4E
P -1500 -850
F 0 "C11" H -1500 -750 40  0000 L CNN
F 1 "1uF" H -1494 -935 40  0000 L CNN
F 2 "~" H -1462 -1000 30  0000 C CNN
F 3 "~" H -1500 -850 60  0000 C CNN
	1    -1500 -850
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR15
U 1 1 53474D54
P -1750 -550
F 0 "#PWR15" H -1750 -550 30  0001 C CNN
F 1 "GND" H -1750 -620 30  0001 C CNN
F 2 "~" H -1750 -550 60  0000 C CNN
F 3 "~" H -1750 -550 60  0000 C CNN
	1    -1750 -550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR16
U 1 1 53474D5A
P -1500 -550
F 0 "#PWR16" H -1500 -550 30  0001 C CNN
F 1 "GND" H -1500 -620 30  0001 C CNN
F 2 "~" H -1500 -550 60  0000 C CNN
F 3 "~" H -1500 -550 60  0000 C CNN
	1    -1500 -550
	1    0    0    -1  
$EndComp
$Comp
L C C7
U 1 1 53474D60
P -3700 -850
F 0 "C7" V -3650 -800 40  0000 L CNN
F 1 "10uF" V -3650 -1050 40  0000 L CNN
F 2 "~" H -3662 -1000 30  0000 C CNN
F 3 "~" H -3700 -850 60  0000 C CNN
	1    -3700 -850
	0    -1   -1   0   
$EndComp
$Comp
L C C6
U 1 1 53474D66
P -3700 -1100
F 0 "C6" V -3650 -1050 40  0000 L CNN
F 1 "0.1uF" V -3650 -1300 40  0000 L CNN
F 2 "~" H -3662 -1250 30  0000 C CNN
F 3 "~" H -3700 -1100 60  0000 C CNN
	1    -3700 -1100
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR14
U 1 1 53474D6C
P -3450 -550
F 0 "#PWR14" H -3450 -550 30  0001 C CNN
F 1 "GND" H -3450 -620 30  0001 C CNN
F 2 "~" H -3450 -550 60  0000 C CNN
F 3 "~" H -3450 -550 60  0000 C CNN
	1    -3450 -550
	1    0    0    -1  
$EndComp
Text Notes -1450 -1300 0    51   ~ 0
VCAP1 should connect\nto Class 1 ceramic or\ntantalum capacitor to\navoid ferroelectric noise.
$Comp
L ADS1291/2 U3
U 1 1 53474D79
P -2600 -2200
F 0 "U3" H -2600 -3250 60  0000 C CNN
F 1 "ADS1291/2" H -2600 -1150 60  0000 C CNN
F 2 "" H -2600 -2400 60  0000 C CNN
F 3 "" H -2600 -2400 60  0000 C CNN
	1    -2600 -2200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR13
U 1 1 53474D7F
P -3450 -2750
F 0 "#PWR13" H -3450 -2750 30  0001 C CNN
F 1 "GND" H -3450 -2820 30  0001 C CNN
F 2 "~" H -3450 -2750 60  0000 C CNN
F 3 "~" H -3450 -2750 60  0000 C CNN
	1    -3450 -2750
	1    0    0    -1  
$EndComp
NoConn ~ -1800 -2300
$Comp
L C C8
U 1 1 53474D86
P -3650 -1900
F 0 "C8" V -3600 -1850 40  0000 L CNN
F 1 "4.7nF" V -3600 -2150 40  0000 L CNN
F 2 "~" H -3612 -2050 30  0000 C CNN
F 3 "~" H -3650 -1900 60  0000 C CNN
	1    -3650 -1900
	0    -1   -1   0   
$EndComp
$Comp
L C C9
U 1 1 53474D8C
P -3650 -1700
F 0 "C9" V -3600 -1650 40  0000 L CNN
F 1 "4.7nF" V -3600 -1950 40  0000 L CNN
F 2 "~" H -3612 -1850 30  0000 C CNN
F 3 "~" H -3650 -1700 60  0000 C CNN
	1    -3650 -1700
	0    -1   -1   0   
$EndComp
$Comp
L C C12
U 1 1 53474D92
P -1350 -3150
F 0 "C12" H -1350 -3050 40  0000 L CNN
F 1 "1.5nF" H -1344 -3235 40  0000 L CNN
F 2 "~" H -1312 -3300 30  0000 C CNN
F 3 "~" H -1350 -3150 60  0000 C CNN
	1    -1350 -3150
	1    0    0    -1  
$EndComp
$Comp
L R R8
U 1 1 53474D98
P -1050 -3150
F 0 "R8" V -970 -3150 40  0000 C CNN
F 1 "1M" V -1043 -3149 40  0000 C CNN
F 2 "~" V -1120 -3150 30  0000 C CNN
F 3 "~" H -1050 -3150 30  0000 C CNN
	1    -1050 -3150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR17
U 1 1 53474D9E
P -1200 -2550
F 0 "#PWR17" H -1200 -2550 30  0001 C CNN
F 1 "GND" H -1200 -2620 30  0001 C CNN
F 2 "~" H -1200 -2550 60  0000 C CNN
F 3 "~" H -1200 -2550 60  0000 C CNN
	1    -1200 -2550
	1    0    0    -1  
$EndComp
$Comp
L FILTER FB1
U 1 1 53474DA4
P -300 -3200
F 0 "FB1" H -300 -3050 60  0000 C CNN
F 1 "BLM18PG121SN1" H -300 -3300 60  0000 C CNN
F 2 "~" H -300 -3200 60  0000 C CNN
F 3 "~" H -300 -3200 60  0000 C CNN
	1    -300 -3200
	1    0    0    -1  
$EndComp
$Comp
L AVDD #PWR23
U 1 1 53474DAA
P 100 -3250
F 0 "#PWR23" H 100 -3150 30  0001 C CNN
F 1 "AVDD" H 100 -3140 30  0000 C CNN
F 2 "~" H 100 -3250 60  0000 C CNN
F 3 "~" H 100 -3250 60  0000 C CNN
	1    100  -3250
	1    0    0    -1  
$EndComp
$Comp
L AVDD #PWR11
U 1 1 53474DB0
P -3650 -3150
F 0 "#PWR11" H -3650 -3050 30  0001 C CNN
F 1 "AVDD" H -3650 -3040 30  0000 C CNN
F 2 "~" H -3650 -3150 60  0000 C CNN
F 3 "~" H -3650 -3150 60  0000 C CNN
	1    -3650 -3150
	1    0    0    -1  
$EndComp
$Comp
L AVDD #PWR19
U 1 1 53474DB6
P -600 -2850
F 0 "#PWR19" H -600 -2750 30  0001 C CNN
F 1 "AVDD" H -600 -2740 30  0000 C CNN
F 2 "~" H -600 -2850 60  0000 C CNN
F 3 "~" H -600 -2850 60  0000 C CNN
	1    -600 -2850
	1    0    0    -1  
$EndComp
$Comp
L DVDD #PWR12
U 1 1 53474DBC
P -3500 -3150
F 0 "#PWR12" H -3500 -3050 30  0001 C CNN
F 1 "DVDD" H -3500 -3040 30  0000 C CNN
F 2 "~" H -3500 -3150 60  0000 C CNN
F 3 "~" H -3500 -3150 60  0000 C CNN
	1    -3500 -3150
	1    0    0    -1  
$EndComp
$Comp
L DVDD #PWR21
U 1 1 53474DC2
P 50 -2850
F 0 "#PWR21" H 50  -2750 30  0001 C CNN
F 1 "DVDD" H 50  -2740 30  0000 C CNN
F 2 "~" H 50  -2850 60  0000 C CNN
F 3 "~" H 50  -2850 60  0000 C CNN
	1    50   -2850
	1    0    0    -1  
$EndComp
NoConn ~ -1800 -1700
NoConn ~ -1800 -1600
$Comp
L TPD6E001RSE U1
U 1 1 53474DCA
P -5300 1300
F 0 "U1" H -5200 900 60  0000 L CNN
F 1 "TPD6E001RSE" H -5200 1700 60  0000 L CNN
F 2 "~" H -4700 1750 60  0000 C CNN
F 3 "~" H -4700 1750 60  0000 C CNN
	1    -5300 1300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR8
U 1 1 53474DD0
P -5300 1900
F 0 "#PWR8" H -5300 1900 30  0001 C CNN
F 1 "GND" H -5300 1830 30  0001 C CNN
F 2 "~" H -5300 1900 60  0000 C CNN
F 3 "~" H -5300 1900 60  0000 C CNN
	1    -5300 1900
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 53474DD6
P -5600 700
F 0 "C1" V -5550 750 40  0000 L CNN
F 1 "0.1uF" V -5550 450 40  0000 L CNN
F 2 "~" H -5562 550 30  0000 C CNN
F 3 "~" H -5600 700 60  0000 C CNN
	1    -5600 700 
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR1
U 1 1 53474DE0
P -5850 800
F 0 "#PWR1" H -5850 800 30  0001 C CNN
F 1 "GND" H -5850 730 30  0001 C CNN
F 2 "~" H -5850 800 60  0000 C CNN
F 3 "~" H -5850 800 60  0000 C CNN
	1    -5850 800 
	1    0    0    -1  
$EndComp
Text Notes -5900 2100 0    51   ~ 0
May need to rearrange channel/pin\nconnections on TVS diodes as\nconvenient for layout
Text Notes -6200 500  0    75   ~ 0
ESD Protection
$Comp
L AVDD #PWR7
U 1 1 53474DE8
P -5300 650
F 0 "#PWR7" H -5300 750 30  0001 C CNN
F 1 "AVDD" H -5300 760 30  0000 C CNN
F 2 "~" H -5300 650 60  0000 C CNN
F 3 "~" H -5300 650 60  0000 C CNN
	1    -5300 650 
	1    0    0    -1  
$EndComp
$Comp
L OPA376 U2
U 1 1 53474E41
P -5250 -300
F 0 "U2" H -5100 -500 60  0000 C CNN
F 1 "OPA376" H -4900 0   60  0000 C CNN
F 2 "SC70-5" H -4900 -100 60  0000 C CNN
F 3 "~" H -5150 0   60  0000 C CNN
	1    -5250 -300
	-1   0    0    -1  
$EndComp
$Comp
L R R5
U 1 1 53474E48
P -5550 -1050
F 0 "R5" V -5470 -1050 40  0000 C CNN
F 1 "51k" V -5543 -1049 40  0000 C CNN
F 2 "~" V -5620 -1050 30  0000 C CNN
F 3 "~" H -5550 -1050 30  0000 C CNN
	1    -5550 -1050
	0    1    -1   0   
$EndComp
Text Label -1800 -3100 0    51   ~ 0
RLDOUT
Text Label -1800 -3000 0    51   ~ 0
RLDIN
Text Label -1800 -2900 0    51   ~ 0
RLDINV
Text Label -4400 -1050 2    51   ~ 0
RLDOUT
$Comp
L GND #PWR10
U 1 1 53474E53
P -5250 50
F 0 "#PWR10" H -5250 50  30  0001 C CNN
F 1 "GND" H -5250 -20 30  0001 C CNN
F 2 "~" H -5250 50  60  0000 C CNN
F 3 "~" H -5250 50  60  0000 C CNN
	1    -5250 50  
	1    0    0    -1  
$EndComp
$Comp
L AVDD #PWR9
U 1 1 53474E59
P -5250 -650
F 0 "#PWR9" H -5250 -550 30  0001 C CNN
F 1 "AVDD" H -5250 -540 30  0000 C CNN
F 2 "~" H -5250 -650 60  0000 C CNN
F 3 "~" H -5250 -650 60  0000 C CNN
	1    -5250 -650
	1    0    0    -1  
$EndComp
Text Notes -6100 -1200 0    75   ~ 0
RLD/Shield Driver
Wire Wire Line
	-100 -2300 -100 -2350
Wire Wire Line
	-100 -2800 -100 -2750
Wire Wire Line
	-600 -2800 -600 -2850
Wire Wire Line
	-600 -2300 -600 -2250
Wire Wire Line
	-750 -2300 -750 -2350
Connection ~ -600 -2300
Wire Wire Line
	-750 -2800 -750 -2750
Wire Wire Line
	-450 -2800 -450 -2750
Connection ~ -600 -2800
Wire Wire Line
	-450 -2300 -450 -2350
Connection ~ 50   -2800
Wire Wire Line
	50   -2250 50   -2300
Connection ~ 50   -2300
Wire Wire Line
	-1800 -1300 -1750 -1300
Wire Wire Line
	-1750 -1300 -1750 -1050
Wire Wire Line
	-1800 -1400 -1500 -1400
Wire Wire Line
	-1500 -1400 -1500 -1050
Wire Wire Line
	-1750 -550 -1750 -650
Wire Wire Line
	-1500 -550 -1500 -650
Wire Wire Line
	-3950 -1400 -3950 -850
Connection ~ -3950 -1100
Wire Wire Line
	-3450 -1300 -3450 -550
Connection ~ -3450 -1100
Wire Wire Line
	-3500 -2900 -3400 -2900
Wire Wire Line
	-3450 -3000 -3400 -3000
Wire Wire Line
	-3450 -3000 -3450 -2750
Wire Wire Line
	-3400 -2800 -3450 -2800
Connection ~ -3450 -2800
Wire Wire Line
	-750 -2800 -450 -2800
Wire Wire Line
	-750 -2300 -450 -2300
Wire Wire Line
	-100 -2800 200  -2800
Wire Wire Line
	-100 -2300 200  -2300
Wire Wire Line
	-1750 -1800 -1800 -1800
Wire Wire Line
	-1750 -2000 -1800 -2000
Wire Wire Line
	-1750 -2100 -1800 -2100
Wire Wire Line
	-1750 -1900 -1800 -1900
Wire Wire Line
	-1800 -2400 -1750 -2400
Wire Wire Line
	-1800 -2200 -1750 -2200
Connection ~ -3450 -850
Wire Wire Line
	-3500 -1100 -3450 -1100
Wire Wire Line
	-3500 -850 -3450 -850
Wire Wire Line
	-3900 -1100 -3950 -1100
Wire Wire Line
	-3950 -850 -3900 -850
Wire Wire Line
	-3950 -1400 -3400 -1400
Wire Wire Line
	-3450 -1300 -3400 -1300
Wire Wire Line
	-3450 -1900 -3400 -1900
Wire Wire Line
	-3450 -1700 -3400 -1700
Wire Wire Line
	-3400 -1800 -3900 -1800
Wire Wire Line
	-3900 -1800 -3900 -1900
Wire Wire Line
	-3900 -1900 -3850 -1900
Wire Wire Line
	-3850 -1700 -3900 -1700
Wire Wire Line
	-3900 -1700 -3900 -1600
Wire Wire Line
	-3900 -1600 -3400 -1600
Wire Wire Line
	-1800 -3100 -1500 -3100
Wire Wire Line
	-1500 -3400 -1500 -3000
Wire Wire Line
	-1500 -3400 -1050 -3400
Wire Wire Line
	-1350 -3350 -1350 -3400
Connection ~ -1350 -3400
Wire Wire Line
	-1800 -2900 -1050 -2900
Wire Wire Line
	-1350 -2900 -1350 -2950
Connection ~ -1350 -2900
Wire Wire Line
	-1500 -3000 -1800 -3000
Connection ~ -1500 -3100
Wire Wire Line
	-1250 -2700 -1200 -2700
Wire Wire Line
	-1200 -2700 -1200 -2550
Wire Wire Line
	-1250 -2600 -1200 -2600
Connection ~ -1200 -2600
Wire Wire Line
	-700 -3200 -650 -3200
Wire Wire Line
	50   -3200 300  -3200
Wire Wire Line
	100  -3200 100  -3250
Wire Wire Line
	-700 -3250 -700 -3200
Wire Wire Line
	200  -2800 200  -2750
Wire Wire Line
	200  -2300 200  -2350
Wire Wire Line
	-3500 -2900 -3500 -3150
Wire Wire Line
	50   -2800 50   -2850
Wire Wire Line
	-5300 650  -5300 750 
Wire Wire Line
	-5300 1900 -5300 1850
Wire Wire Line
	-5900 1100 -5950 1100
Wire Wire Line
	-5900 1300 -5950 1300
Wire Wire Line
	-5900 1500 -5950 1500
Wire Wire Line
	-4650 1300 -4700 1300
Wire Wire Line
	-4650 1500 -4700 1500
Wire Wire Line
	-5300 700  -5400 700 
Connection ~ -5300 700 
Wire Wire Line
	-5850 800  -5850 700 
Wire Wire Line
	-5850 700  -5800 700 
Wire Notes Line
	-6300 2300 -4200 2300
Wire Notes Line
	-4200 350  -6300 350 
Wire Wire Line
	-3750 -2600 -3400 -2600
Wire Wire Line
	-3750 -2500 -3400 -2500
Wire Wire Line
	-3750 -2400 -3400 -2400
Wire Wire Line
	-3750 -2300 -3400 -2300
Wire Wire Line
	-5300 -1050 -4400 -1050
Wire Wire Line
	-5800 -1050 -6000 -1050
Wire Wire Line
	-5900 -300 -5800 -300
Wire Wire Line
	-4750 -1050 -4750 -400
Wire Wire Line
	-4750 -400 -4800 -400
Wire Wire Line
	-5850 -300 -5850 250 
Wire Wire Line
	-5850 250  -4750 250 
Wire Wire Line
	-4750 250  -4750 -200
Wire Wire Line
	-4750 -200 -4800 -200
Connection ~ -5850 -300
Wire Wire Line
	-5250 50   -5250 0   
Wire Wire Line
	-5250 -650 -5250 -600
Connection ~ -4750 -1050
Wire Notes Line
	-6300 -1350 -4350 -1350
Wire Notes Line
	-4350 -1350 -4350 300 
Wire Notes Line
	-4350 300  -6300 300 
Wire Notes Line
	-6300 300  -6300 -1350
Wire Notes Line
	-6300 350  -6300 2300
Wire Notes Line
	-4200 2300 -4200 350 
Wire Wire Line
	-3400 -3100 -3650 -3100
Wire Wire Line
	-3650 -3150 -3650 -2100
Wire Wire Line
	-3650 -2100 -3400 -2100
Connection ~ -3650 -3100
Wire Wire Line
	-3400 -2200 -3650 -2200
Connection ~ -3650 -2200
Wire Wire Line
	-4650 1100 -4700 1100
$Comp
L DVDD #PWR18
U 1 1 53474EF4
P -700 -3250
F 0 "#PWR18" H -700 -3150 30  0001 C CNN
F 1 "DVDD" H -700 -3140 30  0000 C CNN
F 2 "~" H -700 -3250 60  0000 C CNN
F 3 "~" H -700 -3250 60  0000 C CNN
	1    -700 -3250
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG1
U 1 1 53474EFA
P 300 -3250
F 0 "#FLG1" H 300 -3155 30  0001 C CNN
F 1 "PWR_FLAG" H 300 -3070 30  0000 C CNN
F 2 "~" H 300 -3250 60  0000 C CNN
F 3 "~" H 300 -3250 60  0000 C CNN
	1    300  -3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	300  -3200 300  -3250
Connection ~ 100  -3200
$Comp
L R R6
U 1 1 53474F02
P -1500 -2700
F 0 "R6" V -1420 -2700 40  0000 C CNN
F 1 "10k" V -1493 -2699 40  0000 C CNN
F 2 "~" V -1570 -2700 30  0000 C CNN
F 3 "~" H -1500 -2700 30  0000 C CNN
	1    -1500 -2700
	0    -1   -1   0   
$EndComp
$Comp
L R R7
U 1 1 53474F08
P -1500 -2600
F 0 "R7" V -1420 -2600 40  0000 C CNN
F 1 "10k" V -1493 -2599 40  0000 C CNN
F 2 "~" V -1570 -2600 30  0000 C CNN
F 3 "~" H -1500 -2600 30  0000 C CNN
	1    -1500 -2600
	0    -1   1    0   
$EndComp
Wire Wire Line
	-1750 -2700 -1800 -2700
Wire Wire Line
	-1750 -2600 -1800 -2600
Text GLabel -3750 -2600 0    51   Input ~ 0
CH1N
Text GLabel -3750 -2500 0    51   Input ~ 0
CH1P
Text GLabel -3750 -2400 0    51   Input ~ 0
CH2N
Text GLabel -3750 -2300 0    51   Input ~ 0
CH2P
Text GLabel 0    -4500 0    51   Input ~ 0
CH1P
Text GLabel 0    -4400 0    51   Input ~ 0
CH2P
Text GLabel 50   -3900 0    51   Input ~ 0
SHIELD
Text GLabel 1550 -4500 2    51   Input ~ 0
CH1N
Text GLabel 1550 -4400 2    51   Input ~ 0
CH2N
Text GLabel 1600 -4000 2    51   Input ~ 0
RLD
Text GLabel 2750 -4200 0    51   Input ~ 0
SCLK
Text GLabel 2750 -4100 0    51   Input ~ 0
MOSI
Text GLabel 2750 -4000 0    51   Input ~ 0
MISO
Text GLabel 2850 -3800 0    51   Input ~ 0
~AFE_CS
Text GLabel 2950 -3700 0    51   Input ~ 0
~AFE_DRDY
Text GLabel 2950 -3600 0    51   Input ~ 0
~AFE_PWDN
Text GLabel -1750 -2400 2    51   Input ~ 0
~AFE_PWDN
Text GLabel -1750 -2200 2    51   Output ~ 0
~AFE_DRDY
Text GLabel -1750 -2100 2    51   Output ~ 0
MISO
Text GLabel -1750 -2000 2    51   Input ~ 0
SCLK
Text GLabel -1750 -1900 2    51   Input ~ 0
MOSI
Text GLabel -1750 -1800 2    51   Input ~ 0
~AFE_CS
Text GLabel -6000 -1050 0    51   Output ~ 0
RLD
Text GLabel -5900 -300 0    51   Output ~ 0
SHIELD
Text GLabel -5950 1100 0    51   Input ~ 0
CH1N
Text GLabel -5950 1300 0    51   Input ~ 0
CH1P
Text GLabel -5950 1500 0    51   Input ~ 0
CH2N
Text GLabel -4650 1500 2    51   Input ~ 0
CH2P
Text GLabel -4650 1300 2    51   Input ~ 0
RLD
Text GLabel -4650 1100 2    51   Input ~ 0
SHIELD
$EndSCHEMATC
