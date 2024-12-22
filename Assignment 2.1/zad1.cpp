#include <iostream>
#include <stack>
#include <string>

bool provjeri_zagrade(std::string tekst) {
  std::stack<char> otvoreneZagrade;

  for (char c : tekst) {
    if (c == '(' || c == '[' || c == '{') {
      otvoreneZagrade.push(c);
    } else if (c == ')' || c == ']' || c == '}') {
      if (otvoreneZagrade.empty()) {
        return false; // zagrada bez zatvorene
      }

      char otvorenaZagrada = otvoreneZagrade.top();
      otvoreneZagrade.pop();

      if ((c == ')' && otvorenaZagrada != '(') ||
          (c == ']' && otvorenaZagrada != '[') ||
          (c == '}' && otvorenaZagrada != '{')) {
        return false;
      }
    }
  }

  return otvoreneZagrade.empty(); // ako su sve dobre onda je ispraznjen
}

int main() {
  std::string tekst;
  std::cout << "Unesi string: ";
  std::getline(std::cin, tekst);
  bool a = provjeri_zagrade(tekst);
  if (a == 1)
    std::cout << "Dobro je zatvoreno" << std::endl;
  else
    std::cout << "Nije dobro zatvoreno";
  return 0;
}