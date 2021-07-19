#include "hashmap.h"

/**
 * Allocates dynamically new hash map element.
 * @param func a function which "hashes" keys.
 * @return pointer to dynamically allocated hashmap.
 * @if_fail return NULL.
 */
hashmap *hashmap_alloc (hash_func func)
{
  if (func == NULL)
    {
      return NULL;
    }
  hashmap *new_hashmap = malloc (sizeof *new_hashmap);
  if (new_hashmap == NULL)
    {
      return NULL;
    }
  // initialize with calloc in order to set all vectors to NULL.
  new_hashmap->buckets = calloc (HASH_MAP_INITIAL_CAP, sizeof (vector *));
  if (new_hashmap->buckets == NULL)
    {
      free (new_hashmap);
      return NULL;
    }

  new_hashmap->size = 0;
  new_hashmap->capacity = HASH_MAP_INITIAL_CAP;
  new_hashmap->hash_func = func;

  return new_hashmap;
}

/**
 * Frees a hash map and the elements the hash map itself allocated.
 * @param p_hash_map pointer to dynamically allocated pointer to hash_map.
 */
void hashmap_free (hashmap **p_hash_map)
{
  if (p_hash_map == NULL)
    { return; }
  if ((*p_hash_map)->buckets == NULL)
    {
      free (*p_hash_map);
      *p_hash_map = NULL;
      return;
    }
  for (size_t i = 0; i < (*p_hash_map)->capacity; i++)
    {
      vector *cur_vec = (*p_hash_map)->buckets[i];
      vector_free (&cur_vec);
    }
  free ((*p_hash_map)->buckets);
  free (*p_hash_map);
  *p_hash_map = NULL;
}

/**
 * fill the pairs array with the original pairs from the map, and free the
 * vectors.
 * @param hash_map the hash map to re-size.
 * @param pairs dynamical array of pairs to fill.
 */
void create_pairs (hashmap *hashmap_p, pair **pairs)
{
  int k = 0;
  for (size_t i = 0; i < hashmap_p->capacity; i++)
    {
      if (hashmap_p->buckets[i] != NULL)
        {
          for (size_t j = 0; j < hashmap_p->buckets[i]->size; j++)
            {
              pairs[k] = (pair *) pair_copy (hashmap_p->buckets[i]->data[j]);
              k++;
            }
          vector_free (&hashmap_p->buckets[i]);
        }
    }
}

/**
 * change the hash map according to the load factor after insertion or
 * deleting.
 * The function creates new buckets list iin the correct capacity and inserts
 * the pairs to it.
 * @param hash_map the hash map to be inserted with new element.
 * @param flag flag represents if we need to increase or decrease the map.
 * 0 = increase, 1 = decrease.
 * @return returns 1 for successful, 0 otherwise.
 */
int change_map (hashmap *hashmap_p, int flag)
{
  // create san array contains copies of the pairs in the map, in order to
  // insert them later to new buckets.
  pair **pairs = malloc (hashmap_p->size * sizeof (pair *));
  if (pairs == NULL)
    {
      return 0;
    }
  create_pairs (hashmap_p, pairs);
  free (hashmap_p->buckets);
  hashmap_p->buckets = NULL;

  if (flag == 0)
    {
      hashmap_p->buckets = calloc (
          hashmap_p->capacity * HASH_MAP_GROWTH_FACTOR, sizeof (vector *));
    }
  else
    {
      hashmap_p->buckets = calloc (
          hashmap_p->capacity / HASH_MAP_GROWTH_FACTOR, sizeof (vector *));
    }
  if (hashmap_p->buckets == NULL)
    {
      return 0;
    }
  if (flag == 0)
    { hashmap_p->capacity *= HASH_MAP_GROWTH_FACTOR; }
  else
    { hashmap_p->capacity /= HASH_MAP_GROWTH_FACTOR; }

  // insert all pairs to new buckets. initialize the size to 0 before it.
  size_t n = hashmap_p->size;
  hashmap_p->size = 0;
  for (size_t i = 0; i < n; i++)
    {
      hashmap_insert (hashmap_p, pairs[i]);
      pair_free ((void **) &pairs[i]);
    }
  // free the pairs array.
  free (pairs);
  return 1;
}

/**
 * Inserts a new in_pair to the hash map.
 * The function inserts *new*, *copied*, *dynamically allocated* in_pair,
 * NOT the in_pair it receives as a parameter.
 * @param hash_map the hash map to be inserted with new element.
 * @param in_pair a in_pair the hash map would contain.
 * @return returns 1 for successful insertion, 0 otherwise.
 */
