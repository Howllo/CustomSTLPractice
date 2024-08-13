#include <iostream>
#include <vector>
#include "Standard/VecArray.h"

void Test(std::string& in_text);

int main() {
    std::string t = "Hello World";
    std::vector<int> test_vector;
    std::vector<int> test_vector2;
    std::vector<int> test_vector3;
    VecArray NewTest = {1, 2, 3};
    NewTest.insert(4, 3);

    for(auto Ele : NewTest) {
        std::cout << Ele << '\n';
    }

    VecArray<std::vector<int>> Test;
    Test.push_back(test_vector);
    Test.push_back(test_vector2);
    Test.insert(test_vector3, 1);
}