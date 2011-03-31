#include "scheme.h"

double Adder::apply(const std::list<double>& args)
  {
    return std::accumulate(args.begin(), args.end(), double(0));
  }

double Multiplier::apply(const std::list<double>& args)
  {
    return std::accumulate(args.begin(), args.end(), double(1),
			   std::multiplies<double>());
  }

double Subtractor::apply(const std::list<double>& args)
  {
    return std::accumulate(args.begin(), args.end(), (2 * (*args.begin())),
			   std::minus<double>());
  }

double Divider::apply(const std::list<double>& args)
{
  if (args.size() == 1)
    return *(args.begin());
  else if (args.size() != 2)
    throw Exception("More than two arguments supplied to division operator");
  else
    return (*(args.begin()) / *(args.begin()++));
}

PairPtr cons(const ElemPtr& car,
	     const ElemPtr& cdr)
{
  return PairPtr(new ConsPair(car, cdr));
}

ElemPtr car(const ElemPtr& e)
{
  PairPtr p = to_cons(e);
  if (!p)
    throw Exception("CAR -- Type error: Not a pair");
  else
    return p->car();
}

ElemPtr cdr(const ElemPtr& e)
{
  PairPtr p = to_cons(e);
  if (!p)
    throw Exception("CDR -- Type error: Not a pair");
  else
    return p->cdr();
}

double value(const ElemPtr& self_eval_expr)
{
  return to_number(self_eval_expr)->value();
}

bool is_self_evaluating(const ElemPtr& expr)
{
  return to_number(expr);
}

bool is_variable(const ElemPtr& expr)
{
  return to_symbol(expr);
}

bool tagged_with(const ElemPtr& expr, const std::string& tag)
{
  return (to_symbol(car(expr))->value() == tag);
}

bool is_definition(const ElemPtr& expr)
{
  return tagged_with(expr, "define");
}

double define_variable(const std::string& var, double value,
		     Environment& env)
{
  Environment::iterator it = env.find(var);
  if (it == env.end())
    {
      env[var] = value;
      return value;
    }
  else
    throw Exception("Variable already defined");
}

double set_variable(const std::string& var, double value,
		    Environment& env)
{
  Environment::iterator it = env.find(var);
  if (it == env.end())
      throw Exception("Variable not defined");
  else
    {
      env[var] = value;
      return value;
    }
}

std::string definition_variable(const ElemPtr& definition)
{
  return to_symbol(car(cdr(definition)))->value();
}

ElemPtr definition_value(const ElemPtr& definition)
{
  return car(cdr(cdr(definition)));
}

bool is_assignment(const ElemPtr& expr)
{
  return tagged_with(expr, "set!");
}

std::string assignment_variable(const ElemPtr& assignment)
{
  return to_symbol(car(cdr(assignment)))->value();
}

ElemPtr assignment_value(const ElemPtr& assignment)
{
  return car(cdr(cdr(assignment)));
}


double eval_definition(const ElemPtr& definition, Environment& var_env,
		       FuncEnv& func_env)
{
  return define_variable(definition_variable(definition),
			 eval(definition_value(definition), var_env,
			      func_env),
			 var_env);
}

double eval_assignment(const ElemPtr& assignment, Environment& var_env,
		       FuncEnv& func_env)
{
  return set_variable(assignment_variable(assignment),
		      eval(assignment_value(assignment), var_env, func_env),
		      var_env);
}

void printenv(const Environment& var_env)
{
  Environment::const_iterator it;
  for (it = var_env.begin(); it != var_env.end(); ++it)
    std::cout << it->first << " -> " << it ->second << '\n';
  std::cout << std::endl;
}

bool is_application(const ElemPtr& expr)
{
  return to_cons(expr);
}

ElemPtr oper(const ElemPtr& application)
{
  return car(application);
}

ElemPtr operands(const ElemPtr& application)
{
  return cdr(application);
}

std::list<double> list_of_values(const ElemPtr& args,
				 Environment& var_env,
				 FuncEnv& func_env)
{
  std::list<double> evaled_args;
  ElemPtr current = args;
  while(!(is_nil(to_cons(current))))
    {
      evaled_args.push_back(eval(car(current), var_env, func_env));
      current = cdr(current);
    }
  return evaled_args;
}

double apply(const FunPtr& F, const std::list<double>& args)
{
  return F->apply(args);
}

double eval(const ElemPtr& expr, Environment& var_env,
	    FuncEnv& func_env)
{
  if (is_self_evaluating(expr))
    return value(expr);
  else if (is_variable(expr))
    return lookup(expr, var_env);
  else if (is_definition(expr))
    return eval_definition(expr, var_env, func_env);
  else if (is_assignment(expr))
    return eval_assignment(expr, var_env, func_env);
  else if (is_application(expr))
    return apply(lookup(oper(expr), func_env),
		 list_of_values(operands(expr), var_env, func_env));
  else
    throw Exception("Unknown expression");
}

bool load_func_env(FuncEnv& func_env)
{
  shared_ptr<Adder> add(new Adder);
  shared_ptr<Multiplier> mult(new Multiplier);
  shared_ptr<Subtractor> sub(new Subtractor);
  shared_ptr<Divider> div(new Divider);
  
  func_env["+"] = boost::static_pointer_cast<Function>(add);
  func_env["*"] = boost::static_pointer_cast<Function>(mult);
  func_env["-"] = boost::static_pointer_cast<Function>(sub);
  func_env["/"] = boost::static_pointer_cast<Function>(div);
  
  return true;
}

