#ifndef _PARSER_H
#define _PARSER_H

#include "shared_ptr.hpp"
#include "util.h"
#include <stack>
#include <map>

class Element;
class Number;
class Symbol;
class ConsPair;
class Function;

using boost::shared_ptr;

typedef shared_ptr<Number> NumberPtr;
typedef shared_ptr<Element> ElemPtr;
typedef shared_ptr<Symbol> SymbolPtr;
typedef shared_ptr<ConsPair> PairPtr;
typedef shared_ptr<Function> FunPtr;

typedef std::map<const std::string, double> Environment;
typedef std::map<const std::string, FunPtr> FuncEnv;


NumberPtr make_number(double);
SymbolPtr make_symbol(const std::string&);

PairPtr to_cons(const ElemPtr&);
SymbolPtr to_symbol(const ElemPtr&);
NumberPtr to_number(const ElemPtr&);

// Do we have an empty list?
bool is_nil(const PairPtr&);

// Our type hierarchy for our parser. A dummy base class Element, and
// then everything else is either a number, a symbol, or a conspair.
class Element
{
 public:
  virtual ~Element() {}
};

class Number : public Element
{
private:
  double val;
public:
  Number(double);
  double value() const;
};

class Symbol : public Element
{
private:
  std::string sym;
public:
  Symbol(const std::string&);
  std::string value() const;
};

// A conspair is a pair of pointers: a 'car' and a 'cdr', in Lisp parlance.
class ConsPair : public Element
{
private:
  ElemPtr car_;
  ElemPtr cdr_;
public:
  ConsPair(const ElemPtr&, const ElemPtr&); 
  ConsPair();
  ElemPtr car();
  ElemPtr cdr(); 
  void set_car(const ElemPtr&);
  void set_cdr(const ElemPtr&);
};

// Our parser, basically a pushdown automaton. It returns an
// expression, which is either a single number or symbol, or a parsed
// S-expression in conspair representation.
class Parser
{
 private:
  std::stack<PairPtr> S;
  ElemPtr expr;
  PairPtr current;
  PairPtr to_process;
  void add_elem(const ElemPtr&);
  void parse_expr(const std::string&);
 public:
  Parser(const std::string&);
  ElemPtr parsed_expr();
};

std::ostream& print(std::ostream& os, const ElemPtr& e, bool first);
std::ostream& operator<<(std::ostream& os, const ElemPtr& e);

#endif
