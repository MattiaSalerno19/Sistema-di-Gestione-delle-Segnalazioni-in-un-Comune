typedef struct hash *hashtable;

//Prototipi delle funzioni
hashtable newHashtable(int size);
int hash_insert(hashtable h, int key, int nuova_pos_heap);
int hashSearch(hashtable h, int key);
int hashDelete(hashtable h, int key);
void DestroyHashtable(hashtable h);

