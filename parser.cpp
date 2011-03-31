#include "parser.h"

Number::Number(double value) : val(value) {}

double Number::value() const
{
  return val;
}

Symbol::Symbol(const std::string& symbol) : sym(symbol) {}

std::string Symbol::value() const
{
  return sym;
}

ConsPair::ConsPair(const ElemPtr& car, const ElemPtr& cdr)
  : car_(car), cdr_(cdr) {}
ConsPair::ConsPair() {}

ElemPtr ConsPair::car()
{
  return car_;
}

ElemPtr ConsPair::cdr()
{
  return cdr_;
}

void ConsPair::set_car(const ElemPtr& car)
{
  car_ = car;
}

void ConsPair::set_cdr(const ElemPtr& cdr)
{
  cdr_ = cdr;
}

bool is_nil(const PairPtr& p)
{
  return ((!(p->car())) && (!(p->cdr())));
}

NumberPtr make_number(double value)
{
  return NumberPtr(new Number(value));
}

SymbolPtr make_symbol(const std::string& symbol)
{
  return SymbolPtr(new Symbol(symbol));
}

PairPtr to_cons(const ElemPtr& e)
{
  return boost::dynamic_pointer_cast<ConsPair>(e);
}

SymbolPtr to_symbol(const ElemPtr& e)
{
  return boost::dynamic_pointer_cast<Symbol>(e);
}

NumberPtr to_number(const ElemPtr& e)
{
  return boost::dynamic_pointer_cast<Number>(e);
}

Parser::Parser(const std::string& input)
{
  parse_expr(input);
}

void Parser::add_elem(const ElemPtr& elem)
{
    if (current->car())
      {
	current->set_cdr(elem);
	current = to_cons(current->cdr());
      }
    else
      {
	current->set_car(elem);
	PairPtr P(new ConsPair());
	current->set_cdr(P);
	current = to_cons(current->cdr());
      }
}

// The heart of our parser, which as mentioned is basically a pushdown
// automaton.  The strategy is: examine the input character by
// character.  When we reach an open-paren, we push an empty conspair
// onto the stack and start adding to that.  When we reach a
// close-paren, we know that our conspair is finished, so we pop the
// stack and continue where we left off previously.
void Parser::parse_expr(const std::string& input)
  {
    size_t currindex = 0;
    while((currindex < (input.length())) &&
	  currindex != std::string::npos)
      {
	switch(input[currindex])
	  {
	  case ' ':
	    currindex++;
	    break;
	  case '(':
	    {
	      // Beginning of conspair
	      PairPtr P(new ConsPair());
	      S.push(P);
	      if (S.size() > 1)
		  add_elem(P);
	      current = S.top();
	      currindex++;
	      break;
	    }
	  case ')':
	    {
	      if (S.size() > 1)
		{
		  // We are done processing this list, so let's pop
		  // the stack and continue where we left off on the
		  // previous one.
		  S.pop();
		  current = to_cons(S.top()->cdr());

		  while(!(is_nil(to_cons(current))))
		    current = to_cons(current->cdr());
		}
	      else
		{
		  // No more to process. Return what we have.
		  expr = S.top();
		  S.pop();
		}
	      currindex++;
	      break;
	    }
	  default:
	    {
	      size_t nextindex = next_index(input, currindex);
	      std::string::const_iterator end;
	      if (nextindex == std::string::npos)
		end = input.end();
	      else
		end = input.begin() + nextindex;
	      std::string token(input.begin() + currindex, end);

	      ElemPtr e;

	      if (is_number(token))
		{
		  double d = convert<double>(token);
		  e = make_number(d);
		}
	      else
		e = make_symbol(token);
	      
	      if (S.empty())
		expr = e;
	      else
		add_elem(e);

	      currindex = nextindex;
	    }
	    break;
	  }
      }
    if (!(S.empty()))
      throw Exception("Parse error: Malformed input"); 
  }

ElemPtr Parser::parsed_expr()
{
  return expr;
}

// Would be nice to make this a virtual member function, but it's
// awkward with the recursion for list printing.
std::ostream& print(std::ostream& os, const ElemPtr& e, bool first=true)
{
  SymbolPtr sym = to_symbol(e);
  NumberPtr num = to_number(e);
  if (sym)
    {
      os << sym->value();
    }
  else if (num)
    {
      os << num->value();
    }
  else
    {
      PairPtr p = to_cons(e);
      if (p)
	{
	  if (first)
	    os << "(";
	  if (is_nil(p))
	    os << ")";
	  else
	    {
	      if (!(to_symbol(p->car())))
		os << "(";
	      print(os, p->car(), false);
	      if (!(is_nil(to_cons(p->cdr()))))
		os << " ";
	      print(os, p->cdr(), false);
	    }
	}
    }
  return os;
}

// Overload function to enable us to print any expression, be it a
// list, single number, or symbol.
std::ostream& operator<<(std::ostream& os, const ElemPtr& e)
{
  return print(os, e);
}
