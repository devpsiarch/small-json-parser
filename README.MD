# Simple JSON Parser

These are two versions of a JSON parser that i wrote mainly for a pet project of mine , the ```jsonParser``` is much worst then the "neeter" ```jsonParserBetter```.

# JsonParserBetter
This is a "better" version of what i wrote a while back , do not include them both they might conflict and am too lazy to provide guards for allowing that ... why would you ?

## How to use
This is a code example , the wrapper provides (in my opinion) a neet recursive formula:
```cpp
#include "jsonParserBetter/jsonParserBetter.h"
int main() {
    // this is neet dont you fucking lie
    Json root = Json::make_json_map();
    root.insert("hello",Json::make_json_string("hello the cute"));
    root.insert("detective",Json::make_json_int(69));
    root.insert("money",Json::make_json_double(1.125));
    root.insert("data",Json::make_json_map());
    root["data"].insert("hope",Json::make_json_string("more then enough"));
    root["data"].insert("super",Json::make_json_map());
    root["data"]["super"].insert("severed",Json::make_json_string("cold harbor"));
    std::cout << root;
    // outoput : 
    /*
    {
		"data": {
		"hope": "more then enough",
		"super": {
		"severed": "cold harbor",
		},
		},
		"detective": 69,
		"hello": "hello the cute",
		"money": 1.1250000000,
		}
    */
    return 0;
}
```
## To be implemented

- [ ] a cuter print (where depending on the depth it indents the sub JSON elements).
- [ ] the extraction functionality derived from the old ```jsonParser```.

## Disclaimer
This is not intended for professional use. It’s a quick, minimal implementation written on and off to fit a specific use case. However, if you have any feedback, feel free to share!
