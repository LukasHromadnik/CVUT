#include "header.cpp"

class Timer {
public:
	Timer(string message, int processNumber = 0);
	void start();
	void end();
	void show();

private:
	string m_Message;
	double m_Start;
	double m_End;
	int m_ProcessNumber;
	void print();
};

Timer::Timer(string message, int processNumber) {
	m_Message = message;
	m_ProcessNumber = processNumber;
	start();
}

void Timer::start() {
	m_Start = MPI_Wtime();
}

void Timer::end() {
	m_End = MPI_Wtime();

	if ( ! USE_DEBUG) return;

	print();
}

void Timer::show() {
	if ( ! USE_DEBUG) {
		print();
	}
}

void Timer::print() {
	cout << "[" << setw(2) << m_ProcessNumber << "] It takes " << m_End - m_Start << "s to " << m_Message << endl;
}