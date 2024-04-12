% Clear EVERYTHING
clc
clear
close all

%% Header / Name Information

fprintf('\n***********************************************************\n')
fprintf('*  Name:  Brennan Lawrence   Date:  10/4/2023             *\n')
fprintf('*  Team: Team 1    Project: High-Altitude Weather Balloon *\n')
fprintf('*  File/Assignment: Weather Balloon Simulation Work       *\n')                   
fprintf('***********************************************************\n')
fprintf('\n')

%% Chosen Parameters (For Testing)

Balloon_Mass = 1.20;                        % kilograms
Balloon_Burst_Diameter = 6.7056;            % meters
Balloon_Filled_Diameter = 1.811950656;      % meters
Balloon_Burst_Altitude = 33528;             % meters
Balloon_Drag_Coefficient = 1;               % Unitless
Ground_Temperature = 294.82;                % Kelvin
Ground_Pressure = 99085.73;                 % Pascals
Ground_Altitude = 238;                      % meters
Payload_Mass = 0.771107;                    % kilograms
Parachute_Area = 0.66;                      % m^2
Parachute_Drag_Coefficient = 1.75;          % Unitless
Time_Step = 0.05;                           % Seconds

%% Load in Data
HAB_DATA = readtable('HAB_TEST.CSV');

%% Read Data

HAB_Latitude = HAB_DATA.Latitude___;
HAB_Longitude = HAB_DATA.Longitude___;
HAB_Int_Temp = HAB_DATA.Interior_Temperature___F_;
HAB_Ext_Temp = HAB_DATA.Exterior_Temperature___F_;
HAB_Pressure = HAB_DATA.Pressure__psi_;
HAB_Time = HAB_DATA.Timestamp_HH_MM_SS_;
HAB_Altitude = HAB_DATA.Altitude_ft_;

HAB_Time_Seconds = seconds(HAB_Time);
HAB_Time_Seconds = HAB_Time_Seconds - HAB_Time_Seconds(1);

%% Assumptions / Givens

R_air = 287;                        % J/kg*K
R_helium = 2077.1;                  % J/kg*K
R_ideal = 8.31446261815324;         % J/K*mol or m^3*Pa/K*mol
Gravity_Initial = 9.80665;          % m/s^2
Earth_Radius = 6.371*10^6;          % Meters
Molar_Mass_of_Air = 0.0289644;      % kg/mol

%% Calculate altitude from pressure

% Convert pressures to pascals
psi2pascal = 6894.76;           % psi to pascals
HAB_Pressure_Pascals = HAB_Pressure .* psi2pascal;      % Pascals

% Use tables to find altitude
meters2feet = 3.28084;                                      % Meters to feet
Pressure_Altitude = atmospalt(HAB_Pressure_Pascals);        % Meters
Pressure_Altitude = Pressure_Altitude * meters2feet;        % Feet

% Calculate max altitudes
Max_GPS_Alt = max(HAB_Altitude);                            % Feet
Max_Pressure_alt = max(Pressure_Altitude);                  % Feet

fprintf('The max height from the GPS is %0.2f Feet\n',Max_GPS_Alt)
fprintf('The max height using pressure tables is %0.2f Feet\n',Max_Pressure_alt)
fprintf('\n')

%% Seperating Plots 

Ascent_Range = 1:1:1433;
Descent_Range = 1449:1:length(HAB_Time);
Unknown_Range = Ascent_Range(end):1:Descent_Range(1);

%% Assent Workspace

% Preliminary Calculations
    
    % Initial Filled Balloon Volume and Surface Area (m^3 and m^2 respectively)
    Initial_Balloon_Volume = Balloon_Volume_Function_1(Balloon_Filled_Diameter);
    Initial_Balloon_Surface_Area = Balloon_Surface_Area_Function(Balloon_Filled_Diameter);

    % Total Moles of Helium Inside Balloon (moles)
    Helium_Moles = Helium_Moles_Amount_Function(Ground_Pressure,Initial_Balloon_Volume,R_ideal,Ground_Temperature);

    % Calculate Ground Air Density (kg/m^3)
    Ground_Density = Density_at_Height_Function(Ground_Pressure,Ground_Temperature,R_air);

    % Calculate Initial Gravity (m/s^2)
    Ground_Gravity = Gravity_at_Height_Function(Ground_Altitude,Earth_Radius,Gravity_Initial);

    % Calculate Initial Velocity (m/s)
    Initial_Balloon_Velocity = Balloon_Velocity_Function(Ground_Density,Initial_Balloon_Surface_Area,Ground_Gravity,Initial_Balloon_Volume,Payload_Mass,Balloon_Mass,Balloon_Drag_Coefficient);

