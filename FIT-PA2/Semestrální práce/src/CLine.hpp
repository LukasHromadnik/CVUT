/**
 * \file CLine.hpp
 * Declaration and implementation of CLine class
 * \author Lukas Hromadnik <hromalu1@fit.cvut.cz>
 */

using namespace std;

/**
 * It stores start and end positions (x, y), pattern and pointer to object where this object will be drawn.
 */

class CLine : public CBase {

public:
	/**
	 * Constructor
	 * \param[in]      x0 X-cord of the beginning of the object
	 * \param[in]      y0 Y-cord of the beginning of the object
	 * \param[in]      x1 X-cord of the end of the object
	 * \param[in]      y1 Y-cord of the end of the object
	 * \param[in] pattern Color of the object
	 * \param[in]  bitmap Place where the object will be drawn
	 */
	CLine (int x0, int y0, int x1, int y1, char pattern, CBitmap * bitmap);

	/**
	 * Draw the object!
	 */
	virtual void Draw (void) const;

protected:
	/// First coordinate of the end of the object
	int m_X1;

	/// Second coordinate of the end of the object
	int m_Y1;
};

// ------------------------------------------------------------------------------

CLine::CLine (int x0, int y0, int x1, int y1, char pattern, CBitmap * bitmap) : CBase (x0, y0, pattern, bitmap) {
	m_X1 = x1;
	m_Y1 = y1;

	Draw();
}

// ------------------------------------------------------------------------------

void CLine::Draw (void) const {

	int x0 = m_X;
	int y0 = m_Y;
	int x1 = m_X1;
	int y1 = m_Y1;

	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);

	int sx = (x0 < x1) ? 1 : -1;
	int sy = (y0 < y1) ? 1 : -1;

	int err = dx - dy;


	while (1) {
		m_Bitmap->Add(x0, y0, m_Pattern);

		if (x0 == x1 && y0 == y1)
			break;

		int e2 = 2 * err;

		if (e2 > -dy) {
			err -= dy;
			x0 += sx;
		}
		
		if (e2 < dx) {
			err += dx;
			y0 += sy;
		}
	}
}
