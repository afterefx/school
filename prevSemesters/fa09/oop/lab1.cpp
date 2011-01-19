 class fraction
{
	int iNum
	int iDen
	public fraction(int iNum, int iDen)
	{
		throw ZeroException
	}
	
	//Math
	fraction operator=(const fraction & right)
	fraction operator+(const fraction & right)
	fraction operator-(const fraction & right)
	fraction operator*(const fraction & right)
	fraction operator/(const fraction & right)

	//Conditional
	bool operator==(const fraction & right)
	bool operator!=(const fraction & right)
	bool operator<(const fraction & right)
	bool operator>(const fraction & right)
	bool operator<=(const fraction & right)
	bool operator>=(const fraction & right)
	
	//Other operations
	fraction reciprocal(const fraction & fTemp)
	fraction reduce(const fraction & fSecond)
	fraction commonDenom(const fraction & fSecond)

	//Conversion
	fraction decimalToFractions(double dDec)
	double fractionToDecimal(fraction fFrac)
		throw ZeroException

	override string ToString()
}

Test Cases
CO,1/4,1/4  		//Copy
A,1/4,1/4,1/2 		//Add
A,1/2,1/4,3/4
A,6/4,1/4,3,3/4
S,1/4,1/4,0 		//Subtract
S,3/4,1/4,1/2
S,1/4,3/4,-1/2
M,1/2 			//Mulitply
D, 			//Divide
EQ,1/2,1/2,1 		//Equal
EQ,3/2,1/2,0
EQ,1/2,1/2,1
EQ,1/2,1/2,1
NEQ,1/2,1/4,1 		//Not Equal
NEQ,1/2,1/2,0
L,1/4,1/2,0 		//Less than
G,3/4,1/4,1 		//Greater than
G,1/4,3/4,0 		//Greater than
LE,1/4,3/4,1 		//Less than or equal
LE,3/4,3/4,1 			
LE,3/4,1/4,0 			
GE, 			//Greater than or equal
GE,3/4,3/4,0
GE,3/4,1/4,1
RP, 			//Reciprocal
RD, 			//Reduce
CD,  			//Common denominator
DF, 			//Decimal to fraction
FD, 			//Fraction to decimal