% Calculating parameters for ascent up to burst altitude for given time steps

    % Initialize Variables
    Time_Current = Time_Step;                               % Seconds
    Balloon_Velocity_Current = Initial_Balloon_Velocity;    % m/s
    Balloon_Diameter_Current = Balloon_Filled_Diameter;     % Meters
    Balloon_Volume_Current = Initial_Balloon_Volume;        % m^3
    Altitude_Current = Ground_Altitude;                     % Meters
    Temperature_Current = Ground_Temperature;               % Kelvin
    Gravity_Current = Ground_Gravity;                       % m/s^2
    Pressure_Current = Ground_Pressure;                     % Pascals
    Density_Current = Ground_Density;                       % kg/m^3

    % Initialize Tables
    Time_Table = 0;                                         % seconds
    Balloon_Velocity_Table = Balloon_Velocity_Current;      % m/s
    Balloon_Diameter_Table = Balloon_Diameter_Current;      % Meters
    Balloon_Volume_Table = Balloon_Volume_Current;          % m^3
    Altitude_Table = Altitude_Current;                      % Meters
    Temperature_Table = Temperature_Current;                % Kelvin
    Gravity_Table = Gravity_Current;                        % m/s^2
    Pressure_Table = Pressure_Current;                      % Pascals
    Density_Table = Density_Current;                        % kg/m^3

    % Initialize Temperature Variables
    j = 1;
    k = 1;
    l = 1;
    m = 1;
    n = 1;
    o = 1;

    while Altitude_Current < Balloon_Burst_Altitude

        % Increase time by one time step
        Time_Current = Time_Current + Time_Step;

        % Calculate distance traveled in time step
        del_h = Balloon_Velocity_Current * Time_Step;

        % Calculate change in height due to velocity
        Altitude_Current = Altitude_Current + del_h;
        Altitude_Difference = Altitude_Current - Ground_Altitude;

        % Calculate change in temperature due to height    
        [Temperature_Current,j,k,l,m,n,o] = Standard_Atmosphere_Temp_Function_Ascent(Altitude_Current,Ground_Temperature,Altitude_Table(j),Altitude_Table(k),Altitude_Table(l),Altitude_Table(m),Altitude_Table(n),Altitude_Table(o),j,k,l,m,n,o);

        % Calculate change in gravity due to height
        Gravity_Current = Gravity_at_Height_Function(Altitude_Current,Earth_Radius,Ground_Gravity);

        % Calculate change in pressure due to height
        Pressure_Current = Pressure_at_Height_Function(Ground_Pressure,Ground_Altitude,Ground_Gravity,Temperature_Current,Altitude_Current,Molar_Mass_of_Air,R_ideal);

        % Calculate change in density due to height
        Density_Current = Density_at_Height_Function(Pressure_Current,Temperature_Current,R_air);

        % Calculate change in balloon volume due to height
        Balloon_Volume_Current = Balloon_Volume_Function_2(Pressure_Current,Temperature_Current,R_ideal,Helium_Moles);

        % Calculate change in balloon diameter due to height
        Balloon_Diameter_Current = Balloon_Diameter_Function(Balloon_Volume_Current);

        % Calculate change in balloon surface area due to height
        Balloon_Surface_Area_Current = Balloon_Surface_Area_Function(Balloon_Diameter_Current);

        % Calculate change in balloon velocity due to height
        Balloon_Velocity_Current = Balloon_Velocity_Function(Density_Current,Balloon_Surface_Area_Current,Gravity_Current,Balloon_Volume_Current,Payload_Mass,Balloon_Mass,Balloon_Drag_Coefficient);

        % Tabulate all change values for ascent phase
        Altitude_Table(end+1) = Altitude_Current;                   % Meters
        Density_Table(end+1) = Density_Current;                     % Meters^3
        Temperature_Table(end+1) = Temperature_Current;             % Meters
        Pressure_Table(end+1) = Pressure_Current;                   % Pascals
        Time_Table(end+1) = Time_Current;                           % Seconds
        Balloon_Velocity_Table(end+1) = Balloon_Velocity_Current;   % m/s
        Balloon_Diameter_Table(end+1) = Balloon_Diameter_Current;   % m
        Gravity_Table(end+1) = Gravity_Current;                     % m/s^2

        % Breaking loop if balloon reaches target burst diameter

        if Balloon_Diameter_Current >= Balloon_Burst_Diameter

            break

        end

    end

    % Store time to apogee

    Time_Apogee = Time_Current;

