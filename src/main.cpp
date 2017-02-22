#include <iostream>

int main() {
  std::cout << "Hello, " << PROJECT_NAME << " " << BUILD_DESC << "!" << std::endl;
  #ifdef DEBUG
    std::cout << "It's debug." << std::endl;
  #endif
  return 0;
}