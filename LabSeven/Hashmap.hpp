//Davonte Littleton
//CSIII
//28 October 2022

#include <cstddef>
#include <utility>
#include <functional>
#include <vector>
#include <list>

using std::vector;
using std::list;
using std::pair;
using std::make_pair;

//////////////////////////////////////////
// hash function implemented as a class
//////////////////////////////////////////

// any Hash Class must provide 
// two methods: hash() and numBuckets().
template <typename T>
class DefaultHash {
public:
    DefaultHash(size_t numBuckets = defaultNumBuckets);
    size_t hash(const T& key) const;
    size_t numBuckets() const { return numBuckets_; }

private:
    // default number of buckets in the hash
    static const size_t defaultNumBuckets = 101;
    size_t numBuckets_;
};

template <typename T>
DefaultHash<T>::DefaultHash(size_t numBuckets) : numBuckets_(numBuckets) {}


// uses the division method for hashing.
// treats the key as a sequence of bytes, sums the ASCII
// values of the bytes, and mods the total by the number
// of buckets.
// note, this function does not work for C++ strings
template <typename T>
size_t DefaultHash<T>::hash(const T& key) const {
    size_t res = 0;
    for (size_t i = 0; i < sizeof(key); ++i) {
        const unsigned char b =
            *(reinterpret_cast<const unsigned char*>(&key) + i);
        res += b;
    }
    return res % numBuckets_;
}


////////////////////////////////////////////////
// container class
////////////////////////////////////////////////

template <typename Key, typename Value,
    typename Compare = std::equal_to<Key>,
    typename Hash = DefaultHash<Key>>
    class hashmap {

    public:
        typedef pair<const Key, Value> Element;

        // constructor
        // invokes constructors for comparison and hash objects
        hashmap(const Compare& comp = Compare(),
            const Hash& hash = Hash());

        Element* find(const Key& x);      // returns pointer to element with key x,
                                          // nullptr if not found
        //void insert(const Element& x);    // inserts the key/value pair 
        pair<Element*, bool> insert(const Element& x);
        pair<Element*, bool> erase(const Key& x);
        Value& operator[] (const Key& x); // returns reference on value of
                                          // element with key, inserts if does not exist
        void rehash(size_t n);

        size_t getNumBuckets() { return hash_.numBuckets(); }
    private:

        // helper function for various searches
        typename list<Element>::iterator findElement(const Key& x, const size_t bucket);

        size_t size_;   // number of elements in the container
        Compare comp_;  // comparison functor, equal_to by default
        Hash hash_;     // hash functor 

        // hash contents: vector of buckets
        // each bucket is a list containing key->value pairs
        vector<list<Element>> elems_;
};


////////////////////////////////////////////////
// container member functions
////////////////////////////////////////////////

// Construct elems_ with the number of buckets.
template <typename Key, typename Value, typename Compare, typename Hash>
hashmap<Key, Value, Compare, Hash>::hashmap(
    const Compare& comp, const Hash& hash) :
    size_(0), comp_(comp), hash_(hash) {
    elems_ = vector<list<Element>>(hash_.numBuckets());
}


// helper function
template <typename Key, typename Value,
    typename Compare, typename Hash>
typename list<pair<const Key, Value>>::iterator // return type
hashmap<Key, Value, Compare, Hash>::findElement(const Key& x, size_t bucket) {

    // look for the key in the bucket
    for (auto it = elems_[bucket].begin(); it != elems_[bucket].end(); ++it)
        if (comp_(it->first, x))
            return it;

    return elems_[bucket].end(); // element not found
}


// returns a pointer to the element with key x
// returns nullptr if no element with this key
template <typename Key, typename Value, typename Compare, typename Hash>
typename hashmap<Key, Value, Compare, Hash>::Element* // return value type
hashmap<Key, Value, Compare, Hash>::find(const Key& x) {

    size_t bucket = hash_.hash(x);
    auto it = findElement(x, bucket);    // use the findElement() helper   

    if (it != elems_[bucket].end())
        // found the element. Return a pointer to it.
        return &(*it); // dereference the iterator to list 
                       // then take the address of list element

    else // didn't find the element -- return nullptr
        return nullptr;
}


