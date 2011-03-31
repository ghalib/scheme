#ifndef _SCHEME_H
#define _SCHEME_H

// Functions directly applicable to evaluating our scheme expressions.

#include "parser.h"
#include <algorithm>
#include <numeric>
#include <functional>
#include <list>


// Class hierarchy of our primitive functions: addition,
// multiplication, subtraction, and division.

class Function {
public:
  virtual double apply(const std::list<double>& args) = 0;
  virtual ~Function() {}
};

class Adder : public Function {
public:
  double apply(const std::list<double>&);
};

class Multiplier : public Function {
public:
  double apply(const std::list<double>&);
};
  
class Subtractor : public Function {
public:
  double apply(const std::list<double>&);
};

class Divider : public Function {
 public:
  double apply(const std::list<double>&);
};

// Operations on cons cells
PairPtr cons(const ElemPtr&, const ElemPtr&);
ElemPtr car(const ElemPtr&);
ElemPtr cdr(const ElemPtr&);

// Extract the value from a Number
double value(const ElemPtr&);

// Check to see if the head of the list is equal to a given string
bool tagged_with(const ElemPtr&, const std::string&);

bool is_self_evaluating(const ElemPtr&);
bool is_variable(const ElemPtr&);
bool is_application(const ElemPtr&);
bool is_definition(const ElemPtr&);

template<typename T>
T lookup(const ElemPtr& expr,
	 std::map<const std::string, T>& env)
{
  typename std::map<const std::string, T>::const_iterator pos;
  std::string var = to_symbol(expr)->value();

  pos = env.find(var);
  if (pos != env.end())
    return pos->second;
  else
    throw Exception("Undefined variable '" + var + "'");
}


// Add a new variable to the symbol table
double define_variable(const std::string&, double,
		       Environment&);

// Extract the assignment/definition variable/value out of our S-expression.
std::string definition_variable(const ElemPtr&);
ElemPtr definition_value(const ElemPtr&);

std::string assignment_variable(const ElemPtr&);
ElemPtr assignment_value(const ElemPtr&);

// Modify the value of an existing variable
double set_variable(const std::string&, double, Environment&);

double eval_definition(const ElemPtr&, Environment&,
		       FuncEnv&);

double eval_assignment(const ElemPtr&, Environment&,
		       FuncEnv&);

void printenv(const Environment&);

ElemPtr oper(const ElemPtr&);
ElemPtr operands(const ElemPtr&);

// Take the arguments given by user, and evaluate them, returning a
// list of evaluated args
std::list<double> list_of_values(const ElemPtr&, Environment&,
				 FuncEnv&);


// This invokes a function on a set of args
double apply(const FunPtr&, const std::list<double>&);

// This is called on each expression entered by the user
double eval(const ElemPtr&, Environment&, FuncEnv&);

// Load the function symbol table with primitive functions (so far
// just the arithmetic ones)
bool load_func_env(FuncEnv&);

#endif
