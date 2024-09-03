#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <time.h>

double shortest = 0.0;

struct Node {                 //Basic component of the list each list has: a pointer to the next and previous node
  int id;                     //the ID of the node, the x-coordinate of the node, the y-coordinate
  double x;
  double y;
  struct Node *next;
  struct Node *prev;
};

struct List {               //A list contains pointers to the head and tail nodes
  struct Node *head;
  struct Node *tail;
  int size;                 //A variable that contains the size of the list
};

struct List *shortestList = NULL;  //Global variable to hold the shortest list

//Returns a pointer to a newly node 
struct Node *create_node(int id) {        //Takes a pointer to a character string, and an id
  struct Node *node = malloc(sizeof(struct Node));    //node is a pointer to the memory space allocated by malloc which allocates the enough memory space for the node component
  if (node == NULL) {
    fprintf (stderr, "%s: Couldn't create memory for the node; %s\n", "linkedlist", strerror(errno));
    exit(-1);
  }
  node->id = id;      //Specify the id of the node that  
  node->next = NULL;  //
  node->prev = NULL;  //Initialize the rear and front pointers of the node
  return node;        //Returns a pointer to the created node
}

//Returns a pointer to a newly created list
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

//Inserts the passed node into the tail of the passed list
void insert_tail(struct Node *node, struct List *list) {    //Takes a pointer to a node, and a pointer to a list
  if (list->head == NULL && list->tail == NULL) {   //If the list contains no elements, set both pointers to the node
    list->head = node;
    list->tail = node;
    list->size++;
  } else {                    //If the list contains at least one other element, do the following:
    list->tail->next = node;  //Set the next pointer of previous tail node to the next node
    node->prev = list->tail;  //Set the previous pointer of the current node to the previous tail node
    list->tail = node;        //Set the tail pointer to the current node
    list->size++;
  }
}

