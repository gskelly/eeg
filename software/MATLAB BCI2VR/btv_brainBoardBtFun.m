function btv_brainBoardBtFun(varargin)
% Brain-Computer Interface to Virtual reality (BCI2VR)
% Functions for interfacing with Brainboard LW 
% (ADS1299 + MPU6050 Bluetooth device)
% Ver. 1.1 12-18-2013 Copyright by Ou Bai

global btv;
if btv.flag.daq~=1, 
    errordlg('You must open DAQ interface before initializing data acquisition objects.');
    return
end

if nargin == 0,
elseif ischar(varargin{1}) % INVOKE NAMED SUBFUNCTION OR CALLBACK
    %     try
    eval([ 'feval(' varargin{:} ');']);
    %     catch
    %         errordlg(lasterr,['BCI2VR: ' mfilename]);
    %         daq_stop;
    %     end
end
return

%% GUI and ADS1299 initialization
function deviceInit %#ok<DEFNU>
global btv;
[~,~,endian] = computer;
btv.daq.settings.daq.endian = endian;

%% ADS1299 constants list:
% Opcodes
btv.daq.settings.daq.adsOpc.ID = uint8(hex2dec('00'));     %% Device ID
btv.daq.settings.daq.adsOpc.RESET = uint8(hex2dec('06'));
btv.daq.settings.daq.adsOpc.START = uint8(hex2dec('08'));
btv.daq.settings.daq.adsOpc.STOP = uint8(hex2dec('0a'));
btv.daq.settings.daq.adsOpc.SDATAC = uint8(hex2dec('11'));
btv.daq.settings.daq.adsOpc.RDATAC = uint8(hex2dec('10'));
% CONFIG1 register
btv.daq.settings.daq.adsOpc.CONFIG1 = uint8(hex2dec('01'));
btv.daq.settings.daq.adsOpc.CONFIG1_const = uint8(hex2dec('90')); %a
btv.daq.settings.daq.adsOpc.DAISY_EN = uint8(hex2dec('40')); %a
btv.daq.settings.daq.adsOpc.CLK_EN = uint8(hex2dec('20')); %a
btv.daq.settings.daq.adsOpc.DR2 = uint8(hex2dec('04'));
btv.daq.settings.daq.adsOpc.DR1 = uint8(hex2dec('02'));
btv.daq.settings.daq.adsOpc.DR0 = uint8(hex2dec('01'));
btv.daq.settings.daq.adsOpc.RES_16k_SPS = uint8(btv.daq.settings.daq.adsOpc.CONFIG1_const); %a
btv.daq.settings.daq.adsOpc.RES_8k_SPS = uint8(btv.daq.settings.daq.adsOpc.CONFIG1_const+ btv.daq.settings.daq.adsOpc.DR0); %a
btv.daq.settings.daq.adsOpc.RES_4k_SPS = uint8(btv.daq.settings.daq.adsOpc.CONFIG1_const+ btv.daq.settings.daq.adsOpc.DR1); %a
btv.daq.settings.daq.adsOpc.RES_2k_SPS = uint8(btv.daq.settings.daq.adsOpc.CONFIG1_const+ btv.daq.settings.daq.adsOpc.DR1+ btv.daq.settings.daq.adsOpc.DR0); %a
btv.daq.settings.daq.adsOpc.RES_1k_SPS = uint8(btv.daq.settings.daq.adsOpc.CONFIG1_const+ btv.daq.settings.daq.adsOpc.DR2); %a
btv.daq.settings.daq.adsOpc.RES_500_SPS = uint8(btv.daq.settings.daq.adsOpc.CONFIG1_const+ btv.daq.settings.daq.adsOpc.DR2+ btv.daq.settings.daq.adsOpc.DR0); %a
btv.daq.settings.daq.adsOpc.RES_250_SPS = uint8(btv.daq.settings.daq.adsOpc.CONFIG1_const+ btv.daq.settings.daq.adsOpc.DR2+ btv.daq.settings.daq.adsOpc.DR1); %a
% CONFIG2 register 
btv.daq.settings.daq.adsOpc.CONFIG2 = uint8(hex2dec('02'));
btv.daq.settings.daq.adsOpc.CONFIG_const = uint8(hex2dec('C0'));
btv.daq.settings.daq.adsOpc.INT_TEST = uint8(hex2dec('10')); %a
btv.daq.settings.daq.adsOpc.TEST_AMP = uint8(hex2dec('04')); %double amplitude of test signal to 2 x -(VREFP-VREFN)/2.4 mV
btv.daq.settings.daq.adsOpc.TEST_FREQ1 = uint8(hex2dec('02'));
btv.daq.settings.daq.adsOpc.TEST_FREQ0 = uint8(hex2dec('01'));
btv.daq.settings.daq.adsOpc.INT_TEST_4HZ_AMP = uint8(btv.daq.settings.daq.adsOpc.CONFIG_const + btv.daq.settings.daq.adsOpc.INT_TEST + btv.daq.settings.daq.adsOpc.TEST_AMP); %Pulsed at 1Hz (0.98) %a
btv.daq.settings.daq.adsOpc.INT_TEST_8HZ_AMP = uint8(btv.daq.settings.daq.adsOpc.CONFIG_const + btv.daq.settings.daq.adsOpc.INT_TEST + btv.daq.settings.daq.adsOpc.TEST_FREQ1 + btv.daq.settings.daq.adsOpc.TEST_AMP);  %Pulsed at 2 Hz (1.95) %a
btv.daq.settings.daq.adsOpc.INT_TEST_DC_AMP = uint8(btv.daq.settings.daq.adsOpc.CONFIG_const + btv.daq.settings.daq.adsOpc.INT_TEST + + btv.daq.settings.daq.adsOpc.TEST_FREQ1 + btv.daq.settings.daq.adsOpc.TEST_FREQ0 + btv.daq.settings.daq.adsOpc.TEST_AMP);  %Pulsed at 2 Hz (1.95) %a
% CONFIG3 register
btv.daq.settings.daq.adsOpc.CONFIG3 = uint8(hex2dec('03'));
btv.daq.settings.daq.adsOpc.CONFIG3_const = uint8(hex2dec('60'));
btv.daq.settings.daq.adsOpc.PD_REFBUF = uint8(hex2dec('80'));
btv.daq.settings.daq.adsOpc.BIASREF_INT = uint8(hex2dec('08'));
btv.daq.settings.daq.adsOpc.PD_BIAS = uint8(hex2dec('04'));
btv.daq.settings.daq.adsOpc.BIAS_LOFF_SENS = uint8(hex2dec('02'));
btv.daq.settings.daq.adsOpc.EXT_REF_EXT_BIAS = uint8(btv.daq.settings.daq.adsOpc.CONFIG3_const);
% LOFF (lead-off) register 
btv.daq.settings.daq.adsOpc.CONFIGLEADOFF = uint8(hex2dec('04'));
btv.daq.settings.daq.adsOpc.LEADOFF_const = uint8(hex2dec('00'));
btv.daq.settings.daq.adsOpc.COMP_TH2 = uint8(hex2dec('80'));
btv.daq.settings.daq.adsOpc.COMP_TH1 = uint8(hex2dec('40'));
btv.daq.settings.daq.adsOpc.COMP_TH0 = uint8(hex2dec('20'));
btv.daq.settings.daq.adsOpc.VLEAD_OFF_EN = uint8(hex2dec('10'));
btv.daq.settings.daq.adsOpc.ILEAD_OFF1 = uint8(hex2dec('08'));
btv.daq.settings.daq.adsOpc.ILEAD_OFF0 = uint8(hex2dec('04'));
btv.daq.settings.daq.adsOpc.FLEAD_OFF1 = uint8(hex2dec('02'));
btv.daq.settings.daq.adsOpc.FLEAD_OFF0 = uint8(hex2dec('01'));
btv.daq.settings.daq.adsOpc.COMP_TH_95 = uint8(hex2dec('00'));
btv.daq.settings.daq.adsOpc.COMP_TH_92_5 = btv.daq.settings.daq.adsOpc.COMP_TH0;
btv.daq.settings.daq.adsOpc.COMP_TH_90 = btv.daq.settings.daq.adsOpc.COMP_TH1;
btv.daq.settings.daq.adsOpc.COMP_TH_87_5 = (btv.daq.settings.daq.adsOpc.COMP_TH1 + btv.daq.settings.daq.adsOpc.COMP_TH0);
btv.daq.settings.daq.adsOpc.COMP_TH_85 = btv.daq.settings.daq.adsOpc.COMP_TH2;
btv.daq.settings.daq.adsOpc.COMP_TH_80 = (btv.daq.settings.daq.adsOpc.COMP_TH2 + btv.daq.settings.daq.adsOpc.COMP_TH0);
btv.daq.settings.daq.adsOpc.COMP_TH_75 = (btv.daq.settings.daq.adsOpc.COMP_TH2 + btv.daq.settings.daq.adsOpc.COMP_TH1);
btv.daq.settings.daq.adsOpc.COMP_TH_70 = (btv.daq.settings.daq.adsOpc.COMP_TH2 + btv.daq.settings.daq.adsOpc.COMP_TH1 + btv.daq.settings.daq.adsOpc.COMP_TH0);
btv.daq.settings.daq.adsOpc.ILEAD_OFF_6nA = uint8(hex2dec('00'));
btv.daq.settings.daq.adsOpc.ILEAD_OFF_12nA = btv.daq.settings.daq.adsOpc.ILEAD_OFF0;
btv.daq.settings.daq.adsOpc.ILEAD_OFF_18nA = btv.daq.settings.daq.adsOpc.ILEAD_OFF1;
btv.daq.settings.daq.adsOpc.ILEAD_OFF_24nA = (btv.daq.settings.daq.adsOpc.ILEAD_OFF1 + btv.daq.settings.daq.adsOpc.ILEAD_OFF0);
btv.daq.settings.daq.adsOpc.FLEAD_OFF_AC = btv.daq.settings.daq.adsOpc.FLEAD_OFF0;
btv.daq.settings.daq.adsOpc.FLEAD_OFF_DC = (btv.daq.settings.daq.adsOpc.FLEAD_OFF1 + btv.daq.settings.daq.adsOpc.FLEAD_OFF0);
% CHnSET individual channel settings (n = 1:8)
% Address = 05h to 0Ch
btv.daq.settings.daq.adsOpc.CONFIGCHnSET = uint8(hex2dec('05'));
btv.daq.settings.daq.adsOpc.PDn = uint8(hex2dec('80')); %power down amplifier
btv.daq.settings.daq.adsOpc.GAINn2 = uint8(hex2dec('40'));
btv.daq.settings.daq.adsOpc.GAINn1 = uint8(hex2dec('20'));
btv.daq.settings.daq.adsOpc.GAINn0 = uint8(hex2dec('10'));
btv.daq.settings.daq.adsOpc.GAIN_1X = uint8(hex2dec('00'));
btv.daq.settings.daq.adsOpc.GAIN_2X = uint8(btv.daq.settings.daq.adsOpc.GAINn0);
btv.daq.settings.daq.adsOpc.GAIN_4X = uint8(btv.daq.settings.daq.adsOpc.GAINn1);
btv.daq.settings.daq.adsOpc.GAIN_6X = uint8(btv.daq.settings.daq.adsOpc.GAINn1+ btv.daq.settings.daq.adsOpc.GAINn0);
btv.daq.settings.daq.adsOpc.GAIN_8X = uint8(btv.daq.settings.daq.adsOpc.GAINn2);
btv.daq.settings.daq.adsOpc.GAIN_12X = uint8(btv.daq.settings.daq.adsOpc.GAINn2+ btv.daq.settings.daq.adsOpc.GAINn0);
btv.daq.settings.daq.adsOpc.GAIN_24X = uint8(btv.daq.settings.daq.adsOpc.GAINn2+ btv.daq.settings.daq.adsOpc.GAINn1);
btv.daq.settings.daq.adsOpc.MUXn2 = uint8(hex2dec('04'));
btv.daq.settings.daq.adsOpc.MUXn1 = uint8(hex2dec('02'));
btv.daq.settings.daq.adsOpc.MUXn0 = uint8(hex2dec('01'));
btv.daq.settings.daq.adsOpc.TEST_SIGNAL = uint8(btv.daq.settings.daq.adsOpc.MUXn2+ btv.daq.settings.daq.adsOpc.MUXn0);
btv.daq.settings.daq.adsOpc.ELECTRODE_INPUT = uint8(hex2dec('00'));
btv.daq.settings.daq.adsOpc.SHORTED = uint8(btv.daq.settings.daq.adsOpc.MUXn0);
% BIAS_SENSP: Bias Drive Positive Sense Selection
btv.daq.settings.daq.adsOpc.CONFIGBIAS_SENSP = uint8(hex2dec('0D'));
btv.daq.settings.daq.adsOpc.BIAS_SENSP_const = uint8(hex2dec('00'));
btv.daq.settings.daq.adsOpc.BIAS8P = uint8(hex2dec('80'));
btv.daq.settings.daq.adsOpc.BIAS7P = uint8(hex2dec('40'));
btv.daq.settings.daq.adsOpc.BIAS6P = uint8(hex2dec('20'));
btv.daq.settings.daq.adsOpc.BIAS5P = uint8(hex2dec('10'));
btv.daq.settings.daq.adsOpc.BIAS4P = uint8(hex2dec('08'));
btv.daq.settings.daq.adsOpc.BIAS3P = uint8(hex2dec('04'));
btv.daq.settings.daq.adsOpc.BIAS2P = uint8(hex2dec('02'));
btv.daq.settings.daq.adsOpc.BIAS1P = uint8(hex2dec('01'));
% BIAS_SENSN: Bias Drive Negative Sense Selection
btv.daq.settings.daq.adsOpc.CONFIGBIAS_SENSN = uint8(hex2dec('0E'));
btv.daq.settings.daq.adsOpc.BIAS_SENSN_const = uint8(hex2dec('00'));
btv.daq.settings.daq.adsOpc.BIAS8N = uint8(hex2dec('80'));
btv.daq.settings.daq.adsOpc.BIAS7N = uint8(hex2dec('40'));
btv.daq.settings.daq.adsOpc.BIAS6N = uint8(hex2dec('20'));
btv.daq.settings.daq.adsOpc.BIAS5N = uint8(hex2dec('10'));
btv.daq.settings.daq.adsOpc.BIAS4N = uint8(hex2dec('08'));
btv.daq.settings.daq.adsOpc.BIAS3N = uint8(hex2dec('04'));
btv.daq.settings.daq.adsOpc.BIAS2N = uint8(hex2dec('02'));
btv.daq.settings.daq.adsOpc.BIAS1N = uint8(hex2dec('01'));
% LOFF_SENSP: Lead Off Positive Sense Selection
btv.daq.settings.daq.adsOpc.CONFIGLOFF_SENSP = uint8(hex2dec('0F'));
btv.daq.settings.daq.adsOpc.LOFF_SENSP_const = uint8(hex2dec('00'));
btv.daq.settings.daq.adsOpc.LOFF8P = uint8(hex2dec('80'));
btv.daq.settings.daq.adsOpc.LOFF7P = uint8(hex2dec('40'));
btv.daq.settings.daq.adsOpc.LOFF6P = uint8(hex2dec('20'));
btv.daq.settings.daq.adsOpc.LOFF5P = uint8(hex2dec('10'));
btv.daq.settings.daq.adsOpc.LOFF4P = uint8(hex2dec('08'));
btv.daq.settings.daq.adsOpc.LOFF3P = uint8(hex2dec('04'));
btv.daq.settings.daq.adsOpc.LOFF2P = uint8(hex2dec('02'));
btv.daq.settings.daq.adsOpc.LOFF1P = uint8(hex2dec('01'));
% LOFF_SENSN: Lead Off Negative Sense Selection
btv.daq.settings.daq.adsOpc.CONFIGLOFF_SENSN = uint8(hex2dec('10'));
btv.daq.settings.daq.adsOpc.LOFF_SENSN_const = uint8(hex2dec('00'));
btv.daq.settings.daq.adsOpc.LOFF8N = uint8(hex2dec('80'));
btv.daq.settings.daq.adsOpc.LOFF7N = uint8(hex2dec('40'));
btv.daq.settings.daq.adsOpc.LOFF6N = uint8(hex2dec('20'));
btv.daq.settings.daq.adsOpc.LOFF5N = uint8(hex2dec('10'));
btv.daq.settings.daq.adsOpc.LOFF4N = uint8(hex2dec('08'));
btv.daq.settings.daq.adsOpc.LOFF3N = uint8(hex2dec('04'));
btv.daq.settings.daq.adsOpc.LOFF2N = uint8(hex2dec('02'));
btv.daq.settings.daq.adsOpc.LOFF1N = uint8(hex2dec('01'));
% LOFF_FLIP: Lead Off Current Direction Control
btv.daq.settings.daq.adsOpc.CONFIGLOFF_FLIP = uint8(hex2dec('11'));
btv.daq.settings.daq.adsOpc.LOFF_FLIP_const = uint8(hex2dec('00'));
btv.daq.settings.daq.adsOpc.LOFF_FLIP8 = uint8(hex2dec('80'));
btv.daq.settings.daq.adsOpc.LOFF_FLIP7 = uint8(hex2dec('40'));
btv.daq.settings.daq.adsOpc.LOFF_FLIP6 = uint8(hex2dec('20'));
btv.daq.settings.daq.adsOpc.LOFF_FLIP5 = uint8(hex2dec('10'));
btv.daq.settings.daq.adsOpc.LOFF_FLIP4 = uint8(hex2dec('08'));
btv.daq.settings.daq.adsOpc.LOFF_FLIP3 = uint8(hex2dec('04'));
btv.daq.settings.daq.adsOpc.LOFF_FLIP2 = uint8(hex2dec('02'));
btv.daq.settings.daq.adsOpc.LOFF_FLIP1 = uint8(hex2dec('01'));
% LOFF_STATP: Lead-Off Positive Input Status (read only)
btv.daq.settings.daq.adsOpc.CONFIGLOFF_STATP = uint8(hex2dec('12'));
% LOFF_STATN: Lead-Off Negative Input Status (read only)
btv.daq.settings.daq.adsOpc.CONFIGLOFF_STATN = uint8(hex2dec('13'));
% GPIO: General-Purpose I/O Register
btv.daq.settings.daq.adsOpc.CONFIGGPIO = uint8(hex2dec('14'));   %% For GPIO settting
btv.daq.settings.daq.adsOpc.GPIO_const = uint8(hex2dec('00'));
btv.daq.settings.daq.adsOpc.GPIOD4 = uint8(hex2dec('80'));
btv.daq.settings.daq.adsOpc.GPIOD3 = uint8(hex2dec('40'));
btv.daq.settings.daq.adsOpc.GPIOD2 = uint8(hex2dec('20'));
btv.daq.settings.daq.adsOpc.GPIOD1 = uint8(hex2dec('10'));
btv.daq.settings.daq.adsOpc.GPIOC4 = uint8(hex2dec('08'));
btv.daq.settings.daq.adsOpc.GPIOC3 = uint8(hex2dec('04'));
btv.daq.settings.daq.adsOpc.GPIOC2 = uint8(hex2dec('02'));
btv.daq.settings.daq.adsOpc.GPIOC1 = uint8(hex2dec('01'));
% MISC1: Miscellaneous 1
btv.daq.settings.daq.adsOpc.CONFIGMISC1 = uint8(hex2dec('15'));
btv.daq.settings.daq.adsOpc.MISC1_const = uint8(hex2dec('00'));
btv.daq.settings.daq.adsOpc.SRB1 = uint8(hex2dec('20'));
% MISC2: Miscellaneous 2 (must be 0)
btv.daq.settings.daq.adsOpc.CONFIGMISC2 = uint8(hex2dec('16'));
btv.daq.settings.daq.adsOpc.MISC2_const = uint8(hex2dec('00'));
% CONFIG4: Configuration Register 4
btv.daq.settings.daq.adsOpc.CONFIG4 = uint8(hex2dec('17'));
btv.daq.settings.daq.adsOpc.CONFIG4_const = uint8(hex2dec('00'));
btv.daq.settings.daq.adsOpc.SINGLE_SHOT = uint8(hex2dec('08'));
btv.daq.settings.daq.adsOpc.PD_LOFF_COMP = uint8(hex2dec('02'));   

