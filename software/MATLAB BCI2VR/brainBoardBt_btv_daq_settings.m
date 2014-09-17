function out=brainBoardBt_btv_daq_settings
%% Brain-Computer Interface to Virtual reality (BCI2VR)
%% Example Setup for EEG amplifier using ADS1299 and MPU6050 accelerometer and gyrosensor via Bluetooth
%% Ver. 1.0 02/25/2013 Copyright by Ou Bai
%% You may add new variables, but you should not change the current variable names.
%%
out.subject.Name                    =   'BCI2VR';
out.subject.DateOfBirth             =   '01-Jan-2014';
out.subject.Sex                     =   'M';
out.subject.Handness                =   'Right';
out.subject.Investigator            =   'BCI2VR Boy';
out.subject.Physician               =   'VCU';
out.subject.DateOfRecording         =   date;
%%
out.daq.HardwareName                =   'brainBoardBt';                                 %% For ads1299-based device only
out.daq.DeviceSerials               =   1;                                              %% how many serial ports (currently only 1 is supported)
out.daq.Ports{1}                    =   1:14;                                           %% Check firmware: 8 for one ADS1299, 6 for one MPU6050
out.daq.Channels{1}                 =   1:length(out.daq.Ports{1});                     %% Signal channels
out.daq.UsedChannels{1}             =   logical([1 1 1 1 1 1 1 1 1 1 1 1 1 1]);         %% Used signal channels for each device: default 1; 0 will be turned off
out.daq.ChannelGain{1}              =   24*ones(1,length(out.daq.Ports{1}));            %% Gain for each ads1299 channel: 1,2,4,6,8,12,24(default)
out.daq.ElectrodeMontage            =   'singleEnded';                                 %% 'singleEnded' or 'Differential'
out.daq.bbBt.RemoteName             =   'RNBT-B312';                                 %% Bluetooth device name, leave it empty if don't know 'RNBT-2427'
out.daq.bbBt.RemoteID               =   '0006666AB312';                                 %% The Mac no. CHeck your Bluetooth device; If you know the Bluetooth device name, you may leave this empty '000666662427'
out.daq.bbBt.BtChannel              =   1;                                              %% You may use matlab function  instrhwinfo to check this
out.daq.bbBt.InputBufferSize        =   4096;                                           %% must be larger than PacketLen * daq.SamplesAcquiredFcnCount
out.daq.bbBt.Headers                =   [165 90];                                       %% Check firmwire: ads device has 3 status types starting with the two headers and one byte of packet index
out.daq.bbBt.PacketLen              =   42;                                             %% Check firmwire: 2 header bytes + 1 packet Number + 3 status bytes + 8 (eegChannel) * 3 bytes (24 bits) + 6 (ax, ay, az, gx, gy, gz) * 2 bytes
out.daq.bbBt.bitResolution          =   [(4.5/(2^23-1))*(ones(1,8)/24) 3.3/(2^15-1)*ones(1,6)];     %% V_REF/(Analog_gain*(2^(ADC_bits-1)-1)) for each channel
%%
%%
out.chan.locs                       =   btv_readlocs( 'brainBoardBt_14Ch.loc' );        %% Must be a loc file
out.chan.Calibration                =   ones(14,1);                                     %% Always 1 for open EEG interface (Hardware has adjusted channel difference), may change in the future
%%
out.daq.SamplingRate                =   250;                                            %% ads1299 sampling at 250Hz; only 250 Hz is supported
out.daq.SamplesAcquiredFcnCount     =   20;                                             %% Buffer update when 2000/125 (ads Sampling Rate/minmal sampling rate output) bytes available 
out.buf.BufferLength                =   5000;                                           %% Must be larger than 10*max(xscale)*SamplingRate, a large number will significant affect the processing speed
out.buf.cntDataLength               =   60*60*out.daq.SamplingRate;                     %% Reserve space for save continuous data
%%
out.view.UpdateTimeInterval         =   0.5;                                            %% in second, too short may cause delay for online calculation
out.view.LineColor                  =   ['m';'r';'b';'k'];                              %% 'k' will be black color for all channels 
out.view.Channels                   =   1:14;                                           %% Combine channels in multiple amplifiers, must be in avaible signal channels
out.view.XScaleUnit                 =   1;                                              %% Horizontal view: initial view range
out.view.xscale                     =   [0.01:0.01:0.09 0.1:0.1:0.9 1.0:1.0:5.0];       %% Horizontal view: adjustable range for offline use only
out.view.YUnit                      =   [10^6*ones(1,8) 10^2*ones(1,6)];                %% Convert to uV, length MUST be the same as view.Channels
out.view.ChannelYAxis               =   200;                                            %% Set Y axis range
%%
out.trigger.DetectTimeInterval      =   0.1;                                            %% Detection Interval for Triggers from Stim/VR/Daq
%%
out.daq.NotchFilter.bandStopFreq      = [];                                             %% bandstop Frequency range (frequency component removal); [] for none. 
out.daq.NotchFilter.order             = 2;                                              %% Butterworth filter order  
out.daq.LowpassFilter.Freq            = [];                                             %% Lowpass frequency; [] for none  
out.daq.LowpassFilter.order           = 2;                                              %% Butterworth filter order;  
out.daq.HighpassFilter.Freq           = [];                                             %% High pass frequency; [] for none.
out.daq.HighpassFilter.order          = 2;                                              %% Butterworth filter order 
%% 
return
