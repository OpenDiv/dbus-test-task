Тестовое задание для компании "Открытая мобильная платформа"  
Собрано под Arch Linux x86_64  
Инструкция по сборке:  

git clone https://github.com/OpenDiv/dbus-test-task  
sudo pacman -S qt5-base gcc sqlite  
  
cd ~/dbus-test-task/  
sudo chmod +x build_files.sh  
./build_files.sh  
  
Затем из терминала или графического интерфейса запустить:  
- ~/dbus-test-task/firstTask/qdbusTestTaskFirst  
- ~/dbus-test-task/secondTask/qdbusTestTaskSecond  

Затем в терминале выполнить:  
cd ~/dbus-test-task/thirdTask/
./thirdTask  


Для завершения firstTask и secondTask можно искать среди сессионных шин по названию com.system.permissions и com.system.time соответственно.
