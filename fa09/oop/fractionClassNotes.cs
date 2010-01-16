class Fraction
{
	//Constructor
	Fraction(int iNum, int iDen);
		throws DenominatorZeroException

	//Struct
	struct Frac
	{
		int iNum;
		int iDen;
	};

	struct mixed
	{
		int iWhole;
		int iNum;
		int iDen;
	};

	//Methods
	double FractionToDecimal();
	static double FractionToDecimal(fraction);
	static Frac Reduce(int, int);
		throw DenominatorZeroException;
	Fraction Reciprocal();
		throw DenominatorZeroException;
	overrride string ToString();	
	string ToStringMixed();	
	void SetFraction(int, int);
	Frac GetFraction();
	mixed GetFractionMixed();

	//Overloaded Operators
	//--Relational Operators
	operator==()
	operator!=()
	operator<()
	operator<=()
	operator>()
	operator>=()

	//--Binary Operators
	operator+()
	operator-()
	operator*()
	operator/()

	//Unary Operators
	operator~()


============================
roman class work
============================

using System;
using System.Collections,Generic;
using System.Linq;
using System.Text;

namespace ConsoleApplication
{
	class RomanNumerals
	{
		short iRoman; 		//The instance value
		RomanNumerals(int iRomanIn)
		{
			if(iRomanIn <= 0 || iRomanIn > 3999)
				throw ArgumentOutOfRangeException(iRomanIn.ToString() + " is not a valid amount");
			else
				iRoman = iRomanIn;	
		}
	}
}


