## Getting and Installing Lich

Please follow the lich instructions here:
[https://github.com/rpherbig/dr-scripts/wiki/First-Time-Setup](https://github.com/rpherbig/dr-scripts/wiki/First-Time-Setup)

### Windows

Open up an administrator command prompt (win+x then A in windows 8+) navigate to
your lich directory and in the command prompt run.

```shell
$ ruby lich.rbw --dragonrealms --frostbite
```

Then connect to a character via Frostbite like normally.

### Mac OS X / \*nix

Open up command prompt Navigate to your lich directory:

For example,

```shell
$ cd ~/Downloads/lich
```

Run Lich.rbw with the following options

```shell
sudo ./lich.rbw --dragonrealms --frostbite
```

Then connect to a character via Frostbite like normally.

### Note for RVM users

If you are using rvm, you will want to use rvmsudo to do the sudo call (else you will use whatever ruby version sudo defaults to which will lead to Problems). Setting up rvmsudo is detailed here: <a href="https://rvm.io/integration/sudo"> https://rvm.io/integration/sudo</a>. The steps are as follows:

1. Add `export rvmsudo_secure_path=0 to ~/.bashrc`
2. Add Defaults /etc/sudoers (carefull...)
```shell
env_keep +="rvm_bin_path GEM_HOME IRBRC MY_RUBY_HOME \
rvm_path rvm_prefix rvm_version GEM_PATH rvmsudo_secure_path \
RUBY_VERSION rvm_ruby_string rvm_delete_flag"
```
3. Comment out *Defaults secure_path=...* in /etc/sudoers</li>

Now run `rvmsudo ./lich.rbw --dragonrealms --frostbite`
