#pragma once

#include <string>

#include "third_party/sigslot/sigslot.h"

struct PeerConnectionClientObserver
{
    virtual void OnSignedIn() = 0; // Called when we're logged on.
    virtual void OnDisconnected() = 0;
    virtual void OnPeerConnected(int id, const std::string &name) = 0;
    virtual void OnPeerDisconnected(int peer_id) = 0;
    virtual void OnMessageFromPeer(int peer_id, const std::string &message) = 0;
    virtual void OnMessageSent(int err) = 0;
    virtual void OnServerConnectionFailure() = 0;

protected:
    virtual ~PeerConnectionClientObserver() {}
};

class PeerConnectionClient : public sigslot::has_slots<>
{
public:
    PeerConnectionClient();
    ~PeerConnectionClient();

    void RegisterObserver(PeerConnectionClientObserver *callback);

protected:
    PeerConnectionClientObserver *callback_;
};