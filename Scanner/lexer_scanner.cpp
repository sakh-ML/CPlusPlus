#include <iostream>
#include <unordered_set>
#include <vector>
#include <cctype>

enum class TokenType{
	KEYWORD,
	IDENTIFIER,
	OPERATOR,
	NUMBER,
	PUNCTUTAION,
	WHITESAPCE,
	COMMENT,
	END_OF_FILE,
	UNKOWN
};

std::ostream& operator<<(std::ostream& os, const TokenType& type) {
    switch (type) {
        case TokenType::KEYWORD:       os << "KEYWORD"; break;
        case TokenType::IDENTIFIER:    os << "IDENTIFIER"; break;
        case TokenType::OPERATOR:      os << "OPERATOR"; break;
        case TokenType::NUMBER:        os << "NUMBER"; break;
        case TokenType::PUNCTUTAION:   os << "PUNCTUTAION"; break;
        case TokenType::WHITESAPCE:    os << "WHITESPACE"; break;
        case TokenType::COMMENT:       os << "COMMENT"; break;
        case TokenType::END_OF_FILE:   os << "EOF"; break;
        case TokenType::UNKOWN:        os << "UNKNOWN"; break;
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


//helper functions TODO !! there is a lot to add

std::unordered_set<char> whitespaces = {'\n', '\t', '\r', ' '};

bool is_whitespace(char c){
	
	if(whitespaces.count(c) > 0){
		return true;
	}

	return false;
}


std::unordered_set<std::string> operators = {"+", "-", "*", "/", "%","<", ">", "<=", ">=", "!=", "==", "="};

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

bool is_punctation(std::string){
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


std::unordered_set<char> punctuations = {
    '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-',
    '.', '/', ':', ';', '<', '=', '>', '?', '@', '[', '\\', ']', '^',
    '_', '`', '{', '|', '}', '~'
};


bool is_punctation(char c){
	return punctuations.count(c) > 0;
}


Token make_token(TokenType type, std::string value_of_token){

	Token new_token;
	new_token.type = type;
	new_token.value_of_token = value_of_token;
	
	return new_token;
}

std::string scann_if_keyword(std::string source_code, int current_posotion){
	
	std::string result = "";
	int i = current_posotion;
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

// i have to make the class scanner than the last function

std::string scann_if_number(std::string source_code, int current_posotion){
	
	std::string result  = "";
	int i = current_posotion;
	while(i < source_code.size() && isdigit(source_code[i])){
		result = result + source_code[i];
		i++;
	}	

	return result;
}

std::string scann_if_identifier(std::string source_code, int current_posotion){

	int i = current_posotion;
	std::string result = "";

	while(i < source_code.size() && (isalnum(source_code[i]) || source_code[i] == '_')){
		result += source_code[i];
		++i;
	}

	return result;
}


std::string scann_if_whitespace(std::string source_code, int current_posotion){
	
	return std::string(1, source_code[current_posotion]);
}

std::string scann_if_comment(std::string source_code, int current_posotion){

	std::string result = "";	

	int i = current_posotion;
	if(i < source_code.size() && source_code[current_posotion + 1] == '/'){
		
		result = "//"; 
		i += 2;
		while(source_code[i] !='\n'){
			result = result + source_code[i];
			i += 1;
		}
		return result;
	}

	if(i < source_code.size() && source_code[current_posotion + 1] == '*'){
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

std::string scann_if_punctation(std::string source_code, int current_posotion){
	
	if(is_punctation(source_code[current_posotion])){
		return std::string(1, source_code[current_posotion]);
	}
	else{
		return "";
	}

}


class Scanner{

	public: 

	int current_posotion = 0;
	
	std::vector<Token> string_to_tokens(std::string source_code){
		std::vector<Token> tokens;
		while(current_posotion < source_code.size()){

			if(is_whitespace(source_code[current_posotion])){
				current_posotion++;
				continue;
			}

			else if(isalpha(source_code[current_posotion])){ // alphabet
				if(scann_if_keyword(source_code, current_posotion) != ""){ // alphabet but keword
					std::string value = scann_if_keyword(source_code, current_posotion);
					Token new_token = make_token(TokenType::KEYWORD,value);
					tokens.push_back(new_token);
					current_posotion = current_posotion + value.length();
					continue;
				}
				else if(scann_if_identifier(source_code, current_posotion) != ""){ // alphapet but identifer
					std::string value = scann_if_identifier(source_code, current_posotion);
                                        Token new_token = make_token(TokenType::IDENTIFIER,value);
                                 	tokens.push_back(new_token);
				        current_posotion = current_posotion + value.length();
					continue;	
		         	}
		 	}
				
			else if(isdigit(source_code[current_posotion])){ // digit (0-9)
				if(scann_if_number(source_code, current_posotion) != ""){
					std::string value = scann_if_number(source_code, current_posotion);
					Token new_token = make_token(TokenType::NUMBER,value);
					tokens.push_back(new_token);				
					current_posotion = current_posotion + value.length();
					continue;
				}
			}

			//check 2 char for comment or operator
                        else if(current_posotion + 1 < source_code.size() && (is_comment(source_code.substr(current_posotion,2)) || is_operator(source_code.substr(current_posotion, 2)))){ 

				std::string two_char = source_code.substr(current_posotion, 2);
       			       if(is_comment(two_char)){
				       std::string value = scann_if_comment(source_code, current_posotion);	
				       Token new_token = make_token(TokenType::COMMENT, value);
               			       tokens.push_back(new_token);
             			       current_posotion += value.length();
        			       continue;
  			        }

			  	else if(is_operator(two_char)){
				       Token new_token = make_token(TokenType::OPERATOR, two_char);
				       tokens.push_back(new_token);
				       current_posotion += 2;
				       continue;
			       }
			}

			else if(is_operator(std::string(1, source_code[current_posotion]))){
				Token new_token = make_token(TokenType::OPERATOR,std::string(1, source_code[current_posotion]));
				tokens.push_back(new_token);
				current_posotion++;
				continue;
			}

			else if(is_punctation(source_code[current_posotion])){
				if(scann_if_punctation(source_code, current_posotion) != ""){
					std::string value = scann_if_punctation(source_code, current_posotion);
					Token new_token = make_token(TokenType::PUNCTUTAION, value);
					current_posotion += value.length();
					tokens.push_back(new_token);
				}
				continue;
			}

			else{
				current_posotion++;
			}
 
		}

		Token new_token = make_token(TokenType::END_OF_FILE, "0");
		tokens.push_back(new_token);

		return tokens;
	}
};


void print_vector_tokens(std::vector<Token> tokens){
	for(auto token : tokens){
		std::cout << "< " << token.type << " , '" << token.value_of_token << "' >";
		std::cout << " , ";
	}
	return;
}


int main(){

	std::string source_code = "15 <= 18 12312 //hello !!! i am a comment \n";
	std::string source_code2 = "int float 124123412  mango //hello\n jaklina hahahah //i love u dog\n <= 235 > 0";
	std::string source_code3 = "( asda asd ) {";


	Scanner my_scanner = Scanner();
	std::vector<Token> tokens = my_scanner.string_to_tokens(source_code3);
	print_vector_tokens(tokens);
	return 0;
}