%% Create and open Bluetooth connection
instrVer=ver('instrument');
if ~isempty(instrVer)
    if str2double(instrVer.Version) < 3.0
        errordlg('The MATLAB Instrument Control Toolbox version must be 3.0 (MATLAB 2011b) or later.');
        return
    end
else
    errordlg('The MATLAB Instrument Control Toolbox is required!');
    return
end

hdl=waitbar(0,'Connecting Bluetooth device, please wait...','Name','BCI2VR');
drawnow;

if ~isempty(btv.daq.settings.daq.bbBt.RemoteName)
    btv.hdl.daq.bbBt=Bluetooth(btv.daq.settings.daq.bbBt.RemoteName,btv.daq.settings.daq.bbBt.BtChannel);
elseif ~isempty(btv.daq.settings.daq.bbBt.RemoteID)
    btv.hdl.daq.bbBt=Bluetooth(['btspp://' btv.daq.settings.daq.bbBt.RemoteID],btv.daq.settings.daq.bbBt.BtChannel);
end

waitbar(1/3);

if isempty(btv.hdl.daq.bbBt)
    close(hdl);
    error('The assigned device is NOT found!');
end
%
set(btv.hdl.daq.bbBt,'InputBufferSize',btv.daq.settings.daq.bbBt.InputBufferSize);
set(btv.hdl.daq.bbBt,'ReadAsyncMode','continuous'); % the serial port object continuously queries the device to determine if data is available to be read
set(btv.hdl.daq.bbBt,'BytesAvailableFcnCount',btv.daq.settings.daq.SamplesAcquiredFcnCount*btv.daq.settings.daq.bbBt.PacketLen);
set(btv.hdl.daq.bbBt,'BytesAvailableFcnMode','Byte','BytesAvailableFcn',[mfilename '(''@btvBufUpdate'')']);

