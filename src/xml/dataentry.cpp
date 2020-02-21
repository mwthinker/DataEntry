#include "dataentry.h"

namespace xml {

	template <>
	std::string extract(tinyxml2::XMLHandle handle) {
		auto element = handle.ToElement();
		if (element == nullptr) {
			throw std::runtime_error{"Missing element!"};
		}
		const char* str = element->GetText();

		if (str == nullptr) {
			throw std::runtime_error{"Missing text!"};
		}

		return str;
	}

	DataEntry DataEntry::addTag(const std::string& name) {
		auto node = xml_->doc.NewElement(name.c_str());
		auto insertIn = tag_.ToNode();
		auto newNode = insertIn->LinkEndChild(node);
		return {xml_, tinyxml2::XMLHandle{newNode}};
	}

	void DataEntry::removeFirstChild() {
		if (tag_.ToNode() != nullptr && tag_.FirstChild().ToNode() != nullptr) {
			tag_.ToNode()->DeleteChild(tag_.FirstChild().ToNode());
		}
	}

	void DataEntry::remove() {
		if (tag_.ToNode() != nullptr) {
			tag_.ToNode()->Parent()->DeleteChild(tag_.ToNode());
		}
	}

	void DataEntry::removeAllChildren() {
		if (tag_.ToNode() != nullptr) {
			tag_.ToNode()->DeleteChildren();
		}
	}

	void DataEntry::removeFirstChild(const std::string& tagName) {
		if (tag_.ToNode() != nullptr && tag_.ToNode()->FirstChildElement(tagName.c_str()) != nullptr) {
			tag_.ToNode()->DeleteChild(tag_.ToNode()->FirstChildElement(tagName.c_str()));
		}
	}

	void DataEntry::removeAllChildren(const std::string& tagName) {
		auto child = tag_.ToNode()->FirstChildElement(tagName.c_str());
		while (child != nullptr && tag_.ToNode()->FirstChildElement(tagName.c_str()) != nullptr ) {
			tag_.ToNode()->DeleteChild(child);
			child = tag_.ToNode()->FirstChildElement(tagName.c_str());
		}
	}

	void DataEntry::save() {
		xml_->doc.SaveFile(xml_->fileName.c_str());
	}

	bool DataEntry::getBool() const {
		return get<bool>();
	}

	float DataEntry::getFloat() const {
		return get<float>();
	}

	double DataEntry::getDouble() const {
		return get<double>();
	}

	int DataEntry::getInt() const {
		return get<int>();
	}

	char DataEntry::getChar() const {
		return get<char>();
	}

	std::string DataEntry::getString() const {
		return get<std::string>();
	}

	// Return the tag's attribute bool value.
	// If it fails the default value is returned.
	bool DataEntry::getBoolAttribute(const std::string& attribute) const {
		auto e = tag_.ToElement();
		if (e != nullptr) {
			return e->BoolAttribute(attribute.c_str());
		}
		return {};
	}

	// Return the tag's attribute float value.
	// If it fails the default value is returned.
	float DataEntry::getFloatAttribute(const std::string& attribute) const {
		auto e = tag_.ToElement();
		if (e != nullptr) {
			return e->FloatAttribute(attribute.c_str());
		}
		return {};
	}

	// Return the tag's attribute double value.
	// If it fails the default value is returned.
	double DataEntry::getDoubleAttribute(const std::string& attribute) const {
		auto e = tag_.ToElement();
		if (e != nullptr) {
			return e->DoubleAttribute(attribute.c_str());
		}
		return {};
	}

	// Return the tag's attribute int value.
	// If it fails the default value is returned.
	int DataEntry::getIntAttribute(const std::string& attribute) const {
		auto e = tag_.ToElement();
		if (e != nullptr) {
			return e->IntAttribute(attribute.c_str());
		}
		return {};
	}

	// Return the tag's attribute string value.
	// If it fails the default value is returned.
	std::string DataEntry::getStringAttribute(const std::string& attribute) const {
		auto e = tag_.ToElement();
		if (e != nullptr) {
			return e->Attribute(attribute.c_str());
		}
		return {};
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

	void DataEntry::setString(const std::string& value) {
		set<std::string>(value);
	}

	DataEntry DataEntry::getSibling(const std::string& siblingName) const {
		return {xml_, tag_.NextSiblingElement(siblingName.c_str())};
	}

	DataEntry DataEntry::getParent() const {
		auto node = tag_.ToNode();
		if (node == nullptr) {
			return {xml_, tag_};
		}
		return {xml_, tinyxml2::XMLHandle{tag_.ToNode()->Parent()}};
	}

	DataEntry::DataEntry(const std::string& file)
		: xml_(std::make_shared<Xml>())
		, tag_(xml_->doc) {
		
		xml_->doc.LoadFile(file.c_str());
		xml_->fileName = file;
		tag_ = tinyxml2::XMLHandle{xml_->doc};
	}

	DataEntry DataEntry::getChildEntry(const std::string& tagName) const {
		return {xml_, tag_.FirstChildElement(tagName.c_str())};
	}

	DataEntry DataEntry::getDeepChildEntry(const std::string& tagNames) const {
		std::stringstream stream{tagNames};
		std::string tag;
		tinyxml2::XMLHandle handleXml{tag_};
		while (stream >> tag) {
			handleXml = handleXml.FirstChildElement(tag.c_str());
		}
		return {xml_, handleXml};
	}

	bool DataEntry::isAttributeEqual(const std::string& name, const std::string& value) const {
		auto element = tag_.ToElement();
		return element == nullptr ? false : element->Attribute(name.c_str(), value.c_str()) != nullptr;
	}	

	DataEntry::DataEntry(const std::shared_ptr<Xml>& xml, tinyxml2::XMLHandle tag)
		: xml_(xml)
		, tag_(tag) {
	}

	bool DataEntry::hasData() const {
		return tag_.ToElement() != nullptr;
	}

	bool DataEntry::isError() const {
		return xml_->doc.Error();
	}

	void DataEntry::printError() const {
		return xml_->doc.PrintError();
	}

} // Namespace xml.
