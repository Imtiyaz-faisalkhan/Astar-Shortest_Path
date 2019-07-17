#include "List.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

static Node* available = 0;

/*** This function for Creating List ***/
List* newList(compFun comp,prFun pr) {
	List* list = malloc(sizeof(List));
	if (list == NULL) {
		return 0;
    }
    else {
	list->nelts = 0;
	list->head = NULL;
	list->comp = comp;
	list->pr = pr;
	return list;
    }
}

/*** This function is used for building a City ***/
City * BuildCity(char* name, int lat, int lgt){
    City * ct = malloc(sizeof(City));
    copy_string(ct->name,name);

    ct->latitude = lat;
    ct->longitude = lgt;
    ct->Startdistance = -HUGE_VAL;
    ct->neighbours = newList(Neighbcmp, CitDisp);
    ct->neighbours->comp=Neighbcmp;
    return ct;
}

/*** This function is used for deleting a List ***/
void delList(List* l)
{
	Node* n1 = l->head;
	while (n1 != NULL)
        {
		Node* n2 = n1->next;
		free(n1);
		n1 = n2;
        }
	free(l);
}

/*** To find the city in the list ***/
City * lookupCity(List * list,char name[100]){
    if(!list->head) return NULL;
    Node * nd = list->head;
    while(nd){

        if(strcmp(((City *)(nd->val))->name, name) == 0){
            return nd->val;
        }
        nd=nd->next;
    }
    return 0;
}

/*** To Retrieve the Nth element ***/
status nthInList(List* l,int n,void** e) {
	Node* nl = l->head;
	while (n--)	{
		if (nl == NULL) {
			break;
        }
		nl = nl->next;
	}
	if (nl == NULL) {
		return ERRINDEX;
	}
	*e = nl->val;
	return OK;
}

/*** To display the City with Latitude and Longitude ***/
void CitDisp(void * disp){
    City * dp = disp;
    printf("\n%s  %d  %d  %d  \t", dp->name,dp->latitude,dp->longitude,dp->neighbours->nelts);
    forEach(dp->neighbours, NeigDisp);
}

/*** To insert element at a given position in the list (O(N)). ***/
status addListAt (List* l, int pos, void* elt) {
    if (pos <= 0 || pos > l->nelts+1)
        return ERRINDEX;
    else {


        Node * toAdd = available;
        if (!toAdd) toAdd = (Node*) malloc(sizeof(Node));
        else available = available->next;
        if (!toAdd) return ERRALLOC;
        l->nelts++;
        toAdd->val = elt;

               if (pos == 1) {
            toAdd->next = l->head;
            l->head = toAdd;
        }


        else {
            Node * tmp = l->head;


            while (pos-- > 2) tmp = tmp->next;
            toAdd->next = tmp->next;
            tmp->next = toAdd;
        }
        return OK;
    }
}

/*** To Compare the distance between the Neighboring cities ***/
int Neighbcmp(void * nbA, void * nbB) {
    int cp = ((Neighbours*)nbA)->distance + (~((Neighbours*)nbB)->distance + 1);
    return cp;
}

/*** To remove the element located at a given position in list (O(N)). ***/
status remFromListAt	(List* l,int pos, void** res) {
    Node *toRem = l->head;

    if (pos <= 0 || pos > l->nelts)
        return ERRINDEX;
    if (pos == 1)
        l->head = toRem->next;
    else {
        Node * prec = toRem;
        while (pos-- > 2) prec = prec->next;
        toRem = prec->next;
        prec->next = toRem->next;
    }

    *res = toRem->val;
    toRem->next = available;
    available = toRem;
    l->nelts--;
    return OK;
}

/***To Compare the two elements ***/
int Eltcmp (void * stA, void * stB) {

    if (!stA && !stB) return 0;
    if (stA && !stB) return 1;
    if (!stA && stB) return -1;

    while (stA && stB && stA == stB) stA++, stB++;

    if (!stA && !stB) return 0;
    else if (stA > stB) return 1;
    else return -1;
}


status remFromList(List* l,void* e) {
	Node* nl = l->head;
	if (nl == NULL) {
		return ERRABSENT;
    }
	if (nl->val == e) {
		l->head = nl->next;
	} else {
		Node* n2 = nl;
		nl = nl->next;
		while (nl != NULL) {
			if (nl->val == e) {
				n2->next = nl->next;
				break;
			}
			n2 = nl;
			nl = nl->next;
		}
		if (nl == NULL) {
			return ERRABSENT;
		}
	}
	free(nl);
	l->nelts--;
	return OK;
}

