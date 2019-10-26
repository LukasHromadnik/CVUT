/**
 * \file CPoint.hpp
 * Declaration and implementation of CPoint class
 * \author Lukas Hromadnik <hromalu1@fit.cvut.cz>
 */

using namespace std;

/**
 * It stores coordinates (x, y), pattern and pointer to object where this object will be drawn.
 */

class CPoint : public CBase {

public:
	/**
	 * Constructor
	 * \param[in]       x X-cord of the object
	 * \param[in]       y Y-cord od the object
	 * \param[in] pattern Color of the object
	 * \param[in]  bitmap Place where the object will be drawn
	 */
	CPoint (int x, int y, char pattern, CBitmap * bitmap);

	/**
	 * Draw the object!
	 */
	virtual void Draw (void) const;
};

// ---------------------------------------------------------------------------

CPoint::CPoint (int x, int y, char pattern, CBitmap * bitmap) : CBase (x, y, pattern, bitmap) {
	Draw();
}

// ---------------------------------------------------------------------------

void CPoint::Draw (void) const {
	m_Bitmap->Add(m_X, m_Y, m_Pattern);
}
