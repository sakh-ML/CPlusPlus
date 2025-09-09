#include <stdio.h>
#include <iostream>
#include <fstream> 
#include <vector>

bool find_pattern_in_line(std::string pattern, std::string line){


    if(pattern.length() > line.length()){
        return false;
    }

    if(pattern.length() == 1){
        for(int i = 0; i < line.length(); ++i){
            if(line[i] == pattern[0]){
                return true;
            }
        }
    }

    else{
        for(int i = 0; i <= line.length() - pattern.length(); ++i){
            if(line[i] == pattern[0]){
                
                bool check_status = true;
                for(int j = 1; j < pattern.length(); ++j){

                    if(line[i + j] != pattern[j]){
                        check_status = false;
                        break;
                    }
                }
                if(check_status == true){
                    return true;
                }
            }
        }
    }

    return false;
}

std::vector<std::string> pattern_matching(std::string pattern, std::string file_name){

    std::ifstream file(file_name);
    std::vector<std::string> result;

	if(!file){
		std::cerr << "Die Datei könnte nicht geöffnet werden !!, bitte Geben Sie eine Datei ein." << std::endl;
        return result;
	}

    std::string line;

    while(std::getline(file, line)){
        if(find_pattern_in_line(pattern, line)){
        std::cout << line << std::endl;
        result.push_back(line);
        }

    }   
    
    return result;
}


int main(int argc, char* argv[]){

	if(argc < 3){
		std::cerr << "Fehler, bitte geben Sie suchmuster und Dateiname ein" << std::endl;
        return -1;
	}

	std::string pattern = argv[1];
	std::string file_name = argv[2];

	pattern_matching(pattern, file_name);
	
	return 0;
}
