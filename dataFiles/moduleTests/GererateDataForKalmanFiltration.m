close all;
clear all;
dt = 0.01;
t = [0:dt:10];
fi = sin(pi/20 * t) * 0.9;
theta = sin(pi/10 * t + pi* 3/4)*0.8;
psi = cos(pi/5 * t);
thetaV = diff(theta)/dt;
fiV = diff(fi)/dt;
psiV = diff(psi)/dt;
t = t(2:end);
fi = fi(2:end);
theta = theta(2:end);
psi = psi(2:end);

figure;
hold on;
plot(t, fi * 180/pi, t, theta * 180/pi, t, psi * 180/pi);
legend('fi', 'theta', 'psi');

akcelerometr = zeros(3, length(t));
magnetometer = zeros(3, length(t));
vecG = [0; 0; -9.81];
vecM = [3.52; 0; 7.25];
for i = 1:length(t)
    st = sin(theta(i));
    ct = cos(theta(i));
    sf = sin(fi(i));
    cf = cos(fi(i));
    sp = sin(psi(i));
    cp = cos(psi(i));
    RPY = [
    ct * cp, sf * st * cp - cf * sp, cf * st * cp + sf * sp;
    ct * sp, sf * st * sp + cf * cp, cf * st * sp - sf * cp;
    -st,          sf * ct          ,      cf * ct
    ]';
    akcelerometr(:, i) = RPY * vecG;
    magnetometer(:, i) = RPY * vecM;
end

noise = fopen('dataForKalmanModuleTestWithNoise.txt','w');
pure = fopen('dataForKalmanModuleTest.txt','w');
expect = fopen('dataForKalmanModuleTestExpectedAngles.txt','w');
fprintf(expect, 'Data for module tests of Kalman filtration. Expected angles after filtration in radians. First and second lines are ignored.\nFormat of data: angle fi  angle theta  angle psi\n');
fprintf(pure, 'Data for module tests of Kalman filtration. First and second lines are ignored.\nFormat of data: ackX  ackY  ackZ  gyroX  gyroY  gyroZ magX magY magZ\n');
fprintf(noise, 'Data for module tests of Kalman filtration. First and second lines are ignored.\nFormat of data: ackX  ackY  ackZ  gyroX  gyroY  gyroZ magX magY magZ\n');
for i = 1:length(t)
    fprintf(expect,'%f %f %f\n',fi(i), theta(i), psi(i));
end
for i = 1:length(t)
    fprintf(pure,'%f %f %f %f %f %f %f %f %f\n',akcelerometr(:, i), fiV(i), thetaV(i), psiV(i), magnetometer(:, i));
end
akcelerometr = akcelerometr + (rand(3, length(t)) - 0.5) * 8;
for i = 1:length(t)
    fprintf(noise,'%f %f %f %f %f %f %f %f %f\n',akcelerometr(:, i), fiV(i) + 0.01, thetaV(i) + 0.006, psiV(i) + 0.002, magnetometer(:, i));
end
fclose(pure);
fclose(noise);
fclose(expect);

angleRoll = atan(akcelerometr(2,:) ./ akcelerometr(3, :));
anglePitch = atan(-cos(angleRoll) .* (akcelerometr(1,:)) ./ (akcelerometr(3, :)));
angleYawl = atan((sin(angleRoll) .* magnetometer(3, :) - cos(angleRoll) .* magnetometer(2, :))./ (cos(anglePitch) .* magnetometer(1, :) + sin(angleRoll) .* sin(anglePitch) .* magnetometer(2, :) + cos(angleRoll) .* sin(anglePitch) .* magnetometer(3, :) ));
figure;
hold on;
plot(t, angleRoll, t, anglePitch, t, angleYawl);
plot(t, fi, 'o', t, theta, 'o', t, psi, 'x');
