#include "test_suite.h"
#include "hash_funcs.h"
#include "test_pairs.h"

void test_insert_same_pair_5_times (void)
{
  pair *pairs[5];
  char key = (char) (48);
  //even keys are capital letters, odd keys are digits
  if (key % 2)
    {
      key += 17;
    }
  int value = 0;
  for (int k = 0; k < 5; k++)
    {
      pairs[k] = pair_alloc (&key, &value, char_key_cpy, int_value_cpy,
                             char_key_cmp, int_value_cmp, char_key_free,
                             int_value_free);
      if ((pairs[k]) == NULL){return;}
    }

  // Create hash-map and inserts elements into it, using pair_char_int.h
  hashmap *map = hashmap_alloc (hash_char);
  if (map == NULL){return;}
  for (int k = 0; k < 5; ++k)
    {
      hashmap_insert (map, pairs[k]);
    }
  assert(map->size == 1);
  hashmap_free (&map);
  for (int k = 0; k < 5; k++)
    {
      pair_free ((void **) &pairs[k]);
    }
}

void test_insert_enlarge_capacity1 ()
{
  pair *pairs[13];
  for (int j = 0; j < 13; ++j)
    {
      char key = (char) (j + 48);
      //even keys are capital letters, odd keys are digits
      int value = j;
      pairs[j] = pair_alloc (&key, &value, char_key_cpy, int_value_cpy,
                             char_key_cmp, int_value_cmp, char_key_free,
                             int_value_free);
      if ((pairs[j]) == NULL){return;}
    }
  // Create hash-map and inserts elements into it, using pair_char_int.h
  hashmap *map = hashmap_alloc (hash_char);
  if (map == NULL) {return;}
  assert(map->capacity == 16);
  for (int k = 0; k < 13; ++k)
    {
      if (k==1)
        {
          assert(map->capacity == 16);
        }
      if (k == 12)
        {
          assert(map->capacity == 16);
        }
      hashmap_insert (map, pairs[k]);
    }
  assert(map->capacity == 32);
  hashmap_free (&map);
  for (int k = 0; k < 13; k++)
    {
      pair_free ((void **) &pairs[k]);
    }
}

void test_key_null ()
{
  int val = 2;
  pair null_pair = {NULL, &val, char_key_cpy, int_value_cpy, char_key_cmp,
                    int_value_cmp, char_key_free, int_value_free};

  // Create hash-map and inserts elements into it, using pair_char_int.h
  hashmap *map = hashmap_alloc (hash_char);
  if (map == NULL){return;}
  assert(hashmap_insert (map, &null_pair) == 0);
  assert(map->size == 0);
  hashmap_free (&map);
  pair_free ((void **) &null_pair);
}

void test_insert_null ()
{
  pair *pairs[5];
  for (int j = 0; j < 5; ++j)
    {
      pairs[j] = NULL;
    }
  // Create hash-map and inserts elements into it, using pair_char_int.h
  hashmap *map = hashmap_alloc (hash_char);
  if (map == NULL){return;}
  for (int k = 0; k < 5; ++k)
    {
      assert(hashmap_insert (map, pairs[k]) == 0);
    }
  assert(map->size == 0);
  hashmap_free (&map);
}

void test_insert_to_same_vec ()
{
  pair *pairs[13];
  for (int j = 0; j < 13; ++j)
    {
      char key = (char) (j + 48);
      //even keys are capital letters, odd keys are digits
      if (key % 2)
        {
          key += 17;
        }
      int value = j;
      pairs[j] = pair_alloc (&key, &value, char_key_cpy, int_value_cpy,
                             char_key_cmp, int_value_cmp, char_key_free,
                             int_value_free);
      if ((pairs[j]) == NULL){return;}
    }

  // Create hash-map and inserts elements into it, using pair_char_int.h
  hashmap *map = hashmap_alloc (hash_zero);
  if (map == NULL){return;}
  for (int k = 0; k < 13; ++k)
    {
      if (k == 12)
        {
          assert(map->buckets[0]->size == 12);
          assert(map->buckets[0]->capacity == 16);
        }
      hashmap_insert (map, pairs[k]);
    }
  assert(map->buckets[0]->size == 13);
  assert(map->buckets[0]->capacity == 32);
  assert(map->size == 13);
  assert(map->capacity == 32);
  hashmap_free (&map);
  for (int k = 0; k < 13; k++)
    {
      pair_free ((void **) &pairs[k]);
    }
}


