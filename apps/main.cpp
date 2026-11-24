#include <iostream>
#include <detect/version.hpp>

int main() {
  std::cout << "detect " << detect::version() << '\n';
  return 0;
}
