# CLIpp: A Simple C++ CLI

CLIpp is a header-only library for creating simple command-line shell interfaces, useful for simple applications and testing.

All you need to do to get started is download and include the `clipp.hpp` header, instantiate a `Shell` object, and call `Run()` on it!

```c++
#include "clipp.hpp"

int main()
{
  using namespace CLIpp;
  auto s = Shell();
  
  // Add custom functions here with
  // s.AddFunction();

  s.Run();
}
```

## Built-In Shell Functions

A Shell has a handful of built-in functions as soon as it is instantiated:

* `exit`, `q`, `quit`: Exits the shell
* `help`, `?`: Displays help text for functions that have it defined.

## Adding Custom Functions

Of course, a Shell is only as useful as the functions it can execute! You can use `AddFunction()` to add any function to the shell, so long as the function has the following method signature:
```c++
void ShellFunction(Shell* s, int argc, string args[])
```
Once you've defined the function you wish to add to the Shell, add it by calling `AddFunction()`, which takes two paremeters, plus a third optional parameter:
```c++
AddFunction(string name, funcdel function, string helptext="")
```

* `name`: The name of the function, which is what the user will type into the shell to call it.
* `function`: The name of the function itself, as you have defined it (do not include the parentheses)
* `helptext`: The text that will display for this function when the `help` command is used. Optional.

Here's a quick `Hello World` Example:
```c++
#include "clipp.hpp"
#include <iostream>

void HelloWorld(Shell* s, int argc, std::string args[])
{
  std::cout << "Hello World!" << std::endl;
}

int main()
{
  using namespace CLIpp;
  auto s = Shell();

  s.AddFunction("helloworld",HelloWorld);

  s.Run();
}
```
Running this program and typing `helloworld` into the shell would print `Hello World!` to the console.

## Customizing the Shell
Several aspects of the shell's appearance can be customized:

* `SetSalutation()`: Sets the greeting text when the shell is opened.
* `SetFarewell()`: Sets the farewell text when the shell is closed with `exit`, `q`, or `quit`.
* `SetPrefix()`: Sets the prompt prefix string that appears before the user's input text.
