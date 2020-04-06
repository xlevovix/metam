#pragma once

#include "Tokens.hpp"

namespace mcomc
{
  using Error = std::tuple<std::wstring, uint64, uint64>;
  
  std::vector<Token> tokens;
  comc::Lexer lexer;
  comc::Reporter reporter;
  std::vector<Error> errors;
  
  void r_error(std::wstring const& s) {
    auto [line, pos] = lexer.currentInfo();
    errors.emplace_back(s, line, pos);
    ++reporter.errors;
  }
  
  void t_readWord() {
    static std::map<std::wstring, TokenType> keywords = {
      {L"import",     Import},
      {L"using",      Using},
      {L"var",        Var},
      {L"const",      Const},
      {L"throw",      Throw},
      {L"try",        Try},
      {L"catch",      Catch},
      {L"if",         If},
      {L"else",       Else},
      {L"match",      Match},
      {L"module",     Module},
      {L"namespace",  Namespace},
      {L"while",      While},
      {L"for",        For},
      {L"in",         In},
      {L"break",      Break},
      {L"continue",   Continue},
      {L"return",     Return},
      {L"times",      Times},
      {L"type",       Type},
      {L"enum",       Enum},
      {L"share",      Share},
      {L"private",    Private},
      {L"public",     Public},
      {L"this",       This},
      {L"thisfn",     Thisfn},
      {L"thiscl",     Thiscl},
      {L"thisns",     Thisns},
      {L"nsup",       Nsup},
      {L"operator",   Operator},
      {L"void",       Void},
      {L"required",   Required},
      {L"expects",    Expects},
    };
    auto s = lexer.readWord();
    if (s == L"_") {
      tokens.emplace_back(ignore);
      return;
    }
    if (lexer.get() == ' ') {
      if (s == L"else" && lexer.getWord(1) == L"catch") {
        lexer.position += 6;
        tokens.emplace_back(ElseCatch);
        return;
      }
      if (s == L"const" && lexer.getWord(1) == L"if") {
        lexer.position += 3;
        tokens.emplace_back(ConstIf);
        return;
      }
      if (s == L"do" && lexer.getWord(1) == L"while") {
        lexer.position += 6;
        tokens.emplace_back(DoWhile);
        return;
      }
      if (s == L"goto" && lexer.getWord(1) == L"case") {
        lexer.position += 5;
        tokens.emplace_back(GotoCase);
        return;
      }
    }
    if (s == L"true" || s == L"false")
      tokens.emplace_back(boolean, s == L"true");
    else if (keywords.contains(s))
      tokens.emplace_back(keywords[s]);
    else
      tokens.emplace_back(word, s);
  }
  bool t_readOperator(){
    wchar c1 = lexer.get(), c2 = lexer.get(1), c3 = lexer.get(2);

#define make(s, type) if (s3 == L##s) { tokens.emplace_back(type); lexer.position += 3; return true; }
    
    std::wstring s3 = {c1, c2, c3};
    make("...", ellipsis)
    make("->=", functionex)
    make("<=>", starship)

#undef make
#define make(s, type) if (s2 == L##s) { tokens.emplace_back(type); lexer.position += 2; return true; }
    
    std::wstring s2 = {c1, c2};
    make("+=", pluseq)
    make("-=", minuseq)
    make("*=", stareq)
    make("/=", slasheq)
    make("++", inc)
    make("--", decr)
    make("..", dotdot)
    make("::", dotcolon)
    make("->", function)
    make("=>", therefore)
    make("==", equals)
    make("!=", aequals)
    make("||", d_or)
    make("&&", d_and)
    make("<:", lb_a)
    make(":>", rb_a)
    make("<=", let)
    make(">=", ret)
    make("<<", lstream)
    make(">>", rstream)

#undef make
#define make(c, type) case c: { tokens.emplace_back(type); ++lexer.position; return true; }
    
    switch (c1) {
      make('.', dot)
      make(',', comma)
      make('+', plus)
      make('-', minus)
      make('*', star)
      make('/', slash)
      make('!', warn)
      make('(', lb_c)
      make(')', rb_c)
      make('{', lb_e)
      make('}', rb_e)
      make('<', lt)
      make('>', rt)
      make('[', lb_s)
      make(']', rb_s)
      make('\\', backslash)
      make('?', question)
      make('#', tag)
      make('@', dog)
      make('%', percent)
      make('~', tild)
      make(':', colon)
      make(';', EOE)
      //make('_', ignore)
      make('=', assign)
      make('|', a_or)
      make('&', a_and)
    }

#undef make
    
    return false;
  }
  void t_readNumber() {
    if (lexer.get() == '0' && lexer.get(1) == 'x') {
      lexer.position += 2;
      tokens.emplace_back(integral, lexer.readHEX());
      return;
    }
    // TODO: 0b001010100011
    auto [d, b] = lexer.readDigit();
    if (b) tokens.emplace_back(relational, d);
    else tokens.emplace_back(integral, uint64(d));
    if (iswalpha(lexer.get())) tokens.emplace_back(suffix, lexer.readWord());
  }
  void t_readString() {
    tokens.emplace_back(string, lexer.readString(reporter));
  }
  void t_readChar() {
    tokens.emplace_back(charic, lexer.readChar(reporter));
  }
  
  void tokenize(std::wistream& input) {
    lexer.begin(input);
    tokens.clear();
    reporter.error = r_error;
    
    for (wchar c = lexer.get(); c != '\0'; c = lexer.get()) {
      while (iswspace(c)) c = lexer.next();
      if (iswalpha(c) || c == '_')
        t_readWord();
      else if (iswdigit(c))
        t_readNumber();
      else if (c == '"')
        t_readString();
      else if (c == '\'')
        t_readChar();
      else if (lexer.match("//>")) {
        lexer.position += 3;
        lexer.skipWhile(L"<//");
      }
      else if (lexer.match("//")) {
        tokens.emplace_back(EOE);
        lexer.skipLine();
      }
      else if (c == '\n')
        tokens.emplace_back(EOE);
      else if (!t_readOperator()) ++lexer.position;
    }
  }
  void tokens_postProcess() {
    bool passEOE = false;
    for (auto it = tokens.begin(); it != tokens.end(); ) {
      if (it->type == EOE && passEOE) {
          it = tokens.erase(it); continue;
      }
      passEOE = it->type == EOE;
      ++it;
    }
  }
}
