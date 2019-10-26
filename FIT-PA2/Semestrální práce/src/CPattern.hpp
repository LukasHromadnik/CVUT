/**
 * \file CPattern.hpp
 * Declaration and implementation of CPattern class
 * \author Lukas Hromadnik <hromalu1@fit.cvut.cz>
 */

using namespace std;

/**
 * This class is pattern for the background. It can be placed in every possible place in the bitmap.
 */

class CPattern : public CBitmap {

public:
	/**
	 * Constructor
	 */
	CPattern (void);

	/**
	 * Destructor
	 */
	virtual ~CPattern (void);

	/**
	 * Add new object to the pattern
	 * \param[in]       x X-cord of the object
	 * \param[in]       y Y-cord of the object
	 * \param[in] pattern Color of the object
	 */
	virtual void Add (int x, int y, char pattern);

	/**
	 * Width is automaticly counted from size of given objects.
	 * \param[in] x New X-cord of the Bitmap
	 */
	void SetX (int x);

	/**
	 * Height is automaticly counted from size of given objects.
	 * \param[in] y New Y-cord of the Bitmap
	 */
	void SetY (int y);

	/**
	 * Print the pattern wherever you want
	 * \param[in]      x X-cord of the top-left corner where the object will be drawn
	 * \param[in]      y Y-cord of the top-left corner where the object will be drawn
	 * \param[in] bitmap Place where the object will be drawn
	 */
	void Print (int x, int y, CBitmap * bitmap);
};

// ------------------------------------------------------------------------------

CPattern::CPattern (void) : CBitmap (0, 0) {
	m_X = 0;
	m_Y = 0;
}

// ------------------------------------------------------------------------------

CPattern::~CPattern (void) {
	
}

// ------------------------------------------------------------------------------

void CPattern::SetX (int x) {
	m_X = ++x;

	for (auto it = m_Data.begin(); it != m_Data.end(); it++)
		it->resize(m_X);
}

// ------------------------------------------------------------------------------

void CPattern::SetY (int y) {
	m_Y = ++y;

	m_Data.resize(m_Y, vector<char>(m_X));
}

// ------------------------------------------------------------------------------

void CPattern::Add (int x, int y, char pattern) {
	if (x >= m_X)
		SetX (x);

	if (y >= m_Y)
		SetY (y);

	m_Data[y][x] = pattern;
}

// ------------------------------------------------------------------------------

void CPattern::Print (int x, int y, CBitmap * bitmap) {
	for (int i = 0; i < m_Y; i++)
		for (int j = 0; j < m_X; j++)
			if (m_Data[i][j])
				bitmap->Add(j + x, i + y, m_Data[i][j]);
}
