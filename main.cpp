//hashmap  ==jai shree Ram
/**
 * ═══════════════════════════════════════════════════════════════════════
 *                    HASHMAP CONCEPTS - COMPLETE GUIDE
 * ═══════════════════════════════════════════════════════════════════════
 * 
 * WHAT IS A HASHMAP?
 * ──────────────────
 * HashMap = Key-Value store with O(1) average lookup
 * Also called: Hash Table, Dictionary, Map
 * 
 * Example:
 *   Name → Phone Number
 *   "Alice" → "123-4567"
 *   "Bob"   → "890-1234"
 * 
 * 
 * HOW IT WORKS:
 * ─────────────
 * 1. Hash Function: Convert key to index
 *    "Alice" → hash("Alice") → 5
 *    Store at buckets[5]
 * 
 * 2. Collision: Two keys hash to same index
 *    "Alice" → 5
 *    "Carol" → 5  (collision!)
 * 
 * 3. Collision Resolution:
 *    - Chaining: Store multiple items at same index (linked list)=> lets implement this
 *    - Open Addressing: Find next available slot
 * 
 * 
 * KEY CONCEPTS:
 * ─────────────
 * 
 * 1. HASH FUNCTION
 *    Input:  Any key (string, int, object)
 *    Output: Integer (array index)
 *    
 *    Good hash function:
 *    - Fast to compute
 *    - Uniform distribution
 *    - Same input → Same output
 * 
 * 2. COLLISION
 *    When two different keys hash to same index
 *    
 *    Example:
 *    hash("Alice") = 3
 *    hash("Carol") = 3  ← Collision!
 *    
 *    Think: Two cars want same parking spot!
 * 
 * 3. CHAINING
 *    Each bucket holds a linked list
 *    
 *    Bucket 3: [Alice→123] → [Carol→456] → null
 *    
 *    Pros: Simple, no table size limit
 *    Cons: Extra memory for pointers
 * 
 * 4. LOAD FACTOR
 *    Load Factor = Size / Capacity
 *    
 *    Example: 7 items, 10 buckets → 0.7
 *    
 *    High load → More collisions → Slower
 *    Solution: Rehash (grow table)
 * 
 * 5. REHASHING
 *    When load factor > threshold (e.g., 0.75):
 *    - Create bigger table (usually 2x)
 *    - Rehash all keys to new table
 *    - Delete old table
 */
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <functional>
#include <iomanip>

using namespace std;

template<typename K,typename V>
class HashMap{
private:
// (KEY-VALUE PAIR)=one item,  and=> each Bucket store list of these
struct KeyValue{
    K key;
    V value;
    KeyValue(const K& k,const V& v):key(k),value(v){}
};

 // Array of buckets (each bucket is a linked list)
 vector<list<KeyValue>>buckets;
 
    size_t numElements;      // How many key-value pairs stored
    size_t numBuckets;       // Size of buckets array
    float maxLoadFactor;     // Threshold for rehashing
    
    size_t collisionCount;   // For statistics
    size_t rehashCount;      // How many times rehashed

/**
     * HASH FUNCTION
     * Converts key to bucket index
     * 
     * Using std::hash<K> (built-in for common types)
     * Then modulo to fit in buckets array
*/
size_t hash(const K&key)const{
    return std::hash<K>{}(key) % numBuckets;
}

/**
     * REHASH: Grow table and redistribute elements
     * Called when load factor exceeds threshold
*/
void rehash(){
    cout<<"Reahsh growing from"<<numBuckets<<"to"<<numBuckets*2<<"buckets\n";
    rehashCount++;

    //saving old buckets
    vector<list<KeyValue>>oldBuckets=std::move(buckets);

    //creating bigger table
    numBuckets*=2;
    buckets=vector<list<KeyValue>>(numBuckets);
    numElements=0;

    //Reinsert all elements
    for(const auto& bucket:oldBuckets)
    {
        for(const auto&kv:bucket)insert(kv.key,kv.value);
    }
    cout<<"Rehash done\n";

}
public:
 /**
     * CONSTRUCTOR
     * Create hash table with initial capacity
*/
    explicit HashMap(size_t initialCapacity = 8) 
        : numElements(0), 
          numBuckets(initialCapacity),
          maxLoadFactor(0.75f),
          collisionCount(0),
          rehashCount(0) {
        
        buckets.resize(numBuckets);
        cout << "[HashMap] Created with " << numBuckets << " buckets\n";
    }
    
/**
     * INSERT (or UPDATE)
     * Add key-value pair to map
     * If key exists, update value
     * 
     * Time: O(1) average, O(n) worst case
*/

