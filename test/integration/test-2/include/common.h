#pragma once
#include <fstream>
#include <sstream>
#include <iostream>
#include <iterator>
#include <string.h>
#include <chrono>
#include <ctime>

using std::string;
using std::cout;
using std::endl;
using std::ios;
using std::ostream;
using std::to_string;


const string COMMON_WEEKDAY[7] = {
	"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
};

const string COMMON_MONTH[12] = {
	"January", "February", "March",
	"April", "May", "June",
	"July", "August", "September",
	"October", "November", "December"
};

struct tm get_time_t_timestamp()
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	return tm;
}

string get_current_datetime_str()
{
	struct tm tm = get_time_t_timestamp();
	return to_string(tm.tm_year + 1900) + ":"
		   + to_string(tm.tm_mon) + ":"
		   + to_string(tm.tm_mday) + " "
		   + to_string(tm.tm_hour) + ":"
		   + to_string(tm.tm_min) + ":"
		   + to_string(tm.tm_sec);
}

bool sys_mkdir()
{
	// Makes system directory if there isn't one
	return false;
}

bool base_str_contains(string _s1 = "", string _s2 = "") { return bool(_s1.find(_s2)); }
bool base_str_contains(const char *_s1 = "", const char *_s2 = "") { return base_str_contains(string(_s1), string(_s2)); }
bool base_str_contains(char *_s1 = "", char *_s2 = "") { return base_str_contains(string(_s1), string(_s2)); }


class Logger {
    private:
        string m_logger_name;

    public:
        Logger(const char *_m_logger_name, bool _add_dot_log = false) : m_logger_name(string(_m_logger_name)) {
			/* const char *logger_name: the file location */
			m_logger_name =   string(_m_logger_name) + ( _add_dot_log  ? ".log" : "");
		}
		
	private:
		void Log(string msg) {
			// Take file handler and open log file
			std::ofstream  m_file_handler(m_logger_name, ios::app);
			m_file_handler << get_current_datetime_str() << "|" << msg << "\n\n";
			m_file_handler.close();
        }
    public:
		void operator<<(string msg) {
			Log(msg);
			cout << msg << endl;
		}
		void operator<<(const char *msg) {
			string s = string(msg);
			Log(s);
			cout << msg << endl;
		}
		void operator<<(uint64_t msg) {
			string s = to_string(msg);
			Log(s);
			cout << msg << endl;
		}
		~Logger() {}
};

// Error logging
#define INST_ERROR_LOGGER(x) x = new Logger("error") 
#define STANDBY system("pause")

class AppError {
	/* Class used as method/function assertions and testing. */
	private:
	public:
		AppError() {}
		~AppError() {}
};
