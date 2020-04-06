#pragma once

#include "Tokens.hpp"

// TODO: Работа по оптимизации: не выполнять считывание файла в строку, считывать прямо из потока, следить за номером строки. (StreamLexer)

namespace comc
{
  struct Lexer
  {
    std::wstring text = L"";
    std::vector<uint64> linesLen;
    uint position = 0;

  private:
    uint length = 0;

  public:
    void begin(std::wistream& ss);
    
    wchar next(uint steps = 1);
    std::wstring nextString(uint len = 2, uint steps = 0);
    
    wchar get(uint steps = 0);
    uint64 currentLine();
    std::tuple<uint64, uint64> currentInfo();
    
    bool match(wchar c, uint steps = 0);
    bool match(std::string_view s, uint steps = 0);
    bool match(std::wstring_view s, uint steps = 0);
    
    void skipLine();
    void skipWhile(std::wstring_view s);
    
    template<class Predicate>
    bool checkadd(Predicate f, std::wstring& w);
    
    std::wstring readWord();
    std::wstring getWord(uint steps = 0);
  
    uint64 readHEX();
    std::pair<double, bool> readDigit();
    std::wstring readString(Reporter& reporter);
    wchar readChar(Reporter& reporter);
  };
  
  
  void Lexer::begin(std::wistream& ss) {
    text.clear();
    linesLen.clear();
    linesLen.emplace_back(0);
    while (true)
    {
      wchar c;
      ss.get(c);
      if (ss.eof()) break;
      text += c;
      ++linesLen.back();
      if (c == '\n')
        linesLen.emplace_back(0);
    }
    if (!ss.eof()) throw std::runtime_error("Cannot fully read file.");
    length = text.length();
    position = 0;
  }
  wchar Lexer::next(uint steps) {
    position += steps;
    if (position >= length) return '\0';
    return text[position];
  }
  wchar Lexer::get(uint steps) {
    uint pp = position + steps;
    return pp < length ? text[pp] : '\0';
  }
  bool Lexer::match(wchar c, uint steps) {
    return get(steps) == c;
  }
  bool Lexer::match(std::string_view s, uint steps) {
    if (position + steps + s.length() >= length) return false;
    for (uint pp = position + steps, i = 0; i < s.length(); ++i, ++pp)
      if (text[pp] != s[i]) return false;
    return true;
  }
  bool Lexer::match(std::wstring_view s, uint steps) {
    if (position + steps + s.length() >= length) return false;
    for (uint pp = position + steps, i = 0; i < s.length(); ++i, ++pp)
      if (text[pp] != s[i]) return false;
    return true;
  }
  template<class Predicate>
  bool Lexer::checkadd(Predicate f, std::wstring& w) {
    auto c = get();
    bool b = f(c);
    if (b) { w += c; ++position; }
    return b;
  }
  std::wstring Lexer::nextString(uint len, uint steps) {
    std::wstringstream ss;
    position += steps;
    while (len > 0) { ss << get(); ++position; --len; }
    return ss.str();
  }
  std::wstring Lexer::readWord() {
    std::wstring s;
    while (checkadd([](wchar c) { return iswalnum(c) || c == '_'; }, s));
    return s;
  }
  std::wstring Lexer::getWord(uint steps) {
    auto pp = position;
    position += steps;
    auto s = readWord();
    position = pp;
    return s;
  }
  uint64 Lexer::readHEX() {
    std::wstring s;
    while (checkadd([](wchar c) {
                      c = towlower(c);
                      std::wstring ss = L"abcdef";
                      return iswdigit(c) || std::find(ss.cbegin(), ss.cend(), c) != ss.cend(); },
                    s));
    std::wstringstream ss;
    uint64 r;
    ss << std::hex << s;
    ss >> r;
    return r;
  }
  std::pair<double, bool> Lexer::readDigit() {
    std::wstring s;
    // TODO: 1'000 is digit
    bool b = false;
    while (checkadd([&b](wchar c) {
                      if (b && c == '.') return false;
                      if (c == '.') b = true;
                      return iswdigit(c) || c == '.'; },
                    s));
    if (s.back() == '.') s.pop_back();
    std::wstringstream ss;
    double r;
    ss << s;
    ss >> r;
    return {r, b};
  }
  std::wstring Lexer::readString(Reporter& reporter) {
    std::wstring s;
    for (wchar c = next(); c != '"' && c != '\0' && c != '\n'; c = next()) {
      if (c == '\\') {
        c = next();
        switch (c) {
          case 'n': s += '\n'; continue;
          case 'r': s += '\r'; continue;
          case '\n': s += '\n'; continue;
          case '0': s += '\0'; continue;
          case '\\': s += '\\'; continue;
          case '"': s += '"'; continue;
          case 'u': {
            std::wstringstream ss;
            ss << std::hex << nextString(4, 1); --position;
            uint64 d;
            ss >> d;
            s += wchar(d);
            continue;
          }
          case 'c': {
            std::wstringstream ss;
            ss << std::hex << nextString(2, 1); --position;
            uint64 d;
            ss >> d;
            s += wchar(d);
            continue;
          }
          default: {
            std::wstringstream ws;
            ws << L"Неизвестный символ экранирования (\\" << c << ")";
            reporter.error(ws.str());
            continue;
          }
        }
      }
      s += c;
    }
    next();
    return s;
  }
  wchar Lexer::readChar(Reporter& reporter) {
    auto c = next();
    if (c == '\\') {
      c = next();
      switch (c) {
        case 'n': c = '\n'; break;
        case 'r': c = '\r'; break;
        case '0': c = '\0'; break;
        case '\\': c = '\\'; break;
        case '\'': c = '\''; break;
        case 'u': {
          std::wstringstream ss;
          ss << std::hex << nextString(4, 1); --position;
          uint64 d;
          ss >> d;
          c = d;
          break;
        }
        case 'c': {
          std::wstringstream ss;
          ss << std::hex << nextString(2, 1); --position;
          uint64 d;
          ss >> d;
          c = d;
          break;
        }
        default: {
          std::wstringstream ws;
          ws << L"Неизвестный символ экранирования (\\" << c << ")";
          reporter.error(ws.str());
        }
      }
    }
    if (!match('\'', 1)) reporter.error(L"Ожидается: '");
    next(2);
    return c;
  }
  uint64 Lexer::currentLine() {
    auto pp = 0;
    auto line = 0;
    while (pp < position) {
      pp -= linesLen[line];
      ++line;
    }
    return line;
  }
  std::tuple<uint64, uint64> Lexer::currentInfo() {
    uint64 pp = 0, p = position;
    auto line = 0;
    while (pp < position) {
      pp += linesLen[line];
      if (pp < position) p -= linesLen[line];
      ++line;
    }
    return {line, p};
  }
  
  void Lexer::skipLine() {
    auto c = get(); while (c != '\n' && c != '\0') c = next();
    ++position;
  }
  void Lexer::skipWhile(std::wstring_view s) {
    while (!match(s) && position < length) ++position;
    position += s.length();
  }
}