waitbar(2/3);

try
    fopen(btv.hdl.daq.bbBt);
    waitbar(3/3);
    close(hdl);
    hdl = waitbar(0,'Configuring data acquisition...','Name','BCI2VR');
catch errCode
    close(hdl);
    errordlg(errCode.message,'BCI2VR');
    return
end
while get(btv.hdl.daq.bbBt,'BytesAvailable') > 0, %Flush serial input buffer
    fread(btv.hdl.daq.bbBt,get(btv.hdl.daq.bbBt,'BytesAvailable'));
end

%% Set up ADS1299 device

deviceStop; %stop ADS1299 sampling
pause(0.1); %delay probably not required, but we need to be in SDATAC to change setup

adsWriteReg(btv.daq.settings.daq.adsOpc.CONFIGGPIO, uint8(hex2dec('00')));
waitbar(1/15);
% Register CONFIG1 sets sample rate, daisy-chain, resolution (high vs power saving) and CLK connection
switch btv.daq.settings.daq.SamplingRate
%     case 16000
%         adsWriteReg(btv.daq.settings.daq.adsOpc.CONFIG1,btv.daq.settings.daq.adsOpc.RES_16k_SPS);
%     case 8000
%         adsWriteReg(btv.daq.settings.daq.adsOpc.CONFIG1,btv.daq.settings.daq.adsOpc.RES_8k_SPS);
%     case 4000
%         adsWriteReg(btv.daq.settings.daq.adsOpc.CONFIG1,btv.daq.settings.daq.adsOpc.RES_4k_SPS);
%     case 2000
%         adsWriteReg(btv.daq.settings.daq.adsOpc.CONFIG1,btv.daq.settings.daq.adsOpc.RES_2k_SPS);
%     case 1000
%         adsWriteReg(btv.daq.settings.daq.adsOpc.CONFIG1,btv.daq.settings.daq.adsOpc.RES_1k_SPS);
%     case 500
%         adsWriteReg(btv.daq.settings.daq.adsOpc.CONFIG1,btv.daq.settings.daq.adsOpc.RES_500_SPS);
    case 250
        adsWriteReg(btv.daq.settings.daq.adsOpc.CONFIG1,btv.daq.settings.daq.adsOpc.RES_250_SPS); %Default for wired data acquisition
    otherwise
        close(hdl);
        errordlg('Currently only 250 Hz data rate is supported.');
        deviceClose;
        return
