#include <metam-comc.hpp>
#include <fstream>
#include <filesystem>
#include <locale>

using namespace std;
using namespace mcomc;

constexpr wchar const* c_red = L"\033[1;31m";
constexpr wchar const* c_blue = L"\033[1;34m";
constexpr wchar const* c_none = L"\033[0m";

int main() {
  locale::global(locale(""));
  
  std::wifstream fs{"file.metm"};
  if (!fs.is_open()) { wcerr << L"Не удалось открыть файл." << endl; return 1; }
  tokenize(fs);
  fs.close();
  tokens_postProcess();
  
  wcout << endl << c_blue << L"Исходный код: " << c_none << "\n\n" << lexer.text << "\n\n";
  
  for (auto const& [error, line, pos] : errors) {
    wcout << c_blue << "[" << line << ", " << pos << "] " << c_red << L"Ошибка: " << c_none << error << std::endl;
  }
  if (reporter.errors > 0) wcout << endl;
  
  wcout << c_blue << L"Токены: " << c_none << "\n\n";
  
  for (auto const& t : tokens) {
    wcout << t << endl;
  }
}

