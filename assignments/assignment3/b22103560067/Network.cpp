#include "Network.h"
#include <string>

Network::Network() {

}

void Network::process_commands(vector<Client> &clients, vector<string> &commands, int message_limit,
                      const string &sender_port, const string &receiver_port) {
    // TODO: Execute the commands given as a vector of strings while utilizing the remaining arguments.
    /* Don't use any static variables, assume this method will be called over and over during testing.
     Don't forget to update the necessary member variables after processing each command. For example,
     after the MESSAGE command, the outgoing queue of the sender must have the expected frames ready to send. */

    for (string command_line: commands) {
        if (command_line.find("MESSAGE") != std::string::npos) {
            std::istringstream iss(command_line.substr(8));
            string sender_id;
            string receiver_id;
            string message;
            iss >> sender_id >> receiver_id;

            cout << "--------------------------------------------------------------------------------------\nCommand: " << command_line << endl;

            size_t startPos = command_line.find('#');
            size_t endPos = command_line.find_first_of(".!?");
            message = command_line.substr(startPos + 1, endPos - startPos);

            cout << "--------------------------------------------------------------------------------------\nMessage to be sent:" << message << endl << endl;

            string sender_ip;
            string receiver_ip;
            string sender_mac;
            string receiver_mac;
            string nextHop_id;

            int senderCli_index;
            for (int i = 0; i < clients.size(); ++i) {
                if (clients[i].client_id == sender_id) {
                    sender_ip = clients[i].client_ip;
                    sender_mac = clients[i].client_mac;
                    nextHop_id = clients[i].routing_table[receiver_id];
                    for (Client &cli:clients) {
                        if (cli.client_id == nextHop_id) {
                            receiver_mac = cli.client_mac;
                        }
                    }
                    senderCli_index = i;
                } else if (clients[i].client_id == receiver_id) {
                    receiver_ip = clients[i].client_ip;
                }
            }

            int frameNum = 0;
            std::vector<std::string> messageParts;
            for (size_t i = 0; i < message.length(); i += message_limit) {
                messageParts.push_back(message.substr(i, message_limit));
                frameNum++;
            }
            for (int i = 0; i < frameNum; ++i) {
                stack<Packet*> stackVector;
                ApplicationLayerPacket * appLayer = new ApplicationLayerPacket(0, sender_id, receiver_id, messageParts[i]);
                appLayer->frame_num = frameNum + 1;
                appLayer->total_frame = frameNum;
                TransportLayerPacket * transLayer = new TransportLayerPacket(1, sender_port, receiver_port);
                transLayer->frame_num = frameNum + 1;
                transLayer->total_frame = frameNum;
                NetworkLayerPacket * netLayer = new NetworkLayerPacket(2, sender_ip, receiver_ip);
                netLayer->frame_num = frameNum + 1;
                netLayer->total_frame = frameNum;
                PhysicalLayerPacket * phyLayer = new PhysicalLayerPacket(3, sender_mac, receiver_mac);
                phyLayer->frame_num = frameNum + 1;
                phyLayer->total_frame = frameNum;
                stackVector.push(appLayer);
                stackVector.push(transLayer);
                stackVector.push(netLayer);
                stackVector.push(phyLayer);
                clients[senderCli_index].outgoing_queue.push(stackVector);
                cout << "Frame #" << i + 1 << endl;
                phyLayer->print();
                netLayer->print();
                transLayer->print();
                appLayer->print();
                cout << "Message chunk carried: \"" << messageParts[i] << "\"\n"
                     << "Number of hops so far: " << 0 << endl << "--------\n";
            }

            auto now = std::chrono::system_clock::now();
            std::time_t now_c = std::chrono::system_clock::to_time_t(now);
            std::tm tm_now = *std::localtime(&now_c);
            std::stringstream ss;
            ss << std::put_time(&tm_now, "%Y-%m-%d %H:%M:%S");
            std::string str_time = ss.str();

            ActivityType activity = ActivityType::MESSAGE_SENT;
            Log message_log = *new Log(str_time, message, frameNum, 0, sender_id, receiver_id, true, activity);
            clients[senderCli_index].log_entries.push_back(message_log);

        } else if (command_line.find("SHOW_FRAME_INFO") != std::string::npos) {
            istringstream iss(command_line.substr(16));
            string client_id;
            string queue_selection;
            string frame_num;
            iss >> client_id >> queue_selection >> frame_num;

            cout << "--------------------------------\nCommand: " << command_line << "\n--------------------------------\n";

            int client_index;
            for (int i = 0; i < clients.size(); ++i) {
                if (clients[i].client_id == client_id) {
                    client_index = i;
                }
            }

            if (queue_selection == "in") {
                if (clients[client_index].incoming_queue.size() < stoi(frame_num)) {
                    cout << "No such frame." << endl;
                } else {
                    cout << "Current Frame #" << frame_num << " on the incoming queue of client " << client_id << endl;

                    queue<stack<Packet*>> assignedQueue;
                    assignedQueue = clients[client_index].incoming_queue;

                    for (int i = 0; i < stoi(frame_num) - 1; ++i) {
                        assignedQueue.pop();
                    }
                    stack<Packet*> reversedVector = assignedQueue.front();
                    stack<Packet*> stackVector;
                    while (!reversedVector.empty()) {
                        Packet *packet = reversedVector.top();
                        reversedVector.pop();
                        stackVector.push(packet);
                    }
                    int hop;
                    while (!stackVector.empty()) {
                        Packet* packet = stackVector.top();
                        ApplicationLayerPacket * appLayerPacket = dynamic_cast<ApplicationLayerPacket*>(packet);
                        if (appLayerPacket) {
                            cout << "Carried Message: \"" << appLayerPacket->message_data << "\"\n";
                            hop = appLayerPacket->hops;
                        }
                        cout << "Layer " << packet->layer_ID << " info: ";
                        packet->print();
                        stackVector.pop();
                    }
                    cout << "Number of hops so far: " << hop << endl;
                }
            } else {
                if (clients[client_index].outgoing_queue.size() < stoi(frame_num)) {
                    cout << "No such frame." << endl;
                } else {
                    cout << "Current Frame #" << frame_num << " on the outgoing queue of client " << client_id << endl;

                    queue<stack<Packet*>> assignedQueue;
                    assignedQueue = clients[client_index].outgoing_queue;

                    for (int i = 0; i < stoi(frame_num) - 1; ++i) {
                        assignedQueue.pop();
                    }

                    stack<Packet*> reversedVector = assignedQueue.front();
                    stack<Packet*> stackVector;
                    while (!reversedVector.empty()) {
                        Packet *packet = reversedVector.top();
                        reversedVector.pop();
                        stackVector.push(packet);
                    }

                    int hop;
                    while (!stackVector.empty()) {
                        Packet* packet = stackVector.top();
                        ApplicationLayerPacket * appLayerPacket = dynamic_cast<ApplicationLayerPacket*>(packet);
                        if (appLayerPacket) {
                            cout << "Carried Message: \"" << appLayerPacket->message_data << "\"\n";
                            hop = appLayerPacket->hops;
                        }
                        cout << "Layer " << packet->layer_ID << " info: ";
                        packet->print();
                        stackVector.pop();
                    }
                    cout << "Number of hops so far: " << hop << endl;
                }
            }

        } else if (command_line.find("SHOW_Q_INFO") != std::string::npos) {
            istringstream iss(command_line.substr(12));
            string client_id;
            string queue_selection;
            iss >> client_id >> queue_selection;

            cout << "--------------------------------\nCommand: " << command_line << "\n--------------------------------\n";

            int client_index;
            for (int i = 0; i < clients.size(); ++i) {
                if (clients[i].client_id == client_id) {
                    client_index = i;
                }
            }
            if (queue_selection == "in") {
                cout << "Client " << client_id << " İncoming Queue Status\n";
                cout << "Current total number of frames: " << clients[client_index].incoming_queue.size() << endl;
            } else {
                cout << "Client " << client_id << " Outgoing Queue Status\n";
                cout << "Current total number of frames: " << clients[client_index].outgoing_queue.size() << endl;
            }

        } else if (command_line.find("SEND") != std::string::npos) {
            cout << "-------------\nCommand: " << command_line << "\n-------------\n";
            for (Client &cli:clients) {
                if (!cli.outgoing_queue.empty()) {
                    string nextHopCli_mac;
                    string receiverCli_id;
                    string newNextHop_id;
                    string nextHopCli_id;

                    while (!cli.outgoing_queue.empty()) {
                        int frame_num;
                        string message;
                        int hop;
                        stack<Packet*> stackVector = cli.outgoing_queue.front();
                        cli.outgoing_queue.pop();
                        stack<Packet*> copyStackVector = stackVector;
                        stack<Packet*> newStackVector;
                        while (!copyStackVector.empty()) {
                            Packet* packet = copyStackVector.top();
                            ApplicationLayerPacket * appLayerPacket = dynamic_cast<ApplicationLayerPacket*>(packet);
                            if (appLayerPacket) {
                                frame_num = appLayerPacket->frame_num;
                                receiverCli_id = appLayerPacket->receiver_ID;
                            }
                            PhysicalLayerPacket * phyLayerPacket = dynamic_cast<PhysicalLayerPacket*>(packet);
                            if (phyLayerPacket) {
                                nextHopCli_mac = phyLayerPacket->receiver_MAC_address;
                            }
                            copyStackVector.pop();
                        }
                        for (Client &client:clients) {
                            if (nextHopCli_mac == client.client_mac) {
                                nextHopCli_id = client.client_id;
                                newNextHop_id = client.routing_table[receiverCli_id];
                            }
                        }
                        cout << "Client " << cli.client_id << " sending frame #" << frame_num << " to client " << nextHopCli_id << endl;

                        while (!stackVector.empty()) {
                            Packet* packet = stackVector.top();
                            packet->print();
                            ApplicationLayerPacket * appLayerPacket = dynamic_cast<ApplicationLayerPacket*>(packet);
                            if (appLayerPacket) {
                                appLayerPacket->hops++;
                                hop = appLayerPacket->hops;
                                message = appLayerPacket->message_data;
                                frame_num = appLayerPacket->frame_num;
                            }
                            newStackVector.push(packet);
                            stackVector.pop();
                        }
                        cout << "Message chunk carried: \"" << message << "\"\n";
                        cout << "Number of hops so far: " << hop << "\n--------\n";
                        stack<Packet*> newReversedStackVector;
                        while (!newStackVector.empty()) {
                            Packet *packet = newStackVector.top();
                            newStackVector.pop();
                            newReversedStackVector.push(packet);
                        }
                        for (Client &client:clients) {
                            if (nextHopCli_id == client.client_id) {
                                client.incoming_queue.push(newReversedStackVector);
                            }
                        }
                    }
                }
            }
        } else if (command_line.find("RECEIVE") != std::string::npos) {
            cout << "-------------\nCommand: " << command_line << "\n-------------\n";
            for (Client &cli: clients) {
                string fullMessage;
                while (!cli.incoming_queue.empty()) {
                    bool isItArrived = false;
                    string message;
                    int initial_frame;
                    int frameNum;
                    int hop;
                    string sender_id;
                    string receiver_id;
                    string comingFrom_id;
                    string comingFor_id;
                    string nextHop_id;

                    stack<Packet*> stackVector = cli.incoming_queue.front();
                    stack<Packet*> copyStackVector = stackVector;
                    stack<Packet*> copyStackVector2 = stackVector;
                    while (!copyStackVector.empty()) {
                        Packet *packet = copyStackVector.top();
                        copyStackVector.pop();
                        ApplicationLayerPacket * appLayerPacket = dynamic_cast<ApplicationLayerPacket*>(packet);
                        if (appLayerPacket) {
                            initial_frame = appLayerPacket->frame_num;
                            frameNum = appLayerPacket->total_frame;
                            message = appLayerPacket->message_data;
                            sender_id = appLayerPacket->sender_ID;
                            receiver_id = appLayerPacket->receiver_ID;
                            hop = appLayerPacket->hops;
                            if (appLayerPacket->receiver_ID == cli.client_id) {
                                isItArrived = true;
                            }
                        }

                    }
                    while (!copyStackVector2.empty()) {
                        Packet *packet = copyStackVector2.top();
                        copyStackVector2.pop();
                        PhysicalLayerPacket *phyLayerPacket = dynamic_cast<PhysicalLayerPacket *>(packet);
                        if (phyLayerPacket) {
                            for (Client cl: clients) {
                                if (cl.client_mac == phyLayerPacket->sender_MAC_address) {
                                    comingFrom_id = cl.client_id;
                                } else if (cl.client_mac == phyLayerPacket->receiver_MAC_address) {
                                    comingFor_id = cl.client_id;
                                    nextHop_id = cl.routing_table[receiver_id];
                                }
                            }
                        }
                    }
                    Packet *packet_x = stackVector.top();
                    PhysicalLayerPacket *phyLayerPacket_x = dynamic_cast<PhysicalLayerPacket *>(packet_x);
                    if (phyLayerPacket_x) {
                        for (Client &client: clients) {
                            if (nextHop_id == client.client_id) {
                                phyLayerPacket_x->receiver_MAC_address = client.client_mac;
                            } else if (comingFor_id == client.client_id) {
                                phyLayerPacket_x->sender_MAC_address = client.client_mac;
                            }
                        }
                    }
                    cout << isItArrived << endl;
                    if (!isItArrived) {
                        Packet *packet = stackVector.top();
                        PhysicalLayerPacket *phyLayerPacket = dynamic_cast<PhysicalLayerPacket *>(packet);
                        if (phyLayerPacket) {
                            bool isThereCli = false;
                            for (Client client: clients) {
                                if (client.client_id == comingFor_id) {
                                    isThereCli = true;

                                    cout << "Client " << comingFrom_id << " receiving a message from client " << sender_id << ", but intended for client " << receiver_id << ". Forwarding...\n";

                                    auto now = std::chrono::system_clock::now();
                                    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
                                    std::tm tm_now = *std::localtime(&now_c);
                                    std::stringstream ss;
                                    ss << std::put_time(&tm_now, "%Y-%m-%d %H:%M:%S");
                                    std::string str_time = ss.str();

                                    ActivityType activity = ActivityType::MESSAGE_FORWARDED;
                                    Log message_log = *new Log(str_time, message, frameNum, hop, sender_id, receiver_id, true, activity);
                                    cli.log_entries.push_back(message_log);

                                    for (int i = 0; i < frameNum; ++i) {
                                        cout << "Frame #" << initial_frame << " MAC address change: New sender MAC " << phyLayerPacket->sender_MAC_address << ", new receiver MAC " << phyLayerPacket->receiver_MAC_address << endl;
                                        cli.incoming_queue.pop();
                                        cli.outgoing_queue.push(stackVector);
                                        if (!cli.incoming_queue.empty()){
                                            stackVector = cli.incoming_queue.front();
                                            Packet *packet_xx = stackVector.top();
                                            PhysicalLayerPacket *phyLayerPacket_xx = dynamic_cast<PhysicalLayerPacket *>(packet_xx);
                                            if (phyLayerPacket_xx) {
                                                for (Client &client_x: clients) {
                                                    if (nextHop_id == client_x.client_id) {
                                                        phyLayerPacket_xx->receiver_MAC_address = client_x.client_mac;
                                                    } else if (comingFor_id == client_x.client_id) {
                                                        phyLayerPacket_xx->sender_MAC_address = client_x.client_mac;
                                                    }
                                                }
                                            }
                                        }
                                        initial_frame++;
                                    }
                                    cout << "--------\n";
                                }
                            }
                            if (!isThereCli) {
                                auto now = std::chrono::system_clock::now();
                                std::time_t now_c = std::chrono::system_clock::to_time_t(now);
                                std::tm tm_now = *std::localtime(&now_c);
                                std::stringstream ss;
                                ss << std::put_time(&tm_now, "%Y-%m-%d %H:%M:%S");
                                std::string str_time = ss.str();

                                ActivityType activity = ActivityType::MESSAGE_DROPPED;
                                Log message_log = *new Log(str_time, message, frameNum, hop, sender_id, receiver_id, false, activity);
                                cli.log_entries.push_back(message_log);
                                for (int i = 0; i < frameNum; ++i) {
                                    cout << "Client " << comingFor_id << " receiving frame #" << initial_frame << " from client " << sender_id << ", but intended for client " << receiver_id <<". Forwarding...\n";
                                    cout << "Error: Unreachable destination. Packets are dropped after " << hop << " hops!\n";
                                    initial_frame++;
                                    cli.incoming_queue.pop();
                                }
                            }
                        }
                    } else {
                        cout << "Client " << receiver_id << " receiving frame #" << initial_frame << " from client " << comingFrom_id << ", originating from client " << sender_id << endl;
                        bool endMessage = false;
                        while (!stackVector.empty()) {
                            Packet *packet = stackVector.top();
                            stackVector.pop();
                            ApplicationLayerPacket * appLayerPacket = dynamic_cast<ApplicationLayerPacket*>(packet);
                            if (appLayerPacket) {
                                size_t found = appLayerPacket->message_data.find_first_of(".!?");
                                if (found != std::string::npos) {
                                    endMessage = true;
                                }
                                fullMessage.append(appLayerPacket->message_data);
                            }
                            packet->print();
                        }
                        cout << "Message chunk carried: \""<< message << "\"\n" << "Number of hops so far: " << hop << "\n--------\n";
                        if (endMessage) {
                            cout << "Client " << receiver_id << " received the message \"" << fullMessage << "\" from client " << sender_id << ".\n--------\n";
                            auto now = std::chrono::system_clock::now();
                            std::time_t now_c = std::chrono::system_clock::to_time_t(now);
                            std::tm tm_now = *std::localtime(&now_c);
                            std::stringstream ss;
                            ss << std::put_time(&tm_now, "%Y-%m-%d %H:%M:%S");
                            std::string str_time = ss.str();

                            ActivityType activity = ActivityType::MESSAGE_RECEIVED;
                            Log message_log = *new Log(str_time, fullMessage, frameNum, hop, sender_id, receiver_id, true, activity);
                            cli.log_entries.push_back(message_log);
                            fullMessage.clear();
                        }
                        cli.incoming_queue.pop();
                    }
                }
            }
        } else if (command_line.find("PRINT_LOG") != std::string::npos) {
            cout << "--------------------\n" << "Command: " << command_line << "\n--------------------\n";
            string client_id;
            std::istringstream iss(command_line.substr(10));
            iss >> client_id;
            for (Client cli:clients) {
                if (cli.client_id == client_id) {
                    if (cli.log_entries.size() > 0) {
                        cout << "Client " << client_id << " Logs:\n";
                    }
                    int i = 1;
                    for (Log log:cli.log_entries) {
                        string success;
                        if (log.success_status) {
                            success = "Yes";
                        } else {
                            success = "No";
                        }
                        cout << "--------------\nLog Entry #" << i <<":\n";
                        if (log.activity_type == ActivityType::MESSAGE_SENT) {
                            cout << "Activity: Message Sent\n" << "Timestamp: " << log.timestamp << endl;
                            cout << "Number of frames: " << log.number_of_frames << "\nNumber of hops: " << log.number_of_hops << endl;
                            cout << "Sender ID: " << log.sender_id << "\nReceiver ID: " << log.receiver_id << endl;
                            cout << "Success: " << success << "\nMessage: \"" << log.message_content << "\"\n";
                        } else if (log.activity_type == ActivityType::MESSAGE_FORWARDED) {
                            cout << "Activity: Message Forwarded\n" << "Timestamp: " << log.timestamp << endl;
                            cout << "Number of frames: " << log.number_of_frames << "\nNumber of hops: " << log.number_of_hops << endl;
                            cout << "Sender ID: " << log.sender_id << "\nReceiver ID: " << log.receiver_id << endl;
                            cout << "Success: " << success << endl;
                        } else if (log.activity_type == ActivityType::MESSAGE_RECEIVED) {
                            cout << "Activity: Message Received\n" << "Timestamp: " << log.timestamp << endl;
                            cout << "Number of frames: " << log.number_of_frames << "\nNumber of hops: " << log.number_of_hops << endl;
                            cout << "Sender ID: " << log.sender_id << "\nReceiver ID: " << log.receiver_id << endl;
                            cout << "Success: " << success << "\nMessage: \"" << log.message_content << "\"\n";
                        } else if (log.activity_type == ActivityType::MESSAGE_DROPPED) {
                            cout << "Activity: Message Dropped\n" << "Timestamp: " << log.timestamp << endl;
                            cout << "Number of frames: " << log.number_of_frames << "\nNumber of hops: " << log.number_of_hops << endl;
                            cout << "Sender ID: " << log.sender_id << "\nReceiver ID: " << log.receiver_id << endl;
                            cout << "Success: " << success << endl;
                        }
                        i++;
                    }
                }
            }
        } else {
            cout << "----------------------\nCommand: " << command_line << "\n----------------------\nInvalid Command.\n";
        }
    }

}

