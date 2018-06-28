## 2: Devices
How do we access this module we've created?

#### New Files
Be sure to check the following files in this directory:
* [module_two.c](module_two.c)

##### ___module_two.c___
We create a new module that behaves in the following way:
* __init__: our module registers a _major number_, registers the device class with that major number and creates the device with the major number
* __exit__: unregister device and device class
* __dev_open__: print a short message
* __dev_release__: print a short message
* __dev_write__: save a _message_ from input
* __dev_read__: copy a _message_ from dev_write to user space

- - - -

#### What does all of this do ???
Open another terminal window and type:
```
 $ tailf /var/log/kern.log
```
Return to our main terminal window, type the following lines and keep your eye on log window:
```
 $ make
 $ sudo insmod module_two.ko
 $ ls dev
 $ ls -l /dev | grep OURDEVICE
 $ cat /proc/devices

 $ cat /dev/OURDEVICE
 $ echo "13 Characters" | sudo tee -a /dev/OURDEVICE
 $ cat /dev/OURDEVICE

 $ ls /sys/class/
 $ ls /sys/class/DEVICECLASS
 $ sudo rmmod module_two
```
I guess this is a lot to take in one example. Walk through the commands line by line and try to figure what they represent (or simply google it) .




#### Be sure to notice ...
We don't use dynamic memory allocation. This can be improved with kmalloc.
What do we do when two files try to access the device at the same time?
Can you think how we can create two devices of the same name and why would we ever do that?





| Previous | Next |
| --- | --- |
| [1. Our First Module](../1_Our_First_Module/README.md) | [3. Hardware](../3_Hardware/README.md) |