end
waitbar(2/15);
% Register CONFIG2 sets internal/external test signal, test amplifier and test frequency
adsWriteReg(btv.daq.settings.daq.adsOpc.CONFIG2, uint8(btv.daq.settings.daq.adsOpc.INT_TEST_4HZ_AMP));  % generate internal test signals
waitbar(3/15);
% You would also specify which channels to use for BIAS
adsWriteReg(btv.daq.settings.daq.adsOpc.CONFIGBIAS_SENSP, uint8(hex2dec('FF')));  % Use all input channels IN1P-IN8P and IN1N-IN8N
waitbar(4/15);
adsWriteReg(btv.daq.settings.daq.adsOpc.CONFIGBIAS_SENSN, uint8(hex2dec('FF')));
waitbar(5/15);
% Register CONFIG3 sets bias operation: Power up the internal bias reference and wait for it to settle
adsWriteReg(btv.daq.settings.daq.adsOpc.CONFIG3,uint8(btv.daq.settings.daq.adsOpc.CONFIG3_const + btv.daq.settings.daq.adsOpc.PD_REFBUF + btv.daq.settings.daq.adsOpc.BIASREF_INT + btv.daq.settings.daq.adsOpc.PD_BIAS )); %PD_REFBUF used for test signal
waitbar(6/15);
% Set single-ended or differential operation
if strcmpi(btv.daq.settings.daq.ElectrodeMontage,'singleEnded')
    adsWriteReg(btv.daq.settings.daq.adsOpc.CONFIGMISC1,uint8(btv.daq.settings.daq.adsOpc.MISC1_const + btv.daq.settings.daq.adsOpc.SRB1));
else
    adsWriteReg(btv.daq.settings.daq.adsOpc.CONFIGMISC1,uint8(btv.daq.settings.daq.adsOpc.MISC1_const));
