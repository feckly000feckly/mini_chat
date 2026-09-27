#pragma once
#include"../includes.h"
#include "../config.h"
#include"entity.h"

using entity_type = std::shared_ptr<entity>;
using entities_type = std::vector<entity_type>;

class DB_API{
    public:

    ~DB_API(){}

    virtual void connect(std::string path) = 0;

    virtual void disconnect() = 0;

    virtual std::vector<std::shared_ptr<entity>> raw_request(std::string request) = 0;

    virtual void commit_copy(std::string path) = 0;

};

class SQLite_API : public DB_API{

enum class const_param {ZERO_PARAM,ONE_PARAM,TWO_PARAM};

   constexpr static int callback(void *option_param, int argc, char **argv, char **azColName) {

        entities_type& buffer = *reinterpret_cast<entities_type*>(option_param);

        auto entity_obj = std::make_shared<entity>();

            entity_obj->id_user = std::atoi(argv[static_cast<int>(const_param::ZERO_PARAM)]);
            entity_obj->id_message = std::atoi(argv[static_cast<int>(const_param::ONE_PARAM)]);
            if(argv[static_cast<int>(const_param::TWO_PARAM)] != nullptr)
            entity_obj->data_message = argv[static_cast<int>(const_param::TWO_PARAM)];

        buffer.push_back(entity_obj);

        return 0;
    }

sqlite3* current_db = nullptr;
    
    void init_db(std::string path,sqlite3** db){
    int err_code = 0;
        err_code = sqlite3_open(path.c_str(), db);
        if(err_code){
            std::cerr << "err_code : "<< err_code << std::endl; 
            throw std::runtime_error("Unable to open the file");
        }
    }    

    public:

    entities_type raw_request(std::string request) override {
        entities_type buffer; 
        int err_code = 0;
        char* err_message = nullptr;
        err_code = sqlite3_exec(current_db,request.c_str(),callback,&buffer,&err_message);

        if(err_code != SQLITE_OK){
            std::cerr << "err_code : "<< err_code << std::endl; 
            throw std::runtime_error("Raw request fault");
        } 
        return buffer;
    }

void commit_copy(std::string path) override { 
    sqlite3* copy_db = nullptr; 
    init_db(path, &copy_db); 

    if (!copy_db) return; 

    sqlite3_backup *pBackup; 

    pBackup = sqlite3_backup_init(copy_db, NAME_TABLE.c_str(), current_db, NAME_TABLE.c_str()); 
    
    if (pBackup) { 
        sqlite3_backup_step(pBackup, -1); 
        sqlite3_backup_finish(pBackup); 
    } else {
        std::string rc_error = sqlite3_errmsg(copy_db); 
    } 
    
    sqlite3_close(copy_db); 
}


    void connect(std::string path) override {
      init_db(path,&current_db);
    }

    void disconnect() override {
        sqlite3_close(current_db);
    }

};