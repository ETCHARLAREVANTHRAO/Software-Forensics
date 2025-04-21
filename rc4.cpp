#include <bits/stdc++.h>
using namespace std;

class rc4_ciph {
private:
    int s_vec[256];
    vector<int> key_strm;

    void init_s_vec() {
        for(int i = 0; i < 256; i++) {
            s_vec[i] = i;
        }
    }

    void shuf_arr(const vector<int>& k) {
        int j = 0;
        init_s_vec();

        for(int i = 0; i < 256; i++) {
            j = (j + s_vec[i] + k[i % k.size()]) % 256;
            swap(s_vec[i], s_vec[j]);
        }
    }

    void gen_key_strm(const vector<int>& m) {
        int i = 0, j = 0;
        key_strm.clear();
        
        for(int k_idx = 0; k_idx < m.size(); k_idx++) {
            i = (i + 1) % 256;
            j = (j + s_vec[i]) % 256;
            swap(s_vec[i], s_vec[j]);
            int k_strm = s_vec[(s_vec[i] + s_vec[j]) % 256];
            key_strm.push_back(k_strm);
        }
    }

public:
    vector<int> enc(const vector<int>& m, const vector<int>& k) {
        shuf_arr(k);
        gen_key_strm(m);
        
        vector<int> c_txt;
        for(int i = 0; i < m.size(); i++) {
            c_txt.push_back(key_strm[i] ^ m[i]);
        }
        return c_txt;
    }

    vector<int> dec(const vector<int>& c_txt, const vector<int>& k) {
        shuf_arr(k);
        gen_key_strm(c_txt);
        
        vector<int> p_txt;
        for(int i = 0; i < c_txt.size(); i++) {
            p_txt.push_back(c_txt[i] ^ key_strm[i]);
        }
        return p_txt;
    }
};

class crypto_intf {
private:
    rc4_ciph ciph;
    
    vector<int> get_in_vec(const string& len_prompt, const string& val_prompt) {
        int n;
        cout << len_prompt;
        cin >> n;
        cout << endl;
        
        vector<int> vec(n);
        cout << val_prompt;
        for(int i = 0; i < n; i++) {
            cin >> vec[i];
        }
        cout << endl;
        
        return vec;
    }

    void prnt_vec(const string& lbl, const vector<int>& vec) {
        cout << lbl;
        for(auto itm : vec) {
            cout << itm << " ";
        }
        cout << endl;
    }

public:
    void exec() {
        vector<int> k = get_in_vec("Enter Key Length:", "Enter Key Values:");
        vector<int> m = get_in_vec("Enter Plain Text Size:", "Enter Plain Text:");

        vector<int> c_txt = ciph.enc(m, k);
        vector<int> p_txt = ciph.dec(c_txt, k);

        prnt_vec("Ciphertext: ", c_txt);
        prnt_vec("Decrypted Text: ", p_txt);
    }
};

int main() {
    crypto_intf crypto;
    crypto.exec();
    return 0;
}