end
waitbar(7/15);
% Channel registers
for i=1:8           % Only one ADS1299 device is supported and Ports is always 1:8
    switch btv.daq.settings.daq.ChannelGain{1}(i)
        case 1
            adsWriteReg(uint8(btv.daq.settings.daq.adsOpc.CONFIGCHnSET + i-1), uint8(btv.daq.settings.daq.adsOpc.ELECTRODE_INPUT+ btv.daq.settings.daq.adsOpc.GAIN_1X));
        case 2
            adsWriteReg(uint8(btv.daq.settings.daq.adsOpc.CONFIGCHnSET + i-1), uint8(btv.daq.settings.daq.adsOpc.ELECTRODE_INPUT+ btv.daq.settings.daq.adsOpc.GAIN_2X));
        case 4
            adsWriteReg(uint8(btv.daq.settings.daq.adsOpc.CONFIGCHnSET + i-1), uint8(btv.daq.settings.daq.adsOpc.ELECTRODE_INPUT+ btv.daq.settings.daq.adsOpc.GAIN_4X));
        case 6
            adsWriteReg(uint8(btv.daq.settings.daq.adsOpc.CONFIGCHnSET + i-1), uint8(btv.daq.settings.daq.adsOpc.ELECTRODE_INPUT+ btv.daq.settings.daq.adsOpc.GAIN_6X));
        case 8
            adsWriteReg(uint8(btv.daq.settings.daq.adsOpc.CONFIGCHnSET + i-1), uint8(btv.daq.settings.daq.adsOpc.ELECTRODE_INPUT+ btv.daq.settings.daq.adsOpc.GAIN_8X));
        case 12
            adsWriteReg(uint8(btv.daq.settings.daq.adsOpc.CONFIGCHnSET + i-1), uint8(btv.daq.settings.daq.adsOpc.ELECTRODE_INPUT+ btv.daq.settings.daq.adsOpc.GAIN_12X));
        case 24
            adsWriteReg(uint8(btv.daq.settings.daq.adsOpc.CONFIGCHnSET + i-1), uint8(btv.daq.settings.daq.adsOpc.ELECTRODE_INPUT+ btv.daq.settings.daq.adsOpc.GAIN_24X));
        otherwise
            close(hdl);
            errordlg('ADS1299 programmable gain is one of 1, 2, 4, 6, 8, 12 and 24.');
            deviceClose;
            return
    end
    if btv.daq.settings.daq.UsedChannels{1}(i) == 0,  % %turn off unused channels
        adsWriteReg(uint8(btv.daq.settings.daq.adsOpc.CONFIGCHnSET + i-1), uint8(btv.daq.settings.daq.adsOpc.PDn+ btv.daq.settings.daq.adsOpc.SHORTED));
    end
    waitbar((7+i)/15);
end
close(hdl);
hdl = waitbar(0,'Configuring filter parameters...','Name','BCI2VR');

%% Set up digital filter
btv.daq.buf.Filter.zeros=[];
btv.daq.buf.Filter.poles=[];
btv.daq.buf.Filter.gains=1;
numChan=0;
for i=1:length(btv.daq.settings.daq.DeviceSerials)
    numChan=numChan+length(btv.daq.settings.daq.Ports{i});
end
waitbar(1/5);
if isfield(btv.daq.settings.daq,'NotchFilter') && ~isempty(btv.daq.settings.daq.NotchFilter.bandStopFreq)
    [btv.daq.buf.NotchFilter.b,btv.daq.buf.NotchFilter.a]=butter(btv.daq.settings.daq.NotchFilter.order,...
        btv.daq.settings.daq.NotchFilter.bandStopFreq/(btv.daq.settings.daq.SamplingRate/2),'stop');
    [btv.daq.buf.NotchFilter.zeros,btv.daq.buf.NotchFilter.poles,btv.daq.buf.NotchFilter.gain]=tf2zp(btv.daq.buf.NotchFilter.b,btv.daq.buf.NotchFilter.a);
    btv.daq.buf.Filter.zeros=[btv.daq.buf.Filter.zeros;btv.daq.buf.NotchFilter.zeros];
    btv.daq.buf.Filter.poles=[btv.daq.buf.Filter.poles;btv.daq.buf.NotchFilter.poles];
    btv.daq.buf.Filter.gains=btv.daq.buf.Filter.gains*btv.daq.buf.NotchFilter.gain;
end
waitbar(2/5);
if isfield(btv.daq.settings.daq,'LowpassFilter') && ~isempty(btv.daq.settings.daq.LowpassFilter.Freq)
    [btv.daq.buf.LowpassFilter.b,btv.daq.buf.LowpassFilter.a]=butter(btv.daq.settings.daq.LowpassFilter.order,...
        btv.daq.settings.daq.LowpassFilter.Freq/(btv.daq.settings.daq.SamplingRate/2),'low');
    [btv.daq.buf.LowpassFilter.zeros,btv.daq.buf.LowpassFilter.poles,btv.daq.buf.LowpassFilter.gain]=tf2zp(btv.daq.buf.LowpassFilter.b,btv.daq.buf.LowpassFilter.a);
    btv.daq.buf.Filter.zeros=[btv.daq.buf.Filter.zeros;btv.daq.buf.LowpassFilter.zeros];
    btv.daq.buf.Filter.poles=[btv.daq.buf.Filter.poles;btv.daq.buf.LowpassFilter.poles];
    btv.daq.buf.Filter.gains=btv.daq.buf.Filter.gains*btv.daq.buf.LowpassFilter.gain;
end
waitbar(3/5);
if isfield(btv.daq.settings.daq,'NotchFilter') && ~isempty(btv.daq.settings.daq.HighpassFilter.Freq)
    [btv.daq.buf.HighpassFilter.b,btv.daq.buf.HighpassFilter.a]=butter(btv.daq.settings.daq.HighpassFilter.order,...
        btv.daq.settings.daq.HighpassFilter.Freq/(btv.daq.settings.daq.SamplingRate/2),'high');
    [btv.daq.buf.HighpassFilter.zeros,btv.daq.buf.HighpassFilter.poles,btv.daq.buf.HighpassFilter.gain]=tf2zp(btv.daq.buf.HighpassFilter.b,btv.daq.buf.HighpassFilter.a);
    btv.daq.buf.Filter.zeros=[btv.daq.buf.Filter.zeros;btv.daq.buf.HighpassFilter.zeros];
    btv.daq.buf.Filter.poles=[btv.daq.buf.Filter.poles;btv.daq.buf.HighpassFilter.poles];
    btv.daq.buf.Filter.gains=btv.daq.buf.Filter.gains*btv.daq.buf.HighpassFilter.gain;
end
waitbar(4/5);
if ~isempty(btv.daq.buf.Filter.zeros)
    [btv.daq.buf.Filter.b,btv.daq.buf.Filter.a]=zp2tf(btv.daq.buf.Filter.zeros,btv.daq.buf.Filter.poles,btv.daq.buf.Filter.gains);
    btv.daq.buf.Filter.Zf=zeros(max(length(btv.daq.buf.Filter.b),length(btv.daq.buf.Filter.a))-1,numChan);
else
    btv.daq.buf.Filter=[];
end

