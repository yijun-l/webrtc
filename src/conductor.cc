#include "conductor.h"

Conductor::Conductor(PeerConnectionClient *client, MainWindow *main_wnd)
    : client_(client), main_wnd_(main_wnd)
{
    client_->RegisterObserver(this);
    main_wnd->RegisterObserver(this);
}

Conductor::~Conductor()
{
}

void Conductor::Close()
{
}

void Conductor::OnSignedIn()
{
}

void Conductor::OnDisconnected()
{
}

void Conductor::OnPeerConnected(int id, const std::string &name)
{
}

void Conductor::OnPeerDisconnected(int id)
{
}

void Conductor::OnMessageFromPeer(int peer_id, const std::string &message)
{
}

void Conductor::OnMessageSent(int err)
{
}

void Conductor::OnServerConnectionFailure()
{
}

void Conductor::StartLogin(const std::string &server, int port)
{
}

void Conductor::DisconnectFromServer()
{
}

void Conductor::ConnectToPeer(int peer_id)
{
}

void Conductor::DisconnectFromCurrentPeer()
{
}

void Conductor::UIThreadCallback(int msg_id, void *data)
{
}
