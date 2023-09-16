#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <shared_mutex>

class Logger
{
private:
	std::string filename = "log.txt";
	std::fstream log_file;
	
	std::shared_mutex shared_mutex;
public:
	Logger();
	~Logger();

	void write_log(std::string);
	std::string read_log();
};

