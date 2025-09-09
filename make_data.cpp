#include <iostream>
#include <string>
#include <fstream>

struct President{
    std::string name;
    std::string party;
    std::string state;
};


std::string read_file_return_string(std::string file_name){
    
    std::string result;
    std::string line;
    std::ifstream MyReadFile(file_name);

    while(std::getline(MyReadFile, line)){
        result += line + "\n";
    }

    MyReadFile.close();

    return result;
}


int main(){

    std::string data = read_file_return_string("president.txt");
    

    return 0;
}