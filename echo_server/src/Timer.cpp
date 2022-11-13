#include <ctime>
#include <string>
#include <chrono>
#include <sstream>
#include <iostream>

#include "Timer.h"

std::string GetCurrentTimeStamp(int time_stamp_type)
{
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

	std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
	std::tm* now_tm = std::localtime(&now_time_t);

	char buffer[128];
	strftime(buffer, sizeof(buffer), "%F %T", now_tm);

	std::ostringstream ss;
	ss.fill('0');

	std::chrono::milliseconds ms;
	std::chrono::microseconds cs;
	std::chrono::nanoseconds ns;
	
	switch (time_stamp_type)
	{
	case 0:
		ss << buffer;
		break;
	case 1:
		ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
		ss << buffer << ":" << ms.count();
		break;
	case 2:
		ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
		cs = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()) % 1000000;
		ss << buffer << ":" << ms.count() << ":" << cs.count() % 1000;
		break;
	case 3:
		ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
		cs = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()) % 1000000;
		ns = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()) % 1000000000;
		ss << buffer << ":" << ms.count() << ":" << cs.count() % 1000 << ":" << ns.count() % 1000;
		break;
	default:
		ss << buffer;
		break;
	}

	return ss.str();
}