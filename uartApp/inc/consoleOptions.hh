#pragma once
#include <uart.hh>

void printIncomingData(UART& uart);
void convertIncomingPidMeasurementDataToFile(UART& uart);
void convertIncomingAltitudeMeasurementDataToFile(UART& uart);
void convertIncomingPidMeasurementDataToFileAndPlot(UART& uart);
void convertIncomingAltitudeMeasurementDataToFileAndPlot(UART& uart);

void printPlot(const std::vector<double>& t, const std::string& setValueName, const std::vector<double>& s, const std::string& measureValueName, const std::vector<double>& m, const std::string& name);
void saveDataToFile(const std::vector<uint8_t>& set, const std::vector<uint8_t>& measure, const std::string& name);
void collectData(UART& uart, std::vector<double>& time, std::vector<double>& measureVal1, std::vector<double>& measureVal2);
void parseRecivedVector(std::vector<uint8_t>& rawData, std::vector<uint8_t>& parsedData);
void convertData(const std::vector<uint8_t>& data, std::vector<double>& measure1, std::vector<double>& measure2);
