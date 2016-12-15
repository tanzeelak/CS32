//
//  IntelWeb.cpp
//  cs32project4
//
//  Created by Tanzeela Khan on 3/8/16.
//  Copyright © 2016 me. All rights reserved.
//

#include "IntelWeb.h"
#include <iostream> // needed for any I/O
#include <fstream>  // needed in addition to <iostream> for file I/O
#include <sstream>  // needed in addition to <iostream> for string stream I/O
#include <string>
//#include <unordered_map>
using namespace std;

IntelWeb::IntelWeb()
{

}
IntelWeb::~IntelWeb()
{
    close();
}
bool IntelWeb::createNew(const std::string& filePrefix, unsigned int maxDataItems)
{
    //maxdataItems/x = load factor;
    //create a bunch of maps;
    freq.createNew(filePrefix + "-file-freq-hash-table.dat", maxDataItems/.75);

    forward.createNew(filePrefix + "-file-forward-hash-table.dat", maxDataItems/.75);
    reverse.createNew(filePrefix + "-file-reverse-hash-table.dat", maxDataItems/.75);
    //freq.createNew(filePrefix + "-file-freq-hash-table.dat", maxDataItems/.75);
    
    m_loadFactor = maxDataItems/.75; //number of buckets!
    m_nMaxDataItems = maxDataItems;
    return true;
}
bool IntelWeb::openExisting(const std::string& filePrefix)
{
    forward.openExisting(filePrefix + "-file-forward-hash-table.dat");
    reverse.openExisting(filePrefix + "-file-reverse-hash-table.dat");
    freq.openExisting(filePrefix + "-file-freq-hash-table.dat");
    
    return true;
}
void IntelWeb::close()
{
    forward.close();
    reverse.close();
    freq.close();
}




