close all;
clear all;

t_1_2_3 = [.01:.01:5];
t_4_5   = [.01:.01:10];

filesNames = [
    'dataFiles/slot1.txt'
    'dataFiles/slot2.txt'
    'dataFiles/slot3.txt'
    'dataFiles/slot4.txt'
    'dataFiles/slot5.txt'
];

figure
hold on;
grid on;
for slotNumber = 1 : size(filesNames, 1)
    settedData = 100;
    measuredData = 100;
    fid = fopen(filesNames(slotNumber,:));
    for i = 1 : 5
        tline = fgetl(fid);
    end
    while ischar(tline)
        % disp(tline)
        tmp = sscanf(tline,'%f');
        settedData = [settedData, tmp(1)];
        measuredData = [measuredData, tmp(2)];
        tline = fgetl(fid);
    end
    fclose(fid);
    clear tmp tline fid i 
    settedData = settedData(2:end)';
    measuredData = measuredData(2:end)';
    subplot(2, 3, slotNumber)

    switch slotNumber
        case 1
            slot1_settedData = settedData;
            slot1_measuredData = measuredData;
            plot(t_1_2_3, slot1_measuredData, t_1_2_3, slot1_settedData)
        case 2
            slot2_settedData = settedData;
            slot2_measuredData = measuredData;
            plot(t_1_2_3, slot2_measuredData, t_1_2_3, slot2_settedData)
        case 3
            slot3_settedData = settedData;
            slot3_measuredData = measuredData;
            plot(t_1_2_3, slot3_measuredData, t_1_2_3, slot3_settedData)
        case 4
            slot4_settedData = settedData;
            slot4_measuredData = measuredData;
            plot(t_4_5, slot4_measuredData, t_4_5, slot4_settedData)
        case 5
            slot5_settedData = settedData;
            slot5_measuredData = measuredData;
            plot(t_4_5, slot5_measuredData, t_4_5, slot5_settedData)
    end
    legend('measure [deg]','set value [%]');
    title(erase(filesNames(slotNumber,:),"dataFiles/"));
end

clear slotNumber settedData measuredData filesNames ans t_1_2_3 t_4_5
