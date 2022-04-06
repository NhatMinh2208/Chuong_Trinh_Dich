#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<set>
#include<algorithm>
using namespace std;

bool sign = false;
bool isCharacter(char c){
    if(c == '.') sign = true;
    if(!(c <= 'z' && c >= 'a') || (c <= 'Z' && c >= 'A')) return 0;
    //if(c == ' ' || c == ',' || c == '.' || c == '(' || c == ')' || c == '!' || c == ';' || c == '?' || c == '-') return 0;
    return 1;
}

bool is_number(string s) {
  return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

struct info{
    string word;
    int count;
    std::set<int> line;
};

bool isProperNoun(string word){
    return (isupper(word[0]));
}

string toLowerCase(string word){
    string temp;
    for(auto i = word.begin(); i < word.end(); i++){
        temp += tolower(*i);
    }
    return temp;
}

bool isBelongTo(string word, std::vector<string> list){
    for(auto x : list){
        if(x == word) return true;
    }
    return false;
}
std::vector<string> split(string text){
    std::vector<string> arr;
    string temp;

    for(auto it = text.begin(); it != text.end(); it++){
        if(isCharacter(*it)){
            if(sign == true){
                auto it2 = it-1;
                if(*it2 == ' '){
                    *it = tolower(*it);
                }
                sign = false;
            }
            temp.push_back(*it);
        }
        else{
            if(temp != ""){
                arr.push_back(temp);
                temp = "";
            }
        }
    }
    if(temp != "") arr.push_back(temp); 
    return arr;    
}

std::vector<string> readFILE(string arg){
    string word;
    ifstream readfile(arg);
    std::vector<string> list;
    while (getline(readfile,word))
    {
        list.push_back(word);
    }
    readfile.close();
    return list;
}

bool compareByString(info &a, info &b)
{
    return a.word < b.word;
}

void writeFILE(string filename,std::vector<info> arr){
    ofstream myfile;
    myfile.open (filename);
    for(auto x : arr){
        myfile << x.word << " " << x.count;
        for(auto y : x.line) myfile << "," << y;
        myfile << endl;
    }
    myfile.close();
}
int main(int argc, char *argv[]){
    std::vector<string> nonsense_list;
    nonsense_list = readFILE("stopw.txt");
    

    string myText;
    ifstream myfile("alice30.txt");
    std::vector<info> arr;
    int line_index = 0;
    while(getline(myfile, myText)){
        std::vector<string> temp = split(myText);
        //int cnt = 0;
        for(auto x : temp){
            int check = 0;
            //if(cnt == 0) x = toLowerCase(x);
            if((isProperNoun(x)) || isBelongTo(x,nonsense_list) || is_number(x)){
                //cnt++;
                continue;
            }
            for(auto &i : arr){
                if(i.word == x){
                    i.line.insert(line_index);
                    i.count++;
                    check = 1;
                    break;
                }
            }
            if(check == 0){
                info temp2;
                temp2.word = x;
                temp2.count = 1;
                temp2.line.insert(line_index);
                arr.push_back(temp2);
            }
            //cnt++;
        }
        line_index++;
    }
    std::sort(arr.begin(), arr.end(), compareByString);
    for(auto x : arr){
        cout << x.word << " " << x.count;
        for(auto y : x.line) cout << "," << y;
        cout << endl;
    }
    myfile.close();
    writeFILE("index.txt", arr);   
}

