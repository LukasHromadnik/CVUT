/**
 * \file CPolyline.hpp
 * Declaration and implementation of CPolyline class
 * \author Lukas Hromadnik <hromalu1@fit.cvut.cz>
 */

using namespace std;

/**
 * It stores vector of coordinates, pattern and pointer to object where this object will be drawn.
 */

class CPolyline : public CBase {

public:
	/**
	 * Constructor
	 * \param[in]    data Vector of coordinates of the points
	 * \param[in] pattern Color of the object
	 * \param[in]  bitmap Place where the object will be drawn
	 */
	CPolyline (vector<array<int, 2> > data, char pattern, CBitmap * bitmap);

	/**
	 * Draw the object!
	 */
	virtual void Draw (void) const;

protected:
	/// Vector of coordinates
	vector<array<int, 2> > m_Data;

	/// Variable that stores sum of lines
	int m_Length;
};

// ------------------------------------------------------------------------------

CPolyline::CPolyline (vector<array<int, 2> > data, char pattern, CBitmap * bitmap) : CBase (data[0][0], data[0][1], pattern, bitmap) {
	m_Length = (int) data.size();
	m_Data.resize(m_Length + 1);
	m_Data = data;

	Draw();
}

// ------------------------------------------------------------------------------

void CPolyline::Draw (void) const {
	for (int i = 0; i < m_Length - 1; i++) {
		int x0 = m_Data[i][0];
		int y0 = m_Data[i][1];
		int x1 = m_Data[i + 1][0];
		int y1 = m_Data[i + 1][1];

		CLine a (x0, y0, x1, y1, m_Pattern, m_Bitmap);
	}
}
