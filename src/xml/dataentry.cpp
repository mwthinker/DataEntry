#include "dataentry.h"

#include <tinyxml2.h>

#include <map>
#include <string>
#include <sstream>
#include <iostream>

namespace xml {

	template <>
	std::string extract(tinyxml2::XMLHandle handle) {
		tinyxml2::XMLElement* element = handle.ToElement();
		if (element == nullptr) {
			throw std::runtime_error("Missing element!");
		}
		const char* str = element->GetText();

		if (str == nullptr) {
			throw std::runtime_error("Missing text!");
		}

		return std::string(str);
	}

	void DataEntry::save() {
		xml_->doc_.SaveFile(xml_->fileName_.c_str());
	}

	bool DataEntry::getBool() const {
		return get<bool>();
	}

	float DataEntry::getFloat() const {
		return get<float>();
	}

	int DataEntry::getInt() const {
		return get<int>();
	}	

	std::string DataEntry::getString() const {
		return get<std::string>();
	}

	// Return the tag's attribute bool value.
	// If it fails the default value is returned.
	bool DataEntry::getBoolAttribute(std::string attribute) const {
		tinyxml2::XMLElement* e = tag_.ToElement();
		if (e != nullptr) {
			return e->BoolAttribute(attribute.c_str());
		}
		return bool();
	}

	// Return the tag's attribute float value.
	// If it fails the default value is returned.
	float DataEntry::getFloatAttribute(std::string attribute) const {
		tinyxml2::XMLElement* e = tag_.ToElement();
		if (e != nullptr) {
			return e->FloatAttribute(attribute.c_str());
		}
		return float();
	}

	// Return the tag's attribute int value.
	// If it fails the default value is returned.
	int DataEntry::getIntAttribute(std::string attribute) const {
		tinyxml2::XMLElement* e = tag_.ToElement();
		if (e != nullptr) {
			return e->IntAttribute(attribute.c_str());
		}
		return int();
	}

	// Return the tag's attribute string value.
	// If it fails the default value is returned.
	std::string DataEntry::getStringAttribute(std::string attribute) const {
		tinyxml2::XMLElement* e = tag_.ToElement();
		if (e != nullptr) {
			return e->Attribute(attribute.c_str());
		}
		return std::string();
	}

	void DataEntry::setBool(bool value) {
		set<bool>(value);
	}

	void DataEntry::setFloat(float value) {
		set<float>(value);
	}

	void DataEntry::setInt(int value) {
		set<int>(value);
	}

	void DataEntry::setString(std::string value) {
		set<std::string>(value);
	}

	DataEntry DataEntry::getSibling(std::string siblingName) const {
		return DataEntry(xml_, tag_.NextSiblingElement(siblingName.c_str()));
	}

	DataEntry::DataEntry(std::string file) : xml_(std::make_shared<Xml>()), tag_(xml_->doc_) {
		xml_->doc_.LoadFile(file.c_str());
		xml_->fileName_ = file;
		tag_ = tinyxml2::XMLHandle(xml_->doc_);
	}

	DataEntry DataEntry::getChildEntry(std::string tagName) const {
		return DataEntry(xml_, tag_.FirstChildElement(tagName.c_str()));
	}

	DataEntry DataEntry::getEntry(std::string tagNames) const {
		std::stringstream stream(tagNames);
		std::string tag;
		tinyxml2::XMLHandle handleXml(tag_);
		while (stream >> tag) {
			handleXml = handleXml.FirstChildElement(tag.c_str());
		}
		return DataEntry(xml_, handleXml);
	}

	bool DataEntry::isAttributeEqual(std::string name, std::string value) const {
		auto element = tag_.ToElement();
		return element == nullptr ? false : element->Attribute(name.c_str(), value.c_str()) != nullptr;
	}	

	DataEntry::DataEntry(std::shared_ptr<Xml> xml, tinyxml2::XMLHandle tag) : xml_(xml), tag_(tag) {
	}

	bool DataEntry::hasData() const {
		return tag_.ToElement() != nullptr;
	}

	bool DataEntry::isError() const {
		return xml_->doc_.Error();
	}

	void DataEntry::printError() const {
		return xml_->doc_.PrintError();
	}

} // Namespace xml.
