/**
 * \file CRectangle.hpp
 * Declaration and implementation of CRectangle class
 * \author Lukas Hromadnik <hromalu1@fit.cvut.cz>
 */

using namespace std;

/**
 * It stores top left coordinates (x, y), width, height, pattern
 * and pointer to object where this object will be drawn.
 */

class CRectangle : public CBase {

public:
	/**
	 * Constructor
	 * \param[in]       x X-cord of the top-left corner of the object
	 * \param[in]       y Y-cord of the top-left corner of the object
	 * \param[in]   width Width of the object
	 * \param[in]  height Height of the object
	 * \param[in] pattern Color of the object
	 * \param[in]  bitmap Place where the object will be drawn
	 */
	CRectangle (int x, int y, int width, int height, char pattern, CBitmap * bitmap);

	/**
	 * Draw the object!
	 */
	virtual void Draw (void) const;

protected:
	/// Width of the Rectangle
	int m_Width;

	/// Height of the Rectagle
	int m_Height;
};

// ------------------------------------------------------------------------------

CRectangle::CRectangle (int x, int y, int width, int height, char pattern, CBitmap * bitmap) : CBase (x, y, pattern, bitmap) {
	m_Width = width;
	m_Height = height;

	Draw();
}

// ------------------------------------------------------------------------------

void CRectangle::Draw (void) const {
	int x0 = m_X;
	int y0 = m_Y;
	int x1 = m_X + m_Width;
	int y1 = m_Y + m_Height;

	CLine a (x0, y0, x1, y0, m_Pattern, m_Bitmap);
	CLine b (x0, y0, x0, y1, m_Pattern, m_Bitmap);
	CLine c (x1, y0, x1, y1, m_Pattern, m_Bitmap);
	CLine d (x0, y1, x1, y1, m_Pattern, m_Bitmap);
}
