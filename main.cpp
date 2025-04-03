#include <iostream>
//#include "include/jsonParser.h"
#include "jsonParserBetter/jsonParserBetter.h"

#define EXPECT_EQ(A,B) if(A == B) {std::cout << "SUCCESS\n";} else {std::cout << "FAILED\n";}


int main() {
    // this is neet dont you fucking lie
    Json root = Json::make_json_map();
    root.insert("hello",Json::make_json_string("hello the cute"));
    root.insert("detective",Json::make_json_int(69));
    root.insert("money",Json::make_json_double(1.125));
    root.insert("data",Json::make_json_map());
    root["data"].insert("ass",Json::make_json_string("big"));
    root["data"].insert("super",Json::make_json_map());
    root["data"]["super"].insert("severed",Json::make_json_string("cold harbor"));
    std::cout << root;
    return 0;
}