int hashmap_insert (hashmap *hash_map, const pair *in_pair)
{
  if (hash_map == NULL || in_pair == NULL || in_pair->key == NULL)
    {
      return 0;
    }
  // check if the key is already in the map.
  if (hashmap_at (hash_map, in_pair->key) != NULL)
    { return 0; }

  size_t ind = (hash_map->hash_func (in_pair->key)) & (hash_map->capacity - 1);
  // if the dest vector is still null (empty) we alloc a new one.
  if (hash_map->buckets[ind] == NULL)
    {
      hash_map->buckets[ind] = vector_alloc (pair_copy, pair_cmp, pair_free);
      if (hash_map->buckets[ind] == NULL)
        {
          return 0;
        }
    }
  if (vector_push_back (hash_map->buckets[ind], in_pair) == 0)
    {
      return 0;
    }
  hash_map->size++;

  // check if the load factor is too big, if it is, change the map.
  if (hashmap_get_load_factor (hash_map) > HASH_MAP_MAX_LOAD_FACTOR)
    {
      change_map (hash_map, 0);
    }
  return 1;
}

/**
 * The function returns the value associated with the given key.
 * @param hash_map a hash map.
 * @param key the key to be checked.
 * @return the value associated with key if exists, NULL otherwise
 * (the value itself, not a copy of it).
 */
valueT hashmap_at (const hashmap *hash_map, const_keyT key)
{
  if (hash_map == NULL || key == NULL)
    {
      return NULL;
    }
  size_t ind = hash_map->hash_func (key) & (hash_map->capacity - 1);
  vector *vec = hash_map->buckets[ind];
  // if the dest vector is null, the key is not in the map - return NULL.
  if (vec == NULL)
    { return NULL; }
  // if not null, check if it is one of it's pairs.
  for (size_t j = 0; j < vec->size; j++)
    {
      pair *cur_pair = vec->data[j];
      if (cur_pair->key_cmp (cur_pair->key, key))
        {
          return cur_pair->value;
        }
    }
  return NULL;
}

/**
 * The function erases the pair associated with key.
 * @param hash_map a hash map.
 * @param key a key of the pair to be erased.
 * @return 1 if the erasing was done successfully, 0 otherwise.
 * (if key not in map, considered fail).
 */
int hashmap_erase (hashmap *hash_map, const_keyT key)
{
  if (hash_map == NULL || key == NULL)
    {
      return 0;
    }
  size_t ind = (hash_map->hash_func (key)) & (hash_map->capacity - 1);
  // if the dest vector is null, the key is not in the map - return 0.
  if (hash_map->buckets[ind] == NULL)
    { return 0; }
  for (size_t i = 0; i < hash_map->buckets[ind]->size; i++)
    {
      pair *pair = hash_map->buckets[ind]->data[i];
      if (pair->key_cmp (key, pair->key))
        {
          vector_erase (hash_map->buckets[ind], i);
          hash_map->size--;
          // if the load factor is too small, change the map.
          if (hashmap_get_load_factor (hash_map) < HASH_MAP_MIN_LOAD_FACTOR)
            {
              change_map (hash_map, 1);
            }
          return 1;
        }
    }
  // the key was not in the map, return 0.
  return 0;
}

/**
 * This function returns the load factor of the hash map.
 * @param hash_map a hash map.
 * @return the hash map's load factor, -1 if the function failed.
 */
double hashmap_get_load_factor (const hashmap *hash_map)
{
  if (hash_map == NULL)
    {
      return -1;
    }
  return hash_map->size / (double) hash_map->capacity;
}

/**
 * This function receives a hashmap and 2 functions, the first checks a
 * condition on the keys, and the seconds apply some modification on the
 * values.
 * The function should apply the modification
 * only on the values that are associated with keys that meet the condition.
 *
 * Example: if the hashmap maps char->int, keyT_func checks if the char is a
 * capital letter (A-Z), and val_t_func multiples the number by 2,
 * hashmap_apply_if will change the map:
 * {('C',2),('#',3),('X',5)}, to: {('C',4),('#',3),('X',10)},
 * and the return value will be 2.
 * @param hash_map a hashmap
 * @param keyT_func a function that checks a condition on keyT and return 1
 * if true, 0 else
 * @param valT_func a function that modifies valueT, in-place
 * @return number of changed values
 */
int
hashmap_apply_if (const hashmap *hash_map, keyT_func keyT_func,
                  valueT_func valT_func)
{
  // variable that represents the number of changed values.
  int counter = 0;
  if (hash_map == NULL || keyT_func == NULL || valT_func == NULL)
    {
      return -1;
    }
  for (size_t i = 0; i < hash_map->capacity; i++)
    {
      if (hash_map->buckets[i] != NULL)
        {
          for (size_t j = 0; j < hash_map->buckets[i]->size; j++)
            {
              pair *cur_pair = (hash_map->buckets[i])->data[j];
              if (cur_pair != NULL)
                {
                  if (keyT_func (cur_pair->key))
                    {
                      valT_func (cur_pair->value);
                      counter++;
                    }
                }
            }
        }
    }
  return counter;
}
