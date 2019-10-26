/**
 * \file CBitmap.hpp
 * Declaration and implementation of CBitmap class
 * \author Lukas Hromadnik <hromalu1@fit.cvut.cz>
 */

using namespace std;

/**
 * This class is the place where objects are being drawn. It's matrix of specified height and width.
 */

class CBitmap {

public:
	/**
	 * Constructor
	 * \param[in] x X-cord
	 * \param[in] y Y-cord
	 */
	CBitmap (int x, int y);

	/**
	 * Destructor
	 */
	virtual ~CBitmap (void);

	/**
	 * Add new object to the canvas
	 * \param[in]       x X-cord of the object
	 * \param[in]       y Y-cord of the object
	 * \param[in] pattern Color of the object
	 */
	virtual void Add (int x, int y, char pattern);

	/**
	 * Add new color at x y cords to canvas
	 * \param[in] pattern Color of the object
	 */
	void SetBackground (char pattern);

	/** Set signal for overflow alert
	 * \param[in] alert Set availability of the alert
	 */
	void SetOverflowAlert (bool alert);

	/** Print the canvas to the specified output
	 * \param[in]     os Reference to the output
	 * \param[in] spaces Choose if every symbol will be surrounded with spaces for better result
	 */
	void Output (ostream & os, bool spaces);

	/**
	 * It fills a part of canvas which is bordered with any other symbols that is not at the [x, y] coordinates.
	 * \param[in]         x X-cord where the filling starts
	 * \param[in]         y Y-cord where the filling starts
	 * \param[in]   pattern Color of the filling
	 * \param[in] direction Number of directions for the filling (4 or 8)
	 */
	void Fill (int x, int y, char pattern, int direction);

protected:
	/// Width of the Bitmap (canvas)
	int m_X;

	/// Height of the Bitmap (canvas)
	int m_Y;

	/// Background pattern - default symbol is space
	char m_Background;

	/// Signal for using overflow
	bool m_OverflowAlert;

	/// It contains drawn objects
	vector<vector <char> > m_Data;
};

// ------------------------------------------------------------------------------

CBitmap::CBitmap (int x, int y) {
	// ++ for better orientation - interval < 0 , x | y >
	m_X = ++x;
	m_Y = ++y;
	m_Background = ' ';
	m_OverflowAlert = false;

	m_Data.resize(m_Y);
	for (auto it = m_Data.begin(); it != m_Data.end(); it++)
		it->resize(m_X, m_Background);
}

// ------------------------------------------------------------------------------

CBitmap::~CBitmap (void) {

}

// ------------------------------------------------------------------------------

void CBitmap::Add (int x, int y, char pattern) {

	if (m_OverflowAlert) {
		if (x >= m_X)
			throw ExceptionBase ("Coordinate X is out of range");
		
		if (y >= m_Y)
			throw ExceptionBase ("Coordinate Y is out of range");
	}

	if (x < m_X && y < m_Y)
		m_Data[y][x] = pattern;
}

// ------------------------------------------------------------------------------

void CBitmap::SetBackground (char pattern) {
	for (int i = 0; i < m_Y; i++)
		for (int j = 0; j < m_X; j++)
			if (m_Data[i][j] == m_Background)
				m_Data[i][j] = pattern;

	m_Background = pattern;
}

// ------------------------------------------------------------------------------

void CBitmap::SetOverflowAlert (bool alert) {
	m_OverflowAlert = alert;
}

// ------------------------------------------------------------------------------

void CBitmap::Output (ostream & os, bool spaces = true) {
	if (spaces) {
		for (auto it = m_Data.begin(); it != m_Data.end(); it++) {
			
			for (auto it2 = it->begin(); it2 != it->end(); it2++)
				os << *it2 << " ";

			os << endl;
		}
	}
	else {
		for (auto it = m_Data.begin(); it != m_Data.end(); it++) {
			
			for (auto it2 = it->begin(); it2 != it->end(); it2++)
				os << *it2;

			os << endl;
		}
	}
}

// ------------------------------------------------------------------------------

void CBitmap::Fill (int x, int y, char pattern, int directions = 4) {
	if (x >= m_X || y >= m_Y)
		throw ExceptionBase ("You cannot fill space which is out of range");

	if ( ! (directions == 4 || directions == 8))
		throw ExceptionBase ("Fill must be only 4-directional or 8-directional");

	char base = m_Data[y][x];
	int vector[8][2] = { {0, -1}, {1, 0}, {0, 1}, {-1, 0}, {1, -1}, {1, 1}, {-1, 1}, {-1, -1} };
	m_Data[y][x] = pattern;
	
	for (int i = 0; i < directions; i++) {
		int x0 = x + vector[i][0];
		int y0 = y + vector[i][1];
		if (x0 < m_X && y0 < m_Y && x0 >= 0 && y0 >= 0 && m_Data[y0][x0] == base)
			this->Fill(x0, y0, pattern, directions);
	}
}
