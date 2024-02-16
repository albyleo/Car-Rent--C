#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <ctype.h>
#include <math.h>

#define MAX_CARS 512
#define MAX_LINE_LEN 15000
  

typedef struct Station{
    int distance;
    int numCars;
    struct car* head;
    struct Station* next;
    struct Station* prec;
} Station;

typedef struct car{
    int Autonomy;
    struct car* next;
} car;

typedef struct Autostrada {
    int numStations;
    Station* head;
} Autostrada;

typedef struct QueueNode {
    int n;
    Station *station;
    struct QueueNode *next;
    struct QueueNode *prec;
} QueueNode;

typedef struct Queue
{
    struct QueueNode *head;
    struct QueueNode *tail;
    int count;
} Queue;

typedef struct StazioneAdiacenti
{
    int distance;
    struct StazioneAdiacenti *next;
    int progressiveNumber;
} StazioneAdiacenti;

typedef struct Vertice
{
    int progressiveNumber;
    StazioneAdiacenti *adiacenti;
    Station *stazione; // Puntatore alla stazione raggiungibile
} Vertice;

Station* FindStation(Autostrada* autostrada, int distance);

Autostrada* createAutostrada(){
        Autostrada* autostrada= calloc(1,sizeof(Autostrada));
        autostrada->head=NULL;
        return autostrada;
}

Station* createStation(int distance){
    Station* stazione=calloc(1,sizeof(Station));
    stazione->distance=distance;
    //stazione->next=NULL;
    return stazione;
}

typedef struct car Car;

Car* createCar(int autonomy) {
    Car* newCar = (Car*)calloc(1, sizeof(Car));
    newCar->Autonomy = autonomy;
    newCar->next = NULL;
    return newCar;
}

// Stessa funzione di aggiungiauto ma senza printf da chiamare in fillAuto
bool aggiungiAuto(Autostrada* autostrada, int distanceStation, int autonomia, Station *lastStation) {

    Station *station; 
    if(lastStation)
        station = lastStation;
    else 
        station = FindStation(autostrada, distanceStation);
    if(station) {
        if(station->numCars == 512) {
            return false;
        }
        // Crea nuova macchina e la metta in testa alla station's list

        Car* newCar = createCar(autonomia);
        if(!station->head) {
            station->head = newCar;
            station->numCars++;
            return true;
        }

        Car *prec = NULL;
        Car *current = station->head;

        if(current->Autonomy <= autonomia) {
            newCar->next = station->head;
            station->head = newCar;
        }else{
            prec = current;
            current = current->next;
            newCar->next = current;
            prec->next = newCar;
        }

        station->numCars++;
        return true;
    } else {
        return false;
    }
}

Station* FindStation(Autostrada* autostrada, int distance){

        for (Station* s=autostrada->head; s!=NULL; s=s->next )
        {
            if(s->distance==distance)
            return s;
        }
        
    return NULL;
}



Station*  aggiungiStazione(Autostrada* autostrada, int distance) {
    
    if(FindStation(autostrada,distance)){
        return NULL;
    }

    Station* newStazione;
    newStazione=createStation(distance);

    
    if (!autostrada->head) {
        // Insert at the start if list is empty or new station's distance is less than the head station's distance
        newStazione->next = autostrada->head;
        autostrada->head = newStazione;
        
        //printf("aggiunta\n");
    } else if(autostrada->head->distance > newStazione->distance) {
        newStazione->next = autostrada->head;
        autostrada->head = newStazione;
        newStazione->next->prec = newStazione;
    
    } else{
        // Insert at the correct position
        Station* current = autostrada->head;
        while (current->next && current->next->distance < newStazione->distance) {
            current = current->next;
        }
        newStazione->next = current->next;
        if(current->next)
            current->next->prec=newStazione;
        current->next = newStazione;
        newStazione->prec=current;
    }
    autostrada->numStations++;

    return newStazione;

}


