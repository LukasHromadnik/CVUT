/**
 * \file Validation.hpp
 * Declaration and implementation of Validation class
 * \author Lukas Hromadnik <hromalu1@fit.cvut.cz>
 */

using namespace std;

/**
 * This class is extension for verification of some variables
 */

class Validation {

public:
	/**
	 * Test if string contains only digital numbers
	 * \param[in] str Test string
	 */
	static bool IsDigit (const string & str);

	/**
	 * Test if string contains only one number from interval < 0, 1 >
	 * \param[in] str Test string
	 */
	static bool IsBool (const string & str);

	/**
	 * Return bool from string
	 * \param[in]  str Input string
	 * \param[out] str String in bool form
	 */
	static bool ReturnBool (const string & str);

	/**
	 * Test if file exists
	 * \param[in] name Name of the test file
	 */
	static bool FileExists (const string & name);
};

// ------------------------------------------------------------------------------

bool Validation::IsDigit (const string & str) {
	if ( ! str.length())
		return false;

	for (int i = 0; i < (int) str.length(); i++)
		if ( ! isdigit(str[i]))
			return false;

	return true;
}

// ------------------------------------------------------------------------------

bool Validation::IsBool (const string & str) {
	if (IsDigit(str))
		return (str.length() == 1 && (str[0] == '0' || str[0] == '1'));
	else if (str == "false" || str == "true")
		return true;
	else
		return false;
}

// ------------------------------------------------------------------------------

bool Validation::ReturnBool (const string & str) {
	if ( ! IsBool(str))
		throw ExceptionBase (str + " is not bool value");

	if (str.length() > 1)
		return (str == "true");
	else
		return (str == "1");
}

// ------------------------------------------------------------------------------

bool Validation::FileExists (const string & name) {
	ifstream file (name);
    
    if (file.good()) {
        file.close();
        return true;
    } else {
        file.close();
        return false;
    }   
}
