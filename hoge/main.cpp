#include <filesystem>
#include <iostream>

int main() {
    std::filesystem::path currentDir = std::filesystem::current_path();
    std::cout << "Current Directory: " << currentDir << std::endl;
    return 0;
}