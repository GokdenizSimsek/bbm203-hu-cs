#ifndef PHYSICAL_LAYER_PACKET_H
#define PHYSICAL_LAYER_PACKET_H

#include "Packet.h"

using namespace std;

// Extends Packet class. Have additional layer-specific member variables and overrides the virtual print function.
class PhysicalLayerPacket : public Packet {
public:
    PhysicalLayerPacket(int layerID, const string& senderMAC, const string& receiverMAC);
    ~PhysicalLayerPacket() override;

    string sender_MAC_address;
    string receiver_MAC_address;
    int frame_num;
    int total_frame;

    void print() override;
};

#endif // PHYSICAL_LAYER_PACKET_H
