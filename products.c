/*
 * This is the file in which you will implement the functionality required by
 * the assignment.  Make sure to add your name and @oregonstate.edu email
 * address below:
 *
 * Name: Anthony Pham
 * Email: phamanth@oregonstate.edu
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "products.h"
#include "dynarray.h"

/*
 * This function should allocate and initialize a single product struct with
 * name, inventory, and price data.
 *
 * Params:
 *   name - a string containing the product's name.  Note that you'll have to
 *     make a copy of this string to store in the product struct instead of
 *     storing this string directly.
 *   inventory - an integer for how many of this product in the store
 *   price - the product's price
 *
 * Return:
 *   Should return a newly-allocated product structure whose fields are
 *   initialized with the values provided.
 */

struct product* create_product(char* name, int inventory, float price) {
    struct product* product_ptr = malloc(sizeof(struct product));
    if (product_ptr == NULL) {
        fprintf(stderr, "Error, Can't allocate any memory for the product.\n");
        return NULL;
    }

    product_ptr->name = malloc(strlen(name) + 1);
    if (product_ptr->name == NULL) {
        fprintf(stderr, "Error, Can't allocate any memory for the product name.\n");
        free(product_ptr);
        return NULL;
    }
    strcpy(product_ptr->name, name);

    product_ptr->inventory = inventory;
    product_ptr->price = price;
    return product_ptr;
}

/*
 * This function should free all memory allocated by create_product() for a
 * single product.  This function must free all relevant memory and cannot
 * result in a memory leak.
 *
 * Params:
 *   product - a pointer to the product whose memory should be freed.  This
 *     function must free any memory allocated to the fields of this struct
 *     as well as memory allocated for the struct itself.
 */
void free_product(struct product* product_ptr) {
    if (product_ptr != NULL) {
        free(product_ptr->name);
        free(product_ptr);
    }
}

/*
 * This function should create a struct product for each product represented
 * in the information provided in the function arguments, and it should store
 * those products in a dynamic array (i.e. struct dynarray) allocated by this
 * function.  In particular, you should create a dynamic array and insert new
 * product structs into it, such that the i'th product in the array has the
 * i'th name, the i'th inventory, and the i'th price from the provided arrays
 * arguments.  You should use your create_product() function to allocate and
 * initialize each product struct stored in the array, and you should use the
 * provided dynamic array functions to allocate and work with the dynamic
 * array.  At the end of the function, you should return the dynamic array
 * with product structures stored in it.
 *
 * Params:
 *   num_products - the number of products to be stored in the newly allocated
 *     dynamic array
 *   names - an array of the names of the products to be stored in the dynamic
 *     array of product structs.  This array will have length num_products.
 *   inventory - an array of the inventory values, how many of the products in
 *   the store.
 *     array of product structs.  This array will have length num_products.
 *   prices - an array of the product prices to be stored in the dynamic
 *     array of product structs.  This array will have length num_products.
 *
 * Return:
 *   Should return a pointer to the newly allocated dynamic array containing
 *   newly-created product structs.  The i'th product in this array should have
 *   the i'th name, the i'th inventory, and the i'th price from the arrays provided as
 *   arguments.
 */

struct dynarray* create_product_array(int num_products, char** names, int* inventories, float* prices) {
    struct dynarray* products_arr = dynarray_create();
    if (products_arr == NULL) {
        fprintf(stderr, "Error, can't create a dynamic array for the products.\n");
        return NULL;
    }
    for (int index = 0; index < num_products; index++) {
        struct product* product_ptr = create_product(names[index], inventories[index], prices[index]);
        if (product_ptr == NULL) {
            fprintf(stderr, "The product %d could not be created.\n", index);
            continue;
        }
        
        dynarray_insert(products_arr, -1, product_ptr);
    }
    return products_arr;
}

/*
 * This function should free all of the memory allocated to a dynamic array of
 * product structs, including the memory allocated to the array itself as
 * well as any memory allocated to the individual product structs.  You
 * should use free_product() to free the memory for each individual product,
 * and you should use provided dynamic array functions to free the memory
 * associated with the dynamic array itself.  This function must free *all*
 * memory associated with a dynamic array of products and must not result in
 * any memory leaks.
 *
 * Params:
 *   products - a pointer to the dynamic array of product structs whose memory
 *     is to be freed
 */
void free_product_array(struct dynarray* products) {
    if (products != NULL) {
        int length = dynarray_length(products);
        for (int index = 0; index < length; index++) {
            struct product* product_ptr = dynarray_get(products, index);
            free_product(product_ptr);
        }
        dynarray_free(products);
    }
}

