# Installing Ruby on Mac OS X

Mac OS X already comes with a Ruby installation prepackaged.
For OS X it is important to upgrade Ruby installation if the version is older than 1.9.3.
To find out which version of ruby is installed run the following command in terminal:

```shell
$ ruby -version
```

If it was necessary to upgrade Ruby then execute the following steps:

Register new ruby installation in the Frostbite frontend.

```shell
$ which ruby
/Users/<*user*>/.rvm/rubies/ruby-1.9.3-p545/bin/ruby
```

Navigate to the location where the application was extracted during
installation.

1. `$ cd FrostBite.app/Contents/MacOS`
2. open/create file called client.ini
3. copy ruby path to client.ini: path=/Users/<*user*>/.rvm/rubies/ruby-1.9.3-p545/bin/ruby