void test_insert_enlarge_capacity2 ()
{
  pair *pairs[25];
  for (int j = 0; j < 25; ++j)
    {
      char key = (char) (j + 48);
      //even keys are capital letters, odd keys are digits
      int value = j;
      pairs[j] = pair_alloc (&key, &value, char_key_cpy, int_value_cpy,
                             char_key_cmp, int_value_cmp, char_key_free,
                             int_value_free);
      if ((pairs[j]) == NULL){return;}
    }
  // Create hash-map and inserts elements into it, using pair_char_int.h
  hashmap *map = hashmap_alloc (hash_char);
  if (map == NULL){return;}
  assert(map->capacity == 16);
  for (int k = 0; k < 25; ++k)
    {
      if (k == 12)
        {
          assert(map->capacity == 16);
        }
      if (k == 13 || k == 24)
        {
          assert(map->capacity == 32);
        }
      hashmap_insert (map, pairs[k]);
    }
  assert(map->capacity == 64);
  assert(map->size == 25);
  hashmap_free (&map);
  for (int k = 0; k < 25; k++)
    {
      pair_free ((void **) &pairs[k]);
    }
}


void test_rehash_map(){
  pair *pairs[13];
  for (int j = 0; j < 13; ++j)
    {
      int key = j + 6;
      int value = j;
      pairs[j] = pair_alloc (&key, &value, int_key_cpy, int_value_cpy,
                             int_key_cmp, int_value_cmp, int_key_free,
                             int_value_free);
      if ((pairs[j]) == NULL){return;}
    }
  hashmap *map = hashmap_alloc (hash_int);
  if (map == NULL){return;}
  for (int k = 0; k < 13; ++k)
    {
      if (k == 12){
        assert(map->buckets[0]->size == 1);
        assert(map->buckets[1]->size == 1);
        assert(map->buckets[6]->size == 1);
        assert(map->buckets[7]->size == 1);
        assert(map->buckets[8]->size == 1);
        assert(map->buckets[9]->size == 1);
        assert(map->buckets[10]->size == 1);
        assert(map->buckets[11]->size == 1);
        assert(map->buckets[12]->size == 1);
        assert(map->buckets[13]->size == 1);
        assert(map->buckets[14]->size == 1);
        assert(map->buckets[15]->size == 1);
      }
      hashmap_insert (map, pairs[k]);
    }
  assert(map->buckets[6]->size == 1);
  assert(map->buckets[7]->size == 1);
  assert(map->buckets[8]->size == 1);
  assert(map->buckets[9]->size == 1);
  assert(map->buckets[10]->size == 1);
  assert(map->buckets[11]->size == 1);
  assert(map->buckets[12]->size == 1);
  assert(map->buckets[13]->size == 1);
  assert(map->buckets[14]->size == 1);
  assert(map->buckets[15]->size == 1);
  assert(map->buckets[16]->size == 1);
  assert(map->buckets[17]->size == 1);
  assert(map->buckets[18]->size == 1);

  assert(map->size == 13 && map->capacity == 32);
  hashmap_free (&map);
  for (int k = 0; k < 13; k++)
    {
      pair_free ((void **) &pairs[k]);
    }

}




