#ifndef YH_PLIST_PLIST_H_
#define YH_PLIST_PLIST_H_

#include "json/json.h"
#include "pugixml.hpp"


class Plist
{
public:
	
    Json::Value getDataFromFile(const std::string& file);

    Json::Value getDataFromContent(const std::string& content);
    
    Json::Value getDataFromDocument(const pugi::xml_document& doc);

protected:

    void parseArray(pugi::xml_node& node,Json::Value& arrayValue);
    
    void parseDict(pugi::xml_node& node,Json::Value& dictValue);
    
    void parseArrayElement(pugi::xml_node& element,Json::Value& arrayValue);
    
    void parseDictElement(pugi::xml_node& element,Json::Value& dictValue);
    
    void parseValue(pugi::xml_node& valueNode,Json::Value& outValue);

};




#endif // YH_PLIST_PLIST_H_