 void insert(const K&key,const V&value){
    //chk if need to rehash
    if(loadFactor()>maxLoadFactor)rehash();

    size_t index=hash(key); 
    auto& bucket=buckets[index];

    //chk if alredy exist=>update
    for(auto& kv:bucket){
        if(kv.key==key){
             cout << "  [UPDATE] " << key << " → " << value 
                     << " (at bucket " << index << ")\n";
                kv.value = value;
                return;
        }
    }
    // Key doesn't exist, insert new
        if (!bucket.empty()) {
            collisionCount++;
            cout << "  💥💥[COLLISION] at bucket " << index << "!\n";
        }
        
        bucket.emplace_back(key, value);
        numElements++;
        
        cout << "  [INSERT] " << key << " → " << value 
             << " (at bucket " << index << ", size=" << numElements << ")\n";
 }
 /**
     * FIND
     * Search for key and return value
     * 
     * Time: O(1) average, O(n) worst case
     */
    bool find(const K& key, V& value) const {
        size_t index = hash(key);
        const auto& bucket = buckets[index];
        
        cout << "  [FIND] Searching for " << key 
             << " in bucket " << index << "... ";
        
        for (const auto& kv : bucket) {
            if (kv.key == key) {
                value = kv.value;
                cout << "Found! Value = " << value << "\n";
                return true;
            }
        }
        
        cout << "Not found!\n";
        return false;
    }
    
    /**
     * CONTAINS
     * Check if key exists
     */
    bool contains(const K& key) const {
        size_t index = hash(key);
        const auto& bucket = buckets[index];
        
        for (const auto& kv : bucket) {
            if (kv.key == key) {
                return true;
            }
        }
        return false;
    }
    
    /**
     * REMOVE
     * Delete key-value pair
     * 
     * Time: O(1) average, O(n) worst case
     */
    bool remove(const K& key) {
        size_t index = hash(key);
        auto& bucket = buckets[index];
        
        cout << "  [REMOVE] Deleting " << key << " from bucket " << index << "... ";
        
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (it->key == key) {
                bucket.erase(it);
                numElements--;
                cout << "Done! (size=" << numElements << ")\n";
                return true;
            }
        }
        
        cout << "Key not found!\n";
        return false;
    }
    
    /**
     * OPERATOR[]
     * Access/Insert like: map[key] = value
     */
    V& operator[](const K& key) {
        size_t index = hash(key);
        auto& bucket = buckets[index];
        
        // Find existing
        for (auto& kv : bucket) {
            if (kv.key == key) {
                return kv.value;
            }
        }
        
        // Not found, insert with default value
        if (loadFactor() > maxLoadFactor) {
            rehash();
            index = hash(key);  // Recalculate after rehash
        }
        
        bucket.emplace_back(key, V());
        numElements++;
        return bucket.back().value;
    }
    
    /**
     * UTILITY FUNCTIONS
     */
    size_t size() const { return numElements; }
    bool empty() const { return numElements == 0; }
    float loadFactor() const { return (float)numElements / numBuckets; }
    
    void clear() {
        for (auto& bucket : buckets) {
            bucket.clear();
        }
        numElements = 0;
        collisionCount = 0;
        cout << "[CLEAR] All elements removed\n";
    }
    

};

int main(){
 
    HashMap<string, int> ages(4);  // Small size to show collisions
    
    cout << "1. Inserting key-value pairs:\n";
    ages.insert("Alice", 25);
    ages.insert("Bob", 30);
    ages.insert("Charlie", 35);

    cout << "2. Finding values:\n";
    int age;
    if (ages.find("Alice", age)) {
        cout << "  Alice's age: " << age << "\n";
    }
    if (ages.find("Bob", age)) {
        cout << "  Bob's age: " << age << "\n";
    }
    
    cout << "\n3. Checking existence:\n";
    cout << "  Contains 'Alice': " << (ages.contains("Alice") ? "Yes" : "No") << "\n";
    cout << "  Contains 'David': " << (ages.contains("David") ? "Yes" : "No") << "\n";
    
    cout << "\n4. Updating value:\n";
    ages.insert("Alice", 26);  // Birthday!
    
    cout << "\n5. Removing key:\n";
    ages.remove("Bob");

    return 0;
}