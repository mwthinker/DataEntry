#include <xml/dataentry.h>

#include <iostream>
#include <string>
#include <cassert>

template <typename T>
bool equal(T a, T b) {
	return std::abs(a - b) < (T) 0.01;
}

template <>
bool equal(bool a, bool b) {
	return a == b;
}

template <>
bool equal(int a, int b) {
	return a == b;
}

template <>
bool equal(char a, char b) {
	return a == b;
}

int main(int argc, char** argv) {
	xml::DataEntry entry("test.xml");
	entry.printError();
	assert(!entry.isError());

	// Do this.
	{
		assert(equal(128.5f, entry.getChildEntry("test").getChildEntry("window").getChildEntry("width").getFloat()));
		assert(equal(129.5, entry.getChildEntry("test").getChildEntry("window").getChildEntry("height").getDouble()));
		assert(equal(true, entry.getChildEntry("test").getChildEntry("window").getChildEntry("maximized").getBool()));
		assert(equal(false, entry.getChildEntry("test").getChildEntry("window").getChildEntry("maximized2").getBool()));
		assert(equal(600, entry.getChildEntry("test").getChildEntry("window").getChildEntry("positionX").getInt()));
		assert(equal('N', entry.getChildEntry("test").getChildEntry("iter").getChildEntry("key").getChar()));
	}
	// Or do this.
	{
		assert(equal(128.5f, entry.getDeepChildEntry("test window width").getFloat()));
		assert(equal(129.5, entry.getDeepChildEntry("test window height").getDouble()));
		assert(equal(true, entry.getDeepChildEntry("test window maximized").getBool()));
		assert(equal(false, entry.getDeepChildEntry("test window maximized2").getBool()));
		assert(equal(600, entry.getDeepChildEntry("test window positionX").getInt()));
		assert(equal('N', entry.getDeepChildEntry("test iter key").getChar()));
	}

	int size = 0;
	xml::DataEntry itE = entry.getDeepChildEntry("test iter testing");
	while (itE.hasData()) {
		++size;
		itE = itE.getSibling("testing");
	}

	assert(entry.getChildEntry("test").getChildEntry("iter").getChildEntry("testing").isAttributeEqual("a", "1"));
	assert(size == 3);
	
	entry.save();

	std::cout << "tests successfully!\n";
	return 0;
}
