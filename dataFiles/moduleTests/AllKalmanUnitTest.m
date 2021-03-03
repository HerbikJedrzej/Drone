close all;
clear all;
dt = 0.01;
KalmanUnitIncludeTest
roll = 1.6;
pitch = -0.46;
Y = [-5.03; 0.013; -9.01; 14.53; 1.03; -2.45];
vectorLength = sqrt(Y(1)^2 + Y(2)^2 + Y(3)^2);

A = [
    (vectorLength^2 - Y(1)^2)/vectorLength^3 * 9.81, -Y(1) * Y(2)/vectorLength^3 * 9.81, -Y(1) * Y(3)/vectorLength^3 * 9.81, 0, cos(roll) * cos(pitch) * dt * 9.81, -sin(roll) * cos(pitch) * dt * 9.81; 
    -Y(2) * Y(1)/vectorLength^3 * 9.81, (vectorLength^2 - Y(2)^2)/vectorLength^3 * 9.81, -Y(2) * Y(3)/vectorLength^3 * 9.81, -cos(roll) * cos(pitch) * dt * 9.81, 0, -sin(pitch) * dt * 9.81; 
    -Y(3) * Y(1)/vectorLength^3 * 9.81, -Y(3) * Y(2)/vectorLength^3 * 9.81, (vectorLength^2 - Y(3)^2)/vectorLength^3 * 9.81, sin(roll) * cos(pitch) * dt * 9.81, sin(pitch) * dt * 9.81, 0;
    0, 0, 0, 1, 0, 0;
    0, 0, 0, 0, 1, 0;
    0, 0, 0, 0, 0, 1;    
]

Xkp = [
    (Y(1) / vectorLength + cos(roll) * cos(pitch) * dt * Y(5) - sin(roll) * cos(pitch) * dt * Y(6)) * 9.81;
    (Y(2) / vectorLength - cos(roll) * cos(pitch) * dt * Y(4) - sin(pitch) * dt * Y(6)) * 9.81;
    (Y(3) / vectorLength + sin(roll) * cos(pitch) * dt * Y(4) + sin(pitch) * dt * Y(5)) * 9.81;
    Y(4);
    Y(5);
    Y(6);
]

Pkp = A * P * A' + eye(6) * Q(1)

Yk = Y + z

P = inv(Pkp+R)

K = Pkp * P

X = Xkp+K*(Yk-Xkp)

P = (eye(6)-K)*Pkp
