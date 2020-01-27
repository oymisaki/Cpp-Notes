#include <list>
#include <unordered_map>

using namespace std;

class LRUCache
{
    size_t m_cap;
    unordered_map<int, list<pair<int, int>>::iterator> m_map;
    list<pair<int, int>> m_list;

public:
    LRUCache(size_t capacity) : m_cap(capacity)
    {
    }

    int get(int key)
    {
        auto found_iter = m_map.find(key);
        if (found_iter == m_map.end()) //key doesn't exist
            return -1;

        m_list.splice(m_list.begin(), m_list, found_iter->second); //move the node corresponding to key to front
        return found_iter->second->second;
    }

    int set(int key, int value)
    {
        auto found_iter = m_map.find(key);
        if (found_iter != m_map.end()) //key exists
        {
            m_list.splice(m_list.begin(), m_list, found_iter->second); //move the node corresponding to key to front
            found_iter->second->second = value;                        //update value of the node
            return;
        }

        if (m_map.size() == m_cap) //reached capacity
        {
            int key_to_del = m_list.back().first;
            m_list.pop_back();       //remove node in list;
            m_map.erase(key_to_del); //remove key in map
        }
        m_list.emplace_front(key, value); //create new node in list
        m_map[key] = m_list.begin();      //create correspondence between key and node
    }
}