void test_insert_pairs_to_different_vectors ()
{
  pair *pairs[16];
  for (int j = 0; j < 16; ++j)
    {
      char key = (char) (j);
      //even keys are capital letters, odd keys are digits
      int value = j;
      pairs[j] = pair_alloc (&key, &value, char_key_cpy, int_value_cpy,
                             char_key_cmp, int_value_cmp, char_key_free,
                             int_value_free);
      if ((pairs[j]) == NULL){return;}
    }
  hashmap *map = hashmap_alloc (hash_char);
  if (map == NULL){return;}
  for (int k = 0; k < 16; ++k)
    {
      hashmap_insert (map, pairs[k]);
    }
  assert(map->size == 16 && map->capacity == 32);
  for (int k = 0; k < 16; ++k)
    {
      assert(map->buckets[k]->size == 1);
    }
  hashmap_free (&map);
  for (int k = 0; k < 16; k++)
    {
      pair_free ((void **) &pairs[k]);
    }
}

void test_hashmap_null ()
{
  hashmap *map = hashmap_alloc (NULL);
  assert(map == NULL);
}

/**
 * This function checks the hashmap_insert function of the hashmap library.
 * If hashmap_insert fails at some points, the functions exits with exit
 * code 1.
 */
void test_hash_map_insert (void)
{
  test_insert_same_pair_5_times ();
  test_insert_enlarge_capacity1 ();
  test_insert_enlarge_capacity2 ();
  test_insert_null ();
  test_insert_to_same_vec ();
  test_key_null ();
  test_hashmap_null ();
  test_insert_pairs_to_different_vectors ();
  test_rehash_map();
}

void test_find_in_empty_map ()
{
  hashmap *map = hashmap_alloc (hash_char);
  if (map == NULL){return;}
  keyT key = "a";
  assert(hashmap_at (map, key) == NULL);
  hashmap_free (&map);
}

void test_find_null_key ()
{
  hashmap *map = hashmap_alloc (hash_char);
  if (map == NULL){return;}
  keyT key = NULL;
  assert(hashmap_at (map, key) == NULL);
  hashmap_free (&map);
}

void test_get_correct_pairs ()
{
  pair *pairs[12];
  for (int j = 0; j < 12; ++j)
    {
      char key = (char) (j);
      int value = j;
      pairs[j] = pair_alloc (&key, &value, char_key_cpy, int_value_cpy,
                             char_key_cmp, int_value_cmp, char_key_free,
                             int_value_free);
      if ((pairs[j]) == NULL){return;}
    }
  hashmap *map = hashmap_alloc (hash_char);
  if (map == NULL){return;}
  for (int k = 0; k < 12; ++k)
    {
      hashmap_insert (map, pairs[k]);
    }
  for (int k = 0; k < 12; k++)
    {
      assert(*(int *) hashmap_at (map, pairs[k]->key) == k);
    }
  for (int k = 0; k < 12; ++k)
    {
      pair_free ((void **) &pairs[k]);
    }
  hashmap_free (&map);
}

void test_get_original_pair ()
{
  pair *pairs[2];
  for (int j = 0; j < 2; ++j)
    {
      char key = (char) (3);
      int value = j;
      pairs[j] = pair_alloc (&key, &value, char_key_cpy, int_value_cpy,
                             char_key_cmp,
                             int_value_cmp, char_key_free, int_value_free);
      if ((pairs[j]) == NULL){return;}
    }
  hashmap *map = hashmap_alloc (hash_zero);
  if (map == NULL){return;}
  for (int k = 0; k < 2; ++k)
    {
      hashmap_insert (map, pairs[k]);
    }
  assert(*(int *) hashmap_at (map, pairs[0]->key) == 0);
  pair_free ((void **) &pairs[0]);
  pair_free ((void **) &pairs[1]);
  hashmap_free (&map);

}

void test_search_do_not_exist_key ()
{
  pair *pairs[7];
  for (int j = 0; j < 7; ++j)
    {
      char key = (char) (j);
      int value = j;
      pairs[j] = pair_alloc (&key, &value, char_key_cpy, int_value_cpy,
                             char_key_cmp,
                             int_value_cmp, char_key_free, int_value_free);
      if ((pairs[j]) == NULL){return;}
    }
  hashmap *map = hashmap_alloc (hash_char);
  if (map == NULL){return;}
  for (int k = 0; k < 6; ++k)
    {
      hashmap_insert (map, pairs[k]);
    }

  assert(hashmap_at (map, pairs[6]->key) == NULL);
  for (int k = 0; k < 7; ++k)
    {
      pair_free ((void **) &pairs[k]);
    }
  hashmap_free (&map);
}

