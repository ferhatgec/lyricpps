#include "include/lyricpps.hpp"
#include <fstream>
int main() {
    std::ifstream test("example.lyrics");
    std::string temp, data;

    while(std::getline(test, temp)) {
        data.append(temp);
        if(temp.back() != '%') { data.push_back('\n'); }
    } test.close();


    lyricpps init;

    init.parse(data);
    init.visit();
}