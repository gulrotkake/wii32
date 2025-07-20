#pragma once

#include <memory>

#include "bluetooth.h"

namespace wiipp {

class Esp32Bluetooth : public Bluetooth {
    struct Impl;
    std::unique_ptr<Impl> m_impl;

public:
    Esp32Bluetooth();
    ~Esp32Bluetooth();

    // Device
    std::span<uint8_t, 6> macAddress() override;

    void onReady(const std::function<void(Bluetooth*)>&) override;
    void process() override;

    // HCI
    void onHCIEvent(const std::function<void(Bluetooth*, const HCIEvent&)>&) override;
    void onHCIConnectionRequest(const std::function<bool(Bluetooth*, const HCIConnectionRequest&)>& listener) override;

    void scan() override;
    void requestRemoteName(const HCIInquiryResult& result) override;
    void connect(const HCIInquiryResult& result) override;
    void auth(uint16_t handle) override;
    void negativeReply(uint64_t bdaddr) override;
    void disconnect(uint16_t handle) override;
    void sendPinReply(uint64_t bdaddr, uint8_t* pinData, size_t len);

    // ACL
    void onACLEvent(const std::function<void(Bluetooth*, const ACLEvent&)>& acl) override;
    void onACLConnectionRequest(const std::function<bool(Bluetooth*, const ACLConnectionRequest&)>& listener) override;
    void l2cap_connect(uint16_t handle, uint16_t psm, uint16_t mtu) override;
    void l2cap_disconnect(uint16_t handle, uint16_t psm) override;
    void l2send_data(uint16_t handle, uint16_t psm, uint8_t* data, size_t len) override;
};

}  // namespace wiipp
