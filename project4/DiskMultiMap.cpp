//
//  DiskMultiMap.cpp
//  cs32project4
//
//  Created by Tanzeela Khan on 3/5/16.
//  Copyright © 2016 me. All rights reserved.
//

#include "DiskMultiMap.h"
#include <functional>
#include "MultiMapTuple.h"


DiskMultiMap::DiskMultiMap()
{
    sb.m_head = sizeof(SuperBlock);
    sb.m_cursor = sizeof(SuperBlock);
    sb.m_endOfHash = sizeof(SuperBlock);
    
}
DiskMultiMap::~DiskMultiMap()
{
    
}
bool DiskMultiMap::createNew(const std::string& filename, unsigned int numBuckets)
{
    
    if (bf.isOpen()) //close file if open
    {
        bf.close();
    }
    
    if(!bf.createNew(filename))
       cout << "Cannot open file " << filename << endl;

    
    
    sb.m_nBuckets = numBuckets;
    
    sb.m_cursor += numBuckets * sizeof(Bucket); //cursor will continue moving after nodes
    sb.m_endOfHash += numBuckets * sizeof(Bucket); //endOfHash stops after buckets
    bf.write(sb, 0); //write the superblock to the beginning

    
    BinaryFile::Offset temp = sb.m_head; //for loop to write empty blocks to binary file
    for (int i = 0; i < numBuckets; i++)
    {
        Bucket bu;
        bf.write(bu, temp);
        temp += sizeof(Bucket);
    }
    
    return true;
}
bool DiskMultiMap::openExisting(const std::string& filename)
{
    bf.openExisting(filename);
    if (bf.read(sb, 0))
        return true;
    return false;
}
void DiskMultiMap::close()
{
    if (bf.isOpen())
        bf.close();
}
bool DiskMultiMap::insert(const std::string& key, const std::string& value, const std::string& context)
{
    
    if (key.size() > 120 || value.size() > 120 || context.size() >120)
        return false;
    
    
    std::size_t val = std::hash<string>()(key); //choose a hashed bucket
    
    Bucket bu;
    DiskNode toAdd;
    DiskNode newNode;

    strcpy(toAdd.m_key, key.c_str());
    strcpy(toAdd.m_value, value.c_str());
    strcpy(toAdd.m_context, context.c_str());
    
    int chosenBuckOff = (val%sb.m_nBuckets) * sizeof(Bucket) + sizeof(SuperBlock);
    
    bf.read(bu, chosenBuckOff); //get our bucket!

    //ADDED
    if (bu.m_headOfNodes == 0) //adding first node to bucket!
    {
        bu.m_headOfNodes = bf.fileLength();
        toAdd.m_next = 0;
        bf.write(toAdd, bf.fileLength());
        bf.write(bu, chosenBuckOff);
    }
    
    else {
    toAdd.m_next = bu.m_headOfNodes;
    bu.m_headOfNodes = bf.fileLength();
    

//    BinaryFile::Offset temp = bu.m_headOfNodes;
//    bf.read(toAdd, temp);

    bf.write(bu, chosenBuckOff);
    bf.write(toAdd, bf.fileLength());
    }
    
    sb.m_cursor += sizeof(DiskNode); //change the  cursor to end of node
        return true;
}

DiskMultiMap::Iterator DiskMultiMap::search(const std::string& key)
{
    
    Bucket bu;
    std::size_t val = std::hash<string>()(key); //choose a hashed bucket
    int chosenBuckOff = (val%sb.m_nBuckets) * sizeof(Bucket) + sizeof(SuperBlock);
//
//    int chosenBuck = val%sb.m_nBuckets + sb.m_head; //chosenBuck is the offset of our chosenBuck descieded by hash function
    bf.read(bu, chosenBuckOff); //get our bucket!
    
    DiskNode newNode;
    //DiskNode saveNode;
    
    BinaryFile::Offset currNodesOff = bu.m_headOfNodes;
    
    //cout << "gonna enter search's while" << endl;
    if (bu.m_headOfNodes == 0)
    {
        DiskMultiMap::Iterator i;//(key, newNode, 0, this);
        return i;
    }
    
    bf.read(newNode, bu.m_headOfNodes);

    DiskMultiMap::Iterator i(key, newNode.m_value, newNode.m_context, newNode, currNodesOff, this);
    //DiskMultiMap::Iterator i(key, newNode, currNodesOff, this);
    return i;
}

