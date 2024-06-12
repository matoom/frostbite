## Basics

The Frostbite client scripting is based on the Ruby scripting language.
Ruby syntax is easy and intuitive to use, it supports multi-paradigm programming
and its functions are very powerful. All of which incorporates
into a very flexible scripting environment.

The descriptions for Ruby functions are available in the
[Ruby documentation](http://ruby-doc.org/core-1.9.3/).


On top of Ruby tools, the Frostbite frontend offers it's own set of functions
which are more specific to the game itself.
The API description is available in the [API](/scripting/api) section.

### Creating and running scripts

Ruby script files are required to be created as *".rb"* extension files and
the script files need to be placed into the client *"scripts"* folder.

The scripts can be executed in the client command line by calling the file name
with a *"."* prefix.
For example, running braid.rb: *".braid vine"*.

### Stopping scripts

Once the script is running it can be stopped by pressing the ESC key on the keyboard.
In the event of any crashes which render the script unable to exit in a normal way,
the ESC key has to be pressed once more to terminate the script forcefully.

In case of system-wide crashes terminate the ruby executable in the process list.
For Windows look for ruby.exe in the task manager, for Linux or Max OS X,
Ruby process can be killed on the command line.

### Reserved parameter descriptions

```$args # array, global variable```

Contains the arguments passed on to the script in the command line.

```$rt_adjust = 0 # number, global scope variable```

Enables to regulate the wait times and make the script run faster by tightening the value.

```:match_start and :match_end```

These are the two reserved match pattern names which can be used to determine
additional match start and end conditions.

For example:

```ruby
{
    :wait => [/\.\.\.wait|you may only type ahead/],
    :lodged => [/lodged shallowly into/],
    :mites => [/red blood mite/],
    # start matching
    :match_start => [/Your body feels/],
    # match end conditions (will match everything in between)
    :match_end => [/>|no significant injuries/]
}
```

### Script structures and blocks

All the Frostbite API driven scripts can be directed to *"finally_do"* method at script exit.
This includes either aborting the script or exiting by natural means.
The final block can be avoided by calling `Kernel::exit!` or `Kernel::abort`.
If the *"finally_do"* function is not defined in the script, no extra action will be taken
at the script exit.
Finally block is useful when you want to perform clean up tasks after running the script.

```ruby
def finally_do {
    put "put my #{$args.first} in my backpack"
}
```

There are different ways to set up the script flow in Ruby.
With some additions to the standard library
it is possible to create Wizard-like label structures.
For labels to work it is required to wrap the label code into
*"labels_start"* and *"labels_end" markers*.

```
labels_start

label(:study) {
    put "study my compendium"
    match = { :study => ["studying", "You continue to study"],
    :turn => ["makes sense to you"],
    :exit => ["study this chart again"] }
    match_wait_goto match
}

label(:turn) {
    put "turn my compendium"
    wait
    goto :study
}

labels_end
```

The match mechanics can also be executed without labels by calling the *"match_wait"*
function instead.
This way the keyword `(:symbol)` for the match will be returned by the function and
any further action depends on the decision logic that handles those responses.

```ruby
put "go rosewood arch"
match = {
    :second => ["The attendant steps in"],
    :continue => ["The Dwarven attendant escorts"]
}
result = match_wait match

case result
    when :second
    move "go ironwood arch"
end
```

For avoiding code repetitions, Ruby enables to create functions and loops
to keep the script clean from any redundant code.

```ruby
def juggle
    put "juggle my " + $args.join(" ")
    wait_for_roundtime
end

10.times do
juggle
end
```

### Observer event module

The observer module can be set up to watch for pattern matches in the game text.
It runs in parallel with the script flow and will not be disturbed by any blocking
parts of the script, other than the matching mechanics.
The observer will wait for any matching to be finished until it can execute
the callback method for the specified event or otherwise it is not possible
to use the matching mechanics in the callback method's body.
Once the observer runs into a specified event, the script flow will be
interrupted for the time it takes to execute the callback method.

To set up the observer module it is required to register at least one observer event.
The observer event contains the name of the callback method and the text match pattern
which is being tracked by the observer.
Each event has a specified callback method and it has to be defined in the script
body or otherwise the script will fail to execute.

The observer module is a singleton object and only one instance of the observer
object can be created in each script.
The singleton instance can be obtained by calling the *"instance"* method of the observer object.
The observer is a lazy initation object, meaning that it will only start running
once the *"register_event"* is called.

```ruby
Observer.instance.register_event({
    :on_roundtime => "Roundtime"
})

Observer.instance.register_event({
    :stop_observer => "were you referring"
})


# callback method for first event
def on_roundtime
    echo "Found roundtime!"
    wait_for_roundtime        # pauses for the duration of roundtime
    put "look"
end

# callback method for second event
def stop_observer
    Observer.instance.terminate # just to demonstrate how to
                                # terminate the observer
end

100.times do
    put "collect " + $args.join(" ")
    wait_for_roundtime
    put "kick pile"
end
```

### Functions and models

The standard API functions have mostly been set up to provide Wizard-like scripting style.
Different kind of approaches might need changes or additions to the core API functionality.
The full description of the Frostbite core API functions and models can be found from the
<a href="api.html">API</a> section.

### Errors in scripting

Error messages coming from scripts are displayed in the Frostbite client main window.
Scripts containing any errors in syntax or structure can not be used until they are fixed.
The error messages, in most cases, point to the errors by the line number in code to
make them easier to find.

For example, when calling a function which expects a string type but has not been
denoted as such by using quotes.
In Ruby, variables can be created using just alphanumeric characters and underscore.
In this case the word - hello - will be handled as a variable which has no value
assigned, therefore causing an error.

```ruby
#script
echo hello

#=>

#error
C:/FrostBite/scripts/hello.rb:1:in `': undefined local variable or method `hello' for main:Object (NameError)
```

Error on line one, undefined local variable. To fix this particular error, the word hello has to be
enclosed in quotes -- "hello".

### Regular expressions

The match functions in scripts are based on regular expressions.
To use any of the regular expression characters as a literal in a match pattern,
they have to be escaped by a backslash.

```ruby
#escaped to match dot characters instead of any single character
:wait => [/\.\.\.wait/, "only one command"]

#case insensitive, bracets are escaped to match bracet characters
:canyon => [/\[North Road, Canyon\]/i]

#not containing any regular expression special characters
:exit => ["study this chart again", "need to be holding"]
```
