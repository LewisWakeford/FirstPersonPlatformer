#ifndef CACHE_H
#define CACHE_H

#include <string>
#include <vector>
#include <unordered_map>

template <class T>
class Cache
{
    public:
        inline Cache(){};
        inline virtual ~Cache(){};

        inline void add(std::string ref, T* object)
        {
            unsigned int index = mVector.size();
            mVector.push_back(object);
            mMap[ref] = index;
        };

        inline T* get(unsigned int index)
        {
            return mVector[index];
        };

        inline T* get(const std::string& ref)
        {
            std::unordered_map<std::string, unsigned int>::iterator index = mMap.find(ref);
            if(index != mMap.end())
            {
                return get(index->second);
            }
            else
            {
                return 0;
            }
        }

        inline void remove(const std::string& ref)
        {
            std::unordered_map<std::string, unsigned int>::iterator index = mMap.find(ref);
            if(index != mMap.end())
            {
                mVector.erase(mVector.begin + index);
            }
        }

        inline void purge()
        {
            mMap.clear();
            for(size_t i = 0; i < mVector.size(); i++)
            {
                delete mVector[i];
            }
            mVector.clear();
        }

    protected:

        std::vector<T*> mVector;
        std::unordered_map<std::string, unsigned int> mMap;

    private:
};

#endif // CACHE_H
