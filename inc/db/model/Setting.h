#pragma once
#include <string>

namespace db::model
{
class Setting
{
public:
    Setting(const std::string& key, const std::string& value, const std::string& updatedAt)
        : _key(key), _value(value), _updatedAt(updatedAt)
    {
    }

    /********** Getters **********/
    const std::string& getKey() const { return _key; }
    const std::string& getValue() const { return _value; }
    const std::string& getUpdatedAt() const { return _updatedAt; }

    /********** Setters **********/
    void setKey(const std::string& key) { _key = key; } /** TODO: probably shouldn't be changed */
    void setValue(const std::string& value) { _value = value; }
    void setUpdatedAt(const std::string& time) { _updatedAt = time; }

private:
    std::string _key;
    std::string _value;
    std::string _updatedAt;
};

} // namespace db::model
