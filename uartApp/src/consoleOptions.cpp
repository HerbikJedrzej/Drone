#include <iostream>
#include <fstream>
#include <consoleOptions.hh>
#include <toggleOnUserInput.hh>
#include <plot.hh>
#include <math.h>

void printIncomingData(UART& uart){
    ToggleOnUserInput run(true);
    run.reset();
    while(run){
        if(uart.size() > 0)
            printf("%c", uart.getChar());
    }
}

void parseRecivedVector(std::vector<uint8_t>& rawData, std::vector<uint8_t>& parsedData){
    uint8_t data;
    parsedData.clear();
    if(rawData.size() % 3 !=0)
        std::cout << "Raw data is not dividable by 3" << std::endl;
    for(unsigned int i = 0; i < rawData.size(); i = i + 3){
        data = (rawData[i] - '0') << 4;
        data |= rawData[i + 1] - '0';
        if(rawData[i + 2] != '\n')
            std::cout << "Every 3rd data should be equal to '\\n'" << std::endl;
        parsedData.push_back(data);
    }
}

namespace {
    union {
        double measurements[2];
        uint8_t rawData[16];
    } converter;
}

void convertData(const std::vector<uint8_t>& data, std::vector<double>& measure1, std::vector<double>& measure2){
    measure1.clear();
    measure2.clear();
    if(data.size() % 16 !=0)
        std::cout << "Data is not dividable by 16" << std::endl;
    for(unsigned int i = 0; i < data.size() / 16; i++){
        for(unsigned int j = 0; j < 8; j++){
            converter.rawData[j]     = data[i*16 + j];
            converter.rawData[j + 8] = data[i*16 + j + 8];
        }
        measure1.push_back(converter.measurements[0]);
        measure2.push_back(converter.measurements[1]);
    }
}

void collectData(UART& uart, std::vector<double>& time, std::vector<double>& measureVal1, std::vector<double>& measureVal2){
    ToggleOnUserInput run(true);
    std::vector<uint8_t> rawData;
    std::vector<uint8_t> data;
    time.clear();
    measureVal1.clear();
    measureVal2.clear();
    run.reset();
    uart.clear();
    std::cout << "reading data. To finish press enter." << std::endl;
    unsigned int dataCounter = 0;
    uint16_t expectedSize = 0;
    uint8_t previousChar;
    bool firstLine = true;
    while(run){
        if(!uart.empty()){
            if(firstLine){
                char c = uart.getChar();
                std::cout << c << std::flush;
                if(c == '\n'){
                    expectedSize = (previousChar - '0' < 3)? 500 : 1000;
                    firstLine = false;
                }
                previousChar = c;
            } else {
                dataCounter++;
                rawData.push_back(uart.getChar());
                std::cout << "\r" << "Colected data (" << dataCounter / 3 / 16 << " of " << expectedSize << ")" << std::flush;
            }
        }
    }
    parseRecivedVector(rawData, data);
    convertData(data, measureVal1, measureVal2);
    if(dataCounter / 3 / 16 > expectedSize)
        std::cout << "There are more collected data than expected! Please send then from device once again!" << std::endl;
    double localTime = 0;
    for(unsigned int i = 0; i < measureVal1.size(); i++){
        time.push_back(localTime);
        localTime += 0.01;
    }
}

void saveDataToFile(const std::vector<double>& set, const std::vector<double>& measure, const std::string& name){
    std::ofstream myfile;
    myfile.open ("./outputs/dataFiles/" + name + ".txt");
    myfile << "Data:   1 - setValue    2 - measured value\n";
    for(unsigned int i = 0; i < set.size(); i++)
        myfile << set[i] << "                " << measure[i] << "\n";
    myfile.close();
    std::cout << "Data saved in: ./outputs/dataFiles/" + name + ".txt" << std::endl;
}

void convertIncomingPidMeasurementDataToFile(UART& uart){
    std::string name;
    std::cout << "Type name if file:" << std::endl;
    std::cin >> name;
    std::vector<double> s, m, t;
    system("clear");
    collectData(uart, t, s, m);
    std::cout << "Recived recorded data size= " << t.size() << std::endl;
    saveDataToFile(s, m, name);
}

void convertIncomingAltitudeMeasurementDataToFile(UART& uart){
    std::string name;
    std::cout << "Type name if file:" << std::endl;
    std::cin >> name;
    std::vector<double> s1, s2, t;
    system("clear");
    collectData(uart, t, s1, s2);
    std::cout << "Recived recorded data size= " << t.size() << std::endl;
    saveDataToFile(s1, s2, name);
}

void convertIncomingPidMeasurementDataToFileAndPlot(UART& uart){
    std::string name;
    std::cout << "Type name if file:" << std::endl;
    std::cin >> name;
    std::vector<double> s, m, t;
    system("clear");
    collectData(uart, t, m, s);
    std::cout << "Recived recorded data size = " << t.size() << std::endl;
    saveDataToFile(s, m, name);
    printPlot(t, "set value [%]", s, "measure [deg]", m, name);
}

void convertIncomingAltitudeMeasurementDataToFileAndPlot(UART& uart){
    std::string name;
    std::cout << "Type name if file:" << std::endl;
    std::cin >> name;
    std::vector<double> setVal, measureVal, t;
    system("clear");
    collectData(uart, t, measureVal, setVal);
    std::cout << "Recived recorded data size = " << t.size() << std::endl;
    saveDataToFile(setVal, measureVal, name);
    printPlot(t, "set value [%]", setVal, "measure [m]", measureVal, name);
}

void printPlot(const std::vector<double>& t, const std::string& setValueName, const std::vector<double>& s, const std::string& measureValueName, const std::vector<double>& m, const std::string& name){
    plot::figure_size(1200, 780);
    plot::title("Filtrer altitude measurement.");
    plot::named_plot(measureValueName, t, m);
    plot::named_plot(setValueName, t, s);
    plot::legend();
    plot::save("./outputs/plots/" + name + ".png");
    plot::figure_close();
    std::cout << "Plot saved in: ./outputs/plots/" + name + ".png" << std::endl;
}
