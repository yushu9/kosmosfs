## Required Packages ##

To compile and run KFS, you need to have the following software packages installed on your machine:
  * Boost (preferably, version 1.34 or higher)
  * cmake (preferably, version 2.4.6 or higher)
  * log4cpp (preferably, version 1.0)
  * gcc version 4.1 (or higher)
  * xfs devel RPMs on Linux
This document assumes that you have downloaded the source to directory: ~/code/kfs. We assume that you want to build the source in ~/code/kfs/build.

There are few parts to compiling the source:
  1. Build the C++ side to get the metaserver/chunkserver binaries, tools, and the C++ client library.
  1. Build the Java side to get a kfs.jar file which contains the wrapper calls to native C++ via JNI; this allows Java apps to access files stored in KFS.
  1. Building Python extension module for Python support ''(optional)''

### Building C++ Components ###

It is preferable to build KFS binaries with debugging symbols. The sequence of steps are:
```
cd ~/code/kfs
mkdir build
cd build
cmake -D CMAKE_BUILD_TYPE=RelWithDebInfo ~/code/kfs/
gmake
gmake install
```
The binaries will be installed in:
  1. Executables will be in: ~/code/kfs/build/bin
  1. Libraries will be in: ~/code/kfs/build/lib

### Building Java Components ###

To build Java support setup:
```
cd ~/code/kfs
ant jar
```
This will produce the following set of files:
  * ~/code/kfs/build/classes --- This will contain the Java class files
  * ~/code/kfs/build/kfs-{version}.jar --- The jar file containing the Java classes

Add .jar file to your CLASSPATH environment variable
```
export CLASSPATH=${CLASSPATH}:~/code/kfs/build/kfs-[version].jar
```

### Building Python Support ###

Build the KFS client library (see above). Let the path to the shared libraries is ~/code/kfs/build/lib. Now, to build the Python extension module:
```
cd to ~/code/kfs/src/cc/access
Edit kfs_setup.py and setup the include path. Specifically,
       kfsext = Extension('kfs', include_dirs ['kfs/src/cc/', '<path to boost>'])
python kfs_setup.py ~/code/kfs/build/lib/ build
```
This will build a shared object library _kfs.so_. The kfs.so library needs to be installed either in the site-packages for python or in an alternate location. To install in site-packages for python:
```
python kfs_setup.py ~/code/kfs/build/lib/ install
```
To install in alternate location such as, ~/code/kfs/build/lib
```
python kfs_setup.py ~/code/kfs/build/lib install --home=~/code/kfs/build/lib
```
If you installed in alternate location, update PYTHONPATH environment variables:
```
export PYTHONPATH=${PYTHONPATH}:~/code/kfs/build/lib/lib64/python
```
Also, update the LD\_LIBRARY\_PATH environment variable:
```
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:~/code/kfs/build/lib
```

### Using Python Client on the Mac (Leopard) ###

For the Mac, update the DYLD\_LIBRARY\_PATH environment variable (so that libkfsClient.dylib will be found):
```
export DYLD_LIBRARY_PATH=${DYLD_LIBRARY_PATH}:~/code/kfs/build/lib
```