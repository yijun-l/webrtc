#pragma once

#include "main_wnd.h"
#include "peer_connection_client.h"

class Conductor : public PeerConnectionClientObserver,
                  public MainWndCallback
{
public:
    Conductor(PeerConnectionClient *client, MainWindow *main_wnd);
    ~Conductor();
    void Close() override;

protected:
    // PeerConnectionClientObserver implementation.
    void OnSignedIn() override;
    void OnDisconnected() override;
    void OnPeerConnected(int id, const std::string &name) override;
    void OnPeerDisconnected(int id) override;
    void OnMessageFromPeer(int peer_id, const std::string &message) override;
    void OnMessageSent(int err) override;
    void OnServerConnectionFailure() override;

    // MainWndCallback implementation.
    void StartLogin(const std::string &server, int port) override;
    void DisconnectFromServer() override;
    void ConnectToPeer(int peer_id) override;
    void DisconnectFromCurrentPeer() override;
    void UIThreadCallback(int msg_id, void *data) override;

    PeerConnectionClient *client_;
    MainWindow *main_wnd_;
};