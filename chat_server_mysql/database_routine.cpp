#pragma once
#include <mysql.h>
#include <iostream>

static MYSQL mysql;
static MYSQL_RES* res;
static MYSQL_ROW row;

inline bool initialize_mysql() {
	mysql_init(&mysql);
	if (&mysql == nullptr) {
		std::cout << "Error: can't create MySQL-descriptor" << std::endl;
		system("pause");

		return false;
	}

	if (!mysql_real_connect(&mysql, "localhost", "root", "qxhq6frf", "udptest", NULL, NULL, 0)) {
		std::cout << "Error: can't connect to database " << mysql_error(&mysql) << std::endl;
		system("pause");

		return false;
	}
	else {
		std::cout << "Success!" << std::endl;
		system("pause");

		mysql_query(&mysql, "CREATE TABLE IF NOT EXISTS Users(id INT AUTO_INCREMENT PRIMARY KEY, login VARCHAR(30), password VARCHAR(30))");
		mysql_query(&mysql, "ALTER TABLE Users ADD CONSTRAINT user_unique UNIQUE (login)");
		mysql_query(&mysql, "INSERT INTO Users(id, login, password) values(default, 'Admin', 'qwerty')");
		mysql_query(&mysql, "INSERT INTO Users(id, login, password) values(default, 'Andrey', '12345')");
		mysql_query(&mysql, "INSERT INTO Users(id, login, password) values(default, 'Nastya', 'asdf')");

		mysql_query(&mysql, "CREATE TABLE IF NOT EXISTS Chat(id INT AUTO_INCREMENT PRIMARY KEY, text TEXT, time TIMESTAMP DEFAULT CURRENT_TIMESTAMP())");

		mysql_query(&mysql, "SELECT * FROM Users"); 

		if (res = mysql_store_result(&mysql)) {
			while (row = mysql_fetch_row(res)) {
				for (int i = 0; i < mysql_num_fields(res); i++) {
					std::cout << row[i] << "  ";
				}
				std::cout << std::endl;
			}
		}
		else {
			std::cout << "MySQL Error: " << mysql_error(&mysql);

			return false;
		}
			
		return true;
	}
}

