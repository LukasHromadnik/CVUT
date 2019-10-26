/**
 * \file CBase.hpp
 * Declaration and implementation of CBase class
 * \author Lukas Hromadnik <hromalu1@fit.cvut.cz>
 */

using namespace std;

/**
 * Abstract class which represents the basic structure for every drawable object.
 * It stores first position (x, y), pattern and pointer to object where will be drawn.
 */

class CBase {

public:
	/**
	 * \param[in]       x X-cord of the object
	 * \param[in]       y Y-cord of the object
	 * \param[in] pattern Color of the object
	 * \param[in]  bitmap Place where the object will be drawn
	 */
	CBase (int x, int y, char pattern, CBitmap * bitmap);

	/**
	 * Print the object!
	 */
	virtual void Draw (void) const = 0;

protected:
	/// First (x) coordinate of the object
	int m_X;

	/// Second (y) coordinate of the object
	int m_Y;

	/// Color of the object
	char m_Pattern;

	/// Pointer to the Bitmap on which will be drawn.
	CBitmap * m_Bitmap;
};

// ------------------------------------------------------------------------------

CBase::CBase (int x, int y, char pattern, CBitmap * bitmap) {
	m_X = x;
	m_Y = y;
	m_Pattern = pattern;
	m_Bitmap = bitmap;
}
