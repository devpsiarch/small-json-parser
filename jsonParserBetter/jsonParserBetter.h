#pragma once
#include <string>
#include <map>
#include <fstream>
#include <cassert>
#include <iostream>
#include <iomanip>
typedef std::string::iterator string_it;
#define JSONMAP std::map<std::string,Json> 

#define CHECK_MAP(object) if(!object->is_map() || !object->sub_json)         \
                          throw std::runtime_error("object not a map\n");    \

#define PRINT_QUOTED(str,printer) printer << "\"" << str << "\""

#define ASSERT_TYPE(thing,what) if(thing->type != what)                        \
                                throw std::runtime_error("wrong type!\n");     \

enum class JsonType { INT, DOUBLE, STRING, MAP };

struct Json {
    JsonType type;
    union {
        int i;
        double d;
        std::string* t;
        JSONMAP* sub_json;
    };
    /*Here i will define some getters to get values from Json element*/
    // these get copies and wont give up or nor allow borrowing of memory
    int getInt(void) const; 
    double getDouble(void) const;
    std::string getString(void) const; 
    // i wont provide a mathode to get a copy of the map becasue of the 
    // way the Json is designed ... use it explicitly as stated in the example
    // for pretty print
    friend std::ostream& operator<<(std::ostream&,const Json&);
    // default constructore , becomes a integer here
    Json();
    // this frees the memory depending on the type stores
    void clear() noexcept;
    // in this struct , every object that goes out of scope will be 
    // deleted (that includes all its recursive objects if they exist)
    ~Json();
    // this is a move constructore , instead of copying from another
    // object , we move what was an objects resources to another
    // this means we are making no copies of the object
    Json(Json&&);
    // that means we have to decribe the operator '='
    // it will delete self then send the values to the other
    Json& operator=(Json&&) noexcept;
    // ill define a bunch of these , they are like small 
    // constructors
    // NOTE: maybe other versions should be added for convinience
    static Json make_json_int(int);
    static Json make_json_double(double);
    static Json make_json_string(const char*);
    static Json make_json_map();
    // since map is the type that we cant add data too 
    // then well provide some neet stuff to do that
    bool is_map(){return this->type == JsonType::MAP;}
    // we overload the index operator only for the map type 
    // to be able to add stuff
    // the operator will allow us to view stuff in the MAP
    Json& operator[](const std::string&);
    // now we want something that inserts element to the map
    // i passed char* instead of string to be able to use temp values
    void insert(const char*,Json&&); 

    /* This part of for the extraction from a std::string type*/
    // a lot will be taken from the old implimentation
    static std::string ReadFile(const std::string&);
    // this will read a snippet of the Json file that has a primative 
    // type (int or double)
    static Json ParsePrimativeType(const std::string&,string_it,string_it);
    // this will parse any subsection of the json file , 
    // that means it will be called recursivly.
    static Json ParseJsonHelper(const std::string&,string_it&);
    // gets the pair value and key (value is JsonVlaue in case it is nested) 
    static std::pair<std::string&,Json&> getTupleValues(const std::string&,string_it&);
    // this is the main Json parser function
    static Json ParseJson(std::string&);

    /* Below well impliment the "std::string" generator from JSON (later...)*/
};
std::ostream& operator<<(std::ostream&,const Json&);
