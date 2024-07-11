#include "PhysicalLayerPacket.h"

PhysicalLayerPacket::PhysicalLayerPacket(int _layer_ID, const string& _sender_MAC, const string& _receiver_MAC)
        : Packet(_layer_ID) {
    sender_MAC_address = _sender_MAC;
    receiver_MAC_address = _receiver_MAC;
}

void PhysicalLayerPacket::print() {
    // TODO: Override the virtual print function from Packet class to additionally print layer-specific properties.

    cout << "Sender MAC address: " << sender_MAC_address << ", Receiver MAC address: " << receiver_MAC_address << endl;

}

PhysicalLayerPacket::~PhysicalLayerPacket() {
    // TODO: Free any dynamically allocated memory if necessary.
}