void rimuoviStazione(Autostrada* autostrada,int distance){
     
     if (!autostrada->head) {
        printf("non demolita\n");
        return;
    }
    // se la stazione da rimuovere è in testa
    if (autostrada->head->distance == distance) {
        Station* temp = autostrada->head;
        if(autostrada->head->next) {
            autostrada->head = autostrada->head->next;
            autostrada->head->prec=NULL;
        } else {
            autostrada->head  = NULL;
        }
        for(Car *car = temp->head; car != NULL;) {
            Car *tmp = car;
            car = car->next;
            free(tmp);
        }
        free(temp);
        printf("demolita\n");
        autostrada->numStations--;
        return;
    }

    // cerca la stazione da rimuovere
    Station* current = autostrada->head;
    while (current->next != NULL && current->next->distance != distance) {
        current = current->next;
    }

    // se la distanza non è presente nella linked list
    if (current->next == NULL) {
        printf("non demolita\n");
        return;
    }
    //else rimozione
    Station* temp = current->next;
    current->next = current->next->next;
    if(current->next) {
        current->next->prec=current;
    }

    free(temp);

    printf("demolita\n");
    autostrada->numStations--;

}


void rottamaAuto(Autostrada* autostrada, int distanceStazione, int autonomyAuto) {
    // Trova la stazione con la distanza specificata
    Station* currentStation = FindStation(autostrada, distanceStazione);

    // Se la stazione non esiste, stampa "non rottamata" e ritorna
    if (!currentStation) {
        printf("non rottamata\n");
        return;
    }

    // Cerca l'auto con l'autonomia specificata
    car* prevCar = (currentStation->head && currentStation->head->next)? currentStation->head : NULL;
    car* currentCar = (currentStation->head && currentStation->head->next)? currentStation->head->next : currentStation->head;
    while (currentCar && currentCar->Autonomy != autonomyAuto) {
        prevCar = currentCar;
       
        currentCar = currentCar->next;
    }
    

    if(!currentCar && currentStation->head && currentStation->head->Autonomy == autonomyAuto) {
        currentCar = currentStation->head;
        prevCar = NULL;
    }

    // Se l'auto non esiste nella stazione, stampa "non rottamata" e ritorna
    if (!currentCar) {
        printf("non rottamata\n");
        return;
    }

    // Rimuove l'auto dalla lista
    if (prevCar) {
        // Se non è la prima auto nella lista,la precedenta alla current la collego alla sua successiva
        prevCar->next = currentCar->next;
    } else {
        //se è la prima ed unica macchina, quindi prevcar=null
        if(currentCar->next == NULL ){
                //devo comunqe collegarla la lista ?
                currentStation->head = currentCar->next;
            goto here;
        }else{
        // Se è la prima auto nella lista
        // Cerca la seconda auto con l'autonomia maggiore    
        int MaxAutonomia = 0; 
        car* ccar = currentStation->head->next;
        while (ccar->next != NULL) {
                if(ccar->Autonomy > MaxAutonomia){
                    MaxAutonomia=ccar->Autonomy;
                }
                ccar = ccar->next;
                }
        
        car* pCar = (currentStation->head && currentStation->head->next)? currentStation->head : NULL;
        car* curCar = (currentStation->head && currentStation->head->next)? currentStation->head->next : currentStation->head;
        while (curCar && curCar->Autonomy != MaxAutonomia) {
                
                pCar = curCar;
                curCar = curCar->next;
                }
        pCar->next = curCar->next;
        curCar->next = currentStation->head->next;
        currentStation->head = curCar;
        }
    }

    // Dealloca memoria per l'auto rottamata e aggiorna il conteggio delle auto
    here:
    free(currentCar);
    currentStation->numCars--;

    printf("rottamata\n");
}




bool queue_empty(Queue *q) {
    return q->head == NULL;
}



QueueNode *queue_pop_front_station(Queue *q)
{
    QueueNode *node = q->head;
    q->head = q->head->next;
    q->count--;
    return node;
}

void queue_push_back(Queue *q, Station* data) {
    QueueNode *new = calloc(1, sizeof(QueueNode));
    new->station = data;
    new->n = data->distance;
    if(!q->head) {
        q->head = new;
        return;
    }
    
    QueueNode* last;
    for(last = q->head; last->next != NULL; last = last->next);
    last->next = new;
    q->count++;
}