/*
 * This function should print the name, inventory, and price of products in an
 * array, one product per line.  You must use provided dynamic array functions
 * to access the product data stored in the array.
 *
 * Params:
 *   products - the dynamic array of products to be printed
 */
void print_products(struct dynarray* products) {
    int length = dynarray_length(products);
    for (int index = 0; index < length; index++) {
        struct product* product_ptr = dynarray_get(products, index);
        printf("Product: %s, Inventory: %d, Price: %.2f\n",
               product_ptr->name,
               product_ptr->inventory,
               product_ptr->price);
    }
}

/*
 * This function should return a pointer to the product in a given array with
 * the highest price.  You should not make a copy of the product being returned,
 * i.e. this function should not allocate any memory.  Instead, you should
 * simply return the pointer to the product struct that's already stored in
 * the array.  You must use the provided dynamic array functions to access
 * the data stored in the array.
 *
 * Params:
 *   products - the array from which to find the product with the highest price
 *
 * Return:
 *   Should return a pointer to the product in the array with the highest price.
 *   You should not make a copy of the product being returned but should
 *   instead return the pointer to the product struct that's already stored in
 *   the array.
 */

struct product* find_max_price(struct dynarray* products) {
    int length = dynarray_length(products);
    if (length == 0)
        return NULL;
    struct product* max_product_ptr = dynarray_get(products, 0);
    for (int index = 1; index < length; index++) {
        struct product* current_ptr = dynarray_get(products, index);
        if (current_ptr->price > max_product_ptr->price) {
            max_product_ptr = current_ptr;
        }
    }
    return max_product_ptr;
}

/*
 * This function should return a pointer to the product in a given array with
 * the largest investment, defined as the product of inventory and price.
 * investment = inventory*price, meaning how many does the store have times
 * how many
 * You should not make a copy of the product being returned,
 * i.e. this function should not allocate any memory.  Instead, you should
 * simply return the pointer to the product struct that's already stored in
 * the array.  You must use the provided dynamic array functions to access
 * the data stored in the array.
 *
 * Params:
 *   products - the array from which to find the product with the lowest price
 *
 * Return:
 *   Should return a pointer to the product in the array with the highest 
 *   investment = price*investment.
 *   You should not make a copy of the product being returned but should
 *   instead return the pointer to the product struct that's already stored in
 *   the array.
 */

struct product* find_max_investment(struct dynarray* products) {
    int length = dynarray_length(products);
    if (length == 0)
        return NULL;
    struct product* max_investment_ptr = dynarray_get(products, 0);
    float max_investment_value = max_investment_ptr->price * max_investment_ptr->inventory;
    for (int index = 1; index < length; index++) {
        struct product* current_ptr = dynarray_get(products, index);
        float current_investment_value = current_ptr->price * current_ptr->inventory;
        if (current_investment_value > max_investment_value) {
            max_investment_value = current_investment_value;
            max_investment_ptr = current_ptr;
        }
    }
    return max_investment_ptr;
}

/*
 * This function should sort the products stored in a dynamic array by
 * ascending inventory (i.e. lowest inventory at the beginning of the array).
 * You may implement any sorting algorithm you want, with the following 
 * constraints:
 *   - You must sort in place, i.e. you can't allocate additional memory.
 *   - You may not use built-in sorting functions like qsort(), i.e. you must
 *     implement the sort yourself.  You may implement any in-place sorting
 *     algorithm you like.  Some possibilities are selection sort, insertion
 *     sort, bubble sort, and quicksort.
 *   - You must use provided dynamic array functions (e.g. dynarray_get(),
 *     dynarray_set(), etc.) to access and modify the contents of the dynamic
 *     array.
 *
 * Params:
 *   products - the dynamic array of products to be sorted.  When the function
 *   returns, this array should be sorted by ascending inventory (lowest to
 *   highest).
 */
void sort_by_inventory(struct dynarray* products) {
    int length = dynarray_length(products);
    for (int i =0; i < length - 1; i++) {
        int min_index = i;
        struct product* min_product_ptr = dynarray_get(products, i);
        for (int j = i + 1; j < length; j++) {
            struct product* current_ptr = dynarray_get(products, j);
            if (current_ptr->inventory < min_product_ptr->inventory) {
                min_index = j;
                min_product_ptr = current_ptr;
            }
        }
        if (min_index != i) {
            struct product* temp_ptr = dynarray_get(products, i);
            dynarray_set(products, i, min_product_ptr);
            dynarray_set(products, min_index, temp_ptr);
        }
    }
}