/**
 * This function checks the hashmap_at function of the hashmap library.
 * If hashmap_at fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_at (void)
{
  test_find_in_empty_map ();
  test_find_null_key ();
  test_get_correct_pairs ();
  test_get_original_pair ();
  test_search_do_not_exist_key ();
}

void test_erase_from_empty_map ()
{
  hashmap *map = hashmap_alloc (hash_char);
  if (map == NULL){return;}
  keyT key = "a";
  assert(hashmap_erase (map, key) == 0);
  hashmap_free (&map);
}

void test_erase_null_key ()
{
  hashmap *map = hashmap_alloc (hash_char);
  if (map == NULL){return;}
  assert(hashmap_erase (map, NULL) == 0);
  hashmap_free (&map);
}

void test_erase_non_exist_key ()
{
  pair *pairs[11];
  for (int j = 0; j < 10; ++j)
    {
      char key = (char) (j);
      int value = j;
      pairs[j] = pair_alloc (&key, &value, char_key_cpy, int_value_cpy,
                             char_key_cmp,
                             int_value_cmp, char_key_free, int_value_free);
      if ((pairs[j]) == NULL){return;}
    }
  char new_key = (char) 16;
  int new_value = 7;
  pairs[10] = pair_alloc (&new_key, &new_value, char_key_cpy, int_value_cpy,
                          char_key_cmp,
                          int_value_cmp, char_key_free, int_value_free);
  if ((pairs[10]) == NULL){return;}
  hashmap *map = hashmap_alloc (hash_char);
  if (map == NULL){return;}
  for (int k = 0; k < 10; ++k)
    {
      hashmap_insert (map, pairs[k]);
    }
  assert(hashmap_erase (map, &new_key) == 0);
  for (int k = 0; k < 11; ++k)
    {
      pair_free ((void **) &pairs[k]);
    }
  hashmap_free (&map);
}

void test_erase_exist_key ()
{
  pair *pairs[11];
  for (int j = 0; j < 10; ++j)
    {
      char key = (char) (j);
      int value = j;
      pairs[j] = pair_alloc (&key, &value, char_key_cpy, int_value_cpy,
                             char_key_cmp,
                             int_value_cmp, char_key_free, int_value_free);
      if ((pairs[j]) == NULL){return;}
    }
  char new_key = (char) 16;
  int new_value = 7;
  pairs[10] = pair_alloc (&new_key, &new_value, char_key_cpy, int_value_cpy,
                          char_key_cmp,
                          int_value_cmp, char_key_free, int_value_free);
  if ((pairs[10]) == NULL){return;}
  hashmap *map = hashmap_alloc (hash_char);
  if (map == NULL){return;}
  for (int k = 0; k < 11; ++k)
    {
      hashmap_insert (map, pairs[k]);
    }
  assert(map->buckets[0]->size == 2);
  assert(hashmap_erase (map, &new_key) == 1);
  assert(map->buckets[0]->size == 1);
  assert(map->buckets[0]->capacity == 8);
  assert(map->capacity == 16);
  char key = (char) 0;
  assert(hashmap_erase (map, &key) == 1);
  assert(map->buckets[0]->size == 0);
  assert(map->buckets[0]->capacity == 4);
  for (int k = 0; k < 11; ++k)
    {
      pair_free ((void **) &pairs[k]);
    }
  hashmap_free (&map);
}

void test_decrease_map1 ()
{
  pair *pairs[2];
  for (int j = 0; j < 2; ++j)
    {
      char key = (char) (j);
      int value = j;
      pairs[j] = pair_alloc (&key, &value, char_key_cpy, int_value_cpy,
                             char_key_cmp,
                             int_value_cmp, char_key_free, int_value_free);
      if ((pairs[j]) == NULL){return;}
    }
  hashmap *map = hashmap_alloc (hash_char);
  if (map == NULL){return;}
  for (int k = 0; k < 2; ++k)
    {
      hashmap_insert (map, pairs[k]);
    }
  assert(map->capacity == 16 && map->size == 2);
  hashmap_erase (map, pairs[0]->key);
  assert(map->capacity == 8 && map->size == 1);
  for (int k = 0; k < 2; ++k)
    {
      pair_free ((void **) &pairs[k]);
    }
  hashmap_free (&map);
}

void test_decrease_map2 ()
{
  pair *pairs[13];
  for (int j = 0; j < 13; ++j)
    {
      char key = (char) (j);
      int value = j;
      pairs[j] = pair_alloc (&key, &value, char_key_cpy, int_value_cpy,
                             char_key_cmp,
                             int_value_cmp, char_key_free, int_value_free);
      if ((pairs[j]) == NULL){return;}
    }
  hashmap *map = hashmap_alloc (hash_char);
  if (map == NULL){return;}
  for (int k = 0; k < 12; ++k)
    {
      hashmap_insert (map, pairs[k]);
    }
  assert(map->capacity == 16 && map->size == 12);
  hashmap_insert (map, pairs[12]);
  assert(map->capacity == 32 && map->size == 13);
  for (int k = 0; k < 6; ++k)
    {
      hashmap_erase (map, pairs[k]->key);
      if (k != 5)
        {
          assert(map->capacity == 32);
        }
    }
  assert(map->capacity == 16 && map->size == 7);
  for (int k = 6; k < 10; ++k)
    {
      hashmap_erase (map, pairs[k]->key);
      if (k != 9)
        {
          assert(map->capacity == 16);
        }
    }
  assert(map->capacity == 8 && map->size == 3);
  for (int k = 10; k < 13; ++k)
    {
      hashmap_erase (map, pairs[k]->key);
      if (k == 11)
        {
          assert(map->capacity == 4);
        }
    }
  assert(map->capacity == 2 && map->size == 0);

  for (int k = 0; k < 13; ++k)
    {
      pair_free ((void **) &pairs[k]);
    }
  hashmap_free (&map);
}

/**
 * This function checks the hashmap_erase function of the hashmap library.
 * If hashmap_erase fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_erase (void)
{
  test_erase_from_empty_map ();
  test_erase_null_key ();
  test_erase_non_exist_key ();
  test_erase_exist_key ();
  test_decrease_map1 ();
  test_decrease_map2 ();
}

void test_get_load_factor_on_empty_map ()
{
  hashmap *map = hashmap_alloc (hash_char);
  if (map == NULL){return;}
  assert(hashmap_get_load_factor (map) == 0);
  hashmap_free (&map);
}

void test_get_load_factor_of_075 ()
{
  pair *pairs[12];
  for (int j = 0; j < 12; ++j)
    {
      char key = (char) (j);
      int value = j;
      pairs[j] = pair_alloc (&key, &value, char_key_cpy, int_value_cpy,
                             char_key_cmp,
                             int_value_cmp, char_key_free, int_value_free);
      if ((pairs[j]) == NULL){return;}
    }

  hashmap *map = hashmap_alloc (hash_char);
  if (map == NULL){return;}
  for (int k = 0; k < 11; ++k)
    {
      hashmap_insert (map, pairs[k]);
    }
  assert(hashmap_get_load_factor (map) < 0.75);
  hashmap_insert (map, pairs[11]);
  assert(hashmap_get_load_factor (map) == 0.75);
  for (int k = 0; k < 12; ++k)
    {
      pair_free ((void **) &pairs[k]);
    }
  hashmap_free (&map);

}

void test_get_load_factor_of_025 ()
{
  pair *pairs[5];
  for (int j = 0; j < 5; ++j)
    {
      char key = (char) (j);
      int value = j;
      pairs[j] = pair_alloc (&key, &value, char_key_cpy, int_value_cpy,
                             char_key_cmp,
                             int_value_cmp, char_key_free, int_value_free);
      if ((pairs[j]) == NULL){return;}
    }

  hashmap *map = hashmap_alloc (hash_char);
  if (map == NULL){return;}
  for (int k = 0; k < 5; ++k)
    {
      hashmap_insert (map, pairs[k]);
    }
  assert(hashmap_get_load_factor (map) > 0.25);
  hashmap_erase (map, pairs[4]->key);
  assert(hashmap_get_load_factor (map) == 0.25);
  hashmap_erase (map, pairs[3]->key);
  assert(hashmap_get_load_factor (map) == 0.375);
  for (int k = 0; k < 5; ++k)
    {
      pair_free ((void **) &pairs[k]);
    }
  hashmap_free (&map);
}

void test_get_load_factor_after_erase ()
{
  pair *pairs[4];
  for (int j = 0; j < 4; ++j)
    {
      char key = (char) (j);
      int value = j;
      pairs[j] = pair_alloc (&key, &value, char_key_cpy, int_value_cpy,
                             char_key_cmp,
                             int_value_cmp, char_key_free, int_value_free);
      if ((pairs[j]) == NULL){return;}
    }

  hashmap *map = hashmap_alloc (hash_char);
  if (map == NULL){return;}
  for (int k = 0; k < 4; ++k)
    {
      hashmap_insert (map, pairs[k]);
    }
  assert(hashmap_get_load_factor (map) < 0.75);
  hashmap_erase (map, pairs[3]->key);
  assert(hashmap_get_load_factor (map) < 0.75);
  for (int k = 0; k < 4; ++k)
    {
      pair_free ((void **) &pairs[k]);
    }
  hashmap_free (&map);
}

void test_get_load_factor_increase_map ()
{
  pair *pairs[25];
  for (int j = 0; j < 25; ++j)
    {
      char key = (char) (j);
      int value = j;
      pairs[j] = pair_alloc (&key, &value, char_key_cpy, int_value_cpy,
                             char_key_cmp,
                             int_value_cmp, char_key_free, int_value_free);
      if ((pairs[j]) == NULL){return;}
    }

  hashmap *map = hashmap_alloc (hash_char);
  if (map == NULL){return;}
  for (int k = 0; k < 12; ++k)
    {
      hashmap_insert (map, pairs[k]);
    }
  assert(hashmap_get_load_factor (map) == 0.75);
  hashmap_insert (map, pairs[12]);
  assert(hashmap_get_load_factor (map) < 0.75);
  for (int k = 13; k < 25; ++k)
    {
      hashmap_insert (map, pairs[k]);
    }
  assert(hashmap_get_load_factor (map) < 0.75);
  for (int k = 0; k < 25; ++k)
    {
      pair_free ((void **) &pairs[k]);
    }
  hashmap_free (&map);
}

/**
 * This function checks the hashmap_get_load_factor function of the
 * hashmap library.
 * If hashmap_get_load_factor fails at some points, the functions exits with
 * exit code 1.
 */
