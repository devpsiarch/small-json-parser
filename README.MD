# Simple JSON Parsers

These are two versions of a JSON parser that i wrote mainly for a pet project of mine , the ```jsonParser``` is much worst then the "neeter" ```jsonParserBetter```.

# JsonParserBetter
This is a "better" version of what i wrote a while back , do not include them both they might conflict and am too lazy to provide guards for allowing that ... why would you ?

## How to use
This is a code example , the wrapper provides (in my opinion) a neet recursive formula:
```cpp
#include "jsonParserBetter/jsonParserBetter.h"

int main() {
    // building a JSON structure 
    Json root = Json::make_json_map();
    root.insert("hello",Json::make_json_string("hello the cute"));
    root.insert("detective",Json::make_json_int(69));
    root.insert("money",Json::make_json_double(1.125));
    root.insert("data",Json::make_json_map());
    root["data"].insert("ass",Json::make_json_string("big"));
    root["data"].insert("super",Json::make_json_map());
    root["data"]["super"].insert("severed",Json::make_json_string("cold harbor"));
    std::cout << root << '\n';
    // for extracting from text :
    // you can use the "ReadFile" to read JSON from a file given its path
    std::string json_text = "{\n \"one\": 1,\n\"stringtest\": \"looking\",\n \"two\": {\n\"three\": 3, \n \"four\": { \n\"five\": 5 \n } \n }, \n \"six\": {\n\"seven\": 7\n } \n}";
    Json root2 = Json::ParseJson(json_text);
    std::cout << root2 << '\n';
    return 0;
}
// output : 
/*
{
"data": {
"ass": "big",
"super": {
"severed": "cold harbor",
},
},
"detective": 69,
"hello": "hello the cute",
"money": 1.1250000000,
}
{
"one": 1,
"six": {
"seven": 7,
},
"stringtest": "looking",
"two": {
"four": {
"five": 5,
},
"three": 3,
},
}
*/
```
## To be implemented

- [x] the extraction functionality derived from the old ```jsonParser```.
- [ ] a cuter print (where depending on the depth it indents the sub JSON elements).
- [ ] a constructor that given a root of a ```Json``` object it returns a ```std::string``` to be send.

## Disclaimer
This is not intended for professional use. It’s a quick, minimal implementation written on and off to fit a specific use case, please raise a issue if you find any problems (leaks/bugs) and any suggestions . if you have any feedback, feel free to share!
