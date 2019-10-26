/**
 * \file InvalidFunctionArgument.hpp
 * Declaration and implementation of InvalidFunctionArgument class
 * \author Lukas Hromadnik <hromalu1@fit.cvut.cz>
 */

using namespace std;

/**
 * This exception is called when given arguments for constructor of some class are in bad format
 */

class InvalidFunctionArgument : public ExceptionBase {

public:
	/**
	 * Constructor
	 * \param[in]   cls Name of the class from which exception was called
	 * \param[in] input Message which will be printed in the terminal
	 */
	InvalidFunctionArgument (const string & cls, const string & input);

	/**
	 * Virtual method for easy message print
	 * \param[in] os Reference to the output
	 */
	virtual void Print (ostream & os) const;

protected:
	/// Object from which is exception called
	string m_Class;
};

// ------------------------------------------------------------------------------

InvalidFunctionArgument::InvalidFunctionArgument (const string & cls, const string & input) : ExceptionBase (input) {
	m_Class = cls;
}

// ------------------------------------------------------------------------------

void InvalidFunctionArgument::Print (ostream & os) const {
	os << "Arguments for " << m_Class << " must be in format (" << m_Input << ")" << endl;
}