Station* queue_pop_back(Queue *q) {
    QueueNode* n = q->head;
    QueueNode *prec = NULL;
    for(; n->next != NULL; n=n->next) {
        prec = n;
    }
    if(prec) {
        prec->next = NULL;
    } else {
        q->head = NULL;
    }
    q->count--;
    //printf("pop count: %d\n", q->count);
    return n->station;
}

int queue_length(Queue *q) {
    return q->count;
}

int queue_get_element(Queue *q, int pos) {
    int i = 0;
    QueueNode *n;
    for(n = q->head; n != NULL; n = n->next, i++) {
        if(i == pos) {
            return n->n;
        }
    }
    return -1;
}

QueueNode* queue_get_node(Queue *q, int pos) {
    int i = 0;
    QueueNode *n;
    for(n = q->head; n != NULL; n = n->next, i++) {
        if(i == pos) {
            return n;
        }
    }
    return NULL;
}

void queue_clear(Queue *q) {
    QueueNode *to_be_freed;
    while(!queue_empty(q)) {
        to_be_freed = queue_pop_front_station(q);
        free(to_be_freed);
    }
}


void findPath(Station* src, Station* dst) {
    Station *currentDst = dst;
    Queue *coda = calloc(1, sizeof(Queue));
    queue_push_back(coda, currentDst);
    while(currentDst->distance > src->distance) {
        Station *st = NULL;
        int maxDist = INT_MIN;
        Station *fartestStation = NULL;
        int autonomia;

        for(st = currentDst->prec; st != NULL && st->distance >= src->distance; st=st->prec) {
            
            if(st->head) {
                autonomia = st->head->Autonomy;
            } else {
                continue;
            }
            int dist = abs(currentDst->distance - st->distance);
            if(dist <= autonomia && dist > maxDist) {
                maxDist = dist;
                fartestStation = st;
            }
        }
        if(fartestStation == NULL) {
            printf("nessun percorso\n");
            return;
        }
        queue_push_back(coda, fartestStation);
        currentDst = fartestStation;
        if(currentDst->distance <= src->distance) {
            break;
        }
    }

    while(!queue_empty(coda)) {
        printf("%d", queue_pop_back(coda)->distance);
        if(coda->count >= 0) {
            printf(" ");
        }
    }
    printf("\n");
}

void findPathReverse(Station* src, Station* dst, Queue* final) {

    Station *currentSrc = src;

    Queue *coda = calloc(1, sizeof(Queue));
    queue_push_back(coda, currentSrc);

    while(currentSrc->distance > dst->distance) {
        Station *st = NULL;
        int maxLen = INT_MAX;
        Station *bestStation = NULL;
        Station *minStation = NULL;
        Station *precSrc = dst;
         int autonomia = 0;
        int srcAutonomy = -1;
        if (currentSrc->head) {
            srcAutonomy = currentSrc->head->Autonomy;
        }

        for(st = currentSrc->prec; st != NULL && st->distance >= dst->distance && abs(currentSrc->distance-st->distance) <= srcAutonomy; st=st->prec) {
            
            if(st->head) {
                autonomia = st->head->Autonomy;
            } else if(st == dst) {
                break;
            } else {
                continue;
            }

            int len = st->distance - autonomia;

            if(len < maxLen) {
                maxLen = len;
                bestStation = st;
            }
            
            if(st->distance - st->head->Autonomy < precSrc->distance) {
                if(!minStation || st->distance < minStation->distance) {
                    minStation = st;
                }
            }
        }

        if(bestStation == NULL) {
            if(st->head) {
                printf("nessun percorso\n");
                return;
            } else if(st == dst) {
                minStation = st;
            } else {
                printf("nessun percorso\n");
                return;
            }
        } 
        
        if(minStation) {
            queue_push_back(coda, minStation);
            currentSrc = minStation;
        } else {
            queue_push_back(coda, bestStation);
            currentSrc = bestStation;
        }

        precSrc = currentSrc;
    }

    if(!queue_empty(coda) && queue_get_element(coda, 0) == src->distance) {
        queue_push_back(final, queue_get_node(coda, queue_length(coda)-1)->station);
    }

    if(queue_length(coda) > 1) {
        findPathReverse(src, queue_get_node(coda, queue_length(coda)-1)->station, final);
        queue_clear(coda);
        free(coda);
    }
    
    if(queue_get_element(final, queue_length(final)-1) == dst->distance) {
        while(!queue_empty(final)) {
            printf("%d", queue_pop_back(final)->distance);
            if(final->count >= 0) {
                printf(" ");
            }
        }
        printf("\n");
        queue_clear(final);
    }
}

