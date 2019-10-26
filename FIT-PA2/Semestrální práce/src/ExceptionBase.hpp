/**
 * \file ExceptionBase.hpp
 * Declaration and implementation of ExceptionBase class
 * \author Lukas Hromadnik <hromalu1@fit.cvut.cz>
 */

using namespace std;

/**
 * This class is base for every custom exception
 */

class ExceptionBase {

public:
	/**
	 * Constructor
	 * \param[in] input Message which will be printed in terminal
	 */
	ExceptionBase (const string & input);

	/**
	 * Virtual method for print child classes
	 * \param[in] os Reference to the output
	 */
	virtual void Print (ostream & os) const;

	/**
	 * Print the message!
	 * \param[in] os Reference to the output
	 * \param[in]  a Reference to this class
	 */
	friend ostream & operator << (ostream & os, const ExceptionBase & a);

protected:
	/// Text message
	string m_Input;
};

// ------------------------------------------------------------------------------

ExceptionBase::ExceptionBase (const string & input) {
	m_Input = input;
}

// ------------------------------------------------------------------------------

void ExceptionBase::Print (ostream & os) const {
	os << m_Input << endl;
}

// ------------------------------------------------------------------------------

ostream & operator << (ostream & os, const ExceptionBase & a) {
	a.Print(os);

	return os;
}
