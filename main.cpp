#include <iostream>
#include <fstream>

int main() {
    std::string line;
    std::ofstream file("/home/logs/log.txt");
    
    while (std::cin >> line) {
        std::cout << line << std::endl;
        file << line << std::endl;
    }
    return 0;
}