bool IntelWeb::ingest(const std::string& telemetryFile)
{
	//	   Open the file for input
    //////cout << "hello2" << endl;
    ifstream inf(telemetryFile);
		  // Test for failure to open
    if ( ! inf)
    {
        ////////cout << "Cannot open expenses file!" << endl;
        return 1;
    }
    
		  // This will hold the expense totals
         // Read each line.  The return value of getline is treated
		  // as true if a line was read, false otherwise (e.g., because
		  // the end of the file was reached).
    string line;
    //////cout << "about2enter!" << endl;
    while (getline(inf, line))
    {
        //////cout << "inside first whle" << endl;
        // To extract the information from the line, we'll
        // create an input stringstream from it, which acts
        // like a source of input for operator>>
        istringstream iss(line);
        string key, value, context;
        // The return value of operator>> acts like false
        // if we can't extract a word followed by a number
        
        //////cout << "about to enter iss if loop" << endl;

        
        if ( ! (iss >> key >> value >> context) )
        {
            //////cout << "entered iss if loop" << endl;
            //////cout << "Ignoring badly-formatted input line: " << line << endl;
            continue;
            
        }
//        //cout << "what's in line "  << endl << line << endl;
//        //cout << "======" <<endl;
//        ////cout << "entered iss dummy loop" << endl;
        
        
        forward.insert(value, context, key); //using second string as key but remembering computer
        //freq.insert(value, context, key);t
        
//        //cout << "what's in forward" << endl;
//        forward.printAll();
//        //cout << "======" <<endl;

        
        DiskMultiMap::Iterator tempFor = forward.search(value);
        ////cout << "inserted this key into forward: " << (*tempFor).key << std::endl;
        ////cout << "inserted this value into forward: " << (*tempFor).value << std::endl;
        
        
//        DiskMultiMap::Iterator tempFreq = freq.search(value);
//        ////cout << "inserted this key into freq: " << (*tempFreq).key << std::endl;

        reverse.insert(context, value, key); //using third string as key but remembering computer
        DiskMultiMap::Iterator tempRev = reverse.search(context);
        ////cout << "inserted this key into reverse: " << (*tempRev).key << std::endl;
        ////cout << "inserted this key into reverse: " << (*tempRev).value << std::endl;

        
        
        //DELETE LATER
        freq.insert("", "1", "");
//        ////cout << "freq's inserted value" << value <<  endl;
//        //freq is only once!
//        DiskMultiMap::Iterator tempFreq = freq.search(value);
//        ////cout << "inserted this key into frequency: " << (*tempFreq).key << endl;

        
        DiskMultiMap::Iterator iterFor = freq.search(value);
        DiskMultiMap::Iterator iterRev = freq.search(context);
        
        
        if (!iterFor.isValid()) //nothing in map
        {
            freq.insert(value, "1", "");
            ////cout << "freq's inserted value" << value <<  endl;
            //freq is only once!
            DiskMultiMap::Iterator tempFreq = freq.search(value);
            ////cout << "inserted this key into frequency: " << (*tempFreq).key << endl;
        }
        else
        {
            ////cout << "entered else! " << endl;
            while (iterFor.isValid()) //while not at end of linked list in bucket
            {
                ////cout << "entered forwards's while!" << endl;
                int freqFor = atoi(((*iterFor).value).c_str());
                if ((*iterFor).key == value)
                {
                    freq.erase(value, std::to_string(freqFor), "");
                    freq.insert(value, std::to_string(freqFor+1), "");
                    break;
                }
                else
                {
                    ++iterFor;
                }
            }
        }
        ////cout << "left forward's while for sure" << endl;
        
        if (!iterRev.isValid())
        {
            freq.insert(context, "1", "");
            //freq is only once!
        }
        else
        {
            while (iterRev.isValid())
            {
                int freqRev = atoi(((*iterRev).value).c_str());
                if ((*iterRev).key == context)
                {
                    freq.erase(context, std::to_string(freqRev), "");
                    freq.insert(context, std::to_string(freqRev+1), "");
                    break;
                }
                else
                {
                    ++iterRev;
                }
            }
        }
        
        ////cout << "about to enter iss dummy loop" << endl;
        ////cout << line << endl;
        
        
        ////cout << "left all loop" << endl;
        // If we want to be sure there are no other non-whitespace
        // characters on the line, we can try to continue reading
        // from the stringstream; if it succeeds, extra stuff
        // is after the double.
        
    }
    
	    return true;
}
unsigned int IntelWeb::crawl(const std::vector<std::string>& indicators,
                   unsigned int minPrevalenceToBeGood,
                   std::vector<std::string>& badEntitiesFound,
                   std::vector<InteractionTuple>& badInteractions
                   )
{
    
    
    //prepare sets to copy values
    set<std::string> badEntSet; //going to copy values here
    set<InteractionTuple> badInterSet; //already empty!
    queue<std::string> foundBad;
    
    badInteractions.clear();
    
    //add all indicators to queue
    std::vector<std::string>::const_iterator toAdd = indicators.begin();
    for (; toAdd != indicators.end(); toAdd++)
    {
        foundBad.push((*toAdd));
        
        ////cout << "adding indicators to queue to reveal" << endl;
        
    } //toAdd no longer matters
    

    ////cout << "==================" << endl << endl;
    
    
    while (!foundBad.empty()) //foundBad's indicators
    {
        //cout << "top of queue " << foundBad.front() << endl;
        
        
        
        DiskMultiMap::Iterator fromIter = forward.search(foundBad.front()); //bucket of wanted key
        //cout << "KEY VALUE in forward's first location " << (*fromIter).key << endl;
        
        DiskMultiMap::Iterator fromIter2 = reverse.search(foundBad.front());
        //cout <<  "KEY VALUE in reverse's first location " << (*fromIter2).key << endl;
        
        
        foundBad.pop();
        ////cout << "fromIter.isValid() " <<  fromIter.isValid() << endl;
        
        
        //starting from bad indicator, want to find all bad keys associated with that location in forward
        while (fromIter.isValid()) //fromIter currnetly points to first bad location in forward
        {
            ////cout << "entered the main while loop to crawl" << endl;
            
            DiskMultiMap::Iterator getFreqLoc = freq.search((*fromIter).value); //looking at associated value
            ////cout << (*getFreqLoc).key << endl;
            
            
            if (badEntSet.find((*fromIter).value) != badEntSet.end())
            {
                ////cout << "if bad indiactor already in bad entity set" << endl;
                InteractionTuple iT;
                iT.from = (*fromIter).key;
                iT.to = (*fromIter).value;
                iT.context = (*fromIter).context;
                badInterSet.insert(iT);
                ++fromIter;
                continue;
            }


            
            int freqGot = atoi((*getFreqLoc).value.c_str());
            if (freqGot < minPrevalenceToBeGood) //found a bad value
            {
                ////cout << "frequency received from atoi " << freqGot << endl;
                
                foundBad.push((*getFreqLoc).key); //push bad key string into queue
                badEntSet.insert((*getFreqLoc).key); //add this bad key string into entities
                //cout << "(*getFreqLoc).key: " << (*getFreqLoc).key << endl;
                //                InteractionTuple iT;
//                iT.from = (*fromIter).key;
//                iT.to = (*fromIter).value;
//                iT.context = (*fromIter).context;
//                badInterSet.insert(iT);
                //fromIter = forward.search(foundBad.front());
                
            }
            ++fromIter; //looks at next bad location
        }
        
        while (fromIter2.isValid())
        {
            DiskMultiMap::Iterator getFreq2Loc = freq.search((*fromIter2).value);
            
            //need to sheck if already checked for value
            if (badEntSet.find((*fromIter2).value) != badEntSet.end())
            {
                ////cout << "if bad indiactor already in bad entity set" << endl;
                InteractionTuple iT2;
                iT2.from = (*fromIter2).key;
                iT2.to = (*fromIter2).value;
                iT2.context = (*fromIter2).context;
                badInterSet.insert(iT2);
                ++fromIter2;
                continue;
            }
            
            int freqGot2 = atoi((*getFreq2Loc).value.c_str());
            if (freqGot2 < minPrevalenceToBeGood)
            {
                ////cout << "frequency received from atoi " << freqGot2 << endl;
                foundBad.push((*getFreq2Loc).key);
                //badEntSet.insert((*getFreq2Loc).key);
//                InteractionTuple iT2;
//                iT2.from = (*fromIter2).key;
//                iT2.to = (*fromIter2).value;
//                iT2.context = (*fromIter2).context;
//                badInterSet.insert(iT2);
                //fromIter2 = reverse.search(foundBad.front());
            }
            ++fromIter2;
        }
    }
    
    //insert everything back!
    std::set<std::string>::iterator insSing = badEntSet.begin();
    std::set<InteractionTuple>::iterator insIter = badInterSet.begin();
    
    ////cout << "revealing bad entities!" << endl;
    while (insSing != badEntSet.end())
    {
        //cout << "insSing" << endl;
        badEntitiesFound.push_back(*insSing);
        ////cout << (*insSing) << endl;
        insSing++;
    }
    
    ////cout << "revealing bad interactions!" << endl;
    while (insIter != badInterSet.end())
    {
        //cout << "insIter" << endl;
        badInteractions.push_back(*insIter);
        ////cout << " from: " <<(*insIter).from << " to: " << (*insIter).to << " context: " << (*insIter).context  << endl;
        insIter++;
    }
    
    
    badEntitiesFound.push_back("hello");
    
    
    return 1;
}

