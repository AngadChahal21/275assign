#ifndef MYMAP_H
#define MYMAP_H

#include <vector>
#include <utility>

template <typename K, typename V>
class Map {
  private:
  std::vector<std::pair<K, V>> entries;

  //helper
  std::size_t findIndex(const K& key) const{
    //binary search
    std::size_t left = 0;
    std::size_t right = entries.size();

    while(left < right){
      std::size_t mid = left + (right - left)/2;

      if(entries[mid].first < key){
        left = mid + 1;
      }
      else{
        right = mid;
      }
    }
    return left;
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
    std::size_t index = findIndex(key);

    //if key exists
    if (index < entries.size() &&
        !(entries[index].first < key) &&
        !(key < entries[index].first)) {
        return entries[index].second;
    }

    //otherwise
    entries.insert(entries.begin() + index, std::pair<K,V>(key, V{}));
    return entries[index].second;

  }

  V operator()(const K& key) const{
    std::size_t index = findIndex(key);

    //if key exists
    if (index < entries.size() &&
        !(entries[index].first < key) &&
        !(key < entries[index].first)) {
        return entries[index].second;
    }

    //otherwise
    return V{};
  }

  class Iterator {
   private:
    const Map* mPtr;
    std::size_t index;

   public:
   //default ctor
    Iterator(const Map* map, std::size_t index) :
    mPtr{map}, index{index} {}

    //isEqual check
    bool operator!=(const Iterator& other) const{
      return mPtr != other.mPtr || index != other.index; 
    }

    //iterate 
    Iterator& operator++(){
      ++index;
      return *this;
    }

    //dereference
    const K& operator*() const{
      return mPtr->entries[index].first;
    }
  };

  Iterator begin() const{
    return Iterator(this, 0);
  }
  Iterator end() const{
    return Iterator(this, entries.size());
  }
};

#endif
