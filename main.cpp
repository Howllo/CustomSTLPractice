#include <iostream>
#include "Standard/VecArray.h"

void Test(std::string& in_text);

int main() {
    VecArray NewTest = {1, 2, 3};
    NewTest.insert(4, 3);
    NewTest.clear();

    for(const auto Ele : NewTest) {
        std::cout << Ele << '\n';
    }

    std::cout << "Is Empty: " << NewTest.empty() << '\n';

    NewTest.shrink_to_fit();
    std::cout << "Shrink Capacity: " << NewTest.capacity() << '\n';
    NewTest.push_back(2);
}