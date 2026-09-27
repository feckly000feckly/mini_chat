#pragma once
#include"../includes.h"
#include "../dominal_model/model.h"
#include "../config.h"

class MODEL_API{

    public:
    //managment data

    virtual void data_delete(std::string conditions) = 0;

    virtual void data_update(std::string conditions) = 0;

    virtual void data_insert(std::string values) = 0;

    virtual entities_type data_read() = 0;


    //managment table

    virtual void table_create() = 0;

    virtual void table_bind(std::string conditions) = 0;
};

class Specific_API_V1 : public MODEL_API{
    enum class Action {CREATE_TABLE,INSERT,DELETE,SELECT};
    std::shared_ptr<DB_API> dominal_api;
        std::unordered_map<Action,std::string> string_action {
        {Action::CREATE_TABLE,"CREATE TABLE %s (%s);"},
        {Action::INSERT,"INSERT INTO %s  (%s) VALUES (%s);"},
        {Action::DELETE,"DELETE FROM %s WHERE %s;"},
        {Action::SELECT,"SELECT * FROM %s;"}};

        constexpr static int MAX_LENGTH_ACTION = 1000;

        template<typename ...Args>
        entities_type universal_call(Action action, Args... args){
        auto sample = string_action.at(action);
        std::string buffer;
        buffer.resize(MAX_LENGTH_ACTION);
        snprintf(buffer.data() ,buffer.size(),sample.c_str(),args.c_str()...);
        std::cout << buffer << std::endl;
        return dominal_api->raw_request(buffer);
        }

    public:

    Specific_API_V1(std::shared_ptr<DB_API> dominal_api_) : dominal_api{dominal_api_}{

    }

    void data_delete(std::string conditions) override{
      universal_call(Action::DELETE,NAME_TABLE,conditions);
    }

    void data_update(std::string conditions) override{
       //TODO
    }

    void data_insert(std::string values) override{
      universal_call(Action::INSERT,NAME_TABLE,NAME_VALUES,values);
    }

    entities_type data_read() override{
      return universal_call(Action::SELECT,NAME_TABLE);
    }


    void table_create() override{
       universal_call(Action::CREATE_TABLE,NAME_TABLE,TYPE_VALUES);
    }

    void table_bind(std::string conditions) override{
       //TODO
    }
};