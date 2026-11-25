/* hash functions for words
 */

/* Example hash function with terrible performance */
unsigned long hash_too_simple(const unsigned char *str);

/* Add the header for your own added hash functions here. You may search online
 * for existing solutions for hashing function, as long as you as you
 * attribute the source, meaning links to the used material.  */

/* FNV-1a hash function
 * Source: https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
 */
unsigned long fnv1a_hash(const unsigned char *str);

/* djb2 hash function by Dan Bernstein
 * Source: http://www.cse.yorku.ca/~oz/hash.html
 */
unsigned long djb2_hash(const unsigned char *str);
