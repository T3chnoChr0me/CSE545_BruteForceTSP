#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <math.h>

struct Node {                 //Basic component of the list each list has: a name value, a unique ID, a pointer to the next and previous node
  int id;
  double x;
  double y;
  struct Node *next;
  struct Node *prev;
};

struct List {               //A list contains pointers to the head and tail nodes
  struct Node *head;
  struct Node *tail;
  int size;
};

struct Node *create_node(int id) {        //Takes a pointer to a character string, and an id
  struct Node *node = malloc(sizeof(struct Node));    //node is a pointer to the memory space allocated by malloc which allocates the enough memory space for the node component
  if (node == NULL) {
    fprintf (stderr, "%s: Couldn't create memory for the node; %s\n", "linkedlist", strerror(errno));
    exit(-1);
  }
  node->id = id;      //name of the node points to the name input (this dynamically creates the space for the character array)
  node->next = NULL;              //
  node->prev = NULL;              //Initialize the rear and front pointers of the node
  return node;        //Returns a pointer to the created node
}

struct List *create_list() {      
  struct List *list = malloc(sizeof(struct List));    //Creates a pointer to a List where the memory space is dynamically created to contain two pointers
  if (list == NULL) {
    fprintf (stderr, "%s: Couldn't create memory for the list; %s\n", "linkedlist", strerror (errno));
    exit(-1);
  }
  list->head = NULL;  //Initialize the rear and front pointers of the node
  list->tail = NULL;  //
  list->size = 0;
  return list;        //Returns a pointer to the created list
}

void insert_tail(struct Node *node, struct List *list) {    //Takes a pointer to a node, and a pointer to a list
  if (list->head == NULL && list->tail == NULL) { //If the list contains no elements, set both pointers to the node
    list->head = node;
    list->tail = node;
    list->size++;
  } else {                    //If the list contains at least one other element, do the following:
    list->tail->next = node;        //Set the next pointer of previous tail node to the next node
    node->prev = list->tail;        //Set the previous pointer of the current node to the previous tail node
    list->tail = node;              //Set the tail pointer to the current node
    list->size++;
  }
}

void insert_head(struct Node *node, struct List *list){
  if (list->head == NULL && list->tail == NULL){
    list->head = node;
    list->tail = node;
    list->size++;
  } else {
    list->head->prev = node;
    node->next = list->head;
    node->prev = NULL;
    list->head = node;
    list->size++;
  }
}

void print_list(struct List *list) {    //Takes a pointer to the list to print
  struct Node *ptr = list->head;      //creates a pointer to a node starting at the head of the list
  while (ptr != NULL) {         //While there are nodes to step through
    if (ptr != list->head) {      //If the node is not the head node, print "->"
      printf("->");
    }
    printf("(%d)", ptr->id);   //Print the name and id of the node
    ptr = ptr->next;      //Navigate to the next node
  }
  printf("\n");
  //printf("The list now has %d elements.\n", list->size);

}

void print_coordinates(struct List *list) {
  struct Node *ptr = list->head;
  while (ptr != NULL) {         //While there are nodes to step through
    
    printf("Node: %d     \tx: %lf\ty: %lf\n", ptr->id, ptr->x, ptr->y);
    ptr = ptr->next;      //Navigate to the next node
  }
  printf("\n");
}

void destroy_list(struct List *list) {    //Takes a pointer to a list
  struct Node *ptr = list->head;      //Pointer to the head node
  struct Node *tmp;             //Temporary node pointer
  while (ptr != NULL) {       //While nodes exist
    tmp = ptr;            //Set the temporary node to the current node
    ptr = ptr->next;      //Point to the next node
    free(tmp);          //Free the temporary node
  }
  free(list);           //After all nodes are freed, free the space for the list
}

void swap_nodes(struct Node *node1, struct Node *node2, struct List *list) {
  struct Node *tmp = NULL;
  
  if (node1 == node2) { return; }

  if (node1->prev != NULL) {
    node1->prev->next = node2;
  } else {
    list->head = node2;
  }

  if (node2->prev != NULL) {
    node2->prev->next = node1;
  } else {
    list->head = node1;
  }

  tmp = node1->prev;
  node1->prev = node2->prev;
  node2->prev = tmp;

  tmp = node1->next;
  node1->next = node2->next;
  node2->next = tmp;

  if (node1->next != NULL) {
    node1->next->prev = node1;
  }

  if(node2->next != NULL) {
    node2->next->prev = node2;
  }

}

double calculate_distance(struct Node *node1, struct Node *node2) {
  return sqrt((((node2->x - node1->x)*(node2->x - node1->x)) + ((node2->y - node1->y)*(node2->y - node1->y))) + 0.0);
}

double permutation_distance(struct Node *node) {
  double distance = 0.0;
  struct Node *tmp = NULL;
  tmp = node;

  while (tmp->next != NULL){
    distance += calculate_distance(tmp, tmp->next);
    tmp = tmp->next;
  }

  return distance;
}

double permutations(struct Node *current, struct List *list) {
  double distance = 0.0;
  double shortest = 0.0;

  
  if (current->next == NULL){
    distance = permutation_distance(list->head);

    if (distance < shortest) {
      shortest = distance;
      printf("New Shortest Distance: %lf\n", shortest);
      print_list(list);
    } 
    //printf("current->next == NULL, current value = %d\n", current->id);
    //compare distance between shortest and current and update if necessary
    //Swap nodes with previous
    return 0.0;
  }
  
  if (list->size == 1) { return 0; }

  //struct Node *node = NULL;
  //node = current;

  for (struct Node *node = current; node != NULL; node = node->next) {
    //printf("Swapping nodes\n");
    swap_nodes(current, node, list);
    print_list(list);
    permutations(node->next, list);
    //printf("Swapping nodes back\n");
    swap_nodes(current, node, list);
    print_list(list);

  }

  return shortest;
}

void bruteForce(struct List *list) {

  double shortest = 0.0;

  shortest = permutations(list->head, list);

  printf("The shortest distance is %lf\n", shortest);
  printf("The recommended path of travel is:\n");

}

int main(int argc, char *argv[]) {              //Main function takes command line arguments
    struct List *list = create_list();        //create a list pointer and create a list
    struct Node *tmp = NULL;                  //create a temporary node pointer and set to 
    int id = 0;
    double x,y = 0;
    //char *string = NULL;

    // Opening file
    FILE *file_ptr = NULL;

    // Opening file in reading mode
    file_ptr = fopen("nodes5.tsp", "r");

    if (file_ptr == NULL) {
        printf("file can't be opened \n");
        return EXIT_FAILURE;
    }

    //printf("Contents of the File are:\n");   

    while(fscanf(file_ptr, "%d %lf %lf", &id, &x, &y) == 3) {
        //printf("Node: %d     \tx: %lf\ty: %lf\n", id, x, y);
        tmp = create_node(id);
        tmp->x = x;
        tmp->y = y;

        insert_tail(tmp, list);
    }

    //print_list(list);

    //bruteForce(list);

    struct Node *node1 = list->head;
    struct Node *node2 = list->head->next;
    printf("Distance: %lf", calculate_distance(node1, node2));
    //swap_nodes(node1, node2, list);

    //print_list(list);

    //find_shortest(list);

    //print_coordinates(list);
    

    destroy_list(list);

    // Closing the file
    fclose(file_ptr);
    
    return 0;
}
