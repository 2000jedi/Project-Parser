#pragma once

#include <string>
#include <vector>
#include "tree.hpp"

class catagory {
public:
    std::string name;
    std::vector<std::string> reps;

    catagory(std::string name){
        this->name = name;
        reps.clear();
    }
};

extern Node parser(std::string str, std::string input);