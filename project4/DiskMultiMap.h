#ifndef DISKMULTIMAP_H_
#define DISKMULTIMAP_H_

#include <string>
#include "MultiMapTuple.h"
#include "BinaryFile.h"

class DiskList;

// Your private member declarations will go here
struct SuperBlock //header structure of hashtable
{
public:
    BinaryFile::Offset m_head; // instead of a DiskNode *
    BinaryFile::Offset m_endOfHash;
    BinaryFile::Offset m_cursor;
    int m_nBuckets;
};

//BinaryFile bf;
//SuperBlock sb;

struct DiskNode
{
public:
    //DiskNode(int v, BinaryFile::Offset n) : next(n) {}
    BinaryFile::Offset m_next = 0; // instead of a DiskNode *
    char m_value[121];
    char m_key[121];
    char m_context[121];
};

struct Bucket //knows of each bucket in header
{
public:
    BinaryFile::Offset m_headOfNodes = 0;  //will find nodes
};



class DiskMultiMap
{
public:
    
    class Iterator
    {
    public:
        Iterator();
        Iterator(const std::string& key, DiskNode &node, BinaryFile::Offset nodesOffset,DiskMultiMap* outer);
        Iterator(const std::string& key, const std::string& value, const std::string& context, DiskNode &node, BinaryFile::Offset nodesOffset,DiskMultiMap* outer);
        // You may add additional constructors
        bool isValid() const;
        Iterator& operator++();
        MultiMapTuple operator*();
        
    private:
        BinaryFile::Offset m_iter;
        DiskMultiMap* get;
        DiskNode m_node;
        string m_key;
        string m_value;
        string m_context;
        // Your private member declarations will go here
    };
    
    DiskMultiMap();
    ~DiskMultiMap();
    bool createNew(const std::string& filename, unsigned int numBuckets);
    bool openExisting(const std::string& filename);
    void close();
    bool insert(const std::string& key, const std::string& value, const std::string& context);
    Iterator search(const std::string& key);
    int erase(const std::string& key, const std::string& value, const std::string& context);
    
    
    
    void printAll();
    void printAllIter();
private:
    BinaryFile bf;
    SuperBlock sb;
    int del;
    //const string deleted = "";
    //DiskList m_deleted(deleted);
    
    int hashFunc(int value) const
    {
        return value % sb.m_nBuckets;
    }
    
    BinaryFile::Offset findOpen()
    {
        bf.read(del, 0); //whenever i delete, always set that place in storage to del
        
        
        return 0;
    }

    
    
    
};



#endif // DISKMULTIMAP_H_