eeg
===

Software, firmware, and CAD files for Brainboard, a mobile EEG project.
Brainboard interfaces with a custom MATLAB program, BCI2VR
(http://www.engineering.vcu.edu/eegbci/bci2vr_index.html), via Bluetooth.
The core hardware components are the ADS1299 EEG front-end from Texas
Instruments, the AT32UC3L064 microcontroller from Atmel, the MPU-6050 
6-axis inertial measurement unit from InvenSense, and the RN-42 Bluetooth
2.1 module from Microchip/Roving Networks.

Current firmware is for the Atmel AT32UC3L064, a 32-bit AVR microcontroller.
Some experimentation was done with on-line digital processing of the acquired
EEG signals from within the AVR firmware, but this is currently incomplete.
The microcontroller acts as a dumb pipe shuttling 8 channels of 24-bit data
from the ADS1299 and 6 channels of 16-bit data from the MPU-6050, where they
are displayed using the BCI2VR MATLAB GUI. Firmware is flashed to the MCU using
a 10-pin JTAG connection and the AVR Dragon (or other AVR32-compatible
programmer). Code was developed using the free Atmel Studio IDE.

The project predates my use of a proper versioning system, and thus
the hardware folder contains designs (some old, some WIP) for the Brainboard. 
***The most recent complete version is brainboard_wearable_v2.***
Older versions may be viewed for completeness's sake and/or compared with the
thesis document for a full picture of the design process.
The folders "muscleboard" and "brainboard_v2" are works in progress. The former
is meant for single-channel EMG acquisition and acceleration measurements in a
small, 3-electrode form factor that can be mounted on a disposable electrode pad.
This will probably ultimately be moved to its own repository. Brainboard v2 is a
BeagleBone Black cape that takes the BBB support from earlier versions of the
Brainboard and re-implements it in a more elegant fashion (see thesis for details).

TO USE THE BCI2VR GUI (assumes you already have MATLAB r2011b or later):
1. Go to the link above and download the required MATLAB files for the basic BCI2VR functionality.
2. Copy btv_brainBoardBtFun.m into the functions folder.
3. Copy brainBoardBt_btv_daq_settings.m and brainBoardBt_14Ch.loc into the setup_files folder.
4. Make sure the toolbox is added to your path. Run bci2vr.m and click "Data Acquisition." 
5. Click the "Load setup file" icon and load brainBoardBt_btv_daq_settings.
6. The program will attempt to open a Bluetooth connection with BrainBoard. It occasionally fails; in this case simply close the GUI and try again.
7. Click the "Play" icon to begin recording.
For more info, check Dr. Ou Bai's documentation for BCI2VR, included with the toolbox.