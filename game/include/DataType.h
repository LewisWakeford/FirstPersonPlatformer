#ifndef DATATYPE_H
#define DATATYPE_H


#include <string>
#include <vector>
#include <regex>
#include <boost/lexical_cast.hpp>


enum GAME_DATATYPE
{
	GAME_INVALID,
	GAME_INT,
	GAME_FLOAT,
	GAME_STRING,
	GAME_VECTORI,
	GAME_VECTORF
};

/*
	Super Class: DataValue
	Descritpion: Represents a variable of a data type that can be read by the parser.
		Contains methods that allow it to be parsed correctly.

	Public Members
	--------------

		bool Func: parse
		Description: reads a chunk of text and attempts to recover a value from it.
		Returns: true if the value was recovered correctly and this value has been updated.
			false if the value was not recovered.

	Protected Members
	-----------------

		DataType Var: mType
		Description: the type of variable this DataValue represents.
*/

class DataValue
{
	public:
		DataValue();
		virtual ~DataValue();

		virtual bool parse(std::string text);
		virtual std::string toString();

		virtual bool getInt(int* pointer);
		virtual bool getFloat(float* pointer);
		virtual bool getString(std::string* pointer);
		virtual bool getVectorI(std::vector<int>* pointer);
		virtual bool getVectorF(std::vector<float>* pointer);

	protected:
		GAME_DATATYPE mType;

	private:	
};

/*
	Class: Integer
	Parses and stores the value of integers.
	text must contain only numbers with no other characters interupting the sequence.
*/
class Integer : public DataValue
{
	public:
		Integer();
		virtual ~Integer();

		virtual bool parse(std::string text);	
		virtual std::string toString();

		bool getInt(int* pointer);

	protected:
		int mValue;
};

/*
	Class: Float
	Parses and stores the value of floats.
	text must contain only numbers with a maximum of one decimal point and no other characters interupting the sequence.
*/
class Float : public DataValue
{
	public:
		Float();
		virtual ~Float();

		virtual bool parse(std::string text);
		virtual std::string toString();

		bool getFloat(float* pointer);

	protected:
		float mValue;
};

/*
	Class: String
	Parses and stores teh value of strings.
	text must start and end with double quotes, everything within is not regarded as a special symbol.
*/
class String : public DataValue
{
	public:
		String();
		virtual ~String();
		
		virtual bool parse(std::string text);
		virtual std::string toString();
		
		bool getString(std::string* pointer);

	protected:
		std::string mValue;
};

/*
	Class: VectorI
	Can parse and store vectors composed of integers.
	Min and Max vector size can be specified.
	text must start and end with braces, each element is seperated by commas.
	Number of elements cannot exceed max size or be below min size.
*/
class VectorI : public DataValue
{
	public:
		VectorI(int minSize);
		VectorI(int minSize, int maxSize);
		virtual ~VectorI();
		
		virtual bool parse(std::string text);
		virtual std::string toString();

		bool getVectorI(std::vector<int>* pointer);

	protected:

		std::vector<Integer> mValue;

		int mMinSize;
		int mMaxSize;
};

/*
	Class: VectorF
	Can parse and store vectors composed of floats.
	Min and Max vector size can be specified.
	text must start and end with braces, each element is seperated by commas.
	Number of elements cannot exceed max size or be below min size.
*/
class VectorF : public DataValue
{
	public:
		VectorF(int minSize);
		VectorF(int minSize, int maxSize);
		virtual ~VectorF();
		
		virtual bool parse(std::string text);
		virtual std::string toString();

		bool getVectorF(std::vector<float>* pointer);

	protected:

		std::vector<Float> mValue;

		unsigned int mMinSize;
		unsigned int mMaxSize;
};

#endif //DATATYPE_H
