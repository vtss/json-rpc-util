Client library proof of concept
===============================

The code in this directory should be considered as example code to explain how a
client library can be auto-generated.

It has not been tested, the implementation is not optimized, and it should not
be used in a production environment.

This code will only be useful if combined with a json-spec generated on a VTSS
Switch. The purpose is to read the json-spec and then auto generate a C++ client
library that will expose all the specified RPC functions as normal C++
functions.


How to get started
------------------

In order to compile the code you need 'libcurl-dev'. Install by:

    $ sudo apt-get install libcurl-dev

Start by cloning the project, notice that this sub-project depends on some
external projects, and it is therefor required to fetch the submodules as well

    $ git clone https://github.com/vtss/json-rpc-util.git
    $ cd json-rpc-util/client-library-proof-of-concept
    $ git submodule init
    $ git submodule update

Build the project

    $ mkdir build
    $ cd build
    $ cmake ..
    $ make -j8

Try run it

    $ ./example-raw-json http://10.99.10.2/json_rpc
    $ ./example-vtss-client-lib http://10.99.10.2/json_rpc


What is going on here?
----------------------

`vtss-json-spec-autogen` is a ruby script which reads the
`vtss-json-rpc-example.spec` file and generate two files: `vtss-client.hxx` and
`vtss-client.cxx`. These files are compiled into a shared library, which the
`example-vtss-client-lib` executable is using.




