#include <stdio.h>
#include "List.h"
#include "status.h"

int main(){

    printf("\n* Cities of France and distance  *");
    printf("\n***************************************");
    char starts[30],finishs[30];

    /*** To Open File ***/
    FILE * file;
    file = fopen("FRANCE.MAP", "r");
    if (!file) return NULL;
    char city[100];
    int latitude;
    int longitude;
    City * ct = malloc(sizeof(City));
    List * dp = newList(Eltcmp, CitDisp);
    int returnVal;
    while(returnVal != -1){

        returnVal = fscanf(file,"%s %d %d\n", city, &latitude, &longitude);
        City * already = lookupCity(dp, city);

        if (returnVal == 3){
            if(already == 0)
                {
                ct =BuildCity(city, latitude, longitude);
                addList(dp, ct);
                }
            else
            {
                ct = already;
                ct->latitude = latitude;
                ct->longitude = longitude;
            }
        }

        else if (returnVal == 2)
            {
            Neighbours * nb = malloc(sizeof(Neighbours));
            if (already == 0){
                City * nb_city = BuildCity(city, -1, -1);
                addList(dp, nb_city);
                nb = CreateNeighbour(nb_city,latitude);
                addList(ct->neighbours, nb);
            }
        else{
                nb=CreateNeighbour(already, latitude);
                addList(ct->neighbours, nb);
            }
        }
    }
    fclose(file);
    forEach(dp, CitDisp);

    List * exposed = newList(citycmp, CitDisp);
    List * bolted = newList(Eltcmp, CitDisp);
    printf("---------------************----------------");
    printf("\n\nCities in France are:\n");
    printf("Avignon\nBordeaux\nBrest\nCaen\nCalais\nDijon\nGrenoble\nLimoges\nLyon\nMarseille\nMontpellier\nNancy\nNantes\nNice\nParis\nRennes\nStrasbourg\nToulouse");
    printf("\nEnter city where you want to go: ");
    gets(starts);
    printf("\nEnter city where you want to reach:");
    gets(finishs);
    City * actual = lookupCity(dp, starts);
    City * reach = lookupCity(dp, finishs);
    addList(exposed, actual);
    actual->Startdistance = 0;
    actual->Finishdistance=rennesTolyon(actual, reach);

    while(exposed->head){
        void * dp;
        remFromListAt(exposed, 1, &dp);
        addList(bolted, dp);
        City * ct = (City *) dp;

        if(ct == reach){
            printf("\nThe Destination Place Reached:\n");
            while(ct){
                printf("%s %d %d \n",ct->name,ct->latitude,ct->longitude);
                ct = ct ->ctr;
            }
            printf("\n");
            return 0;
        }

        Node * nd = ct->neighbours->head;

        while(nd)
            {
            City * achieve = ((Neighbours *)nd->val)->city;
            int cover =((Neighbours *)nd->val)->distance;
            int covered = ct->Startdistance + cover;

            if((isInList(exposed, achieve)!=0) ||
              ((isInList(bolted, achieve)!=0) &&
                    (covered > achieve->Startdistance)))
            {
                    nd = nd->next;
            }
            else
            {
                if(isInList(exposed, achieve)!=0)
                {
                    remFromList(exposed, achieve);
                }
                if(isInList(bolted, achieve)!=0)
                {
                    remFromList(bolted, achieve);
                }
                achieve->Startdistance = covered;
                achieve->ctr = ct;
                addList(exposed, achieve);
                nd = nd->next;
            }
        }
    }
    printf("Sorry! Not available");
    return 0;
}
