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
