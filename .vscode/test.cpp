#include <iostream>
#include <string>

int main() {
    std::string str = "aaa";
    for (int i = 0; i< str.length(); i++) {
        std::cout<<str[i]<<std::endl;
    }
    system("pause");
    return 0;
}