#include <iostream>
#include "json/reader.h"
#include "json/writer.h"
#include "restclient-cpp/restclient.h"

std::string uri_json_rpc;
Json::Value vtss_json_call(const std::string &method,
                           const std::vector<std::string> &args = {}) {
    RestClient::setAuth("admin", "");

    Json::Value msg(Json::objectValue);
    msg["method"] = method;
    msg["id"] = "jsonrpc";
    msg["params"] = Json::Value(Json::arrayValue);

    for (const auto &e : args) msg["params"].append(e);

    auto res =
            RestClient::post(uri_json_rpc, "text/json", msg.toStyledString());

    if (res.code < 200 || res.code > 299)
        throw std::runtime_error("Network error");

    Json::Reader reader;
    Json::Value root;

    if (!reader.parse(res.body, root, false))
        throw std::runtime_error("Failed to parse!");
    if (!root.isObject()) throw std::runtime_error("Not a object");
    if (!root.isMember("error")) throw std::runtime_error("No error member");
    if (!root["error"].isNull()) throw std::runtime_error("Error in response");
    if (!root.isMember("result")) throw std::runtime_error("No result member");

    return root["result"];
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Usage: example-raw-json http://<ip-address>/json_rpc"
                  << std::endl;
        return -1;
    }
    uri_json_rpc = std::string(argv[1]);

    // Print status for all ports
    auto all_ports = vtss_json_call("port.status.get");
    std::cout << all_ports << std::endl;

    // Print status for port Gi 1/1
    std::cout << vtss_json_call("port.status.get", {"Gi 1/1"}) << std::endl;

    // Print name of all ports which is up
    for (const auto &p : all_ports)
        if (p["val"]["Link"].asBool()) std::cout << p["key"] << std::endl;

    return 0;
}

