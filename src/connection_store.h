#include <vector>
#include <algorithm>

namespace wiipp {

struct L2CapConnection {
    uint16_t localCid;
    uint16_t psm;
    uint16_t remoteCid;
    uint16_t mtu;

    bool localConfigured;
    bool remoteConfigured;
};

class ConnectionStore {
    std::vector<L2CapConnection> l2CapConnections;

public:
    ConnectionStore() {}
    ConnectionStore(const ConnectionStore&) = delete;
    ConnectionStore& operator=(const ConnectionStore&) = delete;

    L2CapConnection* findLocal(uint16_t handle, uint16_t localCid) {
        auto itr = std::find_if(l2CapConnections.begin(), l2CapConnections.end(), [handle, localCid](const L2CapConnection& connection) {
            return handle == handle && connection.localCid == localCid;
        });
        return &*itr;
    }

    L2CapConnection* findPsm(uint16_t handle, uint16_t psm) {
        auto itr = std::find_if(l2CapConnections.begin(), l2CapConnections.end(), [handle, psm](const L2CapConnection& connection) {
            return handle == handle && connection.psm == psm;
        });
        return &*itr;
    }

    bool remove(L2CapConnection& connection) {
        auto itr = std::remove_if(l2CapConnections.begin(), l2CapConnections.end(), [&connection](const L2CapConnection& e) {
            return &e == &connection;
        });
        if (itr == l2CapConnections.end()) {
            return false;
        }
        l2CapConnections.erase(itr, l2CapConnections.end());
        return true;
    }

    void emplace(L2CapConnection connection) {
        l2CapConnections.emplace_back(std::move(connection));
    }
};

}
