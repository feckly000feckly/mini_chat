#include "../includes.h"
#include "../service/chat_logic.h";
#include "http_message_json.h";
#include "server_http.h";

template<typename T>
class sendRoute : public IRoute{

std::function<T> action;
crow::App<crow::CORSHandler>& app;
    public:
    sendRoute(std::function<T> action_ , crow::App<crow::CORSHandler>& app_) : action{action_} , app(app_){

    }

    void route() override{
    CROW_ROUTE(app,"/send").methods("POST"_method)([&](const crow::request req){
       return std::invoke(action,req);
    });
    }

};

template<typename T>
class getMessengeRoute : public IRoute{

std::function<T> action;
crow::App<crow::CORSHandler>& app;
    public:

    getMessengeRoute(std::function<T> action_,crow::App<crow::CORSHandler>& app_) : action{action_},app{app_}{

    }

    void route() override{
    CROW_ROUTE(app,"/history").methods(crow::HTTPMethod::Get, crow::HTTPMethod::Options)([&](const crow::request req){
       return std::invoke(action,req);
    });
    }
};

template<typename T>
class deleteRoute : public IRoute{

std::function<T> action;
crow::App<crow::CORSHandler>& app;
    public:
    deleteRoute(std::function<T> action_ , crow::App<crow::CORSHandler>& app_) : action{action_} , app(app_){

    }

    void route() override{
    CROW_ROUTE(app,"/delete").methods("POST"_method)([&](const crow::request req){
       return std::invoke(action,req);
    });
    }

};

class server_fasade{
crow::App<crow::CORSHandler> app;
server server_http;

std::shared_ptr<Ichat_logic> chat_logic;
    public:
    server_fasade(std::shared_ptr<Ichat_logic> chat_logic_,std::string host,int port) :chat_logic{chat_logic_}, server_http{app,host,port}{
       server_http.add_route(std::make_shared<getMessengeRoute<crow::response(const crow::request req)>>([&](const crow::request req){
        auto history_dto = chat_logic->get_all_messages();
        auto history_string = http_message_json::serialization_dtos_to_json(history_dto);
        return crow::response(history_string);
       },app));
       
        server_http.add_route(std::make_shared<sendRoute<crow::response(const crow::request req)>>([&](const crow::request req){
        dto_type dto_obj = http_message_json::serialization_json_to_dto(req.body);
        chat_logic->message_send(dto_obj);
        return crow::response(200);
       },app));

       server_http.add_route(std::make_shared<deleteRoute<crow::response(const crow::request req)>>([&](const crow::request req){
        int id_message = http_message_json::parse_json_delete(req.body);
        chat_logic->message_delete(id_message);
        return crow::response(200);
       },app));

       server_http.init_routes();
    }

    void run(){
        server_http.run();
    }

};