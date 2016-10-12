CLASSES = ./Classes
THREADFLAGS = -lpthread
MYSQLPPFLAGS = -I/usr/include/mysql -I/usr/local/include/mysql++ -L/usr/local/lib -lmysqlpp -lmysqlclient -lnsl -lz -lm


controller: ${CLASSES}/Tools.cpp ${CLASSES}/SerialPort.cpp ${CLASSES}/ComDispatcher.cpp ${CLASSES}/Controller.cpp $(CLASSES)/DBManager.cpp controller.cpp
	g++ -std=c++11 $(THREADFLAGS) $(MYSQLPPFLAGS) ${CLASSES}/Tools.cpp ${CLASSES}/SerialPort.cpp ${CLASSES}/ComDispatcher.cpp ${CLASSES}/Controller.cpp $(CLASSES)/DBManager.cpp controller.cpp -o controller.o
