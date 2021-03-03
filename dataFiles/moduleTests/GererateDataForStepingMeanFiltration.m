close all;
clear all;

data = [
    0, 0, 0, 0, 0, 0, 0, 0, 0, 2.53, 5.33, 2.75, 2.356, 3.76, 3.54, 2.46, 3.35124, 1.214, 2.15, 3.53, 5.33, 3.75, 1.356, 3.36, 4.54, 3.46, 2.35124, 3.214, 1.15, 3.15, 6.23, 2.15, 3.122, 4.15, 2.51, 3.12, 3.12241, 5.121, 1.12
];

resultMean = zeros(1, 30);
resultVariacion = zeros(1, 30);
resultStandardDeviation = zeros(1, 30);

for i = 10 : length(data)
    resultMean(i - 9) = mean(data(i-9 : i));
    resultVariacion(i - 9) = var(data(i-9 : i));
    resultStandardDeviation(i - 9) = std(data(i-9 : i));
end
resultMean
resultVariacion
resultStandardDeviation
