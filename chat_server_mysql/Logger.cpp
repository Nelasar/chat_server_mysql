#include "Logger.h"

Logger::Logger() {
	log_file.open(filename);

	if (log_file) {
		std::cout << "OPENED\n";
		log_file << "LOGGER INITIALIZATION COMPLETED\nTHIS IS A SERVER LOG FILE\n";
	}
	else {
		throw std::runtime_error("Could not create the file\n");
	}
}
Logger::~Logger() {
	log_file.close();
}
void Logger::write_log(std::string message) {
	shared_mutex.lock();

	log_file.seekg(0, std::ios_base::end);
	log_file << message;

	log_file.flush();

	log_file.seekg(0, std::ios_base::end);

	shared_mutex.unlock();

}
std::string Logger::read_log() {
	shared_mutex.lock_shared();

	if (log_file.is_open()) {
		std::string buffer;/*
		log_file.seekg(-2, std::ios_base::end);

		while (true) {
			char ch;
			log_file.get(ch);

			if ((int)log_file.tellg() <= 1) {
				log_file.seekg(0);
				break;
			}
			else if (ch == '\n') {
				break;
			}
			else {
				log_file.seekg(-1, std::ios_base::cur);
			}
		}*/

		log_file.seekg(-17, std::ios_base::end);

		std::getline(log_file, buffer);
		std::cout << "READ LOG BUFFER: " << buffer;

		shared_mutex.unlock_shared();

		return buffer;
	}
	else {
		shared_mutex.unlock_shared();
		throw std::runtime_error("Error! Could not open the file!.\n");
	}
}