bool IntelWeb::purge(const std::string& entity)
{
    
    bool purgedOnce = false;
    
    DiskMultiMap::Iterator iterFor = forward.search(entity);
    DiskMultiMap::Iterator iterRev = reverse.search(entity);
    DiskMultiMap::Iterator iterFreq = freq.search(entity);
    string killKey, killValue, killContext;

    //run through forward's!
    while (iterFor.isValid())
    {
        killKey = (*iterFor).key;
        killValue = (*iterFor).value;
        killContext = (*iterFor).context;
        
        //erase it from forward's map
        forward.erase(killKey, killValue, killContext);
        //erase its reverse form too!
        reverse.erase(killValue, killKey, killContext);
        purgedOnce = true;
        ++iterFor;
    }
    
    while (iterRev.isValid())
    {
        killKey = (*iterRev).key;
        killValue = (*iterRev).value;
        killContext = (*iterRev).context;
        
        //erase it from forward's map
        reverse.erase(killKey, killValue, killContext);
        //erase its reverse form too!
        forward.erase(killValue, killKey, killContext);
        purgedOnce = true;
        ++iterRev;
    }
    
    //take care of frequency;
    if (purgedOnce)
        freq.erase(entity, (*iterFreq).value, "");
    
    return purgedOnce;
}







