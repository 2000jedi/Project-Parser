#include "parser.hpp"

int main(int argc, char** argv){
    initialize_table("parse_table");
    parser("SELECT * FROM table;");
}