%% Descent Workspace

% Calculating parameters for descent back to earth for same time step as above

    % Calculate initial velocity down as soon as balloon pops if it has a parachute

    Initial_Balloon_Velocity_Descent = -Balloon_Descent_Velocity_Function(Gravity_Current,Density_Current,Parachute_Drag_Coefficient,Parachute_Area,Payload_Mass);
                                                
    % Initialize Variables

    Balloon_Velocity_Current = Initial_Balloon_Velocity_Descent;    % m/s

    while Altitude_Current > Ground_Altitude

        % Increase time by one time step
        Time_Current = Time_Current + Time_Step;

        % Calculate distance traveled in time step
        del_h = Balloon_Velocity_Current * Time_Step;

        % Calculate change in height due to velocity
        Altitude_Current = Altitude_Current + del_h;
        Altitude_Difference = Altitude_Current - Ground_Altitude;

        % Break loop before it breaks code
        if Altitude_Current <= 0

            break

        end

        % Calculate change in temperature due to height    
        Temperature_Current = Standard_Atmosphere_Temp_Function_Descent(Altitude_Current,Ground_Temperature,Altitude_Table(j),Altitude_Table(k),Altitude_Table(l),Altitude_Table(m),Altitude_Table(n),Altitude_Table(o));

        % Calculate change in gravity due to height
        Gravity_Current = Gravity_at_Height_Function(Altitude_Current,Earth_Radius,Ground_Gravity);

        % Calculate change in pressure due to height
        Pressure_Current = Pressure_at_Height_Function(Ground_Pressure,Ground_Altitude,Ground_Gravity,Temperature_Current,Altitude_Current,Molar_Mass_of_Air,R_ideal);

        % Calculate change in density due to height
        Density_Current = Density_at_Height_Function(Pressure_Current,Temperature_Current,R_air);

        % Calculate change in velocity due to height
        Balloon_Velocity_Current = -Balloon_Descent_Velocity_Function(Gravity_Current,Density_Current,Parachute_Drag_Coefficient,Parachute_Area,Payload_Mass);       

        % Tabulate all change values for ascent phase
        Altitude_Table(end+1) = Altitude_Current;                   % Meters
        Density_Table(end+1) = Density_Current;                     % Meters^3
        Temperature_Table(end+1) = Temperature_Current;             % Meters
        Pressure_Table(end+1) = Pressure_Current;                   % Pascals
        Time_Table(end+1) = Time_Current;                           % Seconds
        Balloon_Velocity_Table(end+1) = Balloon_Velocity_Current;   % m/s
        Balloon_Diameter_Table(end+1) = 0;                          % Meters
        Balloon_Volume_Table(end+1) = 0;                            % Meters
        Gravity_Table(end+1) = Gravity_Current;                     % m/s^2

    end

% Linear interpolate final values to get them at original altitude

Temperature_Interp = Linear_Interpolation_Function(Altitude_Table(end-1),Ground_Altitude,Altitude_Table(end),Temperature_Table(end-1),Temperature_Table(end));
Balloon_Velocity_Interp = Linear_Interpolation_Function(Altitude_Table(end-1),Ground_Altitude,Altitude_Table(end),Balloon_Velocity_Table(end-1),Balloon_Velocity_Table(end));
Pressure_Interp = Linear_Interpolation_Function(Altitude_Table(end-1),Ground_Altitude,Altitude_Table(end),Pressure_Table(end-1),Pressure_Table(end));
Time_Interp = Linear_Interpolation_Function(Altitude_Table(end-1),Ground_Altitude,Altitude_Table(end),Time_Table(end-1),Time_Table(end));
Gravity_Interp = Linear_Interpolation_Function(Altitude_Table(end-1),Ground_Altitude,Altitude_Table(end),Gravity_Table(end-1),Gravity_Table(end));
Density_Interp = Linear_Interpolation_Function(Altitude_Table(end-1),Ground_Altitude,Altitude_Table(end),Density_Table(end-1),Density_Table(end));

% Insert interpolated end
Altitude_Table(end) = Ground_Altitude;                      % Meters
Density_Table(end) = Density_Interp;                     % Meters^3
Temperature_Table(end) = Temperature_Interp;             % Meters
Pressure_Table(end) = Pressure_Interp;                   % Pascals
Time_Table(end) = Time_Interp;                           % Seconds
Balloon_Velocity_Table(end) = Balloon_Velocity_Interp;   % m/s
Gravity_Table(end) = Gravity_Interp;                     % m/s^2

