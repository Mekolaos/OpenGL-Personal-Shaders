#include <iostream>
#include "helper.h"


int main(){
    std::cout << "Bda el program akhi" << std::endl;
    std::string path = "shader.vert";
    char temnik = ReadShaderFile(path);
    std::cout << temnik << std::endl;
    return 0;

}