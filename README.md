json-rpc-util
=============

A collection of JSON-RPC utilities designed explore and debug the JSON-RPC
interface found in VTSS SW products.

vson
----

The `vson` script is a simple script which allow the call JSON-RPC commands on a
Vitesse JSON-RPC enabled target and (tries to ) present the result in a human
readable way.

This script uses the VTSS `json_spec` to complete its job. The VTSS `json_spec`
is a JSON description of the JSON-RPC methods available on the target along with
documentation on the arguments and results associated to a method. The VTSS
`json_spec` is downloaded directly from the target and it is documented in
the document VTSS-AN1126.

Examples of usage:

    $ ./vson --help
    Usage: vson [common-options] <command> [<args>]
    
    Common-options:
        -d, --dut ADDRESS                Hostname or IP address of the DUT
        -u, --dut-username USER          Username for the HTTP session
        -p, --dut-password PASS          Password for the HTTP session
            --spec-cache=[SPEC-FILE]     Use a cached spec file. If no file is
                                         specified it will try use
                                         ~/.vtss-json-rpc.spec
        -c                               Use ~/.vtss-json-rpc.spec as cached
                                         spec file
        -h, --help                       Show this message
    
    Commands:
      call         Call a json RPC function on target using the spec file.
      update-spec  Download the newest spec file from the target, and save it
                   as a cache.
      spec         Inspect the spec file.
      grep         List type and methods who's name matches the pattern.
    
    $ ./vson call --help
    Usage: vson call [Options] <method> [<args>]
    
    Options:
            --dump-requests              Dump the raw requests
            --dump-response              Dump the raw response
            --no-pretty-print            Disable pretty printing the responses
        -h, --help                       Show this message
    
    Method: this is the json-rpc method to call.
    
    $ ./vson -c -d 10.99.10.2 update-spec
    Downloading spec file
    
    $ ./vson -c grep port.status
    port.status.veriphy.result.get
    port.status.veriphy.result.get
    port.status.interface.get
    port.status.interface.get
    port.status.get
    port.status.get
    synce.status.ports.port-status.get
    synce.status.ports.port-status.get
    
    $ ./vson -c spec port.status.get
    Method name:
      port.status.get
    Parameters:
      Param #0 {"name"=>"ifindex", "type"=>"vtss_ifindex_t"}
               {"type-name"=>"vtss_ifindex_t", "class"=>"TypeSpecification", "description"=>"Interface identification", "json-encoding-type"=>"String"}
    
    Results:
      Result #0 {"name"=>"status", "type"=>"vtss_appl_port_mib_status_t"}
                {"type-name"=>"vtss_appl_port_mib_status_t", "class"=>"Struct", "description"=>"", "encoding-type"=>"Object", "elements"=>[{"name"=>"Link", "type"=>"vtss_bool_t", "description"=>"Shows whether interface has link."}, {"name"=>"Fdx", "type"=>"vtss_bool_t", "description"=>"Shows whether interface is running in full duplex."}, {"name"=>"Fiber", "type"=>"vtss_bool_t", "description"=>"Shows whether interface is a fiber link (fiber SFP or cu SFP)."}, {"name"=>"Speed", "type"=>"vtss_port_speed_t", "description"=>"Shows the current interface speed."}, {"name"=>"SFPType", "type"=>"vtss_appl_sfp_tranceiver_t", "description"=>"Shows the current interface SFP type."}, {"name"=>"SFPVendorName", "type"=>"char *", "description"=>"Shows the SFP vendor name."}, {"name"=>"SFPVendorPN", "type"=>"char *", "description"=>"Shows the SFP vendor Product Number."}, {"name"=>"SFPVendorRev", "type"=>"char *", "description"=>"Shows the SFP vendor Revision."}, {"name"=>"LossOfSignal", "type"=>"vtss_bool_t", "description"=>"SFP Loss Of Signal."}, {"name"=>"TxFault", "type"=>"vtss_bool_t", "description"=>"SFP Transmit Fault."}, {"name"=>"Present", "type"=>"vtss_bool_t", "description"=>"SFP module present."}]}
    
    $ ./vson -c -d 10.99.10.2 call port.status.get
    Calling port.status.get:
        key      Link  Fdx   Fiber Speed     SFPType SFPVendorName SFPVendorPN SFPVendorRev LossOfSignal TxFault Present
        -------- ----- ----- ----- --------- ------- ------------- ----------- ------------ ------------ ------- -------
          Gi 1/1  true  true false   speed1G    none                                                true    true   false
          Gi 1/2  true  true false   speed1G    none                                                true    true   false
          Gi 1/3  true  true false   speed1G    none                                                true    true   false
          Gi 1/4  true  true false   speed1G    none                                                true    true   false
          Gi 1/5 false false  true undefined    none                                                true    true   false
          Gi 1/6 false false  true undefined    none                                                true    true   false
          Gi 1/7 false false  true undefined    none                                                true    true   false
          Gi 1/8 false false  true undefined    none                                                true    true   false
        2.5G 1/1 false false  true undefined    none                                                true    true   false
        2.5G 1/2 false false  true undefined    none                                                true    true   false
          Gi 1/9 false false false undefined    none                                                true    true   false
    
    $ ./vson -c -d 10.99.10.2 call port.status.get "Gi 1/1"
    Calling port.status.get:
        Link:             true
        Fdx:              true
        Fiber:           false
        Speed:         speed1G
        SFPType:          none
        SFPVendorName:
        SFPVendorPN:
        SFPVendorRev:
        LossOfSignal:     true
        TxFault:          true
        Present:         false


