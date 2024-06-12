## Scripting

> The Frostbite client scripting support is provided by the [Ruby](http://www.ruby-lang.org/en/)
> scripting engine and will therefore require the [Ruby](http://www.ruby-lang.org/en/) runtime
> libraries to be [installed](/installation).

Ruby scripting engine adds a flexible and powerful scripting support for the game client. It is easy to use
for beginners but still offers more complex scripting possibilities for advanced users.

### High level description

The scripting API is entirely written in Ruby and is located in the *"scripts/lib"* folder. A script is executed
as a sub process of the frontend by calling the *"main.rb"* file in Ruby. All real time data for the Ruby process
is provided over standard output (pipes) and on-demand data exchange is conducted over a custom TCP client->server solution.

The API Server opens a localhost socket on a random available port and stores it in the *"api.ini"* settings file.
Using these stored settings the client is able to open up a socket connection to start a data sharing session which is
based on a simple custom protocol. i.e "GET RT -> 0"

The script flow from the frontend is as following:

For example: *game client -> main.rb "braid.rb vines" -> runs -> braid.rb vines*.

The scripting API includes:

* main.rb - general functions used by scripts;
* text.rb - functions for text matching;
* models.rb - wraps and handles API protocol queries;
* observer.rb - observer module, reacts to text events;
* roby_goto.rb - enables label based scripting.

The scripting API functions are described in more detail in the [API](/scripting/api.) section.