//Inerts the passed node into the head of the passed list
void insert_head(struct Node *node, struct List *list){ //Same thing as the insert_tail but with modified pointer switch
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

//Prints the passed list out to the console
void print_list(struct List *list) {    //Takes a pointer to the list to print
  struct Node *ptr = list->head;        //creates a pointer to a node starting at the head of the list
  while (ptr != NULL) {                 //While there are nodes to step through
    if (ptr != list->head) {            //If the node is not the head node, print "->"
      printf("->");
    }
    printf("(%d)", ptr->id);            //Print the name and id of the node
    ptr = ptr->next;                    //Navigate to the next node
  }
  printf("\n");
  //printf("The list now has %d elements.\n", list->size);

}

//Print the nodes and their coordinates out to the console
void print_coordinates(struct List *list) {
  struct Node *ptr = list->head;
  while (ptr != NULL) {         //While there are nodes to step through
    //Prints out the node id, and coordinates in the specified pattern
    printf("Node: %d     \tx: %lf\ty: %lf\n", ptr->id, ptr->x, ptr->y); 
    ptr = ptr->next;      //Navigate to the next node
  }
  printf("\n");
}

//Freess the memory of all nodes in the list and the list itself
void destroy_list(struct List *list) {    //Takes a pointer to a list
  struct Node *ptr = list->head;          //Pointer to the head node
  struct Node *tmp;                       //Temporary node pointer
  while (ptr != NULL) {                   //While nodes exist
    tmp = ptr;                            //Set the temporary node to the current node
    ptr = ptr->next;                      //Point to the next node
    free(tmp);                            //Free the temporary node
  }
  free(list);           //After all nodes are freed, free the space for the list
}

//Copies the current shortest list arrangement into the global variable "shortestList"
void copy_list(struct List *list){
  if (shortestList != NULL) { destroy_list(shortestList); } //We do not want to destroy the list if it has not been created

  shortestList = create_list();       //Allocate a new memory space for the list
  struct Node *tmp = NULL;            //Creates a new temporary node and sets it to NULL
  struct Node *ptr = list->head;      //Creates a pointer to a node starting at the head of the list
  
  while (ptr != NULL) {               //While there are nodes to step through
    tmp = create_node(ptr->id);       //Create a new node with the id of the node to copy
    tmp->x = ptr->x;                  //Assign the x-coordinate of the node to the new temporary node
    tmp->y = ptr->y;                  //Assign the y-coordinate of the node to the new temporary node

    insert_tail(tmp, shortestList);   //Insert the new temporary node into the shortest list
    ptr = ptr->next;                  //Navigate to the next node
  }
  //printf("The list now has %d elements.\n", list->size);
}

//Swap node1 with node2 in a list
void swap_nodes(struct Node *node1, struct Node *node2, struct List *list) {
  struct Node *tmp = NULL;          //Create a new temporary node
  
  if (node1 == node2) { return; }   //If node1 is node2, don't modify anything

  if (node1->prev != NULL) {        //If node1 is not at the head of the list
    node1->prev->next = node2;      //Set the next pointer of node1's previous node to node2
  } else {                          //If node1 is at the head of the list
    list->head = node2;             //Set the head of the list to node2
  }

  if (node2->prev != NULL) {        //If node2 is not at the head of the list
    node2->prev->next = node1;      //Set the next pointer of node2's previous node to node 1
  } else {                          //If node1 is at the head of the list
    list->head = node1;             //Set the head of the list to node2
  }

  tmp = node1->prev;                //Set tmp to node1->prev
  node1->prev = node2->prev;        
  node2->prev = tmp;                //Swap the prev pointers

  tmp = node1->next;                //Set tmp to node1->next
  node1->next = node2->next;        
  node2->next = tmp;                //Swap the next pointers

  if (node1->next != NULL) {        //If node1 is not the tail of the list
    node1->next->prev = node2;      //Set the previous pointer of node1's next node to node 2
  } else {                          //If node1 is at the tail of the list
    list->tail = node2;             //Set the tail of the list to node2
  }

  if(node2->next != NULL) {         //If node2 is not the tail of the list
    node2->next->prev = node1;      //Set the previous pointer of node2's next node to node 1
  } else {                          //If node2 is at the tail of the list
    list->tail = node1;             //Set the tail of the list to node2
  }

}

//Calculate the distance between two nodes using the distance formula
double calculate_distance(struct Node *node1, struct Node *node2) {
  return sqrt((((node2->x - node1->x)*(node2->x - node1->x)) + ((node2->y - node1->y)*(node2->y - node1->y))) + 0.0);
}

//Calculates the total distance for the current permutation
//The current permutation is decided by the current order of the list
//Proper use of this function passes in list->head into this function
double permutation_distance(struct Node *node) {
  double distance = 0.0;              //Keeps track of the permutation distance
  struct Node *tmp = NULL;            //Create a pointer to a temporary node
  tmp = node;                         //Set tmp = node for safe handling of pointers.
  //printf("Distance for this permutation: ");

  while (tmp->next != NULL){          //While the next node from temp is != NULL
    //printf("Distance between %d & %d:\n", tmp->id, tmp->next->id);
    distance += calculate_distance(tmp, tmp->next); //Add the distance from the current node to the next node
    tmp = tmp->next;                  //Move to the next node
  }

  if (tmp->next == NULL){             //If tmp is at the end of the list
    //printf("Distance between %d & %d:\n", tmp->id, node->id);
    distance += calculate_distance(tmp, node);  //Add the distance from the list end to the list head
  }

  //printf("%lf\n", distance);
  return distance;                    //Return the distance
}

//Iterate through all the permutations of the list
double permutations(struct Node *current, struct List *list) {
  double distance = 0.0;                //Keep track of the distance 
  
  //Base case of the recursion
  //This will indicate that a new permutation has been created
  if (current->next == NULL){           //If the next node is at the end of the list
    distance = permutation_distance(list->head);  //Calculate the distance for the permutation
    //printf("A new challenger approaches: %lf\n", distance);

    if (distance < shortest) {          //If the current distance is the current shortest distance
      shortest = distance;              //Set the shortest distance to the current distance
      //printf("New Shortest Distance: %lf\n", shortest);
      //print_list(list);
      copy_list(list);                  //copies the current permutation into the shortest list
    } else if (shortest == 0.0){        //If the shortest distance has not been updated yet
      copy_list(list);                  //Update the shortest list and distance to start
      shortest = distance;
    }
    //printf("current->next == NULL, current value = %d\n", current->id);
    return shortest;                    //Return the shortesst distance
  }
  
  //If the list is only size 1, there is nowhere to travel
  if (list->size == 1) { return 0; }

  //Recursive case to generate permutations
  //Will loop starting at the list->head until it reaches the end of the list, this will end up looping n! times.
  for (struct Node *node = current; node != NULL; node = node->next) {
    //printf("Swapping nodes\n");
    swap_nodes(current, node, list);  //Swap the current and node nodes. For the first recursive case, it will not swap anything
    //print_list(list);
    permutations(node->next, list);   //Recursively call permutations with the next node in the list, effectively does the permutations of a list with (n-1) size
    //printf("Swapping nodes back\n");
    swap_nodes(current, node, list);  //Swap the nodes back from earlier to get the next permutations
    //print_list(list);

  }
  //copy_list(list);
  return shortest;     //Return the shortest distance
}


//Starts the bruteForce process by initially calling the permutations function
void bruteForce(struct List *list) {

  //printf("%lf\n", permutations(list->head, list));

  shortest = permutations(list->head, list);          //Sets the shortest distance to the distance returned by permutations

  printf("The shortest distance is %lf\n", shortest); //prints out what the shortest distance is
  printf("The recommended path of travel is:\n");     //Prints out the permutation that results in the shortest distance
  print_list(shortestList);

}

int main(int argc, char *argv[]) {            //Main function takes command line arguments
    struct List *list = create_list();        //create a list pointer and create a list
    struct Node *tmp = NULL;                  //create a temporary node pointer and set to 
    int id = 0;                               //Variable to read the id of the node
    double x,y = 0;                           //Variables to store the coordinates of the nodes
    
    
    double startingClock = clock();           //Takes the clock time to calculate hwo long the program has been running later
      
    // Opening file
    FILE *file_ptr = NULL;                    //Creates a file pointer to read from the tsp file

    // Opening file in reading mode
    file_ptr = fopen("nodes20.tsp", "r");     //Opens the tsp file to read

    if (file_ptr == NULL) {                   //If the file was not opened successfully
      printf("file can't be opened \n");      
        return EXIT_FAILURE;
    }

    //printf("Contents of the File are:\n");   

    while(fscanf(file_ptr, "%d %lf %lf", &id, &x, &y) == 3) {   //While the fscanf function is able to map the id and coordinates of nodes
      //printf("Node: %d     \tx: %lf\ty: %lf\n", id, x, y);
      tmp = create_node(id);                  //Create a temporary node with the id read from the file
      tmp->x = x;                             //Assign the x and y coordinates to the node from the file
      tmp->y = y;

      insert_tail(tmp, list);                 //Insert the node into the tail of the list
    }

    bruteForce(list);                         //Brute force through all permutations of the list
      
    destroy_list(list);                       //Destroy the list when we are done with it

    // Closing the file
    fclose(file_ptr);

    double finishingClock = clock();          //Take the clock value when the program is finished
    
    //Print out the program run time in seconds
    printf("Program run time: %lf seconds\n", (finishingClock - startingClock) / CLOCKS_PER_SEC);
    
    return 0;
}
