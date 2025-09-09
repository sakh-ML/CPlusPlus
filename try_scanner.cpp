#include <iostream>
#include <unordered_set>
#include <vector>
#include <cctype>

enum class TokenType{
	KEYWORD,
	IDENTIFIER,
	OPERATOR,
	NUMBER,
	PUNCTUATION,    
	WHITESPACE,      
	COMMENT,
	END_OF_FILE,
	UNKNOWN          
};

std::ostream& operator<<(std::ostream& os, const TokenType& type) {
    switch (type) {
        case TokenType::KEYWORD:       os << "KEYWORD"; break;
        case TokenType::IDENTIFIER:    os << "IDENTIFIER"; break;
        case TokenType::OPERATOR:      os << "OPERATOR"; break;
        case TokenType::NUMBER:        os << "NUMBER"; break;
        case TokenType::PUNCTUATION:   os << "PUNCTUATION"; break; 
        case TokenType::WHITESPACE:    os << "WHITESPACE"; break;  
        case TokenType::COMMENT:       os << "COMMENT"; break;
        case TokenType::END_OF_FILE:   os << "EOF"; break;
        case TokenType::UNKNOWN:       os << "UNKNOWN"; break;     
        default:                       os << "???"; break;
    }
    return os;
}

struct Token{
	TokenType type;
	std::string value_of_token;
	//int line;
	//int column;
};

// helper functions TODO !! there is a lot to add

std::unordered_set<char> whitespaces = {'\n', '\t', '\r', ' '};

bool is_whitespace(char c){
	if(whitespaces.count(c) > 0){
		return true;
	}
	return false;
}

std::unordered_set<std::string> operators = {"+", "-", "*", "/", "%","<", ">", "<=", ">=", "!=", "=="};

bool is_operator(std::string str){
	if(operators.count(str) > 0){
		return true;
	}
	return false;
}

bool is_comment(std::string str){
	if(str == "//" || str == "/*"){
		return true;
	}
	return false;
}

bool is_punctuation(std::string){   
	return false;
}

std::unordered_set<std::string> keywords = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "inline", "int", "long", "register", "restrict", "return", "short",
    "signed", "sizeof", "static", "struct", "switch", "typedef", "union",
    "unsigned", "void", "volatile", "while", "_Alignas", "_Alignof",
    "_Atomic", "_Bool", "_Complex", "_Generic", "_Imaginary", "_Noreturn",
    "_Static_assert", "_Thread_local"
};

bool is_keyword(std::string str){
	return keywords.count(str) > 0;
}

Token make_token(TokenType type, std::string value_of_token){
	Token new_token;
	new_token.type = type;
	new_token.value_of_token = value_of_token;
	return new_token;
}

std::string scan_if_keyword(std::string source_code, int current_position){   
	std::string result = "";
	int i = current_position;
	while(i < source_code.size() && isalpha(source_code[i])){
		result += source_code[i];
		i++;
	}

	if(is_keyword(result)){
		return result;
	}
	else{
		return "";
	}
}

// i have to make the class scanner then the last function

std::string scan_if_number(std::string source_code, int current_position){  
	std::string result  = "";
	int i = current_position;
	while(i < source_code.size() && isdigit(source_code[i])){
		result = result + source_code[i];
		i++;
	}	
	return result;
}

std::string scan_if_identifier(std::string source_code, int current_position){  
	int i = current_position;
	std::string result = "";

	while(i < source_code.size() && (isalnum(source_code[i]) || source_code[i] == '_')){
		result += source_code[i];
		++i;
	}

	return result;
}

std::string scan_if_whitespace(std::string source_code, int current_position){   
	return std::string(1, source_code[current_position]);
}

std::string scan_if_comment(std::string source_code, int current_position){  
	std::string result = "";	
	int i = current_position;
	if(i < source_code.size() && source_code[current_position + 1] == '/'){
		result = "//"; 
		i += 2;
		while(source_code[i] !='\n'){
			result = result + source_code[i];
			i += 1;
		}
		return result;
	}

	if(i < source_code.size() && source_code[current_position + 1] == '*'){
		result = "/*";
		i += 2;
		while(!(source_code[i] == '*' && source_code[i + 1] == '/' )){
			result = result + source_code[i];
			i += 1;
		}
		return result;
	}  
	return result;	
}

std::string scan_if_punctuation(std::string source_code, int current_position){   
	return "";
}

class Scanner{
	public: 
	int current_position = 0;   
	
	std::vector<Token> string_to_tokens(std::string source_code){
		std::vector<Token> tokens;
		while(current_position < source_code.size()){
			if(is_whitespace(source_code[current_position])){
				current_position++;
				continue;
			}
			else if(isalpha(source_code[current_position])){ // alphabet
				if(scan_if_keyword(source_code, current_position) != ""){ // alphabet but keyword
					std::string value = scan_if_keyword(source_code, current_position);
					Token new_token = make_token(TokenType::KEYWORD,value);
					tokens.push_back(new_token);
					current_position = current_position + value.length();
					continue;
				}
				else if(scan_if_identifier(source_code, current_position) != ""){ // alphabet but identifier
					std::string value = scan_if_identifier(source_code, current_position);
					Token new_token = make_token(TokenType::IDENTIFIER,value);
					tokens.push_back(new_token);
					current_position = current_position + value.length();
					continue;	
		        }
		 	}
			else if(isdigit(source_code[current_position])){ // digit (0-9)
				if(scan_if_number(source_code, current_position) != ""){
					std::string value = scan_if_number(source_code, current_position);
					Token new_token = make_token(TokenType::NUMBER,value);
					tokens.push_back(new_token);				
					current_position = current_position + value.length();
					continue;
				}
			}
            else if(current_position + 1 < source_code.size()){ // check 2 char for comment and operator!
		       std::string two_char_op = source_code.substr(current_position,2);
       		   if(is_comment(two_char_op)){
			       std::string value = scan_if_comment(source_code, current_position);	
			       Token new_token = make_token(TokenType::COMMENT, value);
                   tokens.push_back(new_token);
                   current_position += value.length();
        		   continue;
  		        }

		       if(is_operator(two_char_op)){
			       Token new_token = make_token(TokenType::OPERATOR, two_char_op);
			       tokens.push_back(new_token);
			       current_position += 2;
			       continue;
		       }
			}	
            else if(is_operator(std::string(1, source_code[current_position]))){
				Token new_token = make_token(TokenType::OPERATOR,std::string(1, source_code[current_position]));
				tokens.push_back(new_token);
				current_position++;
				continue;
			}

			else if(is_punctuation(std::string(1,source_code[current_position]))){
				if(scan_if_punctuation(source_code, current_position) != ""){
					std::string value = scan_if_punctuation(source_code, current_position);
					Token new_token = make_token(TokenType::PUNCTUATION, value);
					current_position += value.length();
					tokens.push_back(new_token);
				}
				continue;
			}

			else{
				current_position++;
			}
		}

		Token new_token = make_token(TokenType::END_OF_FILE, "0");
		tokens.push_back(new_token);

		return tokens;
	}
};

void print_vector_tokens(std::vector<Token> tokens){
	for(auto token : tokens){
		std::cout << "< " << token.type << " , " << token.value_of_token << " >";
		std::cout << " , ";
	}
	return;
}

int main(){
	std::string source_code = "15 <= 18 12312 //hello !!! i am a comment \n";
	std::string source_code2 = "int float 124123412  mango //hello\n";

	Scanner my_scanner = Scanner();
	std::vector<Token> tokens = my_scanner.string_to_tokens(source_code2);
	print_vector_tokens(tokens);
	return 0;
}