listen
------

The `listen` application implements a simple web server which are listing on a
port. This web-server will print all JSON-RPC notifications which are received
in a http-post message.

When used along with a VTSS target which has JSON-RPC notification support, this
application can be used to print/inspect/debug the notifications.

    $ ./listen --help
    Usage: listen -p <port>
        -p, --port PORT                  TCP port to listen on
        -h, --help                       Show this message


In the following iCli session I configure the target to post notification on
port events to the web-server at 10.99.10.1:5000:

    # configure terminal
    (config)# json notification host h1
    (config-json-noti-host)# url http://10.99.10.1:5000
    (config-json-noti-host)# exit
    (config)# json notification listen port.status.interface.update h1
    (config)# end
    #

At 10.99.10.1, this web server can now be started using the `listen`
application, and it will print notifications when they are received.

    $ ./listen -p 5000
    10.99.10.2/:         port.status.interface.update             modify     Gi 1/4 -> {"Link"=>false, "Fdx"=>true, "Fiber"=>false, "Speed"=>"speed1G", "SFPType"=>"none", "SFPVendorName"=>"", "SFPVendorPN"=>"", "SFPVendorRev"=>"", "LossOfSignal"=>true, "TxFault"=>true, "Present"=>false}
    10.99.10.2/:         port.status.interface.update             modify     Gi 1/3 -> {"Link"=>false, "Fdx"=>true, "Fiber"=>false, "Speed"=>"speed1G", "SFPType"=>"none", "SFPVendorName"=>"", "SFPVendorPN"=>"", "SFPVendorRev"=>"", "LossOfSignal"=>true, "TxFault"=>true, "Present"=>false}
    10.99.10.2/:         port.status.interface.update             modify     Gi 1/4 -> {"Link"=>true, "Fdx"=>true, "Fiber"=>false, "Speed"=>"speed1G", "SFPType"=>"none", "SFPVendorName"=>"", "SFPVendorPN"=>"", "SFPVendorRev"=>"", "LossOfSignal"=>true, "TxFault"=>true, "Present"=>false}
    10.99.10.2/:         port.status.interface.update             modify     Gi 1/3 -> {"Link"=>true, "Fdx"=>true, "Fiber"=>false, "Speed"=>"speed1G", "SFPType"=>"none", "SFPVendorName"=>"", "SFPVendorPN"=>"", "SFPVendorRev"=>"", "LossOfSignal"=>true, "TxFault"=>true, "Present"=>false}


