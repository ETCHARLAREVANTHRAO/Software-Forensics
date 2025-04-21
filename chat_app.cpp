#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 5000
#define GROUP "239.0.0.2"

using namespace std;

class rc4_cipher {
private:
    string encry_key = "mysecret";

    void init_state(unsigned char s_box[256]) {
        for (int i = 0; i < 256; i++)
            s_box[i] = i;
    }

public:
    void encry_dec(string &data) {
        unsigned char s_box[256];
        init_state(s_box);
        
        int j_idx = 0;
        for (int i = 0; i < 256; i++) {
            j_idx = (j_idx + s_box[i] + encry_key[i % encry_key.length()]) % 256;
            swap(s_box[i], s_box[j_idx]);
        }

        int i_idx = 0;
        j_idx = 0;
        for (int k = 0; k < data.length(); k++) {
            i_idx = (i_idx + 1) % 256;
            j_idx = (j_idx + s_box[i_idx]) % 256;
            swap(s_box[i_idx], s_box[j_idx]);
            data[k] ^= s_box[(s_box[i_idx] + s_box[j_idx]) % 256];
        }
    }
};

class multicast_chat {
private:
    string user_name;
    int socket_fd;
    rc4_cipher cipher_obj;
    sockaddr_in group_addr;

    void setup_sock() {
        socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
        
        int reuse_flag = 1;
        setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse_flag, sizeof(reuse_flag));

        sockaddr_in local_addr{};
        local_addr.sin_family = AF_INET;
        local_addr.sin_port = htons(PORT);
        local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        bind(socket_fd, (sockaddr *)&local_addr, sizeof(local_addr));

        ip_mreq multi_req{};
        multi_req.imr_multiaddr.s_addr = inet_addr(GROUP);
        multi_req.imr_interface.s_addr = htonl(INADDR_ANY);
        setsockopt(socket_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &multi_req, sizeof(multi_req));

        group_addr.sin_family = AF_INET;
        group_addr.sin_port = htons(PORT);
        group_addr.sin_addr.s_addr = inet_addr(GROUP);
    }

    void recv_msgs() {
        char msg_buffer[1024];
        while (true) {
            sockaddr_in src_addr{};
            socklen_t src_len = sizeof(src_addr);
            int data_len = recvfrom(socket_fd, msg_buffer, sizeof(msg_buffer) - 1, 0, (sockaddr *)&src_addr, &src_len);
            if (data_len > 0) {
                msg_buffer[data_len] = '\0';
                string encry_data(msg_buffer);
                cipher_obj.encry_dec(encry_data);
                string full_msg(encry_data);
                string sender_name = full_msg.substr(0, full_msg.find(":"));
                string msg_content = full_msg.substr(full_msg.find(":") + 1);
                if (sender_name != user_name && 
                    (msg_content.find("@" + user_name) != string::npos || msg_content.find("@ALL") != string::npos)) {
                    cout << "\n" << sender_name << ": " << msg_content << endl;
                    cout << "Send to (name or ALL): ";
                    cout.flush();
                }
            }
        }
    }

public:
    multicast_chat() {
        cout << "Enter your name: ";
        getline(cin, user_name);
        setup_sock();
    }

    void start_chat() {
        thread recv_thread(&multicast_chat::recv_msgs, this);
        
        while (true) {
            string recip_name, chat_msg;
            cout << "Send to (name or ALL): ";
            getline(cin, recip_name);
            cout << "Message: ";
            getline(cin, chat_msg);
            if (chat_msg == "/exit")
                break;
            
            string complete_msg = user_name + ":@" + recip_name + " " + chat_msg;
            cipher_obj.encry_dec(complete_msg);
            sendto(socket_fd, complete_msg.c_str(), complete_msg.size(), 0, 
                  (sockaddr *)&group_addr, sizeof(group_addr));
        }

        recv_thread.detach();
        close(socket_fd);
    }
};

int main() {
    multicast_chat chat_obj;
    chat_obj.start_chat();
    return 0;
}