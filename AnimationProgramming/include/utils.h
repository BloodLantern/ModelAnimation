#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "rapidjson/document.h"

#define VAR_AND_NAME(var) var, #var

namespace utils
{
    template<typename T>
    void Read(std::ifstream& file, T& var)
    {
        file.read(reinterpret_cast<char*>(&var), sizeof(var));
    }

    inline void Read(std::ifstream& file, std::string& str)
    {
        file.read(str.data(), static_cast<std::streamsize>(str.size()));
    }

    inline void Read(std::ifstream& file, std::string& str, const std::streamsize length)
    {
        str.resize(length);
        Read(file, str);
    }

    template<typename T>
    void SetFromJson(T& var, const char* const varName, const rapidjson::Value& json)
    {
        var = json[varName].Get<T>();
    }

    inline void SetFromJson(std::string& var, const char* const varName, const rapidjson::Value& json)
    {
        var = json[varName].GetString();
    }

    inline void SetFromJson(rapidjson::Document& var, const char* const varName, const rapidjson::Value& json)
    {
        var.CopyFrom(json[varName], var.GetAllocator());
    }

    template<typename T>
    void SetFromJsonSafe(T& var, const char* const varName, const rapidjson::Value& json)
    {
        if (json.HasMember(varName))
            SetFromJson(var, varName, json);
    }
    
    inline void SetFromJsonSafe(std::string& var, const char* const varName, const rapidjson::Value& json)
    {
        if (json.HasMember(varName))
            SetFromJson(var, varName, json);
    }
    
    inline void SetFromJsonSafe(rapidjson::Document& var, const char* const varName, const rapidjson::Value& json)
    {
        if (json.HasMember(varName))
            SetFromJson(var, varName, json);
    }
    
    template<typename T>
    void SetFromJsonClass(T& var, const char* const name, const rapidjson::Value& json)
    {
        var = T(json[name]);
    }
    
    template<typename T>
    void SetFromJsonClassSafe(T& var, const char* const name, const rapidjson::Value& json)
    {
        if (json.HasMember(name))
            SetFromJsonClass(var, name, json);
    }
    
    template<typename T>
    void SetFromJsonArray(std::vector<T>& vector, const char* const name, const rapidjson::Value& arrayParent)
    {
        auto&& array = arrayParent[name].GetArray();
        vector.resize(array.Size());
        for (unsigned int i = 0; i < array.Size(); i++)
            vector[i] = array[i].Get<T>();
    }

    inline void SetFromJsonArray(std::vector<rapidjson::Document>& vector, const char* const name, const rapidjson::Value& arrayParent)
    {
        auto&& array = arrayParent[name].GetArray();
        vector.resize(array.Size());
        for (unsigned int i = 0; i < array.Size(); i++)
        {
            rapidjson::Document& doc = vector[i];
            doc.CopyFrom(array[i], doc.GetAllocator());
        }
    }
    
    template<typename T>
    void SetFromJsonArraySafe(std::vector<T>& vector, const char* const name, const rapidjson::Value& arrayParent)
    {
        if (arrayParent.HasMember(name))
            SetFromJsonArray(vector, name, arrayParent);
    }

    inline void SetFromJsonArraySafe(std::vector<rapidjson::Document>& vector, const char* const name, const rapidjson::Value& arrayParent)
    {
        if (arrayParent.HasMember(name))
            SetFromJsonArray(vector, name, arrayParent);
    }
    
    template<typename T>
    void SetFromJsonArrayClass(std::vector<T>& vector, const char* const name, const rapidjson::Value& arrayParent)
    {
        auto&& array = arrayParent[name].GetArray();
        vector.resize(array.Size());
        for (unsigned int i = 0; i < array.Size(); i++)
            vector[i] = T(array[i]);
    }
    
    template<typename T>
    void SetFromJsonArrayClassSafe(std::vector<T>& vector, const char* const name, const rapidjson::Value& arrayParent)
    {
        if (arrayParent.HasMember(name))
            SetFromJsonArrayClass(vector, name, arrayParent);
    }
}
