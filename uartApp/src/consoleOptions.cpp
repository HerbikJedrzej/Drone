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
    else
        std::cout << "Raw data is dividable by 3" << std::endl;
    for(unsigned int i = 0; i < rawData.size(); i = i + 3){
        data = (rawData[i] - '0') << 4;
        data |= rawData[i + 1] - '0';
        if(rawData[i + 2] != '\n')
            std::cout << "Every 3rd data should be equal to '\\n'" << std::endl;
        parsedData.push_back(data);
    }
}

void collectData(UART& uart, double& power, double& amplitude, double& frequency, std::vector<double>& time, std::vector<double>& setVal, std::vector<double>& measureVal){
    ToggleOnUserInput run(true);
    std::vector<uint8_t> rawData;
    std::vector<uint8_t> data;
    time.clear();
    setVal.clear();
    measureVal.clear();
    run.reset();
    printf("reading data. To finish press enter.");
    while(run){
        if(!uart.empty())
            rawData.push_back(uart.getChar());
    }
    uart.clear();
    parseRecivedVector(rawData, data);
    double localTime = 0;
	power = double(data[0]) / 3;
	amplitude = double(data[1]) / 8 + 0.25;
	frequency = double(data[2]) * 9.9 / 255 + 0.1;
    for(unsigned int i = 3; i < data.size(); i += 2){
        time.push_back(localTime);
        localTime += 0.01;
        setVal.push_back(power + amplitude * sin(6.28318530718 * frequency * localTime));
        measureVal.push_back(double(uint16_t((data[i] << 8) | data[i + 1])) * 360.0 / 65535.0 - 180.0);
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
    printf("reading data. To finish press enter.");
    while(run){
        if(!uart.empty())
            rawData.push_back(uart.getChar());
    }
    uart.clear();
    parseRecivedVector(rawData, data);
    double localTime = 0;
    double minValue = 300000;
    double removePart = 0;
    for(unsigned int i = 3; i < data.size(); i += 4){
        time.push_back(localTime);
        localTime += 0.01;
        if(i == 3)
            removePart = double(uint16_t((data[i] << 8) | data[i + 1])) / 10000.0;
        measureVal1.push_back((double(uint16_t((data[i] << 8) | data[i + 1])) / 10000.0) - removePart);
        measureVal2.push_back(double(uint16_t((data[i + 2] << 8) | data[i + 3])) / 100.0);
        minValue = std::min(minValue, measureVal2.back());
    }
    for(auto& elem : measureVal2)
        elem -= minValue; 
}

void saveDataToFile(const double power, const double amplitude, const double frequency, const std::vector<double>& set, const std::vector<double>& measure, const std::string& name){
    std::ofstream myfile;
    myfile.open ("./outputs/dataFiles/" + name + ".txt");
    myfile << "Power: " << power << "\n";
    myfile << "Amplitude: " << amplitude << "\n";
    myfile << "Frequency: " << frequency << "\n";
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
    double p, a, f;
    collectData(uart, p, a, f, t, s, m);
    system("clear");
    std::cout << "Recived recorded data size= " << t.size() << std::endl;
    saveDataToFile(p, a, f, s, m, name);
}

void convertIncomingAltitudeMeasurementDataToFile(UART& uart){
    std::string name;
    std::cout << "Type name if file:" << std::endl;
    std::cin >> name;
    std::vector<double> s1, s2, t;
    collectData(uart, t, s1, s2);
    system("clear");
    std::cout << "Recived recorded data size= " << t.size() << std::endl;
    saveDataToFile(0, 0, 0, s1, s2, name);
}

void convertIncomingPidMeasurementDataToFileAndPlot(UART& uart){
    std::string name;
    std::cout << "Type name if file:" << std::endl;
    std::cin >> name;
    std::vector<double> s, m, t;
    double p, a, f;
    collectData(uart, p, a, f, t, s, m);
    system("clear");
    std::cout << "Recived recorded data size= " << t.size() << std::endl;
    saveDataToFile(p, a, f, s, m, name);
    print(t, s, m, name);
}

void convertIncomingAltitudeMeasurementDataToFileAndPlot(UART& uart){
    std::string name;
    std::cout << "Type name if file:" << std::endl;
    std::cin >> name;
    std::vector<double> s1, s2, t;
    collectData(uart, t, s1, s2);
    system("clear");
    std::cout << "Recived recorded data size= " << t.size() << std::endl;
    saveDataToFile(0, 0, 0, s1, s2, name);
    printAltitudeMeasurement(t, s1, s2, name);
}

void print(const std::vector<double>& t, const std::vector<double>& s, const std::vector<double>& m, const std::string& name){
    plot::figure_size(1200, 780);
    plot::title("Filtred roll angle.");
    plot::named_plot("measure [deg]", t, m);
    plot::named_plot("set value [%]", t, s);
    plot::legend();
    plot::save("./outputs/plots/" + name + ".png");
    plot::figure_close();
    std::cout << "Plot saved in: ./outputs/plots/" + name + ".png" << std::endl;
}

void printAltitudeMeasurement(const std::vector<double>& t, const std::vector<double>& s, const std::vector<double>& m, const std::string& name){
    plot::figure_size(1200, 780);
    plot::title("Filtrer altitude measurement.");
    plot::named_plot("measure [m]", t, m);
    plot::named_plot("set value [%]", t, s);
    plot::legend();
    plot::save("./outputs/plots/" + name + ".png");
    plot::figure_close();
    std::cout << "Plot saved in: ./outputs/plots/" + name + ".png" << std::endl;
}