% Store time to apogee

Time_Descent = Time_Current;

%% Standard Atmosphere Temperature Function

function [Answer,j,k,l,m,n,o] = Standard_Atmosphere_Temp_Function_Ascent(Altitude_Current,Ground_Temperature,Altitude_j,Altitude_k,Altitude_l,Altitude_m,Altitude_n,Altitude_o,j,k,l,m,n,o)


    % Temperature for 0 - 11000 meters
    if Altitude_Current >= 0 && Altitude_Current < 11000

    Answer = Ground_Temperature + (Altitude_Current - 0)*(-6.5*10^-3);
    j = j + 1;
    k = k + 1;
    l = l + 1;
    m = m + 1;
    n = n + 1;
    o = o + 1;

    % Temperature for 11000 - 25000 meters
    elseif Altitude_Current >= 11000 && Altitude_Current < 25000

    Temp_Step_j = Ground_Temperature + Altitude_j*(-6.5*10^-3);

    Answer = Temp_Step_j;
    
    k = k + 1;
    l = l + 1;
    m = m + 1;
    n = n + 1;
    o = o + 1;

    % Temperature for 25000 - 47000 meters
    elseif Altitude_Current >= 25000 && Altitude_Current < 47000
        
    Temp_Step_j = Ground_Temperature + (Altitude_j-0)*(-6.5*10^-3);
    Temp_Step_k = Temp_Step_j;

    Answer = Temp_Step_k + (Altitude_Current-Altitude_k)*(3*10^-3);
    l = l + 1;
    m = m + 1;
    n = n + 1;
    o = o + 1;

    % Temperature for 47000 - 53000 meters
    elseif Altitude_Current >= 47000 && Altitude_Current < 53000

    Temp_Step_j = Ground_Temperature + (Altitude_j - 0)*(-6.5*10^-3);
    Temp_Step_k = Temp_Step_j;
    Temp_Step_l = Temp_Step_k + (Altitude_l - Altitude_k)*(3*10^-3);

    Answer = Temp_Step_l;
    m = m + 1;
    n = n + 1;
    o = o + 1;

    % Temperature for 53000 - 79000 meters
    elseif Altitude_Current >= 53000 && Altitude_Current < 79000

    Temp_Step_j = Ground_Temperature + Altitude_j*(-6.5*10^-3);
    Temp_Step_k = Temp_Step_j;
    Temp_Step_l = Temp_Step_k + (Altitude_l - Altitude_k)*(3*10^-3);  
    Temp_Step_m = Temp_Step_l;

    Answer = Temp_Step_m + (Altitude_Current - Altitude_m)*(-4.5*10^-3);
    n = n + 1;
    o = o + 1;

    % Temperature for 79000 - 90000 meters
    elseif Altitude_Current >= 79000 && Altitude_Current < 90000

    Temp_Step_j = Ground_Temperature + Altitude_j*(-6.5*10^-3);
    Temp_Step_k = Temp_Step_j;
    Temp_Step_l = Temp_Step_k + (Altitude_l - Altitude_k)*(3*10^-3);  
    Temp_Step_m = Temp_Step_l;
    Temp_Step_n = Temp_Step_m + (Altitude_n - Altitude_m)*(-4.5*10^-3);

    Answer = Temp_Step_n;
    o = o + 1;

    % Temperature for 90000 - 105000 meters
    elseif Altitude_Current >= 90000 && Altitude_Current < 105000

    Temp_Step_j = Ground_Temperature + Altitude_j*(-6.5*10^-3);
    Temp_Step_k = Temp_Step_j;
    Temp_Step_l = Temp_Step_k + (Altitude_l - Altitude_k)*(3*10^-3);  
    Temp_Step_m = Temp_Step_l;
    Temp_Step_n = Temp_Step_m + (Altitude_n - Altitude_m)*(-4.5*10^-3);
    Temp_Step_o = Temp_Step_n;

    Answer = Temp_Step_o + (Altitude_Current - Altitude_o)*(4*10^-3);

    % Outside available range
    else 

    fprintf('FUCKING BROKE\n')

    end

end

%% Standard Atmosphere Temperature Function

