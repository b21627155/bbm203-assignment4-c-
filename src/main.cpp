#include <iostream>
#include <fstream>
#include <iterator>
#include <map>
#include <vector>

using namespace std;

vector<string> keys;
vector<string> values;
map<string,string> keyAndvalue;

ofstream outputFile;

struct TrieNode{
    struct TrieNode *children[26];

    bool leaf;
};

struct TrieNode *getNode(){
    struct TrieNode *pNode =  new TrieNode;

    pNode->leaf = false;

    for (int i = 0; i < 26; i++) {
        pNode->children[i] = NULL;
    }

    return pNode;
}

void insert(struct TrieNode *root, string key){
    struct TrieNode *pCrawl = root;

    for (int i = 0; i < key.length(); i++){
        int index = key[i] - 'a';
        if (!pCrawl->children[index]) {
            pCrawl->children[index] = getNode();
        }
        pCrawl = pCrawl->children[index];
    }

    pCrawl->leaf = true;
}

bool search(struct TrieNode *root, string key){
    struct TrieNode *pCrawl = root;

    for (int i = 0; i < key.length(); i++){
        int index = key[i] - 'a';
        if (!pCrawl->children[index]) {
            return false;
        }
        pCrawl = pCrawl->children[index];
    }
    return (pCrawl->leaf);
}

bool isEmpty(TrieNode* root){
    for (int i = 0; i < 26; i++){
        if (root->children[i]) {
            return false;
        }
    }
    return true;
}

TrieNode* remove(TrieNode* root, string key, int depth = 0){
    if (!root) {
        return NULL;
    }

    if (depth == key.size()) {
        if (root->leaf) {
            root->leaf = false;
        }
        if (isEmpty(root)) {
            delete (root);
            root = NULL;
        }
        return root;
    }

    int index = key[depth] - 'a';
    root->children[index] = remove(root->children[index], key, depth + 1);

    if (isEmpty(root) && root->leaf == false) {
        delete (root);
        root = NULL;
    }
    return root;
}

bool LeafNode(struct TrieNode* root){
    return root->leaf != false;
}

void printTrie(struct TrieNode* root, char str[], int level){
    if (LeafNode(root)){
        str[level] = '\0';
        outputFile <<"-" <<str <<"( "<< keyAndvalue[str]<<" )" << endl;
    }

    for (int i = 0; i < 26; i++){
        if (root->children[i]){
            str[level] = i + 'a';
            printTrie(root->children[i], str, level + 1);
        }
    }
}

int main(int x, char** argv) {
    ifstream inputFile;
    inputFile.open(argv[1]);
    outputFile.open(argv[2]);
    string line;
    string key;
    string value;
    string searchKey;
    string deleteKey;
    int level=0;
    char str[26];

    struct TrieNode *root = getNode();

    while (getline(inputFile,line)) {
        string y ;
        y = line[0];
        if(y == "i"){
            key = line.substr(7,line.find(',')-7);
            value = line.substr(line.find(',')+1,line.find(')')-line.find(',')-1);

            if(search(root, key) == true){
                outputFile<< "\""<< key<< "\""<< " already exist\n";
            }
            else{
                outputFile<< "\""<< key<< "\""<< " was added"<< "\n";
                insert(root, key);
                keyAndvalue.insert(pair<string,string>(key,value));
                keys.push_back(key);
                values.push_back(value);
            }
        }
        else if(y == "s"){
            searchKey = line.substr(7,line.find(')')-7);
            if(search(root, searchKey)==true){
                outputFile<<  "The English equivalent is "<< keyAndvalue[searchKey]<< "\n";
            }

            else{
                outputFile<< "no record\n";

            }
        }
        else if(y == "d"){
            deleteKey = line.substr(7,line.find(')')-7);
            if(search(root, deleteKey)==true){
                outputFile<< "\"" <<deleteKey << "\"" <<" deletion is successful" <<"\n";
                remove(root, deleteKey);

            }

            else{
                outputFile<<  "no record\n";
            }
        }
        else{
            printTrie(root, str,level);
        }
    }
    outputFile.close();
    return 0;
}
