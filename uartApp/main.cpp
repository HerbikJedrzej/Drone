#include <iostream>
#include <uart.hh>
#include <consoleOptions.hh>

bool mainChoice(UART& uart){
    std::string choose;
    std::cout << std::endl <<
    "Choose bellow options:" << std::endl <<
    "    1 - print incoming data" << std::endl <<
    "    2 - convert incoming PID measurements data to file" << std::endl <<
    "    3 - convert incoming PID measurements data to file and plot" << std::endl <<
    "    4 - convert incoming altitude measurements data to file" << std::endl <<
    "    5 - convert incoming altitude measurements data to file and plot" << std::endl <<
    "    q - exit" << std::endl;
    std::cin >> choose;
    system("clear");
    switch(choose[0]){
        case '1':
            printIncomingData(uart);
            system("clear");
            return true;
        case '2':
            convertIncomingPidMeasurementDataToFile(uart);
            return true;
        case '3':
            convertIncomingPidMeasurementDataToFileAndPlot(uart);
            return true;
        case '4':
            convertIncomingAltitudeMeasurementDataToFile(uart);
            return true;
        case '5':
            convertIncomingAltitudeMeasurementDataToFileAndPlot(uart);
            return true;
        case 'q':
            return false;
        default:
            std::cout << "There is no suc option as " << choose << std::endl;
    }
    return true;
}

int main(int argc, char *argv[]){
    system("clear");
    if(argc != 2){
        std::cout << "Wrong calling of application." <<
        std::endl << "run conamd: ./uartApp number_of_port" << std::endl;
        return 1;
    }
    UART uart(argv[1], UART::Barude::b115200);
    std::cout << "UART port is open." << std::endl;
    while(mainChoice(uart));
    return 0;
}
