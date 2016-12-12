#pragma once
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>

enum class LogType {
	eDebugInfo,
	eWarning,
	eError,
	eCritical,
};

class Logger {
public:
	static void start() {
		logfile.open("log.txt");
	}

	static void end() {
		logfile.close();
	}

	static void write(LogType type, const std::string &log, bool writeToConsole = false) {
		std::string str = to_string(type) + ": " + log + "\n";

		if(writeToConsole) {
			printf("%s", str.c_str());
		}

		logfile.write(str.c_str(), str.length());		// fprintf(logfile, "%s %s\n", to_string(type).c_str(), log.c_str());
	}

	static std::string to_string(LogType type) {
		switch(type) {
			case LogType::eDebugInfo 	: return "DEBUG";
			case LogType::eWarning 		: return "WARNING";
			case LogType::eError 		: return "ERROR";
			case LogType::eCritical 	: return "CRITICAL";
		}
		return "NO VALID LOGGING TYPE";
	}


private:
	static std::ofstream logfile;
	Logger() {};
};

std::ofstream Logger::logfile;