%% Add toolbar on daq monitor
btv.hdl.daq.tb1.calibration=uitoggletool(btv.hdl.daq.toolbar1,'separator','on','CData',image2icondata('calibration.gif'),'state','off',...
    'TooltipString','Calibration','ClickedCallback',[mfilename '(''@adsCalibration'')']);
btv.hdl.daq.tb1.checkImpedance=uipushtool(btv.hdl.daq.toolbar1,'CData',image2icondata('checkImpedance.gif'),...
    'TooltipString','Check Impedance','ClickedCallback',[mfilename '(''@adsImpedancePanel'')']);
waitbar(1,hdl,'Configuration complete.');
pause(0.5);
close(hdl);

%% Start acquiring data
function deviceStart 
    global btv;
    % Start data streaming
    pause(.5);
    btv.daq.buf.adsResidual=[];
    btv.daq.buf.packetNumberRead=[];
    adsSendCommand(btv.daq.settings.daq.adsOpc.RDATAC); %restart streaming
    adsSendCommand(btv.daq.settings.daq.adsOpc.START);

%% Update data buffer
function btvBufUpdate %#ok<DEFNU>   % Consider to move into btv_daq
global btv;
% Updating data buffer using FIFO
updatedData = adsStream; 
if ~isempty(updatedData)
    btv.daq.buf.currentTimePoint=btv.daq.buf.currentTimePoint+size(updatedData,1); %% For using getdata
    btv.daq.buf.dataTimePoints=(btv.daq.buf.currentTimePoint-btv.daq.settings.buf.BufferLength+1:btv.daq.buf.currentTimePoint)';
    % Apply digital filters
    if isfield(btv.daq.buf,'Filter')
        if ~isempty(btv.daq.buf.Filter)
            [updatedData,btv.daq.buf.Filter.Zf] = filter(btv.daq.buf.Filter.b, btv.daq.buf.Filter.a, updatedData, btv.daq.buf.Filter.Zf, 1);
        end
    end

    btv.daq.buf.data=[btv.daq.buf.data;updatedData];
    btv.daq.buf.data(1:end-btv.daq.settings.buf.BufferLength,:)=[];
    if btv.daq.flag.scopeon == 1, btv.daq.buf.viewUpdate=[btv.daq.buf.viewUpdate;updatedData]; end
    btv.daq.buf.socketRDA=updatedData;

    if btv.daq.flag.tcpip == 1,
        if btv.hdl.daq.conRDA == -1,
            btv.hdl.daq.conRDA=pnet(btv.hdl.daq.sockconRDA,'tcplisten');
            if btv.hdl.daq.conRDA~=-1
                pnet(btv.hdl.daq.conRDA,'setwritetimeout',.1);
                btv.daq.buf.nBlock=0;
            end
        end
        if btv.hdl.daq.conRDA ~= -1,
            if pnet(btv.hdl.daq.conRDA,'status') <= 0, %% Remote client is disconnected
                pnet(btv.hdl.daq.conRDA,'close');
                btv.hdl.daq.conRDA=-1;
            elseif ~isempty(btv.daq.buf.socketRDA),
                writeRDAPort;
            end
        end
    end
end

%% Begin recording data to disk
function deviceRecord %#ok<DEFNU>
global btv;
% Stop data streaming
deviceStop;
% Flush serial input buffer
while get(btv.hdl.daq.bbBt,'BytesAvailable') > 0, %there is often some latency with USB communications
    fread(btv.hdl.daq.bbBt,get(btv.hdl.daq.bbBt,'BytesAvailable'));
    pause(0.01);
end
[tempname,temppath] = uiputfile({'*_bb.bin';'*_bb.txt'},'Save data to...');
if isequal(tempname,0) || isequal(temppath,0), return; end
[spath,sname,sext]=fileparts(fullfile(temppath,tempname));
btv.daq.settings.data.datasavingpathname=spath;
btv.daq.settings.data.datasavingext=sext;
aa=strfind(sname,'_bb');
if ~isempty(aa)
    btv.daq.settings.datasavingfilename=sname(1:aa-1);
else
    btv.daq.settings.datasavingfilename=sname;
end
btv.daq.settings.datasavingfilename=[btv.daq.settings.datasavingfilename '_bb'];
switch btv.daq.settings.data.datasavingext
   case '.bin'
       fid=fopen(fullfile(btv.daq.settings.data.datasavingpathname,...
            [btv.daq.settings.datasavingfilename btv.daq.settings.data.datasavingext]),'w+','ieee-le');
   case '.txt'
       fid = fopen(fullfile(btv.daq.settings.data.datasavingpathname,...
            [btv.daq.settings.datasavingfilename btv.daq.settings.data.datasavingext]),'w+');
       fprintf(fid,'Recording started on %s\r\n',datestr(clock));
       fprintf(fid,'Device: %s\r\n',btv.daq.settings.daq.HardwareName);
       fprintf(fid,'Sampling Rate: %d Hz\r\n',btv.daq.settings.daq.SamplingRate);
       channels = btv.daq.settings.daq.Ports{1};
       usedChannels = channels(btv.daq.settings.daq.UsedChannels{1});
       fprintf(fid,'CH%d\t',usedChannels(1:end-1));
       fprintf(fid,'CH%d\r\n',usedChannels(end));
end
if fid < 0,
    errordlg('BCI2VR can''t open the file to save data');
    deviceClose;
    return
end
fclose(fid);
deviceStart;                                                               

%% Stop recording to disk
function deviceRecordOff %#ok<DEFNU>
global btv;
btv.daq.flag.recordon=0;

%% Stop acquiring data
function deviceStop 
global btv;
% Stop data streaming
adsSendCommand(btv.daq.settings.daq.adsOpc.SDATAC);
adsSendCommand(btv.daq.settings.daq.adsOpc.STOP);

% Flush serial input buffer
while get(btv.hdl.daq.bbBt,'BytesAvailable') > 0,
    fread(btv.hdl.daq.bbBt,get(btv.hdl.daq.bbBt,'BytesAvailable'));

    pause(0.01);
end

%% Close Bluetooth connection
function deviceClose 
global btv;
deviceStop;
pause(0.1);
fclose(btv.hdl.daq.bbBt);
delete(btv.hdl.daq.bbBt);

%% Send command to the ADS1299
function adsSendCommand(cmd)
global btv;
pause(0.1);
% MCU has 3-byte DMA buffer for USART RX; need to fill it to
% trigger an interrupt
fwrite(btv.hdl.daq.bbBt,[cmd 0 0]);
pause(0.1);

%% Write to ADS1299 register
function adsWriteReg(reg, val)
global btv;
pause(0.1);
fwrite(btv.hdl.daq.bbBt,[64+reg,0,val]);
pause(0.5);
readval = adsReadReg(reg);
if val ~= readval
    disp(['Wrote ' int2str(val) ' to register ' int2str(reg) ', read back ' int2str(readval) '.']);
    errordlg('ADS1299 register read/write error! Please try again.');
    deviceClose;
    return
