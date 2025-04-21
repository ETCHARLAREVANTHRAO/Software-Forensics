#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>

using namespace std;

class PPMImage {
private:
    int width, height, max_range;
    vector<int> pixels;

public:
    PPMImage(const string& filename) {
        read_img(filename);
    }

    void read_img(const string& filename) {
        ifstream file(filename);
        if (!file) {
            cerr << "Error: Could not open file " << filename << endl;
            exit(1);
        }

        string format;
        file >> format;
        if (format != "P3") {
            cerr << "Error: Only PPM (P3) format is supported!" << endl;
            exit(1);
        }

        file >> width >> height >> max_range;
        pixels.resize(width * height * 3);

        for (int& pixel : pixels)  
            file >> pixel;

        file.close();
    }

    void write_img(const string& filename) {
        ofstream file(filename);
        if (!file) {
            cerr << "Error: Could not write to file " << filename << endl;
            exit(1);
        }

        file << "P3\n" << width << " " << height << "\n" << max_range << "\n";

        for (size_t i = 0; i < pixels.size(); i++) {
            file << pixels[i] << " ";
            if ((i + 1) % (width * 3) == 0)  
                file << "\n";
        }

        file.close();
    }

    void encode_msg(const string& msg) {
        string binary_msg;
        for (char c : msg)  
            binary_msg += bitset<8>(c).to_string();  // Convert each character to binary

        size_t msg_index = 0;
        for (size_t i = 0; i < pixels.size() && msg_index < binary_msg.size(); i++) {
            pixels[i] = (pixels[i] & ~1) | (binary_msg[msg_index++] - '0');  // Store message bits in LSB
        }
    }

    string decode_msg() {
        string binary_msg;
        
        for (size_t i = 0; i < pixels.size(); i++) {
            if((i+1) % 9 == 0){
                if (pixels[i] & 1) break;
            }else{
                binary_msg += (pixels[i] & 1) ? '1' : '0';
            }
            
        }

        string msg;
        for (size_t i = 0; i + 8 <= binary_msg.size(); i += 8) {
            bitset<8> charBits(binary_msg.substr(i, 8));
            if (charBits.to_ulong() == 0) break;  // Stop when we hit a null character
            msg += static_cast<char>(charBits.to_ulong());
        }
        return msg;
    }
};

int main() {
    string input_file = "C:/Users/etche/OneDrive/Desktop/MTECH Sem 2/SF_LAB/LAB codes/sea.ppm";
    string output_file = "C:/Users/etche/OneDrive/Desktop/MTECH Sem 2/SF_LAB/LAB codes/output1.ppm";

    PPMImage image(input_file);

    string secret_msg;
    cout << "Enter the secret message to hide: ";
    getline(cin, secret_msg);

    image.encode_msg(secret_msg);
    image.write_img(output_file);

    PPMImage new_image(output_file);
    cout << "Decoded message: " << new_image.decode_msg() << endl;

    return 0;
}
