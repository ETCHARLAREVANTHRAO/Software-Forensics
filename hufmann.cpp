#include <iostream>
#include <queue>
#include <vector>
#include <unordered_map>
#include <cctype>

using namespace std;

struct Node {
    char ch;
    int freq;
    Node *left, *right;

    Node(char c, int f) {
        ch = c;
        freq = f;
        left = right = nullptr;
    }
};

struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

void generateCodes(Node* root, string code, unordered_map<char, string>& huffmanDict) {
    if (!root) return;
    if (root->ch != '#') huffmanDict[root->ch] = code;
    generateCodes(root->left, code + "0", huffmanDict);
    generateCodes(root->right, code + "1", huffmanDict);
}

double calculateAvgBits(unordered_map<char, int>& frequencies, unordered_map<char, string>& huffmanDict, int& originalBits, int& compressedBits) {
    int totalBits = 0, totalFreq = 0;
    originalBits = 0;
    compressedBits = 0;
    
    for (auto& pair : frequencies) {
        int charFreq = pair.second;
        int charBits = huffmanDict[pair.first].length();
        totalBits += charFreq * charBits;
        totalFreq += charFreq;
        originalBits += charFreq * 8; // Assuming 8-bit ASCII encoding
        compressedBits += charFreq * charBits;
    }
    return (double)totalBits / totalFreq;
}

void buildHuffmanTree(unordered_map<char, int>& frequencies) {
    priority_queue<Node*, vector<Node*>, Compare> minHeap;
    unordered_map<char, string> huffmanDict;
    
    for (auto& pair : frequencies) {
        minHeap.push(new Node(pair.first, pair.second));
    }
    
    while (minHeap.size() > 1) {
        Node* left = minHeap.top(); 
        minHeap.pop();
        Node* right = minHeap.top(); 
        minHeap.pop();
        Node* merged = new Node('#', left->freq + right->freq);
        merged->left = left;
        merged->right = right;
        minHeap.push(merged);
    }
    
    generateCodes(minHeap.top(), "", huffmanDict);
    
    cout << "Huffman Codes:\n";
    for (auto& pair : huffmanDict) {
        cout << pair.first << ": " << pair.second << endl;
    }
    
    int originalBits, compressedBits;
    double avgBitsPerSymbol = calculateAvgBits(frequencies, huffmanDict, originalBits, compressedBits);
    
    cout << "\nAverage number of bits per symbol: " << avgBitsPerSymbol << endl;
    cout << "Original size (in bits): " << originalBits << endl;
    cout << "Compressed size (in bits): " << compressedBits << endl;
    cout << "Bits saved: " << originalBits - compressedBits << endl;
}

int main() {
    string paragraph;
    cout << "Enter a paragraph: ";
    getline(cin, paragraph);

    unordered_map<char, int> frequencies;
    for (char c : paragraph) {
        if (isalpha(c)) {
            c = tolower(c);
            frequencies[c]++;
        }
    }
    
    buildHuffmanTree(frequencies);
    return 0;
}