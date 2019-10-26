/**
 * \mainpage Kreslic obrazku v ASCII-artu
 * V textovém soubor je popsáno, co má být nakresleno v obrázku. Program textový soubor interpretuje a výstupem
 * bude textový soubor ve kterém bude výsledný obrázek. V konfiguraci se specifikuje velikost obrázku ve znacích.
 * Kreslič podporuje minimálně: čáru, polyline, kruh, elipsu, obdélník, semínkové vyplňování, různé barvy.
 * Celý obrázek je třeba v paměti reprezentovat pomocí hierarchie objektů. Objekty se potom umí vykreslit do nějakého
 * 2D pole(canvasu). V popisu obrázku je možné definovat pattern, který se bude vykreslovat podobně jako třeba čára.
 * Tzn. nakreslím si kruh a do něj trojúhelník a celý tento pattern můžu vložit na libovolné místo v obrázku.
 * Navíc můžu vložit tento vzor vícekrát třeba v mřížce NxM. Textovy souboru muze, ale nemusi byt ve formatu XML.
 * Výstup je do textového souboru, který obsahuje NxM znaků (velikost je zadána v konf. souboru). Výplně mohou být různé znaky.
 */

#include <iostream>
#include <iomanip>
#include <math.h>
#include <vector>
#include <fstream>
#include <array>
#include <sstream>
#include <cstdlib>
#include <string>

#include "ExceptionBase.hpp"
#include "InvalidFunctionArgument.hpp"

#include "Validation.hpp"

#include "CBitmap.hpp"
#include "CPattern.hpp"
#include "CBase.hpp"
#include "CPoint.hpp"
#include "CLine.hpp"
#include "CPolyline.hpp"
#include "CEllipse.hpp"
#include "CCircle.hpp"
#include "CRectangle.hpp"
#include "CSquare.hpp"

using namespace std;

