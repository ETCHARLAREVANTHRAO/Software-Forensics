#include<iostream>
#include<fstream>
#include<vector>
#include<bitset>
#include<limits>

using namespace std;

vector<int> read_img(string &filename, int &width, int &height, int& max_range){
    ifstream file(filename);
    string format;
    file >> format;
    if (format != "P3"){
        cerr<< "the image type is not supported";
        exit(1);
    }
    file >> width >> height >> max_range;

    vector<int> pixels(width * height * 3);
    for(int i=0 ; i< pixels.size(); i++){
        file >> pixels[i];
    }
    file.close();
    return pixels;

}

vector<int> encode_msg(vector<int> &pixels, string &msg){

    string binary_msg;
    for(char c : msg){
        binary_msg += bitset<8>(c).to_string();
    }
    int req_size = binary_msg.size() + msg.size();

    //cout<<msg<<" "<<binary_msg<<" "<<endl;
    vector<int> changed(req_size);
    //cout<<changed.size()<<endl;
    int msg_index = 0;
    for(int i=0; i< req_size; i++){
        if((i+1) % 9 == 0){
            changed[i] = (msg_index < binary_msg.size()) ? (pixels[i] & ~1) : (pixels[i] | 1);
        }
        else if (msg_index < binary_msg.size()){
            changed[i] = (pixels[i] & ~1) | (binary_msg[msg_index++] - '0');
        }
        //cout<<changed[i]<<" "<<endl;
        else {
            changed[i] = pixels[i];
        }
    }
    return changed;

}


int main(){
    string input_file1 = "C:/Users/etche/OneDrive/Desktop/MTECH Sem 2/SF_LAB/LAB codes/sea.ppm";
    string input_file2 = "C:/Users/etche/OneDrive/Desktop/MTECH Sem 2/SF_LAB/LAB codes/mount.ppm";


    string secret_msg;
    cout << "Enter the secret message to hid: ";
    getline(cin, secret_msg);

    vector<string> images = {input_file1, input_file2};
    vector<int> diff_cnt(images.size());

    for (int i =0 ; i< images.size(); i++){
        string &image = images[i];
        int width, height, max_range;
        vector<int> pixels = read_img(image, width, height, max_range);

  
        vector<int> ans = encode_msg(pixels, secret_msg);

        int diff = 0;

        for(int j= 0; j< ans.size(); j++){
            if (pixels[j] != ans[j]) diff += 1;
        }
        diff_cnt[i] = diff;
    }
    int min= INT_MAX;
    int index = 0;
    for(int i=0; i<diff_cnt.size(); i++){
        if (diff_cnt[i] < min){ 
            min = diff_cnt[i];
            index = i;
        }
    }
    cout<<"The best modified image is image "<<index<<" with minimum modifications of "<<min<<endl;

    /*
    cout<<"origninal"<<" ";
    for(int i=0 ; i< ans.size(); i++){
        
        cout<<pixels[i]<<" ";
    }
    cout<<endl;
    cout<<"modified: "<<" ";
    for(int i=0 ; i< ans.size(); i++){
        cout<<ans[i]<<" ";
    }
    */


}