function [Answer] = Standard_Atmosphere_Temp_Function_Descent(Altitude_Current,Ground_Temperature,Altitude_j,Altitude_k,Altitude_l,Altitude_m,Altitude_n,Altitude_o)

    % Temperature for 90000 - 105000 meters
    if Altitude_Current >= 90000 && Altitude_Current < 105000

    Temp_Step_j = Ground_Temperature + Altitude_j*(-6.5*10^-3);
    Temp_Step_k = Temp_Step_j;
    Temp_Step_l = Temp_Step_k + (Altitude_l - Altitude_k)*(3*10^-3);  
    Temp_Step_m = Temp_Step_l;
    Temp_Step_n = Temp_Step_m + (Altitude_n - Altitude_m)*(-4.5*10^-3);
    Temp_Step_o = Temp_Step_n;

    Answer = Temp_Step_o + (Altitude_Current - Altitude_o)*(4*10^-3);

    % Temperature for 79000 - 90000 meters
    elseif Altitude_Current >= 79000 && Altitude_Current < 90000

    Temp_Step_j = Ground_Temperature + Altitude_j*(-6.5*10^-3);
    Temp_Step_k = Temp_Step_j;
    Temp_Step_l = Temp_Step_k + (Altitude_l - Altitude_k)*(3*10^-3);  
    Temp_Step_m = Temp_Step_l;
    Temp_Step_n = Temp_Step_m + (Altitude_n - Altitude_m)*(-4.5*10^-3);

    Answer = Temp_Step_n;

    % Temperature for 53000 - 79000 meters
    elseif Altitude_Current >= 53000 && Altitude_Current < 79000

    Temp_Step_j = Ground_Temperature + Altitude_j*(-6.5*10^-3);
    Temp_Step_k = Temp_Step_j;
    Temp_Step_l = Temp_Step_k + (Altitude_l - Altitude_k)*(3*10^-3);  
    Temp_Step_m = Temp_Step_l;

    Answer = Temp_Step_m + (Altitude_Current - Altitude_m)*(-4.5*10^-3);

    % Temperature for 47000 - 53000 meters
    elseif Altitude_Current >= 47000 && Altitude_Current < 53000

    Temp_Step_j = Ground_Temperature + (Altitude_j - 0)*(-6.5*10^-3);
    Temp_Step_k = Temp_Step_j;
    Temp_Step_l = Temp_Step_k + (Altitude_l - Altitude_k)*(3*10^-3);

    Answer = Temp_Step_l;

    % Temperature for 25000 - 47000 meters
    elseif Altitude_Current >= 25000 && Altitude_Current < 47000
        
    Temp_Step_j = Ground_Temperature + (Altitude_j-0)*(-6.5*10^-3);
    Temp_Step_k = Temp_Step_j;

    Answer = Temp_Step_k + (Altitude_Current-Altitude_k)*(3*10^-3);

    % Temperature for 11000 - 25000 meters
    elseif Altitude_Current >= 11000 && Altitude_Current < 25000

    Temp_Step_j = Ground_Temperature + Altitude_j*(-6.5*10^-3);

    Answer = Temp_Step_j;

    % Temperature for 0 - 11000 meters
    elseif Altitude_Current >= 0 && Altitude_Current < 11000

    Answer = Ground_Temperature + (Altitude_Current - 0)*(-6.5*10^-3);

    % Outside available range
    else 

    fprintf('FUCKING BROKE\n')

    end

end

%% Plots

% Figure 1 - Temperature vs Time Plot
figure(1)
plot(Time_Table,(Temperature_Table - 273.15)*(9/5) + 32,'r','Linewidth',2)
hold on
plot(HAB_Time_Seconds(Ascent_Range),HAB_Ext_Temp(Ascent_Range),'b','Linewidth',2)
hold on
plot(HAB_Time_Seconds(Unknown_Range),HAB_Ext_Temp(Unknown_Range),'b--','Linewidth',2)
plot(HAB_Time_Seconds(Descent_Range),HAB_Ext_Temp(Descent_Range),'b','Linewidth',2)
grid on
legend('Simulation Data','Flight Test Data','Gap in Flight Test Data','location','northeast')
xlabel('Time (Seconds)')
ylabel('Temperature (°Fahrenheit)')
title('Temperature vs Time Comparison Plot')

% Figure 2 - Pressure vs Time Plot
figure(2)
plot(Time_Table,Pressure_Table/6894.76,'r','Linewidth',2)
hold on
plot(HAB_Time_Seconds(Ascent_Range),HAB_Pressure(Ascent_Range),'b','Linewidth',2)
hold on
plot(HAB_Time_Seconds(Unknown_Range),HAB_Pressure(Unknown_Range),'b--','Linewidth',2)
plot(HAB_Time_Seconds(Descent_Range),HAB_Pressure(Descent_Range),'b','Linewidth',2)
grid on
legend('Simulation Data','Flight Test Data','Gap in Flight Test Data','location','northwest')
xlabel('Time (Seconds)')
ylabel('Pressure (psi)')
title('Pressure vs Time Comparison Plot')

