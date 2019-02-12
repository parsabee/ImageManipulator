#include "logging.h"
#include <iostream>
#include <fstream>

using namespace std;
ofstream Logger::logger("logger.txt");

DataFlowException::DataFlowException(const char *type, const char *error) {
		sprintf(msg, "Throwing exception: (%s): %s ", type, error); 
		Logger::LogEvent(msg);
}

void Logger::LogEvent(const char *event) {
	logger << event << endl;
}


void Logger::Finalize() {
	logger.close();
}