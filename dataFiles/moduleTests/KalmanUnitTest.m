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

Xp = [
    (Y(1) / vectorLength + cos(roll) * cos(pitch) * dt * Y(5) - sin(roll) * cos(pitch) * dt * Y(6)) * 9.81;
    (Y(2) / vectorLength - cos(roll) * cos(pitch) * dt * Y(4) - sin(pitch) * dt * Y(6)) * 9.81;
    (Y(3) / vectorLength + sin(roll) * cos(pitch) * dt * Y(4) + sin(pitch) * dt * Y(5)) * 9.81;
    Y(4);
    Y(5);
    Y(6);
]

Pp = A * P * A' + eye(6) * Q(1)


z = [
    123.24;
	-24.14;
	2.15;
	-53.1;
	12.5;
    0.053
];
Yk = Y + z

Pkp = [
	0.244, 12.24, 0.21, -1.21, -2.51, 12.34;
	21.42, -35.1, -0.2, 1.425, -5.12, 1.004;
	25.12, -12.2, -4.1, 2.511, 2.251, -2.12;
	3.125, 23.35, 12.4, -12.1, 1.551, 24.12;
	-24.1, -5.12, 11.5, 0.125, 0.012, 25.12;
	47.12, 26.23, -3.2, 23.36, 6.231, 236.1
];
R = [
    12.34 0 0 0 0 0;
    0 1.004 0 0 0 0;
    0 0 12.34 0 0 0;
    0 0 0 1.004 0 0;
    0 0 0 0 12.34 0;
    0 0 0 0 0 1.004;
]
P=inv(Pkp+R)
K = Pkp * P

K = [
  	 21.7272,    2.7535,   -3.0707,   -5.3476,    7.2619,   -1.3432;
  	 -0.5285,    0.9593,    0.0604,    0.1327,   -0.1683,    0.0326;
  	-78.8184,  -10.2494,   11.1243,   19.9679,  -27.1821,    5.0845;
  	 -6.4625,   -0.7952,    0.8165,    2.7105,   -2.2119,    0.4073;
  	111.0321,   14.9277,  -15.3741,  -28.1979,   38.6763,   -7.1024;
  	  0.0361,   -0.0049,    0.0065,   -0.0145,    0.0087,    0.9945
];
Xkp = [
	-5.03;
	0.013;
	-9.01;
	14.53;
	1.03;
	-2.45;
];
Yk = [
	-4.5693;
	-0.0570;
	-7.3338;
	14.5300;
	1.0300;
	-2.4500;
];

newX = Xkp+K*(Yk-Xkp)


Pkp = [
	0.244, 12.24, 0.21, -1.21, -2.51, 12.34;
	21.42, -35.1, -0.2, 1.425, -5.12, 1.004;
	25.12, -12.2, -4.1, 2.511, 2.251, -2.12;
	3.125, 23.35, 12.4, -12.1, 1.551, 24.12;
	-24.1, -5.12, 11.5, 0.125, 0.012, 25.12;
	47.12, 26.23, -3.2, 23.36, 6.231, 236.1
];
K = [
  	  1.7272,    2.7535,   -3.0707,   -5.3476,    7.2619,   -1.3432;
  	 -0.5285,    0.9593,    0.0604,    0.1327,   -0.1683,    0.0326;
  	-78.8184,  -10.2494,    1.1243,    1.9679,   -2.1821,    5.0845;
  	 -6.4625,   -0.7952,    0.8165,    2.7105,   -2.2119,    0.4073;
  	111.0321,   14.9277,  -15.3741,   -8.1979,    3.6763,   -7.1024;
  	  0.0361,   -0.0049,    0.0065,   -0.0145,    0.0087,    0.9945
];
P = (eye(6)-K)*Pkp
