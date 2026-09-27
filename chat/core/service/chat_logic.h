#pragma once
#include "../includes.h"
#include "../repositories/api.h"
#include "dto.h"

using dto_type = std::shared_ptr<dto>;
using dtos_type = std::vector<dto_type>;

template<typename T>
entity_type entity_cast(const T& dto){
//This is also possible using reinterpret_cast if the types match. 
entity_type new_entity = std::make_shared<entity>();
new_entity->data_message = dto->data_message;
new_entity->id_message = dto->id_message;
new_entity->id_user = dto->id_user;
return new_entity;
}

template<typename T>
dto_type dto_cast(const T& entity){
//This is also possible using reinterpret_cast if the types match. 
dto_type new_dto = std::make_shared<dto>();
new_dto->data_message = entity->data_message;
new_dto->id_message = entity->id_message;
new_dto->id_user = entity->id_user;
return new_dto;
}

template<typename T>
std::string db_format_param(const T& obj){
std::stringstream ss;
ss << obj->id_user << ',' << obj->id_message << ',' << '\'' << obj->data_message << '\'' ;
return ss.str();
}

class Ichat_logic {

  public:

    virtual dtos_type get_all_messages() = 0;

    virtual void message_delete(int id_message) = 0;

    virtual void message_send(dto_type dto_obj) = 0;

};

class chat_logic : public Ichat_logic{

    std::shared_ptr<MODEL_API> model_api;

    public:

    chat_logic(std::shared_ptr<MODEL_API> model_api_ ) : model_api{model_api_}{
    }

    dtos_type get_all_messages(){
    entities_type entitys = model_api->data_read();

    dtos_type dto_container;

    for(auto& entity : entitys){
    dto_container.push_back(dto_cast(entity));
    }

    return dto_container;
    }

    void message_delete(int id_message) {
       std::string condition = "id_message == " + std::to_string(id_message);
       model_api->data_delete(condition);
    }

    void message_send(dto_type dto_obj) {
      auto dto_str = db_format_param(dto_obj);
      model_api->data_insert(dto_str);
    }


};