#ifndef X
#define X(x)
#endif
// ключевые слова
X(Import)
X(Using)
X(Var)
X(Const)
X(Throw)
X(Try)
X(Catch)
X(ElseCatch)
X(If)
X(ConstIf)
X(Else)
X(Match)
X(Module)
X(Namespace)
X(While)
// do - не ключевое слово
X(DoWhile)
X(For)
X(In)
X(Break)
X(Continue)
X(Return)
X(Times)
X(Type)
X(Enum)
X(Share)
X(Private)
X(Public)
X(This)
X(Thisfn)
X(Thiscl)
X(Thisns)
X(Nsup)
X(Operator)
X(Void)
X(Required)
X(Expects)
X(GotoCase)

// #...
//tag_if
//tag_else
//tag_elif
//tag_define

X(word)       // foo
X(string)     // "hello, world"
X(integral)   // 10
X(relational) // 1.0
X(boolean)    // true false
X(charic)     // 'a'

X(suffix)     // 1.0f

// спецсимволы
// NOTE: // и //> <// (комментарии) опускаются во время токенизации
X(plus)       // +
X(minus)      // -
X(star)       // *
X(slash)      // /
X(pluseq)     // +=
X(minuseq)    // -=
X(stareq)     // *=
X(slasheq)    // /=
X(inc)        // ++
X(decr)       // --

X(warn)       // !
X(backslash)  // \

X(question)   // ?
X(tag)        // #
X(dog)        // @
X(percent)    // %
X(tild)       // ~

X(dot)        // .
X(comma)      // ,
X(dotdot)     // ..
X(colon)      // :
X(EOE)        // ; (end of expression)
X(dotcolon)   // ::
X(ellipsis)   // ...

X(function)   // ->
X(therefore)  // =>
X(functionex) // ->=
X(ignore)     // _

X(assign)     // =
X(equals)     // ==
X(aequals)    // !=

X(a_or)       // |
X(a_and)      // &
X(d_or)       // ||
X(d_and)      // &&

// скобки
X(lb_c) X(rb_c) // ()
X(lb_s) X(rb_s) // []
X(lb_e) X(rb_e) // {}
X(lb_a) X(rb_a) // <::>

X(lt) X(rt)     // <>
X(let) X(ret)   // <= >=
X(starship)   // <=>

X(lstream)    // <<
X(rstream)    // >>