int main (int argc, const char * argv[]) {
	CBitmap * bitmap = NULL;
	CPattern * pattern = NULL;
	CBitmap * canvas = NULL;
	bool patternInUse = false;
	bool bitmapDefined = false;
	bool inputFound = false;
	bool outputFound = false;
	bool useCout = false;
	
	string inputFile;
	string outputFile;
	bool spaces = false;
	string value;
	char divider = ',';
	char configDivider = '=';

	ifstream configFile;
	string configSrc = "./src/config/config.ini";
	
	if (argc == 2)
		configSrc = "./examples/" + string(argv[1]) + "/config.ini";

	try {
		if ( ! Validation::FileExists(configSrc))
			throw ExceptionBase ("Config file not found");
	}
	catch (ExceptionBase & a) {
		cout << a;
		return 0;
	}

	configFile.open(configSrc);

	ofstream output;

	try {
		while (getline(configFile, value)) {	
			if ( ! value.length())
				continue;

			istringstream line(value);
			string item;
			vector<string> data;

			while (getline(line, item, configDivider))
				data.push_back(item);

			if (data[0] == "input") {
				if ( ! Validation::FileExists (data[1]))
					throw ExceptionBase ("Input file not found");

				inputFile = data[1];
				inputFound = true;
				
			}
			else if (data[0] == "output") {
				if (data[1] == "cout")
					useCout = true;
				else
					outputFile = data[1];

				outputFound = true;
			}
			else if (data[0] == "spaces") {
				spaces = Validation::ReturnBool(data[1]);
			}
			else if (data[0] == "divider") {
				if (data[1].length() > 1)
					throw ExceptionBase ("Divider must be char - it must have length equal 1");
				
				divider = (char) data[1][0];
			}
			else
				throw ExceptionBase ("Unexpected key value: "  + data[0]);
		}

		if ( ! useCout)
			output.open(outputFile);

		if ( ! (inputFound && outputFound))
			throw ExceptionBase ("Input or output is not specified");

		ifstream file (inputFile);

		while (getline(file, value)) {
			if ( ! value.length())
				continue;

			int counter = 0;
			istringstream linestream(value);
			string item;
			vector<string> data;

			while (getline(linestream, item, divider)) {
				data.push_back(item);
				counter++;
			}

			string cls = data[0];

			if (cls != "bitmap" && ! bitmapDefined)
				throw ExceptionBase ("You must defined Bitmap (canvas) at first");

			if (cls == "bitmap") {

				if ( ! (Validation::IsDigit(data[1]) && Validation::IsDigit(data[2])))
					throw InvalidFunctionArgument (cls, "int, int[, char]");

				bitmap = new CBitmap (stoi(data[1]), stoi(data[2]));

				if (counter > 3)
					bitmap->SetBackground((char) data[3][0]);

				if (counter > 4 && Validation::IsBool(data[4]))
					bitmap->SetOverflowAlert((stoi(data[4]) == 1) ? true : false);

				bitmapDefined = true;
				canvas = bitmap;
			}

			else if (cls == "line") {
				if ( ! (Validation::IsDigit(data[1])
						&& Validation::IsDigit(data[2])
						&& Validation::IsDigit(data[3])
						&& Validation::IsDigit(data[4])
						&& data[5].length()
						)
					)
					throw InvalidFunctionArgument (cls, "int, int, int, int, char");

				CLine a (stoi(data[1]), stoi(data[2]), stoi(data[3]), stoi(data[4]), (char) data[5][0], canvas);
			}

			else if (cls == "polyline") {
				int dataLength = (int) data.size();
				
				if (dataLength % 2)
					throw ExceptionBase ("Polyline must not have an odd number of coordinates and must have pattern");

				int index1 = 0, index2 = 0;

				vector<array<int, 2> > lines;
				lines.resize((dataLength - 2) / 2);

				for (int i = 1; i < dataLength - 1; i++) {
					if ( ! Validation::IsDigit(data[i]))
						throw InvalidFunctionArgument (cls, "ints, char");

					lines[index1][index2] = stoi(data[i]);

					if (i % 2 == 0)
						index1++;

					index2 = (index2 == 0) ? 1 : 0;
				}

				CPolyline a (lines, (char) data[dataLength - 1][0], canvas);
			}

			else if (cls == "ellipse") {
				if ( ! (Validation::IsDigit(data[1])
						&& Validation::IsDigit(data[2])
						&& Validation::IsDigit(data[3])
						&& Validation::IsDigit(data[4])
						&& data[5].length()
						)
					)
					throw InvalidFunctionArgument (cls, "int, int, int, int, char");

				CEllipse a (stoi(data[1]), stoi(data[2]), stoi(data[3]), stoi(data[4]), (char) data[5][0], canvas);
			}

			else if (cls == "circle") {
				if ( ! (Validation::IsDigit(data[1])
						&& Validation::IsDigit(data[2])
						&& Validation::IsDigit(data[3])
						&& data[4].length()
						)
					)
					throw InvalidFunctionArgument (cls, "int, int, int, char");

				CCircle a (stoi(data[1]), stoi(data[2]), stoi(data[3]), (char) data[4][0], canvas);
			}

			else if (cls == "rectangle") {
				if ( ! (Validation::IsDigit(data[1])
						&& Validation::IsDigit(data[2])
						&& Validation::IsDigit(data[3])
						&& Validation::IsDigit(data[4])
						&& data[5].length()
						)
					)
					throw InvalidFunctionArgument (cls, "int, int, int, int, char");

				CRectangle a (stoi(data[1]), stoi(data[2]), stoi(data[3]), stoi(data[4]), (char) data[5][0], canvas);
			}

			else if (cls == "square") {
				if ( ! (Validation::IsDigit(data[1])
						&& Validation::IsDigit(data[2])
						&& Validation::IsDigit(data[3])
						&& data[4].length()
						)
					)
					throw InvalidFunctionArgument (cls, "int, int, int, char");

				CSquare a (stoi(data[1]), stoi(data[2]), stoi(data[3]), (char) data[4][0], canvas);
			}

			else if (cls == "point") {
				if ( ! (Validation::IsDigit(data[1]) && Validation::IsDigit(data[2]) && data[3].length()))
					throw InvalidFunctionArgument (cls, "int, int, char");

				CPoint a (stoi(data[1]), stoi(data[2]), (char) data[3][0], canvas);
			}

			else if (cls == "fill") {
				if ( ! (Validation::IsDigit(data[1])
						&& Validation::IsDigit(data[2])
						&& data[3].length())
						|| (counter == 5 && ! Validation::IsDigit(data[4]))
					)
					throw InvalidFunctionArgument (cls, "int, int, char[, int]");

				canvas->Fill(stoi(data[1]), stoi(data[2]), (char) data[3][0], (counter == 5) ? stoi(data[4]) : 4);
			}

			else if (cls == "pattern") {
				if ( ! data[1].length())
					throw ExceptionBase ("Pattern action not exists");

				if (data[1] == "begin") {
					if (patternInUse)
						throw ExceptionBase ("Pattern in pattern is forbidden");

					delete pattern;
					
					pattern = new CPattern();
					canvas = pattern;
					patternInUse = true;
					continue;
				}

				if (data[1] == "end") {
					if ( ! patternInUse)
						throw ExceptionBase ("Pattern is not defined");
					canvas = bitmap;
					patternInUse = false;
					continue;
				}

				if (patternInUse)
					throw ExceptionBase ("Pattern is not defined, you cannot print it");

				if ( ! (Validation::IsDigit(data[1]) && Validation::IsDigit(data[2])))
					throw InvalidFunctionArgument (cls, "int, int");

				pattern->Print(stoi(data[1]), stoi(data[2]), canvas);
			}

			else
				throw ExceptionBase (cls + " is not defined");
		}

		if ( ! bitmapDefined)
			throw ExceptionBase ("Input file is empty");
	}
	catch (ExceptionBase & a) {
		cout << a;
		return 0;
	}

	if (useCout)
		bitmap->Output(cout, spaces);
	else
		bitmap->Output(output, spaces);

	delete pattern;
	delete bitmap;

	return 0;
}
