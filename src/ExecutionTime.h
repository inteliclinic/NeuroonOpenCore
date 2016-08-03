#include <iostream>
#include <chrono>

class ExecutionTime {
	std::chrono::time_point<std::chrono::steady_clock> m_start;
	std::string m_message;

public:
	ExecutionTime(std::string message)
	: m_message(message)
	{
		m_start = std::chrono::steady_clock::now();

	}

	~ExecutionTime() {
		auto end = std::chrono::steady_clock::now();
		auto diff = end - m_start;
		std::cout << m_message << ": " << std::chrono::duration<double, milli>(diff).count() << " ms" << std::endl;
	}

};
