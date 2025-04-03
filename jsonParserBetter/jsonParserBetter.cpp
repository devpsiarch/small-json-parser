#include "jsonParserBetter.h"
std::ostream& operator<<(std::ostream&out,const Json&other){
    if(other.type == JsonType::INT){
        out << other.i;
    }
    if(other.type == JsonType::DOUBLE){
        out << std::fixed << std::setprecision(10) << other.d;
    }
    if(other.type == JsonType::STRING){
        out << (*other.t);
    }
    if(other.type == JsonType::MAP){
        out << "{\n";
        bool first = true;
        for (const auto& [key, value] : *other.sub_json) {
            PRINT_QUOTED(key,out);
            out << ": ";
            if(value.type == JsonType::STRING){
                PRINT_QUOTED(value,out);
            }
            else{
                out << value;
            }
            out << ",\n";
            //out << "\"" << key << "\": " << value;
        }
        out << "}";
        //out << (*other.sub_json);
    }
    return out;
}
Json::Json(): type(JsonType::INT),i(0){}
void Json::clear() noexcept{
    if(this->type == JsonType::STRING){
        delete t;
        t = nullptr;
    }else if(this->type == JsonType::MAP){
        delete sub_json;
        sub_json = nullptr;
    }
}
Json::~Json(){
    clear();
}
Json& Json::operator=(Json&&other) noexcept{
    // if they are not in the same address (aka they are the same)
    if(this != &other){
        // delete this 
        this->clear();
        this->type = other.type;
        if(this->type == JsonType::INT){
            this->type = JsonType::INT;
            this->i = other.i;
        }
        if(this->type == JsonType::DOUBLE){
            this->type = JsonType::DOUBLE;
            this->d = other.d;
        }
        // in the case for STRING and MAP 
        // we give up the address (memory) and null the other
        if(this->type == JsonType::STRING){
            this->type = JsonType::STRING;
            this->t = other.t;
            other.t = nullptr;
        }
        if(this->type == JsonType::MAP){
            this->type = JsonType::MAP;
            this->sub_json = other.sub_json;
            other.sub_json = nullptr;
        }
    }
    return *this;
}
Json::Json(Json&&other){
    *this = std::move(other);
}
Json Json::make_json_int(int val){
    Json jv;
    jv.i = val;
    return jv;
}
Json Json::make_json_double(double val){
    Json jv;
    jv.type = JsonType::DOUBLE;
    jv.d = val;
    return jv;
}
Json Json::make_json_string(const char*val){
    Json jv;
    jv.type = JsonType::STRING;
    jv.t = new std::string(val);
    return jv;
}
Json Json::make_json_map(){
    Json jv;
    jv.type = JsonType::MAP;
    jv.sub_json = new JSONMAP();
    return jv;
}
Json& Json::operator[](const std::string&key){
    CHECK_MAP(this);    
    return (*sub_json)[key];
}
void Json::insert(const char*key,Json&&other){
    CHECK_MAP(this);    
    sub_json->emplace(key, std::move(other)); 
}
std::string Json::ReadFile(const std::string&filepath){
    std::ifstream file(filepath);
    std::string buffer;
    std::string result;
    while (std::getline(file,buffer)) {
        result += buffer; 
    }
    return result;
}
Json Json::ParsePrimativeType(const std::string&text,string_it start,string_it end){
    std::string snippet = text.substr(start-text.begin(),end-start);
    size_t float_dot_pos = snippet.find(".");
    if(float_dot_pos >= end-start) {
        return Json::make_json_int(std::stoi(snippet));
    }else{
        return Json::make_json_double(std::stod(snippet));
    }
}
Json Json::ParseJsonHelper(const std::string&text,string_it&it){
    // begin of the nested json element
    assert(*it == '{');
    it++;
    
    Json created_json = Json::make_json_map();
    do {
        // get the key and the value from said line ("name":"josh")
        const auto [key,value] = getTupleValues(text,it);
        // asign the value 
        created_json.insert(key.c_str(),std::move(value));
        // skip spaces and endlines.
        while(*it == ' ' || *it == '\n'){
            it++;
        }
    } while(*it != '}');
    // we need to be over the '}' char
    it++;
    return created_json;
}
std::pair<std::string&,Json&> Json::getTupleValues(const std::string&text,string_it&it){
    assert(it != text.end());
    // skip spaces and endlines
    while(*it == ' ' || *it == '\n'){
        it++;
    }   
    // prepare some vars to extract the key and value
    string_it current_it;
    std::string *key = new std::string();
    Json *value = new Json();
    // the first double quotes for the key , now we get the key (std::string)
    if(*it == '\"'){
        current_it = ++it;
        while(*it != '\"'){
            it++;
        }
        // now , current_it points to the first char after the "\""
        *key = text.substr(current_it-text.begin(),it-current_it);
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
    // now we have some cases in case we have : a 
    // (string,nested,decimal,double) values.
    
    // case: nested json
    if(*it == '{'){
        *value = ParseJsonHelper(text,it);
    }
    // case: value is string , we need to extract it
    else if(*it == '\"'){
        current_it = ++it;
        while(*it != '\"'){
            it++;
        }
        //std::cout << "string value gotten:" << 
        //text.substr(current_it-text.begin(),it-current_it) << '\n';
        // store the string 
        std::string temp = 
            std::string(text.substr(current_it-text.begin(),it-current_it));
        *value = Json::make_json_string(temp.c_str());
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
        *value = ParsePrimativeType(text,current_it,it);
        //std::cout << "primative gotten(i):" << value.i << '\n';
        //std::cout << "primative gotten(d):" << value.d << '\n';
    } 
    // check if we accualy got there 
    if(*it == ','){
        it++;
    }
    return std::make_pair(std::ref(*key),std::ref(*value));
}
Json Json::ParseJson(std::string&text){
    string_it main_it = text.begin();
    return ParseJsonHelper(text,main_it);
}
