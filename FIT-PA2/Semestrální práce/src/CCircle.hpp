/**
 * \file CCircle.hpp
 * Declaration and implementation of CCircle class
 * \author Lukas Hromadnik <hromalu1@fit.cvut.cz>
 */

using namespace std;

/**
 * Very simple simplification of the CEllipse object that set width and height to the same value.
 */

class CCircle : public CEllipse {

public:
	/**
	 * Constructor
	 * \param[in]       x X-cord of the center of the object
	 * \param[in]       y Y-cord of the center of the object
	 * \param[in]   width Size of the major semi-axis
	 * \param[in] pattern Color of the object
	 * \param[in]  bitmap Place where the object will be drawn
	 */
	CCircle (int x, int y, int width, char pattern, CBitmap * bitmap);

	/**
	 * Draw the object!
	 */
	virtual void Draw (void) const;	
};

// ------------------------------------------------------------------------------

CCircle::CCircle (int x, int y, int width, char pattern, CBitmap * bitmap) : CEllipse (x, y, width, width, pattern, bitmap) {
}

// ------------------------------------------------------------------------------

void CCircle::Draw (void) const {
	CEllipse::Draw();
}
