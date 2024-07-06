## Scripting

> The Frostbite client scripting support is provided by the [Ruby](http://www.ruby-lang.org/en/)
> scripting engine and will therefore require the [Ruby](http://www.ruby-lang.org/en/) runtime
> libraries to be [installed](introduction/installation).

Ruby scripting engine provides a flexible and powerful scripting support. It is easy to use
for beginners while offering countless advanced features to tackle more complex tasks when needed.

### High level description

The scripting API is entirely written in Ruby, the code is located and executed in the *"scripts/lib"* folder. Scripts 
are run by calling the *"main.rb"* Ruby wrapper in the background. All real time data for 
the Ruby process is provided over standard output (pipes). On-demand data can be exchanged using a custom 
TCP client->server solution.

The API Server opens a localhost socket on a random available port and stores the connection info into *"api.ini"* settings file.
Using these stored settings the client is able to open up a socket connection to start a data sharing session which is
based on a simple plain-text protocol. i.e "GET RT -> 0"

The script execution flow from the frontend is as following:

For example: *command line -> .braid vines -> main.rb "braid.rb vines" -> runs -> braid.rb vines*.

The scripting API includes:

* main.rb - general functions used by scripts;
* text.rb - functions for text matching;
* models.rb - wraps and handles API protocol queries;
* observer.rb - observer module, reacts to text events;
* roby_goto.rb - enables label based scripting.

The scripting API functions are described in more detail in the [API](scripting/api) section.