int DiskMultiMap::erase(const std::string& key, const std::string& value, const std::string& context)
{
    DiskNode daNode;
    DiskNode prev;
    Bucket bu;
    int counter = 0;
    
    
    std::size_t val = std::hash<string>()(key); //choose a hashed bucket
    int chosenBuckOff = (val%sb.m_nBuckets) * sizeof(Bucket) + sizeof(SuperBlock);
    
    bf.read(bu, chosenBuckOff); //get our bucket!
    //found bucket with certain while

    BinaryFile::Offset daNodeLoc = bu.m_headOfNodes;
    BinaryFile::Offset prevLoc = 0;
    
    while(daNodeLoc != 0)
    {
        bf.read(daNode, daNodeLoc);
        if (strcmp(daNode.m_key, key.c_str()) == 0&&
        strcmp(daNode.m_value, value.c_str()) == 0 &&
        strcmp(daNode.m_context, context.c_str()) == 0)
        {
            //DELETE PART
            if (prevLoc == 0)
            {
                daNodeLoc = daNode.m_next;
                bu.m_headOfNodes = daNodeLoc;
                bf.write(bu, chosenBuckOff);
            }
            else  //not at the beginning of the linked list
            {
                bf.read(prev, prevLoc);
                prev.m_next = daNode.m_next;
                daNode.m_next = 0;
                bf.write(prev, prevLoc);
                bf.write(daNode, daNodeLoc);
                daNodeLoc = prev.m_next;
            }
            counter++;
        }
        
        else {
            prevLoc = daNodeLoc;
            daNodeLoc = daNode.m_next;
        }
    }
    
    return counter;
}



DiskMultiMap::Iterator::Iterator()
{
    m_iter = 0; //setting iterator to null
}

DiskMultiMap::Iterator::Iterator(const std::string& key, DiskNode &node, BinaryFile::Offset nodesOffset, DiskMultiMap* outer)
{

    m_node = node;
    m_key = key;
    
    get = outer;
    
    m_iter = nodesOffset;
}

DiskMultiMap::Iterator::Iterator(const std::string& key, const std::string& value, const std::string& context, DiskNode &node, BinaryFile::Offset nodesOffset,DiskMultiMap* outer)
{
    m_node = node;
    m_key = key;
    m_value = value;
    m_context = context;
    get = outer;
    m_iter = nodesOffset;
}




// You may add additional constructors
bool DiskMultiMap::Iterator::isValid() const
{
    if (m_iter != 0)
        return true;
    return false;
}
DiskMultiMap::Iterator& DiskMultiMap::Iterator::operator++()
{
    DiskNode curDiskNode;
    if (!isValid())
        return *this;
    
    //BinaryFile::Offset storeInitIter = m_iter;
    //BinaryFile::Offset storeNextIter;

    get->bf.read(curDiskNode, m_iter);
    //const char* storeInitKey = curDiskNode.m_key; //THIS IS REFERENCE OF THE KEY YOU R TO REMEM
    
    if (curDiskNode.m_next != 0)
    {
       // get->bf.read(curDiskNode, curDiskNode.m_next);
        m_iter = curDiskNode.m_next;
    }
    else
        m_iter = 0;
    

    return *this;
}
MultiMapTuple DiskMultiMap::Iterator::operator*()
{
    DiskNode curDiskNode;
    MultiMapTuple locMMT;
    
    if (!isValid())
    {
        return locMMT;
    }
    
    get->bf.read(curDiskNode, m_iter);
    locMMT.key = curDiskNode.m_key;
    locMMT.context = curDiskNode.m_context;
    locMMT.value = curDiskNode.m_value;

    
    return locMMT;
}


void DiskMultiMap::printAll()
{
    DiskNode curr;

    for(int i = 0; i<sb.m_nBuckets; ++i)
    {
        Bucket buck;
        BinaryFile::Offset buckOff;
        buckOff = sb.m_head+ sizeof(Bucket)*i;
        bf.read(buck, buckOff);
        BinaryFile::Offset head= buck.m_headOfNodes;
       while (head != 0)
        {
            bf.read(curr, head);
            cout << curr.m_key << " " << curr.m_value << " " << curr.m_context << " m_next " << curr.m_next << endl;
            head = curr.m_next;
        }
        
    }
}
