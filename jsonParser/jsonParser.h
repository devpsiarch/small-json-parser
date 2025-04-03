#pragma once
#include <string>
#include <map>
#include <fstream>
#include <cassert>
#include <iostream>
typedef std::string::iterator string_it;

// this allows to store data types in one contiguios space instead of a class or a struct 
// that means changin one of the values changes the others , this is perfect since 
// a JsonValue can store one of the specified values but not all of them at once.
// making it more memory efficient for our use case.
union JsonValue {
    int i;
    double d;
    std::string* t;
    std::map<std::string,JsonValue>* sub_json;
};

class JsonParser {
// we can make the JsonParser class store a vector of said Values ...
public:
    static std::string readFile(const std::string&);
    // this will read a snippet of the Json file that has a primative type (int or double)
    static JsonValue ParsePrimativeType(const std::string&,string_it,string_it);
    // this will parse any subsection of the json file , 
    // that means it will be called recursivly.
    static JsonValue ParseJsonHelper(const std::string&,string_it&);
    // gets the pair value and key (value is JsonVlaue in case it is nested) 
    static std::pair<std::string,JsonValue> getTupleValues(const std::string&,string_it&);
    // this is the main Json parser function
    static JsonValue ParseJson(std::string&);
};
