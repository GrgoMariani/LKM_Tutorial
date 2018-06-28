## 0: Preparation

Check your current linux kernel release with ___uname -r___ command. It should output something similar to this.
```
 $ uname -r


 3.13.0-24-generic
```

Install the required headers
```
$ sudo apt-get update
$ sudo apt-get install linux-headers-$(uname .r)
```

You should be all set now.


#### What does all of this do ???
What we have done now is installed required source code  with which we will be able to compile our kernel modules code.

| Previous | Next |
| --- | --- |
| [Home](../README.md) | [1. Our First Module](../1_Our_First_Module/README.md) |
