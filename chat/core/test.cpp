#include "dominal_model/model.h"
#include "repositories/api.h"
#include "Helper/File_Manager.h"
#include "service/chat_logic.h"
#include "controller/server_fasade.h"
#include "config.h"

int main(){
std::shared_ptr<DB_API> api = std::make_shared<SQLite_API>();
std::shared_ptr<FILE_METHODS> file_manager = std::make_shared<File_Manager>();
std::shared_ptr<MODEL_API> repositories = std::make_shared<Specific_API_V1>(api);

if(!file_manager->file_exists(NAME_DB)){
file_manager->open_file(NAME_DB); 
api->connect(NAME_DB);
repositories->table_create();
}else{
api->connect(NAME_DB);
}


std::shared_ptr<Ichat_logic> chat_logic_api = std::make_shared<chat_logic>(repositories);
std::shared_ptr<server_fasade> server_fasade_api = std::make_shared<server_fasade>(chat_logic_api,NAME_HOST,PORT_HOST);
server_fasade_api->run();
api->disconnect();
}


// все переменные передаваймые серверу должен генерировать клиент 

/*
примеры HTTP API запросов на сервер 

====================================================

отправка(POST):
curl -X POST 127.0.0.1:9090/send \
     -d '{
           "id_user": 52,
           "id_message": 101,
           "data_message": "Привет! Это тестовое сообщение"
         }'

====================================================

удаление(POST) :
curl -X POST 127.0.0.1:9090/delete \
     -d '{
           "delete_message": 101
         }' 

====================================================

получение истории(GET):
curl 127.0.0.1:9090/history   
*/