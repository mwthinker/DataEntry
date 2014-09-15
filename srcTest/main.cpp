#include <xml/dataentry.h>

#include <iostream>
#include <string>
#include <cassert>

int main(int argc, char** argv) {
	xml::DataEntry entry("test.xml");
	entry.printError();
	assert(entry.isValid());

	// Do this.
	{
		float width = entry.getChildEntry("test").getChildEntry("window").getChildEntry("width").getFloat();
		float height = entry.getChildEntry("test").getChildEntry("window").getChildEntry("height").getFloat();
		bool maximized = entry.getChildEntry("test").getChildEntry("window").getChildEntry("maximized").getBool();
		int positionX = entry.getChildEntry("test").getChildEntry("window").getChildEntry("positionX").getInt();
	}
	// Or do this.
	{
		float width = entry.getEntry("test window width").getFloat();
		float height = entry.getEntry("test window height").getFloat();
		bool maximized = entry.getEntry("test window maximized").getBool();
		int positionX = entry.getEntry("test window positionX").getInt();
	}

	int size = 0;
	entry.getChildEntry("test").getChildEntry("iter").iterateChilds("testing", [&](xml::DataEntry dEntry) {
		++size;
		return true;
	});

	assert(entry.getChildEntry("test").getChildEntry("iter").getChildEntry("testing").isAttributeEqual("a", "1"));

	assert(size == 3);

	return 0;
}
