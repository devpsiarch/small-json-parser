#include "../include/jsonParser.h"

std::string JsonParser::readFile(const std::string&filepath){
    std::ifstream file(filepath);
    std::string buffer;
    std::string result;
    while (std::getline(file,buffer)) {
        result += buffer; 
    }
    return result;
}

JsonValue JsonParser::ParsePrimativeType(const std::string&text,string_it start,string_it end){
    std::string snippet = text.substr(start-text.begin(),end-start);
    size_t float_dot_pos = snippet.find(".");
    JsonValue result;
    if(float_dot_pos >= end-start) {
        result.i = std::stoi(snippet);
    }else{
        result.d = std::stod(snippet);
    }
    return result;
}

JsonValue JsonParser::ParseJsonHelper(const std::string&text,string_it&it){
    // begin of the nested json element
    assert(*it == '{');
    it++;
    
    std::map<std::string,JsonValue>* created_map = new std::map<std::string,JsonValue>;
    do {
        // get the key and the value from said line ("name":"josh")
        const auto [key,value] = getTupleValues(text,it);
        // asign the value 
        (*created_map)[key] = value;
        // skip spaces and endlines.
        while(*it == ' ' || *it == '\n'){
            it++;
        }
    } while(*it != '}');
    // we need to be over the '}' char
    it++;
    JsonValue result;
    result.sub_json = created_map;
    return result;
}
std::pair<std::string,JsonValue> JsonParser::getTupleValues(const std::string&text,string_it&it){
    assert(it != text.end());
    // skip spaces and endlines
    while(*it == ' ' || *it == '\n'){
        it++;
    }   
    // prepare some vars to extract the key and value
    string_it current_it;
    std::string key;
    JsonValue value;
    // the first double quotes for the key , now we get the key (std::string)
    if(*it == '\"'){
        current_it = ++it;
        while(*it != '\"'){
            it++;
        }
        // now , current_it points to the first char after the "\""
        key = text.substr(current_it-text.begin(),it-current_it);
        //std::cout << "key gotten : " << key << '\n'; 
        // there much be a ':' after the end double quote
        assert(*(++it) == ':');
        // now it is after the ':'
        it++;
    }
    // now we look for the key value 
    // skip again the empty and endlines
    while(*it == ' ' || *it == '\n'){
        it++;
    }
    // now we have some cases in case we have : a (string,nested,decimal,double) values.
    
    // case: nested json
    if(*it == '{'){
        value = ParseJsonHelper(text,it);
    }
    // case: value is string , we need to extract it
    else if(*it == '\"'){
        current_it = ++it;
        while(*it != '\"'){
            it++;
        }
        //std::cout << "string value gotten:" << text.substr(current_it-text.begin(),it-current_it) << '\n';
        // store the string 
        value.t = new std::string(text.substr(current_it-text.begin(),it-current_it));
        while(*it != ','){
            it++;
        }
    }
    // case: primative type (int,double)
    else {
        current_it = it;
        while(isdigit(*it) || *it == '.'){
            it++;
        }
        value = ParsePrimativeType(text,current_it,it);
        //std::cout << "primative gotten(i):" << value.i << '\n';
        //std::cout << "primative gotten(d):" << value.d << '\n';
    } 
    // check if we accualy got there 
    if(*it == ','){
        it++;
    }
    return std::make_pair(key,value);
}


JsonValue JsonParser::ParseJson(std::string&text){
    string_it main_it = text.begin();
    return ParseJsonHelper(text,main_it);
}
