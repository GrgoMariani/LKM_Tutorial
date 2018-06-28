## 1: Our First Module
Let's create our first module and see how to use it.
#### New Files
Be sure to check the following files in this directory:
* [Makefile](Makefile)
* [module_one.c](module_one.c)

##### ___Makefile___
Everything should be clear here. We are actually calling makefile from our _linux headers_ directory.

##### ___module_one.c___
We've got a couple of things going on here. (_check .c code for ordering_)

* imports - most of the headers have ___macro___ definitions and ___enum___ constants in them
* Some module information we'll check later with ___modinfo___ command
* __name__ and __somenumber__ will be our ___params___ (argument variables)
* Two functions are defined which we'll register with _module_init_ and _module_exit_

- - - -

#### What does all of this do ???
Compile the module using
```
 $ sudo make
```
list everything in directory with __ls__ and you should see new files created. ' ___.ko___ ' file is what we are interested in.
```
 $ modinfo ./module_one.ko
```
You should see the information we've used in MODULE macros in our _.c_ file.

Let's do a brief walkthrough of what we did. Use ___lsmod___ to list all currently loaded modules:
```
 $ lsmod
```
Let's insert our module using ___insmod___:
```
 $ sudo insmod module_one.ko
```
If we use ___lsmod___ now we should see _module_one_ on the list.
```
 $ lsmod | grep module_one
```
Open another terminal and __cd__ to '_/sys/module_' directory
```
 $ cd /sys/module
 $ ls
```
You should see ___module_one___ directory. Let's enter it and list the files and it's contents:
```
 $ cd module_one
 $ ls
 $ cat version
```
You can also find some information in '_/proc/modules_' file.
```
 $ cat /proc/modules
```
Return to our main terminal and remove module.
```
 $ sudo rmmod module_one
```
- - - -
Try repeating all we did but using module params this time:
```
 $ sudo insmod module_one.ko name="SOME NAME" somenumber=1230
 $ sudo rmmod module_one
```
Once you've done that check '_/var/log/kern.log_' for log information.
```
 $ nano /var/log/kern.log
```
What about rebooting? Does our module stay loaded? __NO IT DOES NOT__.
```
mkdir /lib/modules/$(uname -r)/custom_modules
sudo cp module_one.ko /lib/modules/$(uname -r)/custom_modules
echo "module_one" | sudo tee -a /etc/modules
sudo depmod -a
sudo modprobe module_one
```
What about now? Try finding out what we did line by line.

#### Be sure to notice ...

We are using ___static___ keywords on variables and functions. This is needed to restrict the scope to within the module.

All those macros are there so you can use them.

| Previous | Next |
| --- | --- |
| [0. Preparation](../0_Preparation/README.md) | [2. Devices](../2_Devices/README.md) |
