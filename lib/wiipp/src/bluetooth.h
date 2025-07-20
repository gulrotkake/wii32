#pragma once

#include <cstdint>
#include <functional>
#include <string_view>
#include <variant>
#include <span>

namespace wiipp {

struct HCIInquiryResult {
    uint64_t bdaddr;
    uint8_t psrm;
    uint32_t classOfDevice;
    uint16_t clkOffset;
};

struct HCIInquiryComplete {
};

struct HCIConnectionEstablished {
    uint64_t bdaddr;
    uint16_t handle;
    bool accepted;
};

struct HCIConnectionFailed {
    uint64_t bdaddr;
    uint16_t handle;
    uint8_t reason;
    bool accepted;
};

struct HCIDisconnected {
    uint16_t handle;
    uint8_t reason;
};

struct HCIRemoteName {
    HCIInquiryResult inquiry;
    std::string_view remoteName;
};

struct HCIConnectionRequest {
    uint64_t bdaddr;
    uint32_t classOfDevice;
};

struct HCILinkKeyRequest {
    uint64_t bdaddr;
    uint8_t keyType;
    uint8_t *linkKeyData;
    size_t size;
};

struct HCIPINRequest {
    uint64_t bdaddr;
};

struct ACLConnectionRequest {
    uint16_t handle;
    uint16_t sourceCid;
    uint16_t psm;
};

struct ACLConnectionFailed {
    uint16_t handle;
    uint16_t sourceCid;
    uint16_t psm;
};

struct ACLDisconnected {
    uint16_t handle;
    uint16_t psm;
};

struct ACLConnectionEstablished {
    uint16_t handle;
    uint16_t sourceCid;
    uint16_t psm;
    bool accepted;
};

struct ACLData {
    uint16_t handle;
    uint16_t channelId;
    uint8_t* data;
    size_t len;
};

using HCIEvent = std::variant<HCIInquiryComplete, HCIInquiryResult, HCIConnectionEstablished, HCIConnectionFailed, HCIDisconnected, HCIRemoteName, HCILinkKeyRequest, HCIPINRequest>;
using ACLEvent = std::variant<ACLDisconnected, ACLConnectionFailed, ACLConnectionEstablished, ACLData>;

class Bluetooth {
public:
    Bluetooth() = default;
    Bluetooth(const Bluetooth&) = delete;
    Bluetooth& operator=(const Bluetooth&) = delete;
    virtual ~Bluetooth() {}

    // Device
    virtual std::span<uint8_t, 6> macAddress() = 0;
    virtual void onReady(const std::function<void(Bluetooth*)>&) = 0;
    // Call from loop/dispatcher
    virtual void process() = 0;

    // HCI
    virtual void onHCIEvent(const std::function<void(Bluetooth*, const HCIEvent&)>&) = 0;
    virtual void onHCIConnectionRequest(const std::function<bool(Bluetooth*, const HCIConnectionRequest&)>& listener) = 0;

    virtual void scan() = 0;
    virtual void requestRemoteName(const HCIInquiryResult& result) = 0;
    virtual void connect(const HCIInquiryResult& result) = 0;
    virtual void auth(uint16_t handle) = 0;
    virtual void negativeReply(uint64_t bdaddr) = 0;
    virtual void disconnect(uint16_t handle) = 0;
    virtual void sendPinReply(uint64_t bdaddr, uint8_t* pinData, size_t len) = 0;

    // ACL
    virtual void onACLEvent(const std::function<void(Bluetooth*, const ACLEvent&)>& acl) = 0;
    virtual void onACLConnectionRequest(const std::function<bool(Bluetooth*, const ACLConnectionRequest&)>& listener) = 0;

    virtual void l2cap_connect(uint16_t handle, uint16_t psm, uint16_t mtu) = 0;
    virtual void l2cap_disconnect(uint16_t handle, uint16_t psm) = 0;
    virtual void l2send_data(uint16_t handle, uint16_t psm, uint8_t* data, size_t len) = 0;
};

}
