#ifndef PERSISTANT_H
#define PERSISTANT_H

#include <string>
#include <vector>

#include "DataType.h"
#include "PersistantNode.h"

class SceneNode;

/*
	Super Class: Persistant
	A Persistant is an object that can be loaded from and saved to .map files.
	They contain an array of data value objects that can be filled out, as well as a name.
	A persistant object is saved as a tag, with the object name being the tag name and with the properties of the persistant
		object saved inside as tags themselves.
*/

class Persistant
{
	public:
		Persistant(App* app);
		Persistant(App* app, std::string name);
		virtual ~Persistant();

		bool parse(std::string text);
		std::string toString();

		virtual bool build();
		virtual PersistantNode* createNode();

	protected:

	App* mApp;

        virtual void updateProperties();

		void addProperty(std::string name, DataValue* value, bool optional);


		/*
		Internal Class: Property
		A Property object is basically a persistant variable that can be saved to a .map file.
		A Property consists of a name and a data value object.
		Properties are saved as tags, with the tag name being the property name and the tag value being the value of the data value object.

		Notable Methods---
			parse: Takes an entire tag set as input, checks the tag name matches this property's name and that the tag value is parsable by this property's data type.
				Returns true if both of these are successful, otherwise false.
			toString: Creates a string that represents this property, can be inserted into a .map file.
		*/

		class Property
		{
			public:
				Property(std::string name, DataValue* value, bool optional);
				virtual ~Property();

				bool parse(std::string text);
				std::string toString();

				std::string getName();

				DataValue* getValue();

			protected:
				std::string mName;
				DataValue* mValue;
				bool mOptional;

			private:
		};

		Property* getPropertyByName(std::string name);

		std::string mName;
		std::vector<Property> mProperties;

		PersistantNode* mSceneNode;

};

#endif //PERSISTANT_H