vector<Client> Network::read_clients(const string &filename) {
    vector<Client> clients;
    // TODO: Read clients from the given input file and return a vector of Client instances.
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        getline(file, line);
        while (getline(file, line)) {
            std::istringstream iss(line);
            std::vector<std::string> words;
            std::string word;
            while (iss >> word) {
                words.push_back(word);
            }
            Client client = *new Client(words[0],words[1],words[2]);
            clients.push_back(client);
        }
    }
    return clients;
}

void Network::read_routing_tables(vector<Client> &clients, const string &filename) {
    // TODO: Read the routing tables from the given input file and populate the clients' routing_table member variable.
    ifstream file(filename);
    if (file.is_open()) {
        int i = 0;
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            if (line == "-") {
                i++;
                continue;
            } else {
                std::string key, value;
                if (iss >> key >> value) {
                    clients[i].routing_table[key] = value;
                }
            }
        }
    }
}

// Returns a list of token lists for each command
vector<string> Network::read_commands(const string &filename) {
    vector<string> commands;
    // TODO: Read commands from the given input file and return them as a vector of strings.
    ifstream file(filename);
    string line;
    getline(file, line);
    while (getline(file, line)) {
        commands.push_back(line);
    }
    return commands;
}

Network::~Network() {
    // TODO: Free any dynamically allocated memory if necessary.
}