end

%% Read ADS1299 register
function [val] =  adsReadReg(reg)
global btv; 
val = -1;
% flush buffer
while get(btv.hdl.daq.bbBt,'BytesAvailable') > 0
   fread(btv.hdl.daq.bbBt,get(btv.hdl.daq.bbBt,'BytesAvailable'));
   pause(0.01);
end
% MCU has 3-byte DMA buffer for USART RX; need to fill it to
% trigger an interrupt
fwrite(btv.hdl.daq.bbBt,[32+reg,0,0]);
pause(0.1);
loop=1;
while (get(btv.hdl.daq.bbBt,'BytesAvailable') < 1) && (loop < 100),
   fwrite(btv.hdl.daq.bbBt,[32+reg,0,0]);
   loop=loop+1;
   pause(0.1);
end
if get(btv.hdl.daq.bbBt,'BytesAvailable') > 0
    [data,~] = fread(btv.hdl.daq.bbBt,get(btv.hdl.daq.bbBt,'BytesAvailable'),'uchar');
    val = data(1);
end


%% Streaming data transmission
function EEGData=adsStream  % Potentially lossy; data only but fast
global btv;
if get(btv.hdl.daq.bbBt,'BytesAvailable') > 0,
   aa=fread(btv.hdl.daq.bbBt,get(btv.hdl.daq.bbBt,'BytesAvailable'),'uchar');
else
   EEGData=[]; 
   return
end
aa=[btv.daq.buf.adsResidual; aa];
pos=1;
samples=0;
len=length(aa);
OKpos = 0;
chanData=zeros(length(btv.daq.settings.daq.Ports{1}),1);
while (len-pos+1) >= btv.daq.settings.daq.bbBt.PacketLen
   if (aa(pos) == btv.daq.settings.daq.bbBt.Headers(1)) && (aa(pos+1) == btv.daq.settings.daq.bbBt.Headers(2)) %header byte 
       samples = samples + 1;
       if ~isempty(btv.daq.buf.packetNumberRead)
           if aa(pos+2) ~= mod(btv.daq.buf.packetNumberRead+1,128) 
               disp('Warning: missing packet.');
               disp(['Expected ',num2str(mod(btv.daq.buf.packetNumberRead+1,4)),'; read ',num2str(aa(pos+2)),'.']);
           end
       end
       btv.daq.buf.packetNumberRead=aa(pos+2);
       %Process channel data; assumes incoming data is
       %big-endian
       for i=1:8
           cc = aa(pos+3+(i*3))*(2^16) + aa(pos+4+(i*3))*(2^8) + aa(pos+5+(i*3));
           if cc >= 8388608, cc = cc - 16777216; end
           chanData(i,samples) =  cc * btv.daq.settings.daq.bbBt.bitResolution(i); % Convert to [V]
       end
       for i=9:14
           cc = aa(pos+30+(i-9)*2)*(2^8) + aa(pos+30+(i-9)*2+1); % Accel/gyro sensor using uint16
           if cc >= 32768, cc = cc - 65536; end
           chanData(i,samples) =  cc * btv.daq.settings.daq.bbBt.bitResolution(i); % Convert to [V]
       end
       pos = pos + btv.daq.settings.daq.bbBt.PacketLen;
       OKpos = pos - 1;%last valid byte
   else
       pos = pos + 1;
   end %if command else data   end
end
if (OKpos < len)
   btv.daq.buf.adsResidual = aa((OKpos+1):end); % retain left over bytes
else
   btv.daq.buf.adsResidual = [];
end

if btv.daq.flag.recordon == 1, % % save data when needed
   fid=fopen(fullfile(btv.daq.settings.data.datasavingpathname,...
               [btv.daq.settings.datasavingfilename btv.daq.settings.data.datasavingext]),'a+');
   switch btv.daq.settings.data.datasavingext
       case '.bin'
           fwrite(fid,chanData(:),'double');
       case '.txt'
           fprintf(fid,'%+1.6e\t%+1.6e\t%+1.6e\t%+1.6e\t%+1.6e\t%+1.6e\t%+1.6e\t%+1.6e\t%+1.6e\t%+1.6e\t%+1.6e\t%+1.6e\t%+1.6e\t%+1.6e\r\n',chanData);
   end
   fclose(fid);
end
EEGData=chanData';

