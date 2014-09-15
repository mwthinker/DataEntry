#ifndef DATAENTRY_H
#define DATAENTRY_H

#include <tinyxml2.h>

#include <string>
#include <functional>
#include <memory>

namespace xml {

	class DataEntry {
	public:
		DataEntry(std::string file);
		
		// Save the current xml file, overwrite the current version.
		void save();

		// Return true if the current tag's attribute (name) value is equal to (value).
		// Else return false.
		bool isAttributeEqual(std::string name, std::string value) const;
		
		// Return the tag's current bool value.
		// If it fails the default value is returned.
		bool getBool() const;
		
		// Return the tag's current float value.
		// If it fails the default value is returned.
		float getFloat() const;

		// Return the tag's current int value.
		// If it fails the default value is returned.
		int getInt() const;

		// Return the tag's current string value.
		// If it fails the default value is returned.
		std::string getString() const;

		// Return the tag's attribute bool value.
		// If it fails the default value is returned.
		bool getBoolAttribute(std::string attribute) const;

		// Return the tag's attribute float value.
		// If it fails the default value is returned.
		float getFloatAttribute(std::string attribute) const;

		// Return the tag's attribute int value.
		// If it fails the default value is returned.
		int getIntAttribute(std::string attribute) const;

		// Return the tag's attribute string value.
		// If it fails the default value is returned.
		std::string getStringAttribute(std::string attribute) const;

		// Return the tag's current Value value.
		// If it fails the default value is returned.
		template <class Value>
		Value get() const {
			try {
				return xml::extract<Value>(tag_);
			} catch (std::runtime_error) {
				return Value();
			}
		}

		// Set the tag's current Value value.
		// If it fails false is returned else true.
		template <class Value>
		bool set(Value value) {
			try {
				xml::insert<Value>(value, tag_);
				return true;
			} catch (std::runtime_error) {
				return false;
			}
		}

		// Set the tag's current bool value.
		// If it fails false is returned else true.
		void setBool(bool value);

		// Set the tag's current float value.
		// If it fails false is returned else true.
		void setFloat(float value);

		// Set the tag's current int value.
		// If it fails false is returned else true.
		void setInt(int value);

		// Set the tag's current string value.
		// If it fails false is returned else true.
		void setString(std::string value);
		
		// Return the first child entry with name of tagName.
		DataEntry getChildEntry(std::string tagName) const;

		// Return the first child entry in the end of all tagNames.
		// E.g. getEntry("test window width")
		// give the same result as 
		// entry.getChildEntry("test").getChildEntry("window").getChildEntry("width")
		DataEntry getEntry(std::string tagNames) const;

		DataEntry getSibling(std::string siblingName) const;
		
		// Return true if the current tag has data.
		bool hasData() const;

		bool isError() const;

		// Print the current error message. Only prints when there is an error.
		void printError() const;

	private:
		struct Xml {
			tinyxml2::XMLDocument doc_;
			std::string fileName_;
		};

		DataEntry(std::shared_ptr<Xml> xml, tinyxml2::XMLHandle tag);

		std::shared_ptr<Xml> xml_;

		// External interface respect the constness.
		mutable tinyxml2::XMLHandle tag_; // In order to avoid usage of XMLConstHandle.
	};

	// Generic template function to extract value from xml tag.
	template <class Output>
	Output extract(tinyxml2::XMLHandle handle) {
		tinyxml2::XMLElement* element = handle.ToElement();
		if (element == nullptr) {
			throw std::runtime_error("Missing element!");
		}
		const char* str = element->GetText();

		if (str == nullptr) {
			throw std::runtime_error("Missing text!");
		}

		std::stringstream stream(str);
		Output output;
		stream >> output;
		if (stream.fail()) {
			throw std::runtime_error("Stream failed!");
		}
		return output;
	}

	// Specialization for string type. In order to insure that the whole text is 
	// returned as a string.
	template <>
	std::string extract(tinyxml2::XMLHandle handle);

	// Returns the value defined in the input string. 
	// E.g. input = "zombieGame interface font", returns the 
	// value defined in the tag inside <zombieGame><interface><font>value</font></interface></zombieGame>
	template <class Output>
	Output getValueFromTag(const tinyxml2::XMLDocument& xmlDoc, std::string input) {
		std::stringstream stream(input);
		std::string tag;
		tinyxml2::XMLConstHandle handleXml(xmlDoc);
		while (stream >> tag) {
			handleXml = handleXml.FirstChildElement(tag.c_str());
		}
		return extract<Output>(handleXml);
	}

	// Saves the value in the tag defined by handle.
	template <class Value>
	void insert(const Value& value, tinyxml2::XMLHandle handle) {
		tinyxml2::XMLElement* element = handle.ToElement();
		if (element == nullptr) {
			throw std::runtime_error("Missing element!");
		}

		std::stringstream stream;
		stream << value;
		if (stream.fail()) {
			throw std::runtime_error("Stream failed!");
		}

		element->SetText(stream.str().c_str());
	}

} // Namespace xml.

#endif // DATAENTRY_H
