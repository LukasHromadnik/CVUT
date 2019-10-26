/**
 * \file CSquare.hpp
 * Declaration and implementation of CSquare class
 * \author Lukas Hromadnik <hromalu1@fit.cvut.cz>
 */

using namespace std;

/**
 * Very simple simplification of the CRectangle object that sets width and height to the same value.
 */

class CSquare : public CRectangle {

public:
	/**
	 * Constructor
	 * \param[in]       x X-cord of the top-left corner of the object
	 * \param[in]       y Y-cord of the top-left corner of the object
	 * \param[in]   width Width (and height) of the object
	 * \param[in] pattern Color of the object
	 * \param[in]  bitmap Place where te object will be drawn
	 */
	CSquare (int x, int y, int width, char pattern, CBitmap * bitmap);

	/**
	 * Draw the object!
	 */
	virtual void Draw (void) const;
};

// ------------------------------------------------------------------------------

CSquare::CSquare (int x, int y, int width, char pattern, CBitmap * bitmap) : CRectangle (x, y, width, width, pattern, bitmap) {
}

// ------------------------------------------------------------------------------

void CSquare::Draw (void) const {
	CRectangle::Draw();
}