void test_hash_map_get_load_factor (void)
{
  test_get_load_factor_on_empty_map ();
  test_get_load_factor_of_075 ();
  test_get_load_factor_after_erase ();
  test_get_load_factor_increase_map ();
  test_get_load_factor_of_025 ();

}

void test_apply_null_funcs ()
{
  pair *pairs[5];
  for (int j = 0; j < 5; ++j)
    {
      char key = (char) (j);
      int value = j;
      pairs[j] = pair_alloc (&key, &value, char_key_cpy, int_value_cpy,
                             char_key_cmp,
                             int_value_cmp, char_key_free, int_value_free);
      if ((pairs[j]) == NULL){return;}
    }

  hashmap *map = hashmap_alloc (hash_char);
  if (map == NULL){return;}
  for (int k = 0; k < 5; ++k)
    {
      hashmap_insert (map, pairs[k]);
    }
  assert(hashmap_apply_if (map, NULL, double_value) == -1);
  assert(hashmap_apply_if (map, always_true, NULL) == -1);
  assert(hashmap_apply_if (NULL, always_true, double_value) == -1);
  for (int k = 0; k < 5; ++k)
    {
      pair_free ((void **) &pairs[k]);
    }
  hashmap_free (&map);
}

void test_apply_change_items ()
{
  pair *pairs[10];
  for (int j = 0; j < 10; ++j)
    {
      char key = (char) (j + 48);
      //even keys are capital letters, odd keys are digits
      if (key % 2)
        {
          key += 17;
        }
      int value = j;
      pairs[j] = pair_alloc (&key, &value, char_key_cpy, int_value_cpy,
                             char_key_cmp,
                             int_value_cmp, char_key_free, int_value_free);
      if ((pairs[j]) == NULL){return;}
    }
  // Create hash-map and inserts elements into it, using pair_char_int.h
  hashmap *map = hashmap_alloc (hash_char);
  if (map == NULL){return;}
  for (int k = 0; k < 10; ++k)
    {
      hashmap_insert (map, pairs[k]);
    }
  assert(hashmap_apply_if (map, is_digit, double_value) == 5);
  for (int k = 0; k < 10; ++k)
    {
      if (k % 2 != 0)
        {
          assert(*(int *) hashmap_at (map, pairs[k]->key)
                 == *(int *) pairs[k]->value);
        }
      if (k % 2 == 0)
        {
          assert(*(int *) hashmap_at (map, pairs[k]->key)
                 == 2 * (*(int *) pairs[k]->value));
        }
    }

  for (int k = 0; k < 10; ++k)
    {
      pair_free ((void **) &pairs[k]);
    }
  hashmap_free (&map);
}

