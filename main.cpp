#include<iostream>
#include<sstream>
#include<string>
#include<fstream>
#define SIZE_M 16
using namespace std;

string MNEMONICS [SIZE_M] = {"nA", "AoBn", "nAeB", "zeroL", "AeBn", "nB", "AxB", "AenB", "nAoB", "AxBn", "copiaB", "AeB", "umL", "AonB", "AoB", "copiaA"};


int searchMnemonic(string s){
    int position = -1;
    for(int i = 0; i < SIZE_M && position == -1; i++){
        if(MNEMONICS[i] == s){
            position = i;
        }
    }
    
    return position;
}

bool isValidValInput(string s){
    bool rtn = s == "" ? false : true;

    if(atoi(s.c_str()) < 16){
        for(int i =0; i < s.length() and rtn; i++){
            if(!isdigit(s[0])){
                rtn = false;
            }
        }
    }else{
        rtn = false;
    }
    return rtn;
}

bool aloc_var(string & field, string & value_field, string vars_program [], int values_vars []){
    bool statusAloc = true;

    if(field == vars_program[0]){          
        values_vars[0] = atoi(value_field.c_str());
    }else if(field == vars_program[1]){
        values_vars[1] = atoi(value_field.c_str());
    }else if(vars_program[0] == "" or vars_program[1] == ""){
        if(vars_program[0] == ""){
            vars_program[0] = field;
            values_vars[0] = atoi(value_field.c_str());
        }else{
            vars_program[1] = field;
            values_vars[1] = atoi(value_field.c_str());
        }
    }else{
        statusAloc = false;    
    }

    return statusAloc;
}

int main(int argc, char * argv []){
    
    ifstream file(argv[1]);
    ofstream fileOut(argv[2]);    

    string line;
    bool isExit = false; 

    string vars_program [2] = {"",""};
    int values_vars [2] = {0,0};

    getline(file,line);
    while(getline(file,line) && !isExit){
        stringstream ss(line);
        string field, value_field;

        if(line == ""){
            cout << "void line!" << endl;
            continue;
        }

        getline(ss,field,'=');

        getline(ss,value_field,'=');

        if(isValidValInput(value_field)){
            if(!aloc_var(field, value_field, vars_program, values_vars)){
                    cout << "var invalid" << endl;
            }
        }else if(value_field != ""){
            value_field.pop_back();
            int posM = searchMnemonic(value_field);
            if(posM != -1){
                fileOut << hex << uppercase << values_vars[0];
                fileOut << hex << uppercase << values_vars[1];
                fileOut << hex << uppercase << posM << endl;
                
            }else{
                cout << "instrucao nao encontrada" << endl;
            }
        }

        if(line == "fim."){
            isExit = true;
        }
    }

    file.close();
    fileOut.close();

    return 0;
}
