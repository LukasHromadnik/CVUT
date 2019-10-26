#include <iostream>
#include <math.h>

using namespace std;

class Point
{
public:
	Point(int x, int y) : m_X(x), m_Y(y) {};
	int getX() const { return m_X; };
	int getY() const { return m_Y; };
protected:
	int m_X;
	int m_Y;
};

int compare(const void * a, const void * b)
{
	double x = *(double *)a;
	double y = *(double *)b;

	double diff = x - y;

	if (diff > 0) {
		return 1;
	} else {
		return -1;
	}
}

int binary(double * arr, int count, double value)
{
	int mid, lower = 0, upper = count - 1;
	
	double tmp = arr[0] - (arr[0] * 1e-12);
	double myVal = value - (value * 1e-12);
	
	if ((tmp - myVal) > 1e-12) {
		return 0;
	}

	while (lower <= upper) {
		mid = (lower + upper) / 2;
		tmp = arr[mid] - (arr[mid] * 1e-12);

		if ((myVal - tmp) > 1e-12) {
			lower = mid + 1;
		} else if ((tmp - myVal) > 1e-12) {
			upper = mid - 1;
		} else {
			return mid;
		}
	}

	return lower;
}

int main(int argc, char const *argv[])
{
	// Nacteni poctu bodu
	int count;
	cin >> count;

	// Celkovy pocet moznych trojuhelniku
	unsigned long long totalCount = (count * (count - 1) * (count - 2)) / 6;
	
	// Counter poctu neostrouhlych trojuhleniku
	unsigned long long total = 0;

	// Pole s body
	Point ** points = new Point * [count];

	// Pole pro úhly
	double ** angles = new double * [count];
	for (int i = 0; i < count; i++) {
		angles[i] = new double [count];
	}

	// Nacteni bodu
	for (int i = 0; i < count; i++) {
		int x, y;
		cin >> x;
		cin >> y;
		points[i] = new Point(x, y);
	}

	// Vypocet uhlu mezi body
	for (int i = 0; i < count; i++) {
		for (int j = 0; j < count; j++) {
			if (i == j) {
				angles[i][j] = 720;
				continue;
			}

			Point * p1 = points[i];
			Point * p2 = points[j];			

			// Base point
			int y = 0;
			if (p1->getY() == 0) {
				y = -1;
			}
			Point * base = new Point(p1->getX(), y);
			// Point * base = new Point(10, 1);

			Point * v1 = new Point(p1->getX() - base->getX(), p1->getY() - base->getY());
			Point * v2 = new Point(p2->getX() - p1->getX(), p2->getY() - p1->getY());

			double angle = atan2(v2->getY(), v2->getX()) - atan2(v1->getY(), v1->getX());
			// double angle = atan2(v2->getY(), v2->getX());
			angle = angle * (180 / M_PI);
			angle *= -1;

			if (angle < 0) {
				angle *= -1;
				angle = 360 - angle;
			}

			angles[i][j] = angle;
		}

		// Serazeni uhlu
		qsort(angles[i], count, sizeof(angles[i][0]), compare);
	}

	for (int i = 0; i < count; i++) {
		for (int j = 0; j < count; j++) {
			double alfa = angles[i][j];

			if (alfa + 90 >= 360) continue;

			int key1 = binary(angles[i], count, alfa + 90);
			int key2 = binary(angles[i], count, alfa + 270);
			int diff = key2 - key1;

			if (fabs(angles[i][key2] - (alfa + 270)) < 1e-12) {
				diff++;
			}

			total += diff;
		}
	}

	unsigned long long out = totalCount - total;
	// if (out > 20000000) {
		// out--;
	// }
	cout << out << endl;

	return 0;
}
