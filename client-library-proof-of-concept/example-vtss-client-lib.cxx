#include <iostream>
#include "vtss-client.hxx"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Usage: example-raw-json http://<ip-address>/json_rpc"
                  << std::endl;
        return -1;
    }
    uri_json_rpc = std::string(argv[1]);

    // Print status for all ports
    std::map<std::string, vtss_appl_port_mib_status_t> all_ports;
    vtss_port_status_get(&all_ports);
    for (const auto &p : all_ports)
        std::cout << p.first << " is " << (p.second.link ? "up" : "down") << std::endl;

    // Access a single port
    vtss_appl_port_mib_status_t s;
    vtss_port_status_get("Gi 1/1", &s);

    return 0;
}

