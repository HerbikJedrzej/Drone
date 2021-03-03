#pragma once
#include <uart.hh>

void printIncomingData(UART& uart);
void convertIncomingPidMeasurementDataToFile(UART& uart);
void convertIncomingAltitudeMeasurementDataToFile(UART& uart);
void convertIncomingPidMeasurementDataToFileAndPlot(UART& uart);
void convertIncomingAltitudeMeasurementDataToFileAndPlot(UART& uart);

void print(const std::vector<double>& t, const std::vector<double>& s, const std::vector<double>& m, const std::string& name);
void printAltitudeMeasurement(const std::vector<double>& t, const std::vector<double>& s, const std::vector<double>& m, const std::string& name);
void saveDataToFile(const double power, const double amplitude, const double frequency, const std::vector<uint8_t>& set, const std::vector<uint8_t>& measure, const std::string& name);
void collectData(UART& uart, double& power, double& amplitude, double& frequency, std::vector<double>& time, std::vector<double>& setVal, std::vector<double>& measureVal);
void collectData(UART& uart, std::vector<double>& time, std::vector<double>& measureVal1, std::vector<double>& measureVal2);
void parseRecivedVector(std::vector<uint8_t>& rawData, std::vector<uint8_t>& parsedData);