void test_apply_on_value ()
{
  pair *pairs[10];
  for (int j = 0; j < 10; ++j)
    {
      int value = 10;
      char key = (char) j;
      pairs[j] = pair_alloc (&key, &value, char_key_cpy, int_value_cpy,
                             char_key_cmp,
                             int_value_cmp, char_key_free, int_value_free);
      if ((pairs[j]) == NULL){return;}
    }
  // Create hash-map and inserts elements into it, using pair_char_int.h
  hashmap *map = hashmap_alloc (hash_char);
  if (map == NULL){return;}
  for (int k = 0; k < 10; ++k)
    {
      hashmap_insert (map, pairs[k]);
    }
  assert(hashmap_apply_if (map, is_ten, double_value) == 0);
  for (int k = 0; k < 10; ++k)
    {
      pair_free ((void **) &pairs[k]);
    }
  hashmap_free (&map);
}

void test_apply_empty_map ()
{
  hashmap *map = hashmap_alloc (hash_char);
  if (map == NULL){return;}
  assert(hashmap_apply_if (map, always_true, double_value) == 0);
  hashmap_free (&map);
}

void test_apply_always_true ()
{
  pair *pairs[10];
  for (int j = 0; j < 10; ++j)
    {
      char key = (char) (j);
      int value = j;
      pairs[j] = pair_alloc (&key, &value, char_key_cpy, int_value_cpy,
                             char_key_cmp,
                             int_value_cmp, char_key_free, int_value_free);
      if ((pairs[j]) == NULL){return;}
    }

  hashmap *map = hashmap_alloc (hash_char);
  if (map == NULL){return;}
  for (int k = 0; k < 10; ++k)
    {
      hashmap_insert (map, pairs[k]);
    }
  assert(hashmap_apply_if (map, always_true, double_value) == 10);
  for (int k = 0; k < 10; ++k)
    {
      pair_free ((void **) &pairs[k]);
    }
  hashmap_free (&map);
}


/**
 * This function checks the HashMapGetApplyIf function of the hashmap library.
 * If HashMapGetApplyIf fails at some points, the functions exits with exit
 * code 1.
 */
void test_hash_map_apply_if ()
{
  test_apply_always_true ();
  test_apply_empty_map ();
  test_apply_null_funcs ();
  test_apply_on_value ();
  test_apply_change_items ();
}
