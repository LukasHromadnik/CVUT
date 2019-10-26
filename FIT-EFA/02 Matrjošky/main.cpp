#include <iostream>
#include <string>
using namespace std;

class Winner
{
public:
	Winner(string name, long value);
	void update(string name, long value);
	string name();
	long value();

protected:
	string m_Name;
	long m_Value;
};

// ----------------------------------------------------------------------

Winner::Winner(string name, long value)
{
	m_Name = name;
	m_Value = value;
}

// ----------------------------------------------------------------------

void Winner::update(string name, long value = 0)
{
	m_Name = name;
	m_Value = value;
}

// ----------------------------------------------------------------------

string Winner::name()
{
	return m_Name;
}

// ----------------------------------------------------------------------

long Winner::value()
{
	return m_Value;
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

class Tree
{
public:
	Tree(string name, long value);
	Tree * left();
	void setLeft(Tree * left);
	Tree * right();
	void setRight(Tree * right);
	void inc(long value);
	Tree * insert(Tree * node, string name, long value);
	string name();
	long value();

protected:
	long m_Value;
	string m_Name;
	Tree * m_Left;
	Tree * m_Right;
};

// ----------------------------------------------------------------------

Tree::Tree(string name, long value) {
	m_Name = name;
	m_Value = value;
	m_Left = NULL;
	m_Right = NULL;
}

// ----------------------------------------------------------------------

Tree * Tree::left()
{
	return m_Left;
}

// ----------------------------------------------------------------------

void Tree::setLeft(Tree * left)
{
	m_Left = left;
}

// ----------------------------------------------------------------------

Tree * Tree::right()
{
	return m_Right;
}

// ----------------------------------------------------------------------

void Tree::setRight(Tree * right)
{
	m_Right = right;
}

// ----------------------------------------------------------------------

void Tree::inc(long value)
{
	m_Value += value;
}

// ----------------------------------------------------------------------

string Tree::name()
{
	return m_Name;
}

// ----------------------------------------------------------------------

long Tree::value()
{
	return m_Value;
}

// ----------------------------------------------------------------------

Tree * Tree::insert(Tree * node, string name, long value)
{
	if (node->name() > name) {
		if ( ! node->left()) {
			node->setLeft(new Tree(name, value));
			return node->left();
		} else {
			return node->left()->insert(node->left(), name, value);
		}
	} else if (node->name() < name) {
		if ( ! node->right()) {
			node->setRight(new Tree(name, value));
			return node->right();
		} else {
			return node->right()->insert(node->right(), name, value);
		}
	} else {
		node->inc(value);
		return node;
	}
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

class Stack
{
public:
	Stack(long value, Stack * left, Stack * right);
	long value();
	long free();
	Stack * left();
	Stack * right();
	void setRight(Stack * right);
	void spaceSub(long num);

protected:
	long m_Value;
	long m_Free;
	Stack * m_Left;
	Stack * m_Right;
};

// ----------------------------------------------------------------------

Stack::Stack(long value, Stack * left = NULL, Stack * right = NULL)
{
	m_Value = m_Free = value;
	m_Left = left;
	m_Right = right;
}

// ----------------------------------------------------------------------

void Stack::spaceSub(long num)
{
	m_Free -= num;
}

// ----------------------------------------------------------------------

long Stack::value()
{
	return m_Value;
}

// ----------------------------------------------------------------------

Stack * Stack::left()
{
	return m_Left;
}

// ----------------------------------------------------------------------

Stack * Stack::right()
{
	return m_Right;
}

// ----------------------------------------------------------------------

void Stack::setRight(Stack * right)
{
	m_Right = right;
}

// ----------------------------------------------------------------------

long Stack::free()
{
	return m_Free;
}

// ----------------------------------------------------------------------

/**
 * Returns positive number from given number
 *
 * @param  int n Given number
 * @return int   Positive number from the given number
 */

long fabs(long n)
{
	if (n < 0) {
		n *= -1;
	}

	return n;
}

// ----------------------------------------------------------------------

void updateWinner(Winner * winner, string name, long value)
{
	if (value > winner->value()) {
		winner->update(name, value);
	} else if (value == winner->value() && name < winner->name()) {
		winner->update(name, value);
	}
}

// ----------------------------------------------------------------------

int main()
{
	// Pocet polozek
	int count;

	// Vitez
	Winner * winner = NULL;

	// Matroska je slozitelna
	bool isPossible = true;

	// Zasobnik s matrjoškami
	Stack * stack = NULL;

	// Spojak s detma
	Tree * children = NULL;

	// Nacteme pocet polozek
	cin >> count;

	// Lichy pocet polozek automaticky vyhodnotime
	if (count % 2 != 0) {
		isPossible = false;
	}

	// Iterace pres pocet polozek
	for (int i = 0; i < count; i++) {
		// Aktualni cast
		long part;

		// Aktualni dite
		string kid;

		// Ulozime levou cast
		cin >> part;
		long ppart = fabs(part);

		// Nahrajeme jmeno ditete a odebereme prvni mezeru
		getline(cin, kid);
		kid = kid.substr(1, kid.length());

		if ( ! winner) {
			winner = new Winner(kid, ppart);
			children = new Tree(kid, ppart);
			// cout << children->name() << " " << children->value() << endl;
		} else {
			Tree * tmp = children->insert(children, kid, ppart);
			// cout << tmp->name() << " " << tmp->value() << endl;
			updateWinner(winner, kid, tmp->value());
		}

		if (isPossible) {
			// Prvni pruchod
			if ( ! stack) {
				stack = new Stack(ppart);
				continue;
			}

			// Leva cast
			if (part < 0) {
				part = ppart;
				
				// Pokud se nevejde, preskocime vyhodnocovani sestavovani 
				if (stack->free() < part) {
					isPossible = false;
					continue;
				}

				// Vytvorime novou levou cast
				Stack * tmp = new Stack(part, stack);

				// Nastavime novou cast jako vnorenou v soucasne
				stack->setRight(tmp);

				// Zmensime misto v aktualni casti
				stack->spaceSub(part);

				// Zmenime pointer na novou cast
				stack = tmp;
			}

			// Prava cast
			else {
				// Zaviraci cast nepasuje na oteviraci
				if (stack->value() != part) {
					isPossible = false;
					continue;
				} else {
					// Uvolnime oteviraci cast pro aktualni zaviraci
					if (stack->left()) {
						stack = stack->left();
						free(stack->right());
					} else {
						free(stack);
					}
				}
			}
		}
	}

	if (stack && stack->left()) {
		isPossible = false;
	}

	// Matroska je slozitelna
	cout << ((isPossible) ? "Ano" : "Ne") << endl;

	// Dite s nejvetsim prispevkem
	cout << winner->name() << endl;
}
