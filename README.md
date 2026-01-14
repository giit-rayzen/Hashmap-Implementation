HashMap Implementation - Complete!
Full Implementation with Chaining:
=>Core Features:

Hash function with std::hash
Collision resolution via chaining
Automatic rehashing (load factor > 0.75)
Insert, Find, Remove operations
Operator[] overload


1. What is HashMap?
Key-Value Store with O(1) lookup

"Alice" → 25
"Bob"   → 30
"Carol" → 35

Fast access by key!
2. How It Works
Step 1: Hash Function
  "Alice" → hash("Alice") → 3
  
Step 2: Store at Index
  buckets[3] = ["Alice" → 25]
  
Step 3: Lookup
  Find "Alice" → hash("Alice") → 3 → buckets[3] → 25
  Direct access! O(1)
3. Collision (Traffic Jam!)
hash("Alice") = 3
hash("Carol") = 3  ← COLLISION! 

Both want bucket[3]:
Bucket[3]: [Alice→25] → [Carol→35] → null
           ^^^^^^^^^^^^^^^^^^^^^^^^
           Chain (linked list)
4. Chaining Solution
Each bucket = Linked list

Bucket[0]: [Bob→30] → null
Bucket[1]: [ empty ]
Bucket[2]: [ empty ]
Bucket[3]: [Alice→25] → [Carol→35] → null  ← Chain!
5. Load Factor
Load Factor = Elements / Buckets

6 elements, 8 buckets → 0.75

High load → More collisions → Slower
Solution: Rehash!
6. Rehashing
When load factor > 0.75:
1. Create bigger array (2x)
2. Rehash all keys
3. Delete old array
[
Old: 4 buckets
New: 8 buckets (better distribution!)
]
