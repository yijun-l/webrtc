#include "peer_connection_client.h"

PeerConnectionClient::PeerConnectionClient()
    : callback_(NULL) {}

PeerConnectionClient::~PeerConnectionClient() = default;

void PeerConnectionClient::RegisterObserver(
    PeerConnectionClientObserver *callback)
{
  callback_ = callback;
}