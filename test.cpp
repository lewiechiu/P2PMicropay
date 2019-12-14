# include <iostream>
#include <string>
using namespace std;
int main(){
    string test = "123243\r\n asdf";
    cout << atoi(test.c_str());
    return 0;
}
