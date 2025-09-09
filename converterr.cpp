#include <iostream>
#include <vector>
#include <cmath>


void print_vector(std::vector<int> v){
    for(int n : v){
        std::cout << n << " ";
    }
    std::cout << std::endl;
}


float n_exp_m(float n, float m){

    if(n == 0){
        return 0;
    }

    int negativ;
    if(m < 0){
        negativ = 1;
        m = -m;
    }

    if(m == 0){
        return 1;
    }

    float sum = 1;
    while(m > 0){
        sum *= n;
        --m;
    }
    if(negativ == 1){
        return 1 / sum;
    }
    return sum;
}


//Convert an number to an array, the array by sucess, otherwise -1 
std::vector<int> convert_number_to_array(int number){

    std::vector<int> result_vector;

    if(number < 0){
        result_vector.push_back(-1);
        return result_vector;
    }

    if(number == 0){
        result_vector.push_back(0);
        return result_vector;
    }

    while(number > 0){
        result_vector.insert(result_vector.begin(), number % 10);
        number /= 10;
    }
    return result_vector;

}


//Convert an array to an integer; 
int convert_array_to_number(std::vector<int> nums){

    int result = 0;
    for(int num : nums){
        result = result * 10 + num;
    }

    return result;

}


int convert_from_n_to_10(int number, int from_basis){

    int sum = 0;
    std::vector<int> nums = convert_number_to_array(number);
    for(int i = 0; i < nums.size(); ++i){

        sum += nums[i] * n_exp_m(from_basis, nums.size() - i - 1);
    }

    return sum;
}


int convert_from_10_to_n(int number, int to_basis){

    std::vector<int> result_vector;
    int sum = 0;
    while(number > 0){
        int rest = number % to_basis;
        result_vector.insert(result_vector.begin(), rest);
        number = number / to_basis;
    }

    int result = convert_array_to_number(result_vector);
    return result;
    
}


int convert_from_n_to_n(int number, int from_basis, int to_basis){
    
    int tmp = convert_from_n_to_10(number, from_basis);
    return convert_from_10_to_n(tmp, to_basis);
}


int main(){

    std::cout << convert_from_n_to_n(325, 10, 2) << std::endl;

    return 0;
}