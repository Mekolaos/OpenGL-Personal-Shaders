#include <iostream>
#include <fstream>
#include <string>

char ReadShaderFile(std::string path){
    using namespace std;
    ifstream shaderFile;
    shaderFile.open(path, ifstream::in);
    char output;
    string line;

    if(shaderFile.is_open()){
        cout << "Shader file open at : " << path << endl;
        while(!shaderFile.eof()){
            output += shaderFile.get();
        }
    }
    else{
        cout << "ma mchatch el file a sahbi" << endl;
    }
    shaderFile.close();
    std::cout << "Shader file closed" << std::endl;

    return output;
}

float Vec3Add(float vect1[], float vect2[])
{
    float new_bitch[3];
    for (int i = 0; i < 3; i++)
    {
        new_bitch[i] = vect1[i] + vect2[i];
    }
    return new_bitch[3];
}