# Требуемые компоненты
qt5-base  
gcc  
sqlite  
# Команды для установки

    git clone https://github.com/OpenDiv/dbus-test-task
    sudo pacman -S qt5-base gcc sqlite
    cd ~/dbus-test-task/
    sudo chmod +x build_files.sh
    ./build_files.sh

Дальше запуск из терминала или граф. оболочки двух файлов:

    ~/dbus-test-task/firstTask/qdbusTestTaskFirst
    ~/dbus-test-task/secondTask/qdbusTestTaskSecond
Затем снова выполнить в терминале

    cd ~/dbus-test-task/thirdTask/
    ./thirdTask

Для завершения firstTask и secondTask искать среди сессионных шин по названию **com.system.permissions** и **com.system.time** соответственно.
