// CLIpp Header-Only Library
#ifndef CLIPP_HPP
#define CLIPP_HPP

#include <string>
#include <map>
#include <memory>
#include <vector>
#include <iostream>
#include <sstream>

namespace CLIpp
{
  using namespace std;
  
  // funcptr
  class Shell;
  typedef void (*funcPtr)(Shell * s, int argc, string args[]);

  // Shell Definitions
  class Shell
  {
  public:
    class Function;
    Shell();
    ~Shell();
    void RunCommand(string command);
    void Run();
    void SetSalutation(string salutation);
    void SetPrefix(string prefix);
    void SetFarewell(string farewell);
    void AddFunction(string name, funcPtr function, std::string description = ""); 
    void DisplayUsage() const;
    void DisplayUsage(string command) const;
    void Quit(string args[]){_quitFlag = true;};
  private:
    string _salutation = "Welcome. Enter 'q','exit',or 'quit' to quit";
    string _prefix = "> ";
    string _farewell = "Goodbye";
    bool _quitFlag = false;
    map<string,shared_ptr<Function>> _funcmap;
  };

  // Function Definitions
  class Shell::Function
  {
  public:
    Function();
    Function(Shell* parent,string name, funcPtr function);
    void SetUsageText(string text);
    string GetUsageText() const;
    void operator()(int argc, string args[]);
  private:
    string _name;
    string _usage;
    Shell* _parentShell;
    funcPtr _function;
  };

  // Shell Functions
  void Quit(Shell* s, int argc, string args[])
  {
    s->Quit(args);
  }

  void Help(Shell* s, int argc, string args[])
  {
    if (argc > 1)
    {
      s->DisplayUsage(args[1]);
    }
    else
    {
      s->DisplayUsage();
    }
  }

  // Constructor/Destructor

  Shell::Shell()
  {
    AddFunction("q",CLIpp::Quit);
    AddFunction("quit",CLIpp::Quit);
    AddFunction("exit",CLIpp::Quit);
    AddFunction("help",CLIpp::Help);
    AddFunction("?",CLIpp::Help);
  }

  Shell::~Shell()
  {

  }

  // Member Functions

  void Shell::RunCommand(string command)
  {

    vector<string> input;
    istringstream istream(command);
    string temp;

    while(std::getline(istream,temp,' '))
    {
      input.push_back(temp);
    }

    string args[input.size()];

    for (int i = 0; i < input.size(); i++)
    {
      args[i] = input.at(i);
    }

    if(_funcmap.find(args[0])!=_funcmap.end())
    {
      (*_funcmap[args[0]])(input.size(), args);
    }
    else
    {
      cout << "Command \"" << args[0] << "\" not found!" << endl;
    }
  }

  void Shell::Run()
  {
    string input;
    cout << _salutation << endl;
    do
    {
      cout << _prefix;
      getline(cin, input);
      RunCommand(input);
    } while (!_quitFlag);
    cout << _farewell << endl;
    this->~Shell();
  }

  void Shell::AddFunction(string name, funcPtr function, string description)
  {
    std::shared_ptr<Function> func(new Function(this,name,function));
    func->SetUsageText(description);
    _funcmap[name] = func;
  }

  void Shell::SetSalutation(string salutation)
  {
    _salutation = salutation;
  }

  void Shell::SetPrefix(string prefix)
  {
    _prefix = prefix;
  }

  void Shell::SetFarewell(string farewell)
  {
    _farewell = farewell;
  }

  void Shell::DisplayUsage() const
  {
    for (auto it = _funcmap.begin(); it != _funcmap.end(); it++)
    {
      if (it->second->GetUsageText() != "")
      {
        cout << it->first << ": " << it->second->GetUsageText() << endl;
      }
    }
  }

  void Shell::DisplayUsage(string command) const
  {
    if(_funcmap.find(command)!=_funcmap.end())
    {
      if (_funcmap.at(command)->GetUsageText() != "")
      {
        cout << command << ": " <<  _funcmap.at(command)->GetUsageText() << endl;
      }
      else
      {
        cout << "No help found for command \"" << command << "\"" << endl;
      }
      
    }
    else
    {
      cout << "Command \"" << command << "\" not found!" << endl;
    }
  }

  // Function class functions
  Shell::Function::Function(Shell* parent,string name, funcPtr function)
  {
    _parentShell = parent;
    _name = name;
    _function = function;
  }
  void Shell::Function::operator()(int argc, string args[])
  {
    (_function)(_parentShell,argc,args);
  }
  void Shell::Function::SetUsageText(string text)
  {
    _usage = text;
  }
  std::string Shell::Function::GetUsageText() const
  {
    return _usage;
  }

} // CLIpp

#endif