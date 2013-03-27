#include "Persistant.h"

Persistant::Persistant(App* app)
{
    mApp = app;
	mName = "INVALID NAME";
}

Persistant::Persistant(App* app, std::string name)
{
    mApp = app;
	mName = name;
}

Persistant::~Persistant()
{

}

void Persistant::addProperty(std::string name, DataValue* value, bool optional)
{
	mProperties.push_back(Property(name, value, optional));
}

bool Persistant::parse(std::string text)
{

	bool inProperty = false;
	Property* currentProperty = 0;

	std::string currentChunk = "";

	for(size_t pos = 0; pos < text.length(); pos++)
	{
		char currentChar = text.at(pos);

		if(inProperty)
		{
				if(currentChar == '<') //Found new tag
				{
					//Find rest of tag.
					std::string tagName;
					std::string currentTag;

					currentTag += currentChar;

					pos++;
					currentChar = text.at(pos);
					currentTag += currentChar;

					while(currentChar != '>')
					{
						tagName += currentChar;
						pos++;
						currentChar = text.at(pos);
						currentTag += currentChar;
					}

					bool closing = (tagName.substr(0, 1) == "/");

					if(closing) tagName.erase(0, 1);

					//If this is the end of this property, try to parse the current chunk.
					if(closing && tagName == currentProperty->getName())
					{
						//Parse the chunk then reset.
						if(!currentProperty->parse(currentChunk))
							return false;
						currentChunk = "";
						inProperty = false;
					}
					else
					{
						currentChunk += currentTag; //If this is not the closing tag it should be passed to the persistant.
					}
				}
				else
				{
					currentChunk += currentChar; //Add char to current chunk.
				}
		}
		else
		{
			if(currentChar == '<')
			{
				//Find rest of tag.
				std::string tagName;
				std::string currentTag;

				currentTag += currentChar;

				pos++;
				currentChar = text.at(pos);
				currentTag += currentChar;

				while(currentChar != '>')
				{
					tagName += currentChar;
					pos++;
					currentChar = text.at(pos);
					currentTag += currentChar;
				}

				bool closing = (tagName.substr(0, 1) == "/");

				if(closing)
				{
					//error
					return false;
				}

				Property* prop = getPropertyByName(tagName);

				if(prop)
				{
					currentProperty = prop;
					inProperty = true;
				}
				else
				{
					//error
					return false;
				}
			}
		}
	}

	return true;
}

std::string Persistant::toString()
{
    //Update if necessary
    updateProperties();

	std::string output = "<"+mName+">\n";
	for(size_t i = 0; i < mProperties.size(); i++)
	{
		output += mProperties[i].toString() + "\n";
	}
	output += "<\\"+mName+">";

	return output;
}

//Property

Persistant::Property::Property(std::string name, DataValue* value, bool optional)
{
	mName = name;

	mValue = value;

	mOptional = optional;
}

Persistant::Property::~Property()
{

}

bool Persistant::Property::parse(std::string text)
{
	return mValue->parse(text);
}

std::string Persistant::Property::toString()
{
	return "<"+mName+">\n" + mValue->toString() + "\n<\\"+mName+">";
}

std::string Persistant::Property::getName()
{
	return mName;
}

DataValue* Persistant::Property::getValue()
{
	return mValue;
}

Persistant::Property* Persistant::getPropertyByName(std::string name)
{
	for(size_t i = 0; i < mProperties.size(); i++)
	{
		if(mProperties[i].getName() == name) return &mProperties[i];
	}
	return NULL;
}

bool Persistant::build()
{
	//Super class does nothing.
	//Sub classes will convert their property list into variables as required.
	return false;
}

PersistantNode* Persistant::createNode()
{
    //Super class does nothing.
    return 0;
}

void Persistant::updateProperties()
{
    //Super class does nothing.
}
