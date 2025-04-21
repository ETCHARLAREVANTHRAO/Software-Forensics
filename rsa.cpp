#include <iostream>
#include <vector>

using namespace std;

int gcd(int m, int n) {
    return n == 0 ? m : gcd(n, m % n);
}

int modInverse(int e, int phi) {
    int t = 0, newT = 1;
    int r = phi, newR = e;

    while (newR != 0) {
        int quotient = r / newR;
        int temp = t;
        t = newT;
        newT = temp - quotient * newT;
        temp = r;
        r = newR;
        newR = temp - quotient * newR;
    }

    if (r > 1) return -1;
    if (t < 0) t += phi;
    return t;
}

int modExp(int base, int power, int mod) {
    int result = 1;
    base = base % mod;
    while (power > 0) {
        if (power % 2 == 1)
            result = (result * base) % mod;
        power = power >> 1;
        base = (base * base) % mod;
    }
    return result;
}

string encrypt(string msg, int e, int n) {
    string cipher = "";
    for (char c : msg) {
        int m = int(c);
        cipher += to_string(modExp(m, e, n)) + " ";
    }
    return cipher;
}

string decrypt(vector<int> cipherText, int d, int n) {
    string decrypted = "";
    for (int c : cipherText) {
        decrypted += char(modExp(c, d, n));
    }
    return decrypted;
}

vector<int> generateKeys(int p, int q) {
    vector<int> value;
    int n = p * q;
    int phi = (p - 1) * (q - 1);

    int e = 2;
    while (e < phi && gcd(e, phi) != 1) {
        e++;
    }

    int d = modInverse(e, phi);
    if (d == -1) {
        exit(1);
    }

    value.push_back(n);
    value.push_back(e);
    value.push_back(d);
    return value;
}

int main() {
    cout << "Choose two prime numbers: ";
    int p, q;
    cin >> p >> q;
    cin.ignore();

    cout << "Enter the message: ";
    string msg;
    getline(cin, msg);

    vector<int> val = generateKeys(p, q);

    cout << "Public key: (" << val[0] << "," << val[1] << ")\n";
    cout << "Private key: (" << val[0] << "," << val[2] << ")\n";

    string cipherText = encrypt(msg, val[1], val[0]);
    cout << "Encrypted message: " << cipherText << endl;

    vector<int> cipherNumbers;
    string temp = "";
    for (char c : cipherText) {
        if (c == ' ') {
            cipherNumbers.push_back(stoi(temp));
            temp = "";
        } else {
            temp += c;
        }
    }

    string decryptedText = decrypt(cipherNumbers, val[2], val[0]);
    cout << "Decrypted message: " << decryptedText << endl;

    return 0;
}