%% Enable calibration mode
function adsCalibration  %#ok<DEFNU>
global btv;
%
hdl=msgbox('Reset device...wait','BCI2VR');
drawnow;
% Turn of continuous data logging to set channel register
deviceStop;
for i=1:8           % Only one ADS1299 device is supported; only 8 channels available
    if strcmpi(get(btv.hdl.daq.tb1.calibration,'State'),'on')
        switch btv.daq.settings.daq.ChannelGain{1}(i)
            case 1
                adsWriteReg(uint8(btv.daq.settings.daq.adsOpc.CONFIGCHnSET + i-1), uint8(btv.daq.settings.daq.adsOpc.TEST_SIGNAL+ btv.daq.settings.daq.adsOpc.GAIN_1X));
            case 2
                adsWriteReg(uint8(btv.daq.settings.daq.adsOpc.CONFIGCHnSET + i-1), uint8(btv.daq.settings.daq.adsOpc.TEST_SIGNAL+ btv.daq.settings.daq.adsOpc.GAIN_2X));
            case 4
                adsWriteReg(uint8(btv.daq.settings.daq.adsOpc.CONFIGCHnSET + i-1), uint8(btv.daq.settings.daq.adsOpc.TEST_SIGNAL+ btv.daq.settings.daq.adsOpc.GAIN_4X));
            case 6
                adsWriteReg(uint8(btv.daq.settings.daq.adsOpc.CONFIGCHnSET + i-1), uint8(btv.daq.settings.daq.adsOpc.TEST_SIGNAL+ btv.daq.settings.daq.adsOpc.GAIN_6X));
            case 8
                adsWriteReg(uint8(btv.daq.settings.daq.adsOpc.CONFIGCHnSET + i-1), uint8(btv.daq.settings.daq.adsOpc.TEST_SIGNAL+ btv.daq.settings.daq.adsOpc.GAIN_8X));
            case 12
                adsWriteReg(uint8(btv.daq.settings.daq.adsOpc.CONFIGCHnSET + i-1), uint8(btv.daq.settings.daq.adsOpc.TEST_SIGNAL+ btv.daq.settings.daq.adsOpc.GAIN_12X));
            case 24
                adsWriteReg(uint8(btv.daq.settings.daq.adsOpc.CONFIGCHnSET + i-1), uint8(btv.daq.settings.daq.adsOpc.TEST_SIGNAL+ btv.daq.settings.daq.adsOpc.GAIN_24X));
            otherwise
                close(hdl);
                errordlg('ADS1299 programmable gain is one of 1, 2, 4, 6, 8, 12 and 24.');
                deviceClose;
                return
        end
    else
        switch btv.daq.settings.daq.ChannelGain{1}(i)
            case 1
                adsWriteReg(uint8(btv.daq.settings.daq.adsOpc.CONFIGCHnSET + i-1), uint8(btv.daq.settings.daq.adsOpc.ELECTRODE_INPUT+ btv.daq.settings.daq.adsOpc.GAIN_1X));
            case 2
                adsWriteReg(uint8(btv.daq.settings.daq.adsOpc.CONFIGCHnSET + i-1), uint8(btv.daq.settings.daq.adsOpc.ELECTRODE_INPUT+ btv.daq.settings.daq.adsOpc.GAIN_2X));
            case 4
                adsWriteReg(uint8(btv.daq.settings.daq.adsOpc.CONFIGCHnSET + i-1), uint8(btv.daq.settings.daq.adsOpc.ELECTRODE_INPUT+ btv.daq.settings.daq.adsOpc.GAIN_4X));
            case 6
                adsWriteReg(uint8(btv.daq.settings.daq.adsOpc.CONFIGCHnSET + i-1), uint8(btv.daq.settings.daq.adsOpc.ELECTRODE_INPUT+ btv.daq.settings.daq.adsOpc.GAIN_6X));
            case 8
                adsWriteReg(uint8(btv.daq.settings.daq.adsOpc.CONFIGCHnSET + i-1), uint8(btv.daq.settings.daq.adsOpc.ELECTRODE_INPUT+ btv.daq.settings.daq.adsOpc.GAIN_8X));
            case 12
                adsWriteReg(uint8(btv.daq.settings.daq.adsOpc.CONFIGCHnSET + i-1), uint8(btv.daq.settings.daq.adsOpc.ELECTRODE_INPUT+ btv.daq.settings.daq.adsOpc.GAIN_12X));
            case 24
                adsWriteReg(uint8(btv.daq.settings.daq.adsOpc.CONFIGCHnSET + i-1), uint8(btv.daq.settings.daq.adsOpc.ELECTRODE_INPUT+ btv.daq.settings.daq.adsOpc.GAIN_24X));
            otherwise
                close(hdl);
                errordlg('ADS1299 programmable gain is one of 1, 2, 4, 6, 8, 12 and 24.');
                deviceClose;
                return
        end
        if btv.daq.settings.daq.UsedChannels{1}(i) == 0,  % %turn off unused channels
            adsWriteReg(uint8(btv.daq.settings.daq.adsOpc.CONFIGCHnSET + i-1), uint8(btv.daq.settings.daq.adsOpc.PDn+ btv.daq.settings.daq.adsOpc.SHORTED));
        end
    end
end
deviceStart;
close(hdl);

%% Open impedance-check panel
function adsImpedancePanel %#ok<DEFNU>
global btv;
if btv.daq.flag.scopeon == 1, return; end
set(btv.hdl.daq.tb1.checkImpedance,'Enable','off');
btv.hdl.daq.impedancePanel=figure('NumberTitle','off','Name','Lead-Off Status');
set(btv.hdl.daq.impedancePanel,'Unit','Normalized','Position',[0.8 0.6 0.12 0.35],'MenuBar','none','Resize','off','Toolbar','none','DeleteFcn',[mfilename '(''@closeImpedancePanel'')']);
annotation('textbox',[0.1 0.85 0.35 0.1],'string','Positive Electrodes','FontWeight','bold','LineStyle','none');
annotation('textbox',[0.55 0.85 0.35 0.1],'string','Negative Electrodes','FontWeight','bold','LineStyle','none');
btv.hdl.daq.impedancePositives=zeros(1,8);
btv.hdl.daq.impedanceNegatives=zeros(1,8);
for i=1:8
    btv.hdl.daq.impedancePositives(8-(i-1))=annotation('textbox',[0.1 0.05+0.1*(i-1) 0.35 0.08],'string','LEAD ON','BackgroundColor','g');
    btv.hdl.daq.impedanceNegatives(8-(i-1))=annotation('textbox',[0.55 0.05+0.1*(i-1) 0.35 0.08],'string','LEAD ON','BackgroundColor','g');
end
btv.hdl.daq.updateImpedanceTimer=timer('Period',1,'ExecutionMode','fixedSpacing','Name','ADS LEAD Status','TimerFcn',[mfilename '(''@updateImpedanceStatus'')']);
% Turn off continuous data logging
deviceStop;
% Start LEADOFF sensing
adsWriteReg(btv.daq.settings.daq.adsOpc.CONFIGLOFF_SENSP, uint8(hex2dec('FF')));
adsWriteReg(btv.daq.settings.daq.adsOpc.CONFIGLOFF_SENSN, uint8(hex2dec('FF')));
adsWriteReg( btv.daq.settings.daq.adsOpc.CONFIG4, btv.daq.settings.daq.adsOpc.PD_LOFF_COMP);
start(btv.hdl.daq.updateImpedanceTimer);
            
%% Close lead-off check panel 
function closeImpedancePanel %#ok<DEFNU>
global btv;
stop(btv.hdl.daq.updateImpedanceTimer);
delete(btv.hdl.daq.updateImpedanceTimer);
% Stop LEADOFF sensing
adsWriteReg(btv.daq.settings.daq.adsOpc.CONFIGLOFF_SENSP, uint8(hex2dec('00')));
adsWriteReg(btv.daq.settings.daq.adsOpc.CONFIGLOFF_SENSN, uint8(hex2dec('00')));
adsWriteReg( btv.daq.settings.daq.adsOpc.CONFIG4, uint8(hex2dec('00')));
set(btv.hdl.daq.tb1.checkImpedance,'Enable','on'); pause(0.1);
% Turn on continuous data logging
deviceStart;

%% Update lead-off display based on ADS1299 status word
function updateImpedanceStatus %#ok<DEFNU>
global btv;
xx=adsReadReg(btv.daq.settings.daq.adsOpc.CONFIGLOFF_STATP);
yy=dec2bin(xx,8);
for i=1:8
    if strcmp(yy(i),'0')
        set(btv.hdl.daq.impedancePositives(i),'string','LEAD ON','BackgroundColor','g')
    else
        set(btv.hdl.daq.impedancePositives(i),'string','LEAD OFF','BackgroundColor','r')
    end
end
xx=adsReadReg(btv.daq.settings.daq.adsOpc.CONFIGLOFF_STATN);
yy=dec2bin(xx,8);
for i=1:8
    if strcmp(yy(i),'0')
        set(btv.hdl.daq.impedanceNegatives(i),'string','LEAD ON','BackgroundColor','g')
    else
        set(btv.hdl.daq.impedanceNegatives(i),'string','LEAD OFF','BackgroundColor','r')
    end
end
