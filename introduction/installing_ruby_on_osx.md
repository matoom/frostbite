# Installing Ruby on Mac OS X

For OS X it is important to upgrade Ruby installation if the version is older than 1.9.3.
To find out which version of ruby is installed run the following command in terminal:

```shell
$ ruby -version
```

If it's necessary to upgrade Ruby then execute the following steps:

Register new ruby installation in the Frostbite frontend.

```shell
$ which ruby
/Users/<*user*>/.rvm/rubies/ruby-1.9.3-p545/bin/ruby
```

Configure Ruby interpreter system path in the client settings.
1. go to script -> script settings
2. script interpreter -> /Users/<*user*>/.rvm/rubies/ruby-1.9.3-p545/bin/ruby
