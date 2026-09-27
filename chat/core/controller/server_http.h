#include "../includes.h"

class IRoute{
    public:
    ~IRoute(){}

    virtual void route() = 0;

};

class server{
    std::vector<std::shared_ptr<IRoute>> routes;
    crow::App<crow::CORSHandler>& app;

    int port = 0;
    std::string host = "";



    public:

    server(crow::App<crow::CORSHandler>& app_,std::string host_,int port_) : app(app_),host(host_),port(port_){
        app.multithreaded();
        app.bindaddr(host);
        app.port(port);
    }

    void run(){
        app.run();
    }

    void add_route(std::shared_ptr<IRoute> route){
    routes.push_back(route);
    }

    void init_routes(){
        for(auto& route : routes){
            route->route();
        }
    }
};
