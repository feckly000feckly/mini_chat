#include "../service/chat_logic.h";
#include "../includes.h";

class http_message_json{
    public:

    static dtos_type serialization_json_to_dtos(std::string json_string){

    nlohmann::json json = nlohmann::json::parse(json_string);

    dtos_type dto_container;

    for (const auto& elem : json)
    {
        nlohmann::json obj = nlohmann::json::parse(elem.get<std::string>());

        int id_user = obj.at("id_user").get<int>();
        int id_message = obj.at("id_message").get<int>();
        std::string data_message = obj.at("data_message").get<std::string>();
        dto_container.push_back(std::make_shared<dto>(id_user,id_message,data_message));

    }

    return dto_container;
    }

       static dto_type serialization_json_to_dto(std::string json_string){

        nlohmann::json json = nlohmann::json::parse(json_string);

        int id_user = json.at("id_user").get<int>();
        int id_message = json.at("id_message").get<int>();
        std::string data_message = json.at("data_message").get<std::string>();
        dto_type dto_obj = std::make_shared<dto>(id_user,id_message,data_message);

    return dto_obj;
    }

    static std::string serialization_dtos_to_json(dtos_type dtos){
     nlohmann::json json_array = nlohmann::json::array();
     for(dto_type dto_one : dtos){
        nlohmann::json json;
        json["id_user"] = dto_one->id_user;
        json["id_message"] = dto_one->id_message;
        json["data_message"] = dto_one->data_message;
        json_array.push_back(json);
     }
        return json_array.dump();
    }

    static int parse_json_delete(std::string json_string){
     nlohmann::json json = nlohmann::json::parse(json_string);
     int id_message = json["id_message"];
    return id_message;
    }


};
