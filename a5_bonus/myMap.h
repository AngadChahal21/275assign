#ifndef MYMAP_H
#define MYMAP_H

#include <vector>
#include <utility>

template <typename K, typename V>
class Map {
  //private 
  std::vector<std::pair<K, V>> entries;

  //helper #1
  int findKeyIndex(const K& key) const{
    //binary search
    int left = 0;
    int right = (int)entries.size();

    while(left < right){
      int mid = (left + right)/2;
      if (!(entries[mid].first < key) && !(key < entries[mid].first)) {
        return mid;
      }
      else if(entries[mid].first < key){
        left = mid + 1;
      }
      else{
        right = mid;
      }
    }
    return -1; // not found 
  }

  //helper #2
  int findInsertIndex(const K& key) const{
    //binary search
    int left = 0;
    int right = (int)entries.size();

    while(left < right){
      int mid = (left + right)/2;
      if (!(entries[mid].first < key) && !(key < entries[mid].first)) {
        return mid;
      }
      else if(entries[mid].first < key){
        left = mid + 1;
      }
      else{
        right = mid;
      }
    }
    return left; //position where key should be inserted
  }

  //public
  public:

  //big 5
  ~Map() = default;
  Map(const Map& other) = default;
  Map(Map&& other) = default;
  Map& operator=(const Map& other) = default;
  Map& operator=(Map&& other) = default;
  
  //default ctor
  Map() = default;

  //operator overloading: 
  V& operator[](const K& key){
    int index = findKeyIndex(key);

    if(index != -1){
      return entries[index].second;
    }

    int insertIndex = findInsertIndex(key);
    entries.insert(entries.begin() + insertIndex, std::pair<K,V>(key, V{}));
    return entries[insertIndex].second;

  }
  V operator()(const K& key) const{
    int index = findKeyIndex(key);

    if(index != -1){
      return entries[index].second; 
    }

    return V{};
  }

  class Iterator {
   private:
    const std::vector<std::pair<K, V>>* entriesPtr;
    unsigned long index;

   public:
   //default ctor
    Iterator(const std::vector<std::pair<K, V>>* entriesPtr, unsigned long index) :
    entriesPtr{entriesPtr}, index{index} {}

    //isEqual check
    bool operator!=(const Iterator& other) const{
      return entriesPtr != other.entriesPtr || index != other.index; 
    }

    //iterate 
    Iterator& operator++(){
      ++index;
      return *this;
    }

    //dereference
    const K& operator*() const{
      return (*entriesPtr)[index].first;
    }
  };

  Iterator begin() const{
    return Iterator(&entries, 0);
  }
  Iterator end() const{
    return Iterator(&entries, entries.size());
  }
};

#endif
