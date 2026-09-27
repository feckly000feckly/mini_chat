#pragma once
#include"../includes.h"
#include "../dominal_model/model.h"

class FILE_METHODS{

    public:

    virtual ~FILE_METHODS(){

    }

    virtual void close_file() = 0;

    virtual void open_file(std::string path) = 0;

    virtual bool file_exists(std::string path) = 0;

};

class File_Manager : public FILE_METHODS{

    std::ofstream of;

    public:

    void open_file(std::string path){
        of.open(path,std::ios::app);
    }

    void close_file(){
        of.close();
    }


    bool file_exists(std::string path){
        std::ifstream ifs(path);
       return ifs.is_open();
    }
};