#include <bits/stdc++.h>
using namespace std;

class elgamal_enc {
private:
    long long p_mod;
    long long prv_key;
    long long base_no;
    long long pub_val;

    long long mod_exp(long long b_val, long long pow, long long mod) {
        long long res = 1;
        b_val %= mod;
        while (pow > 0) {
            if (pow & 1) {
                res = (res * b_val) % mod;
            }
            pow >>= 1;
            b_val = (b_val * b_val) % mod;
        }
        return res;
    }

    long long find_prim(long long num) {
        for (long long cand = 2; cand < num; cand++) {
            bool is_prim = true;
            for (long long exp = 1; exp < num - 1; exp++) {
                if (mod_exp(cand, exp, num) == 1) {
                    is_prim = false;
                    break;
                }
            }
            if (is_prim) return cand;
        }
        return -1;
    }

public:
    elgamal_enc(long long in_p, long long in_k, long long r_opt) 
        : p_mod(in_p), prv_key(in_k) {
        base_no = (r_opt == -1) ? find_prim(p_mod) : r_opt;
        pub_val = mod_exp(base_no, prv_key, p_mod);
    }

    void get_pub(long long& m_out, long long& b_out, long long& v_out) const {
        m_out = p_mod;
        b_out = base_no;
        v_out = pub_val;
    }

    pair<long long, long long> enc_msg(long long p_txt, long long r_num) {
        long long c_p1 = mod_exp(base_no, r_num, p_mod);
        long long c_p2 = (p_txt * mod_exp(pub_val, r_num, p_mod)) % p_mod;
        return {c_p1, c_p2};
    }

    long long dec_msg(long long c_p1, long long c_p2) {
        long long tmp = mod_exp(c_p1, prv_key, p_mod);
        long long inv = mod_exp(tmp, p_mod - 2, p_mod);
        return (c_p2 * inv) % p_mod;
    }
};

class crypto_mgr {
private:
    elgamal_enc* enc_obj;

    long long get_p_in() {
        long long num;
        cout << "Enter big prime: ";
        cin >> num;
        return num;
    }

    long long get_k_in(long long max) {
        long long key;
        cout << "Enter key (1 to " << max - 2 << "): ";
        cin >> key;
        while (key < 1 || key > max - 2) {
            cout << "Invalid key. Retry: ";
            cin >> key;
        }
        return key;
    }

    long long get_r_opt() {
        long long opt;
        cout << "Enter base or -1: ";
        cin >> opt;
        return opt;
    }

    long long get_r_val(long long max) {
        long long val;
        cout << "Pick rand (1 to " << max - 2 << "): ";
        cin >> val;
        while (calc_gcd(val, max - 1) != 1 || val < 1 || val > max - 2) {
            cout << "Invalid rand. Retry: ";
            cin >> val;
        }
        return val;
    }

    long long calc_gcd(long long a, long long b) {
        while (b) {
            long long tmp = b;
            b = a % b;
            a = tmp;
        }
        return a;
    }

public:
    void exec() {
        long long u_p = get_p_in();
        long long u_k = get_k_in(u_p);
        long long u_r = get_r_opt();
        
        enc_obj = new elgamal_enc(u_p, u_k, u_r);

        long long o_p, o_b, o_v;
        enc_obj->get_pub(o_p, o_b, o_v);
        cout << "Pub Key - P: " << o_p 
             << ", B: " << o_b 
             << ", V: " << o_v << endl;

        long long msg;
        cout << "Enter msg: ";
        cin >> msg;

        long long r_v = get_r_val(u_p);

        auto [e_p1, e_p2] = enc_obj->enc_msg(msg, r_v);
        cout << "Enc Msg (P1, P2): (" << e_p1 << ", " << e_p2 << ")" << endl;

        long long d_msg = enc_obj->dec_msg(e_p1, e_p2);
        cout << "Dec Msg: " << d_msg << endl;

        delete enc_obj;
    }
};

int main() {
    crypto_mgr mgr;
    mgr.exec();
    return 0;
}