% Figure 3 - Altitude vs Time Plot
figure(3)
plot(Time_Table,Altitude_Table*3.28084,'r','Linewidth',2)
hold on
plot(HAB_Time_Seconds(Ascent_Range),Pressure_Altitude(Ascent_Range),'b','Linewidth',2)
hold on
plot(HAB_Time_Seconds(Unknown_Range),Pressure_Altitude(Unknown_Range),'b--','Linewidth',2)
plot(HAB_Time_Seconds(Descent_Range),Pressure_Altitude(Descent_Range),'b','Linewidth',2)
grid on
legend('Simulation Data','Flight Test Data','Gap in Flight Test Data','location','northwest')
xlabel('Time (Seconds)')
ylabel('Altitude (Feet)')
title('Altitude vs Time Comparison Plot')

%% Variation in Gravity due to Height Function

function Answer = Gravity_at_Height_Function(Altitude,R_Earth,g_initial)

    Answer = g_initial * (1 + Altitude/R_Earth)^-2;

end

%% Balloon Ascent Rate / Vertical Velocity Function

function Answer = Balloon_Velocity_Function(Density_Current,Balloon_Surface_Area_Current,Gravity_Current,Balloon_Volume_Current,Payload_Mass,Balloon_Mass,Drag_Coefficient)

    Answer = sqrt( (1 / (0.5 * Density_Current * Balloon_Surface_Area_Current * Drag_Coefficient)) * (Density_Current*((6.2 * Gravity_Current * Balloon_Volume_Current) / 7.2) - (Payload_Mass*Gravity_Current) - (Balloon_Mass*Gravity_Current)));

end

%% Variation in Pressure due to Height Function

function Answer = Pressure_at_Height_Function(Ground_Pressure,Ground_Altitude,Ground_Gravity,Temperature_Current,Altitude_Current,Molar_Mass_of_Air,R_ideal)

    Answer = Ground_Pressure * exp((-(Ground_Gravity * Molar_Mass_of_Air * (Altitude_Current - Ground_Altitude))) / (R_ideal * Temperature_Current));
   
             
end

%% Variation in Density due to Height Function

function Answer = Density_at_Height_Function(Pressure_Current,Temperature_Current,R_air)

    Answer = Pressure_Current / (R_air * Temperature_Current);

end

%% Balloon Volume Function 1 (Assuming Spherical)

function Answer = Balloon_Volume_Function_1(Ballon_Current_Diameter)

    Answer = (4/3)*pi*(Ballon_Current_Diameter/2)^3;

end

%% Balloon Surface Area Function (Assuming Spherical)

function Answer = Balloon_Surface_Area_Function(Balloon_Diameter_Current)

    Answer = 4*pi*(Balloon_Diameter_Current/2)^2;

end

%% Calculate Helium Moles Function

function Answer = Helium_Moles_Amount_Function(Pressure,Volume,Universal_Gas_Constant,Temperature)

    Answer = (Pressure*Volume) / (Universal_Gas_Constant*Temperature);

end

%% Balloon Volume Function 2 (Assuming Spherical)

function Answer = Balloon_Volume_Function_2(Pressure_Current,Temperature_Current,Universal_Gas_Constant,Helium_Moles)

    Answer = (Helium_Moles * Universal_Gas_Constant * Temperature_Current) / Pressure_Current;

end

%% Balloon Diameter Function (Assuming Spherical)

function Answer = Balloon_Diameter_Function(Balloon_Volume_Current)

    Answer = ((Balloon_Volume_Current * (3/4)) / pi)^(1/3);
    
end

%% Balloon Descending Velocity Function

function Answer = Balloon_Descent_Velocity_Function(Gravity_Current,Density_Current,Parachute_Drag_Coefficient,Parachute_Area,Payload_Mass)

    Answer = sqrt((2*Payload_Mass*Gravity_Current)/(Parachute_Drag_Coefficient*Density_Current*Parachute_Area));

end

%% Linear Interpolation Function

function Answer = Linear_Interpolation_Function(x,x_0,x_1,y_0,y_1)

    Answer = y_0 + (x -x_0) * (y_1 - y_0)/(x_1-x_0);

end