#include<bits/stdc++.h>

using namespace std;


int main(){

    static const string root = "../main.cpp";
    ifstream file(root);
    if(file.is_open()){
        cout<< "main.cpp is read" <<endl;
    }else{
        cout << "Unable to find the given root"<< endl;
    }
    
    string s;
    while(getline(file, s)){
        cout<<s<<endl;
    }




    return 0;
}