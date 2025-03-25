#include <iostream>
#include "include/jsonParser.h"

#define EXPECT_EQ(A,B) if(A == B) {std::cout << "SUCCESS\n";} else {std::cout << "FAILED\n";}

int main(void){
    std::string json_text = "{\n \"one\": 1,\n\"stringtest\": \"looking\",\n \"two\": {\n\"three\": 3, \n \"four\": { \n\"five\": 5 \n } \n }, \n \"six\": {\n\"seven\": 7\n } \n}";
    
    std::cout << json_text << '\n';
    JsonValue parsed = JsonParser::ParseJson(json_text);

    
    JsonValue two = (*parsed.sub_json)["two"];
    JsonValue four = (*two.sub_json)["four"];
    JsonValue six = (*parsed.sub_json)["six"];
    JsonValue test = (*parsed.sub_json)["stringtest"];

    EXPECT_EQ((*parsed.sub_json)["one"].i, 1);
    EXPECT_EQ((*two.sub_json)["three"].i, 3);
    EXPECT_EQ((*four.sub_json)["five"].i, 5);
    EXPECT_EQ((*six.sub_json)["seven"].i, 7);
    EXPECT_EQ(*((*parsed.sub_json)["stringtest"]).t,"looking");
    std::cout << *((*parsed.sub_json)["stringtest"]).t;
    return 0;
}
