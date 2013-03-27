#include "DataType.h"

//Super Class Implementation
DataValue::DataValue()
{
	mType = GAME_INVALID;
}

DataValue::~DataValue()
{

}

bool DataValue::parse(std::string text)
{
	//Super Class does nothing.
	return false;
}

std::string DataValue::toString()
{
	//Super Class does nothing.
	return "";
}


bool DataValue::getInt(int* pointer)
{
	return false;
}

bool DataValue::getFloat(float* pointer)
{
	return false;
}



bool DataValue::getString(std::string* pointer)
{
	return false;
}


bool DataValue::getVectorI(std::vector<int>* pointer)
{
	return false;
}

bool DataValue::getVectorF(std::vector<float>* pointer)
{
	return false;
}

//Specialised Functions

//Integer
Integer::Integer() : DataValue()
{
	mValue = 0;
	mType = GAME_INT;
}

Integer::~Integer()
{

}

bool Integer::parse(std::string text)
{
	mValue = atoi(text.c_str());
	return true;
}

std::string Integer::toString()
{
	return boost::lexical_cast<std::string , int>(mValue);
}

//Float
Float::Float() : DataValue()
{
	mValue = 0.0f;
	mType = GAME_FLOAT;
}

Float::~Float()
{

}

bool Float::parse(std::string text)
{
	mValue = atof(text.c_str());
	return true;
}

std::string Float::toString()
{
	return boost::lexical_cast<std::string, float>(mValue);
}

//String
String::String() : DataValue()
{
	mValue = "";
	mType = GAME_STRING;
}

String::~String()
{

}

bool String::parse(std::string text)
{
	size_t start = text.find_first_of("\"");
	size_t end = text.find_first_of("\"", start+1);

	if(start == std::string::npos || end == std::string::npos)
	{
		return false;
	}
	else
	{
		mValue = text.substr(start+1, end-start-1);
		return true;
	}
}

std::string String::toString()
{
	return "\"" + mValue + "\"";
}

//VectorI
VectorI::VectorI(int minSize, int maxSize) : DataValue()
{
	mMinSize = minSize;
	mMaxSize = maxSize;
	mType = GAME_VECTORI;
}

VectorI::VectorI(int minSize) : DataValue()
{
	mMinSize = minSize;
	mMaxSize = -1;
	mType = GAME_VECTORI;
}

VectorI::~VectorI()
{

}

bool VectorI::parse(std::string text)
{
	size_t start = text.find_first_of("{");
	size_t end = text.find_first_of("}", start+1);

	if(start == std::string::npos || end == std::string::npos)
	{
		return false;
	}
	else
	{
		//Remove Braces
		std::string theString = text.substr(start+1, end-start-1);

		std::string tok = "";

		int prevComma = 0;
		int nextComma = theString.find_first_of(",");

		bool tokValid = (prevComma != std::string::npos && nextComma != std::string::npos);
		if(tokValid) tok = theString.substr(prevComma, nextComma-prevComma);



		for(size_t i = 0; (i < mMaxSize || mMaxSize == -1) && tokValid; i++)
		{
			Integer temp;

			if(!temp.parse(tok))
			{
				return false;
			}

			mValue.push_back(temp);

			prevComma = nextComma;
			nextComma = theString.find_first_of(",", prevComma+1);

			if(nextComma != std::string::npos && prevComma != std::string::npos)
			{
				tok = theString.substr(prevComma+1, nextComma-prevComma-1);
				tokValid = true;
			}
			else if(prevComma != std::string::npos)
			{
				tok = theString.substr(prevComma+1);
				tokValid = true;
			}
			else
			{
				tokValid = false;
			}
		}

		if(mValue.size() < mMinSize || (mValue.size() > mMaxSize && mMaxSize > -1))
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}

std::string VectorI::toString()
{
	std::string output =  "{";
	for(size_t i = 0; i < mValue.size(); i++)
	{
		output += mValue[i].toString();
		if(mValue.size() - i > 1) output += ","; //Only add comma if not last element in vector.
	}
	output += "}";

	return output;

}

//VectorF
VectorF::VectorF(int minSize, int maxSize) : DataValue()
{
	mMinSize = minSize;
	mMaxSize = maxSize;
	mType = GAME_VECTORF;
}

VectorF::VectorF(int minSize) : DataValue()
{
	mMinSize = minSize;
	mMaxSize = -1;
	mType = GAME_VECTORF;
}

VectorF::~VectorF()
{

}

bool VectorF::parse(std::string text)
{
	size_t start = text.find_first_of("{");
	size_t end = text.find_first_of("}", start+1);

	if(start == std::string::npos || end == std::string::npos)
	{
		return false;
	}
	else
	{
		//Remove Braces
		std::string theString = text.substr(start+1, end-start-1);

		std::string tok = "";

		int prevComma = 0;
		int nextComma = theString.find_first_of(",");

		bool tokValid = (prevComma != std::string::npos && nextComma != std::string::npos);
		if(tokValid) tok = theString.substr(prevComma, nextComma-prevComma);



		for(size_t i = 0; (i < mMaxSize || mMaxSize == -1) && tokValid; i++)
		{
			Float temp;

			if(!temp.parse(tok))
			{
				return false;
			}

			mValue.push_back(temp);

			prevComma = nextComma;
			nextComma = theString.find_first_of(",", prevComma+1);

			if(nextComma != std::string::npos && prevComma != std::string::npos)
			{
				tok = theString.substr(prevComma+1, nextComma-prevComma-1);
				tokValid = true;
			}
			else if(prevComma != std::string::npos)
			{
				tok = theString.substr(prevComma+1);
				tokValid = true;
			}
			else
			{
				tokValid = false;
			}
		}

		if(mValue.size() < mMinSize || (mValue.size() > mMaxSize && mMaxSize > -1))
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}

std::string VectorF::toString()
{
	std::string output =  "{";
	for(size_t i = 0; i < mValue.size(); i++)
	{
		output += mValue[i].toString();
		if(mValue.size() - i > 1) output += ","; //Only add comma if not last element in vector.
	}
	output += "}";

	return output;

}

//Getting values:

bool Integer::getInt(int* pointer)
{
	*pointer = mValue;
	return true;
}

bool Float::getFloat(float* pointer)
{
	*pointer = mValue;
	return true;
}

bool String::getString(std::string* pointer)
{
	*pointer = mValue;
	return true;
}

bool VectorI::getVectorI(std::vector<int>* pointer)
{
	pointer->clear();
	for(size_t i = 0; i < mValue.size(); i++)
	{
		int temp;
		mValue[i].getInt(&temp);
		pointer->push_back(temp);
	}

	return true;
}

bool VectorF::getVectorF(std::vector<float>* pointer)
{
	pointer->clear();
	for(size_t i = 0; i < mValue.size(); i++)
	{
		float temp;
		mValue[i].getFloat(&temp);
		pointer->push_back(temp);
	}

	return true;
}
