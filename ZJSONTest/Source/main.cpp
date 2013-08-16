#include <iostream>
#include <cstdlib>

#include "Types.h"
#include "Data\JSONLoader.h"
/*
#include "rapidjson/document.h"
#include "rapidjson/filestream.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/reader.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/internal/pow10.h"
#include "rapidjson/internal/stack.h"
#include "rapidjson/internal/strfunc.h"
#include "jsoncpp/include/json/json.h"
#include "jsoncpp/include/json/reader.h" */

using namespace Anubis;
using namespace std;
using namespace rapidjson;

int main()
{
	//load JSON file
	ASTRING filename = "Resources\\droid_data.json";

	JSONLoader::Parse(filename);

	cout << "name: "		<< JSONLoader::GetString("name")		<< endl;
	cout << "strength: "	<< JSONLoader::GetAINT32("strength")	<< endl;
	cout << "speed: "		<< JSONLoader::GetAREAL32("speed")		<< endl;
	cout << "active: "		<< JSONLoader::GetBool("active")		<< endl;

	//try to parse array ^^
	JSONLoader::ParseArray("weapons");

	cout << "Number of weapons: "	<< JSONLoader::ArraySize()	<< endl;

	cout << "first weapon: "	<< JSONLoader::GetStringFromArray(0)	<< endl;
	cout << "second weapon: "	<< JSONLoader::GetStringFromArray(1)	<< endl;

	system("pause");
}