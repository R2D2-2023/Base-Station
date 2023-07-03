//
// Created by jasie on 30-6-2023.
//

#ifndef LEDMATRIX_R2D2_DROPBOX_H
#define LEDMATRIX_R2D2_DROPBOX_H
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <curl.h>
#include <vector>
#include "json.hpp"

class Dropbox {
public:
    Dropbox();

    void run(const std::string& local_file_path);
    std::string getAccessToken();
private:
    std::string access_token;
};


#endif //LEDMATRIX_R2D2_DROPBOX_H
