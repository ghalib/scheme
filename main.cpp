#include "scheme.h"

// Our Read-Eval-Print Loop
void repl()
{
  Environment var_env;
  FuncEnv func_env;

  load_func_env(func_env);  

  std::string input;

  while(1)
    {
      std::cout << "> ";
      std::getline(std::cin, input);

      if (input == "quit")
	break;
      
      try
	{
	  Parser parser(input);
	  
	  ElemPtr expr = parser.parsed_expr();

  	  std::cout << eval(expr, var_env, func_env) << std::endl;
	}
      catch(Exception& e)
	{
	  std::cout << e.what() << std::endl;
	}
    }  
}

void usage()
{
  std::cout << "Arithmetic Scheme interpreter. Available operations: define set! + - / *"  << "\n\n"
	    << "Example usage:\n"
	    << ">(define x 2)\n"
	    << "2\n"
	    << ">(define y 5)\n"
	    << "5\n"
	    << ">(* x (* y (+ 5 x)))\n"
	    << "70\n"
	    << ">(set! x 5)\n"
	    << "5\n"
	    << ">(+ x y)\n"
	    << "10\n\n"
	    << "Type 'quit' to exit.\n"
	    << std::endl;
}

int main()
{
  usage();
  repl();
}
