 % Clear EVERYTHING
clc
clear
close all

%% Header / Name Information

fprintf('\n***********************************************************\n')
fprintf('*  Name:  Brennan Lawrence   Date:  03/25/2024            *\n')
fprintf('*  File/Assignment: HAB Live Plot Code                    *\n')                    
fprintf('***********************************************************\n')
fprintf('\n')

%% Things to Note

% Cannot have Arduino IDE and Matlab open and connected to receiver at the
% same time

% This assumes the serial port data is in the form:
    % Packet#,YYYY-MM-DD,HH:MM:SS,Interior Temperature, Exterior
    % Temperature, Pressure, Latitude, Longitude, Altitude, # of Satellites

%% Setting up Arduino and Matlab Communication

% Set up serial port communication
% Note: Make sure to update COMXX to the actual COM channel
% Note: Make sure baud rate is the same as Arduino code

Port = serialport('COM18',9600);

%% Workspace

% Initialize latitude and longitude arrays
Latitude = [];
Longitude = [];

% Initialize extra arrays for shits and giggles
Identifier = []; 
Packet_Number = [];
Date = [];
Time = [];
Interior_Temp = [];
Exterior_Temp = [];
Pressure = [];
Altitude = [];
Satellite_Number = [];


% Initialize geoplot
figure(1)
Geoplot_Axes = geoaxes;
title('High-Altitude Balloon Tracking');

%% Live Plotting Loop

while true

    % Read data from serial port
    Current_Data = readline(Port);

    % Interpret serial port data
    Interpreted_Data = str2double(strsplit(Current_Data, ','));     % This splits data

    Identifier = [Identifier Interpreted_Data(1)];                  % Append same identifiying name
    Packet_Number = [Packet_Number Interpreted_Data(2)];            % Append new packet number data
    Date = [Date Interpreted_Data(3)];                              % Append new date data
    Time = [Time Interpreted_Data(4)];                              % Append new time data
    Interior_Temp = [Interior_Temp Interpreted_Data(5)];            % Append new exterior temperature data
    Exterior_Temp = [Exterior_Temp Interpreted_Data(6)];            % Append new interior temperature data
    Pressure = [Pressure Interpreted_Data(7)];                      % Append new pressure data
    Latitude = [Latitude Interpreted_Data(8)];                      % Append new latitude data
    Longitude = [Longitude Interpreted_Data(9)];                    % Append new longitude data
    Altitude = [Altitude Interpreted_Data(10)];                     % Append new altitude data
    Satellite_Number = [Satellite_Number Interpreted_Data(11)];     % Append new satellite number data

    % Update geoplot
    geoplot(Geoplot_Axes,Latitude,Longitude,'r')
    geobasemap landcover

    % Pauser that helps keep things moving smoothly
    pause(2)

end

% Close serial port when done
fclose(Port);
