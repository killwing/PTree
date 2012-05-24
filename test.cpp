#include "PTree.h"

#include <iostream>
#include <string>
#include <cassert>
using namespace std;

struct Car {
    string color;
    unsigned int price;
};

int main() {
    PTree<string> pt;    

    // normal get
    string name("Alice");
    pt.put("person.name", name);
    string nameOut = pt.get("person.name"); // assignment syntax
    assert(name == nameOut);
    string nameEx(pt.get<string>("person.name")); // explicit syntax, need convert explicitly
    assert(name == nameEx);

    // const get
    const PTree<string>& cpt = pt;
    string nameOutC = cpt.get("person.name");
    assert(name == nameOutC);
    string nameExC;
    nameExC = cpt.get<string>("person.name");
    assert(name == nameExC);
    const string& nameRefC = cpt.get("person.name");
    assert(name == nameRefC);

    // ref get
    string& nameRef = pt.get("person.name");
    nameRef = "Bob";
    nameOut = pt.get<string>("person.name");
    assert(nameRef == nameOut);

    // other types
    int age = 25;
    pt.put("person.age", age);
    int ageOut(pt.get("person.age"));
    assert(age == ageOut);

    float height = 1.65;
    pt.put("person.height", height);
    float heightOut(pt.get("person.height"));
    assert(height == heightOut);

    Car car;
    car.color = "black";
    car.price = 50000;
    pt.put("person.car", car);
    Car carOut(pt.get("person.car"));
    assert(car.color == carOut.color);
    assert(car.price == carOut.price);

    // push as vector
    int income = 100;
    pt.push("person.deposit", income++);
    pt.push("person.deposit", income++);
    pt.push("person.deposit", income++);
    const std::vector<int>& sum = pt.get("person.deposit");
    assert(sum.size() == 3);

    // del
    string error;
    pt.del("person.name");
    try {
        pt.get("person.name");
    } catch (const PTreeError& e) {
        error = e.what();
        cout << "catched PTreeError: " << error << endl;
    }
    assert(!error.empty());

    cout << "all test passed" << endl;
    return 0;
}

