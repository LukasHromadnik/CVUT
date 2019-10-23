using namespace std;

class Timer {
public:
	Timer(string message, int processNumber = 0);
	void start();
	void end();
	void show();

private:
	string m_Message;
	clock_t m_Start;
	clock_t m_End;
	int m_ProcessNumber;
	void print();
};

Timer::Timer(string message, int processNumber) {
	m_Message = message;
	m_ProcessNumber = processNumber;
	start();
}

void Timer::start() {
	m_Start = clock();
}

void Timer::end() {
	m_End = clock();

	print();
}

void Timer::print() {
	double difference = float(m_End - m_Start) / CLOCKS_PER_SEC;
	cout << "It takes " << difference << "s to " << m_Message << endl;
}