void calcolaPercorso(Autostrada *autostrada, int src, int dest) {
    
    Station *stSrc = FindStation(autostrada, src);
    Station *stDst = FindStation(autostrada, dest);

    if(!stSrc || !stDst) {
        printf("nessun percorso\n");
        return;
    }
 
    if(src == dest) {
        printf("%d\n", src);
        return;
    }
   
    if(src < dest) {
        findPath(stSrc, stDst);
    } 
    else {
        Queue* final = calloc(1, sizeof(Queue));
        queue_push_back(final, stDst);
        findPathReverse(stSrc, stDst, final);
    }
}


void fillAuto(Autostrada* autostrada, int distance, int count, char* command, Station *staz) {
    
 char* endptr;
     while(command) {
        int autonomy = (int)strtol(command, &endptr, 10);
        if(command == endptr) {
            return;
        }
        aggiungiAuto(autostrada, distance, autonomy, staz);
        command = endptr;
    }

}

int main() {
    char command[MAX_LINE_LEN];

    Autostrada* autostrada=createAutostrada();

//    FILE *file = fopen("../Desktop/open_111.txt", "r");

    Station *lastStation = NULL;

    while (fgets(command, sizeof(command), stdin)) {

        if(strlen(command) <=  2) {
            continue;
        }

        char cmd[20];
        sscanf(command, "%s", cmd);

        if (strcmp(cmd, "aggiungi-stazione") == 0) {
            int distanza, numeroAuto; 
            sscanf(command, "%*s %d %d", &distanza, &numeroAuto);
            Station *staz = aggiungiStazione(autostrada, distanza);
            if(staz) {
                char* elab;
                strtol(command+strlen(cmd)+1, &elab, 10);
                strtol(elab, &elab, 10);
                fillAuto(autostrada, distanza, numeroAuto, elab, staz);
                printf("aggiunta\n");
            } else {
                printf("non aggiunta\n");
            }
        } else if (strcmp(cmd, "demolisci-stazione") == 0) {
            int distanza;
            sscanf(command, "%*s %d", &distanza);
            rimuoviStazione(autostrada, distanza);
        } else if (strcmp(cmd, "aggiungi-auto") == 0) {
            int distanzaStazione, autonomiaAuto;
            sscanf(command, "%*s %d %d", &distanzaStazione, &autonomiaAuto);
            bool aggiunta = false;
            if(lastStation && distanzaStazione == lastStation->distance)
                aggiunta = aggiungiAuto(autostrada, distanzaStazione, autonomiaAuto, lastStation);
            else {
                Station *toAdd = FindStation(autostrada, distanzaStazione);
                aggiunta = aggiungiAuto(autostrada, distanzaStazione, autonomiaAuto, toAdd);
                lastStation = toAdd;
            }
            if(aggiunta)
            {
                printf("aggiunta\n");
            } else {
                printf("non aggiunta\n");
            }
        } else if (strcmp(cmd, "rottama-auto") == 0) {
            int distanzaStazione, autonomiaAuto;
            sscanf(command, "%*s %d %d", &distanzaStazione, &autonomiaAuto);
            rottamaAuto(autostrada, distanzaStazione, autonomiaAuto);
        } else if (strcmp(cmd, "pianifica-percorso") == 0) {
            int stazionePartenza, stazioneArrivo;
            sscanf(command, "%*s %d %d", &stazionePartenza, &stazioneArrivo);
            calcolaPercorso(autostrada, stazionePartenza, stazioneArrivo);
        }
    }
   return 0;
}










