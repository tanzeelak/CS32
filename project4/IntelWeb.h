#ifndef INTELWEB_H_
#define INTELWEB_H_

#include "InteractionTuple.h"
#include <string>
#include <vector>
#include <queue>
#include <set>
#include "DiskMultiMap.h"
#include <fstream>  // needed in addition to <iostream> for file I/O
#include <sstream>  // needed in addition to <iostream> for string stream I/O
#include <string>
#include <unordered_map>

class IntelWeb
{
public:
    IntelWeb();
    ~IntelWeb();
    bool createNew(const std::string& filePrefix, unsigned int maxDataItems);
    bool openExisting(const std::string& filePrefix);
    void close();
    bool ingest(const std::string& telemetryFile);
    unsigned int crawl(const std::vector<std::string>& indicators,
                       unsigned int minPrevalenceToBeGood,
                       std::vector<std::string>& badEntitiesFound,
                       std::vector<InteractionTuple>& badInteractions
                       );
    bool purge(const std::string& entity);
    
private:
    // Your private member declarations will go here
    int m_nMaxDataItems;
    double m_loadFactor;
    DiskMultiMap forward, reverse, freq;
};


inline
bool operator<(const InteractionTuple& lhs, const InteractionTuple& rhs)
{
    if (lhs.from > rhs.from)
        return true;
    else
        return false;
}




#endif // INTELWEB_H_
