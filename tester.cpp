#include "PTree.h"

#include <iostream>
#include <string>
using namespace std;

int main() {
    PTree<string> pt;    
    //pt.put("a.b.c", 123);
    pt.put("a.b.c", string("456"));
    pt.put("a.x", 123);
    pt.put("x", string("fdsaf"));
    
    PTree<string>& t = pt.get("a");
    int a = t.get("x");

    cout<<a<<endl;
    return 0;
}