// finds the element with key x, inserts an
// element with that key if none exists yet. Returns a reference to
// the value corresponding to that key.

template <typename Key, typename Value, typename Compare, typename Hash>
pair<typename hashmap<Key, Value, Compare, Hash>::Element*, bool>  
  hashmap<Key, Value, Compare, Hash>::insert(const Element& x) {

    //Initializes the bucket size (same as void insert)
    size_t bucket = hash_.hash(x.first);
    auto bucketItr = findElement(x.first, bucket);

    //If-else statement to see if it reaches the end, new element if true
    //Doesn't insert if false
    if (bucketItr == elems_[bucket].end()) {
        //Process of creating new bucket
        ++size_;
        elems_[bucket].push_back(x);
        bucketItr = findElement(x.first, bucket);
        return make_pair(&(*bucketItr), true);
    }
    else return make_pair(&(*bucketItr), false); //Found element so no insert

}


template<typename Key, typename Value, typename Compare, typename Hash>
pair<typename hashmap<Key, Value, Compare, Hash>::Element*, bool>
hashmap<Key, Value, Compare, Hash>::erase(const Key& x) {
    size_t bucket = hash_.hash(x);
    auto it = findElement(x, bucket);    // try to find the element

    //If-statement to see if it is not at the end of the hashmap
    if (it != elems_[bucket].end()) {    
        //Erases the value we were looking for & decrements size.
        auto itTwo = elems_[bucket].erase(it); 
        --size_;

        //If-statement to determine where to point to next
        if (itTwo == elems_[bucket].end()) {
            //Size to store the incremented val of bucket
            size_t incrementBucket = bucket + 1;

            //Keeps going until increment bucket is >= elems_.size()
            while (incrementBucket < elems_.size()) {
                if (!elems_[incrementBucket].empty()) 
                    return make_pair(&(*elems_[incrementBucket].begin()), true);
                ++incrementBucket; //Increments until elems_[i] is empty
            }
            //Return statement if it could not find anything else to point to.
            //Last element of last bucket
            return make_pair(nullptr, true); 
        } else return make_pair(&(*it), true); //else statement for if itTwo is at the end
    }

    //Gets to this return statement if the if statement above isn't true
    return make_pair(nullptr, false);
 }


// returns reference to value of element with key x,
// inserts if does not exist
template <typename Key, typename Value, typename Compare, typename Hash>
Value& hashmap<Key, Value, Compare, Hash>::operator[] (const Key& x) {
    
    Element* found = find(x);
    if (found == nullptr) { // if key not found, create new element with empty value
        //Sets the pair to var hashPair
        auto hashPair = insert(make_pair(x, Value())); // calling default constructor on Value

        //Returning the Value() associated w/ second value of map from first of hashPair
        return hashPair.first->second; 
    }
    return found->second;
    

}

template<typename Key, typename Value, typename Compare, typename Hash>
void hashmap<Key, Value, Compare, Hash>::rehash(size_t n) {
    //If n is less than the current # of buckets, then it returns
    //and doesn't do anything to the hashmap
    if (n <= hash_.numBuckets()) return;

    //Sets hash to its new size
    hash_ = DefaultHash<Key>(n);
    list<Element> tmp;

    //Taking in every value into tmp
    for (auto& it : elems_)
        for (auto& itTwo : it) tmp.push_back(itTwo);

    //Removes everything store in elems
    elems_.clear();

    //Sets elem to a new vector of lists and puts old elements back
    elems_ = vector<list<Element>>(hash_.numBuckets());
    for (auto& insertion : tmp) {
        size_t x = hash_.hash(insertion.first);
        elems_[x].push_back(insertion);
    }
}