/*** To Display the Neighbor city and distance ***/
void NeigDisp(void * disp){
    Neighbours * nbs = disp;
    printf("%s %d\n",nbs->city->name,nbs->distance);
}

/*** To Display list elements as "[ e1 ... eN ]" (O(N)). ***/
status displayList(List* l) {
	printf("[ ");
	Node* nl = l->head;
	while (nl != NULL) {
		(*l->pr)(nl->val);
		putchar(' ');
		nl = nl->next;
	}
	printf("]");
}

/*** This method to find the distance between two cities ***/
int DisttwoCity(void * citA, void * citB) {
    int cp = ((City *)citA)->Startdistance + (~((City *)citA)->Startdistance + 1);
    return cp;
}


void forEach(List* l,void(*f)(void*)) {
	Node* nl = l->head;
	while (nl != NULL) {
		(*f)(nl->val);
		nl = nl->next;
	}
}

/*** Compute and return the number of elements in given list (O(1)) ***/
int lengthList(List* l) {
	return l->nelts;
}


int rennesTolyon(City * ct,City * finish){
    float distance;
    distance = sqrt((ct->latitude - finish->latitude) * (ct->latitude - finish->latitude) + (ct->longitude - finish->longitude) * (ct->longitude - finish->longitude));
    return distance;
}

/**
 * Private function to get the node preceding the one we're looking for
 * @param e the searched element
 * @return 0 if element is not found (typically, list is empty)
 * @return 1 if element is head of list
 * @return a pointer to the predecessor otherwise
 */
static Node* previous(List* l, void* e) {
    Node * prec = l->head;
    if (l->nelts == 0) return 0;
    if (!(l->comp)(e,prec->val)) return (Node*)1;

    /* prec must address element prior to the one to remove */
    while (prec->next && (l->comp)(e,prec->next->val))
        prec = prec->next;

    /* here,
     *  - either we address nothing (no such element)
     *  - or we address the element prior to the one we're looking for
     */
    return prec;
}

/*** Go to the current city ***/
int curr_to_go(City * ct){

    while(ct->Startdistance) {
        int t = (ct->Startdistance & ct->Finishdistance) <<1;
        ct->Finishdistance ^= ct->Startdistance;
        ct->Startdistance = t;
    }
    return ct->Finishdistance;
}

/*** Insert element at a given position in the list (O(N)). ***/
status addList(List* l,void* e) {
	Node* n1 = malloc(sizeof(Node));
	if (n1 == NULL)
		return ERRALLOC;
	n1->val = e;
	Node* n2 = l->head;
	if (n2 == NULL) {
		l->head = n1;
		n1->next = NULL;
	} else if ((*l->comp)(e, n2->val) <= 0) {
		n1->next = l->head;
		l->head = n1;
	} else {
		Node* prev = n2;
		n2 = n2->next;
		while (n2 != NULL) {
			if ((*l->comp)(e, n2->val) <= 0) {
				break;
			}
			prev = n2;
			n2 = n2->next;
		}
		prev->next = n1;
		n1->next = n2;
	}
	l->nelts++;
	return OK;
}

/*** To Test whether the list contains the given element ***/
Node* isInList(List* l,void* e) {
	Node* nl = l->head;
	while (nl != NULL) {
		if (e == nl->val) {
			return (Node*)1;
        }
		nl = nl->next;
	}
	return 0;
}

/*** Comparing the distance between two cities ***/
int citycmp(void * ct1, void * ct2){
    City * c1 = ct1;
    City * c2 = ct2;
    return (curr_to_go(c1)+curr_to_go(c2))/2;
}

/*** Used to Copy string ***/
void copy_string(char d[], char s[]) {
   int c = 0;

   while (s[c] != '\0') {
      d[c] = s[c];
      c++;
   }
   d[c] = '\0';
}

/*** To create a Neighbor city ***/
Neighbours * CreateNeighbour(City * city, int distance){
    Neighbours * nb = malloc(sizeof(Neighbours));
    nb->city = city;
    nb->distance = distance;
    return nb;
}
