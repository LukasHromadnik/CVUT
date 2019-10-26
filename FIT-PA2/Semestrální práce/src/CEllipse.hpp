/**
 * \file CEllipse.hpp
 * Declaration and implementation of CEllipse class
 * \author Lukas Hromadnik <hromalu1@fit.cvut.cz>
 */

using namespace std;

/**
 * It stores coordinates of the center of an ellipse (x, y), height, width
 * pattern and pointer to object where this object will be drawn.
 */

class CEllipse : public CBase {

public:
	/**
	 * Constructor
	 * \param[in]       x X-cord of the center of the object
	 * \param[in]       y Y-cord of the center of the object
	 * \param[in]   width Size of the major semi-axis
	 * \param[in]  height Size of the minor semi-axis
	 * \param[in] pattern Color od the object
	 * \param[in]  bitmap Place where the object will be drawn
	 */
	CEllipse (int x, int y, int width, int height, char pattern, CBitmap * bitmap);

	/**
	 * Draw the object!
	 */
	virtual void Draw (void) const;

protected:
	/// Width of the ellipse
	int m_Width;

	/// Height of the ellipse
	int m_Height;
};

// ------------------------------------------------------------------------------

CEllipse::CEllipse (int x, int y, int width, int height, char pattern, CBitmap * bitmap) : CBase (x, y, pattern, bitmap) {
	m_Width = width;
	m_Height = height;

	Draw();
}

// ------------------------------------------------------------------------------

void CEllipse::Draw (void) const {
	int a2 = m_Width * m_Width;
	int b2 = m_Height * m_Height;
	int fa2 = 4 * a2, fb2 = 4 * b2;
	int x, y, sigma;

	for (x = 0, y = m_Height, sigma = 2 * b2 + a2 * (1 - 2 * m_Height); b2 * x <= a2 * y; x++) {
		m_Bitmap->Add(m_X + x, m_Y + y, m_Pattern);
		m_Bitmap->Add(m_X - x, m_Y + y, m_Pattern);
		m_Bitmap->Add(m_X + x, m_Y - y, m_Pattern);
		m_Bitmap->Add(m_X - x, m_Y - y, m_Pattern);
		
		if (sigma >= 0) {
			sigma += fa2 * (1 - y);
			y--;
		}

		sigma += b2 * ((4 * x) + 6);
	}

	for (x = m_Width, y = 0, sigma = 2 * a2 + b2 * ( 1 - 2 * m_Width); a2 * y <= b2 * x; y++) {
		m_Bitmap->Add(m_X + x, m_Y + y, m_Pattern);
		m_Bitmap->Add(m_X - x, m_Y + y, m_Pattern);
		m_Bitmap->Add(m_X + x, m_Y - y, m_Pattern);
		m_Bitmap->Add(m_X - x, m_Y - y, m_Pattern);
		
		if (sigma >= 0) {
			sigma += fb2 * (1 - x);
			x--;
		}

		sigma += a2 * ((4 * y) + 6);
	}
}
