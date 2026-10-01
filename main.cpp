#include<iostream>
#include<sstream>
#include<string>
#include<fstream>
#include<cstring>

#define SIZE_M 16
using namespace std;

string MNEMONICS [SIZE_M] = {"nA", "AoBn", "nAeB", "zeroL", "AeBn", "nB", "AxB", "AenB", "nAoB", "AxBn", "copiaB", "AeB", "umL", "AonB", "AoB", "copiaA"};

string str_trim(std::string s) {

    size_t start = s.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) {
        return ""; 
    }

    size_t end = s.find_last_not_of(" \t\n\r");

    return s.substr(start, end - start + 1);
}


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

    short int count_lines = 1;
    string line;
    bool isExit = false; 

    string vars_program [2] = {"",""};
    int values_vars [2] = {0,0};

    if(file.is_open()){
        getline(file,line);
        count_lines++;

        if(str_trim(line) != "inicio:"){
            cout << "\033[31m > Error: "<< "\033[37m"<<"Arquivo deve definir o escopo, indicando 'inicio:' e 'fim.'. Line: "<< count_lines;
            return -1;
        }

        while(getline(file,line) && !isExit){
            line = str_trim(line);
            
            stringstream ss(line);
            string field, value_field;

            if(line == ""){
                cout << "\033[33m> Warning:" << "\033[37m"<< " Linha Vazia. Line:" << count_lines << endl;
                count_lines++;
                continue;
            }


            getline(ss,field,'=');

            getline(ss,value_field,'=');

            if(isValidValInput(value_field)){
                if(!aloc_var(field, value_field, vars_program, values_vars)){
                        cout << "\033[33m> Warning:" << "\033[37m" << " Variavel '"<< field <<"' Invalida. Line: "<< count_lines << endl;
                }
            }else if(value_field != ""){
                value_field.pop_back();
                int posM = searchMnemonic(value_field);
                if(posM != -1){
                    fileOut << hex << uppercase << values_vars[0];
                    fileOut << hex << uppercase << values_vars[1];
                    fileOut << hex << uppercase << posM << endl;
                    
                }else{
                    cout << "\033[33m> Warning:"<< "\033[37m" << " Instrucao '"<< value_field <<"' Invalida. Line: "<< count_lines << endl;
                }
            }

            count_lines++;

            if(line == "fim."){
                isExit = true;
            }        
        }
        count_lines--;
        cout << "\033[36m> Total Lines: "<< "\033[37m" << count_lines << endl;
        cout << "\033[30m> Interpretacao Finalizada!" << "\033[37m" << endl;
    }else{
        cout << "\033[31m > Error: "<< "\033[37m" << "Arquivo nao encontrado!" << endl;
    }


    file.close();
    fileOut.close();

    return 0;
}
