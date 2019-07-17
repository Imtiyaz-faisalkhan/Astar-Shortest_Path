#ifndef FRANCE_H_INCLUDED
#define FRANCE_H_INCLUDED


#include <stdlib.h>
#include "status.h"

/*** Typical simple link structure: a Node is a "value / next" pair ***/
typedef struct Node {
    void		*val;
    struct Node	*next;
} Node;

/**
 * Comparison function for list elements.
 * Must follow the "strcmp" convention: result is negative if e1 is less
 * than e2, null if they are equal, and positive otherwise.
 */
typedef int(*compFun)   (void* e1, void* e2);

/*** Display function for list elements ***/
typedef void(*prFun)   (void*);

/*** The list embeds a counter for its size and the two function pointers ***/
typedef struct List {
    int nelts;
    Node * head;
    compFun comp;
    prFun pr;
} List;

/**
 * Empty List creation by dynamic memory allocation (O(1)).
 * @param comp comparison function between elements (ala strcmp())
 * @param pr display function for list elements
 * @return a new (empty) list if memory allocation OK
 * @return 0 otherwise
 */
List*	newList	(compFun,prFun);

/**
 * Destroy the list by deallocating used memory (O(N)).
 * @param l the list to destroy
 */
void 	delList	(List*);

/**
 * Get the Nth element of the list (O(N)).
 * @param l the list
 * @param n the index of the element in list
 * @param e (out) the searched element
 * @return OK if element found
 * @return ERRINDEX if index out of list bounds
 */
status 	nthInList	(List*,int,void**);

/**
 * Insert element at a given position in the list (O(N)).
 * @param l the list to store the element in
 * @param p the position of the insertion point
 * @param e the element to insert
 * @return ERRINDEX if position is out of list bounds
 * @return ERRALLOC if memory allocation failed
 * @return OK otherwise
 */
status 	addListAt	(List*,int,void*);

/**
 * Remove the element located at a given position in list (O(N)).
 * @param l the list to remove the element from
 * @param p the position of the element to remove
 * @param e (out) the removed element
 * @return ERRINDEX if position is out of list bounds
 * @return OK otherwise
 */
status 	remFromListAt	(List*,int,void**);

/**
 * Remove given element from given list (O(N)).
 * Implies the user has given a comparison function.
 * @param l the list to remove the element from
 * @param e the element to remove
 * @return ERRABSENT if element is not found in list
 * @return ERRUNABLE if no comparison function has been provided
 * @return OK otherwise
 */
status 	remFromList	(List*,void*);

/**
 * Display list elements as "[ e1 ... eN ]" (O(N)).
 * The display of the element is delegated to the prFun function
 * @param l the list to display.
 * @return ERRUNABLE if no comparison function has been provided
 * @return OK otherwise
*/
status 	displayList	(List*);

/**
 * Sequencially call given function with each element of given list (O(NxF)).
 * @param l the list
 * @param f the function
 */
void	forEach		(List*,void(*)(void*));

/**
 * Compute and return the number of elements in given list (O(1)).
 * @param l the list
 * @return the number of elements in given list
 */
int	lengthList	(List*);

/**
 * Add given element to given list according to compFun function (O(N)).
 * @param l the list (supposedly sorted according to compFun function)
 * @param e the element to add
 * @return ERRALLOC if memory allocation failed
 * @return ERRUNABLE if no comparison function has been provided
 * @return OK otherwise
 */
status	addList	(List*,void*);

/**
 * Tests whether the list contains given element (O(N)).
 * @param l the list
 * @param e the searched element
 * @return 0 if element is not found in list
 * @return 1 if element is at the head of the list (no predecessor)
 * @return (a pointer to) the predecessor of the search element otherwise
 */
Node*	isInList	(List*,void*);

/*** The struct holds the information about city in a Map file ***/
typedef struct City{
    char name[100];
    int latitude;
    int longitude;
    List * neighbours;
    int Startdistance;
    int Finishdistance;
    struct City * ctr;

}City;

/*** This struct holds the Neighbour city and the distance ***/
typedef struct Neighbours{
    int distance;
    City * city;
}Neighbours;

/*** This method is used to find the distance between two cities ***/
int DisttwoCity(void *, void *);

/*** Go to the current city ***/
int curr_to_go(City *);

/*** Comparing the distance between two cities ***/
int citycmp(void *, void *);

/*** Calculating the distance between Rennes to Lyon ***/
int rennesTolyon(City *, City *);

/*** Comparing the distance between the Neighboring cities ***/
int Neighbcmp(void *, void *);

/*** Used to build city with City Name, Latitude and Longitude ***/
City * BuildCity(char*,int, int);

/*** Used to Copy string ***/
void copy_string(char [], char []);

/*** Compare the two elements ***/
int Eltcmp(void *, void *);

/*** Used to display the City with Latitude and Longitude ***/
void CitDisp(void * );

/*** To find the city in the list ***/
City * lookupCity(List * l, char[100]);

/*** To create a Neighbor city ***/
Neighbours * CreateNeighbour(City *,int);

/*** Display the Neighbor city and distance ***/
void NeigDisp(void *);

#endif

