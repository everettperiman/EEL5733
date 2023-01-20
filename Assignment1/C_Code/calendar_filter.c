#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
TODO:
Create a function that searches through and sees how many unique days there are and how many there are
    If a delete function is being called and is going to make any of these numbers zero print the respective message
    ->Record the date and search all nodes to see if any share that same date? If none of them do then print the respective message
Create a function that finds the earliest node, if the new one is earlier then define that one as earlier and print the respective message
    ->This can probably be done by creating a node * that holds the location of the earliest one, then compare each time

*/



const int STRLEN = 42;

typedef struct CalendarEvent
{
    char type[2];
    char name[11];
    char location[11];
    int day;
    int month;
    int year;
    int hour;
    int minute;
    struct CalendarEvent * next_event;
    struct CalendarEvent * prev_event;

} event_node_t;

void delete_node(event_node_t * node);
void print_node(event_node_t * node);
void print_all_nodes(event_node_t * node);
int insert_node(event_node_t * root_node, event_node_t * temp_node);
void update_node(event_node_t * root_node, event_node_t * search_node);
int compare_node_date(event_node_t * left_node, event_node_t * right_node);
int compare_node_details(event_node_t * left_node, event_node_t * right_node);
event_node_t * find_node(event_node_t * root_node, event_node_t * search_node);
event_node_t * find_node_date(event_node_t * root_node, event_node_t * search_node);
event_node_t * populateEvent(char event_str[STRLEN], event_node_t * previous_event);


event_node_t * populateEvent(char event_str[STRLEN], event_node_t * previous_event)
{
    // Create new event node
    char temp[2];
    char long_temp[4];
    event_node_t * new_event = NULL;
    new_event = (event_node_t *)malloc(sizeof(event_node_t));
    
    strncpy(new_event->type, &(event_str[0]), 1);
    new_event->type[1] = '\0';
    strncpy(new_event->name, &(event_str[2]), 10);
    new_event->name[11] = '\0';
    strncpy(new_event->location, &(event_str[30]), 10);
    new_event->location[11] = '\0';
    new_event->day = atoi(strncpy(temp, &(event_str[13]), 2));
    new_event->month = atoi(strncpy(temp, &(event_str[16]), 2));
    new_event->hour = atoi(strncpy(temp, &(event_str[24]), 2));
    new_event->minute = atoi(strncpy(temp, &(event_str[27]), 2));
    new_event->year = atoi(strncpy(long_temp, &(event_str[19]), 4));
    new_event->next_event = NULL;
    new_event->prev_event = previous_event;
    // Debug printout
    return new_event;
}

int insert_node(event_node_t * root_node, event_node_t * temp_node)
{
    if(root_node->next_event == NULL)
    {   // If the root node is the soonest or equal
        root_node->next_event = temp_node;
        root_node->next_event->prev_event = root_node;
        return 1;      
    }
    else
    {
        if(compare_node_details(root_node, temp_node))
        {
            fprintf(stdout, "Duplicate Node\n");
            return 1;
        }
        insert_node(root_node->next_event, temp_node);
    }
}

void print_node(event_node_t * node)
{
    fprintf(stdout, "Title: %s Location: %s Type: %s Day: %d Month: %d Year: %d Hour: %d Minute: %d\n", 
    node->name, node->location, node->type, node->day, node->month, node->year, node->hour, node->minute);
}

void print_all_nodes(event_node_t * node)
{
    print_node(node);
    if(node->next_event != NULL) print_all_nodes(node->next_event);
}

int compare_node_date(event_node_t * left_node, event_node_t * right_node)
{
    // Return -1 if the left node is less than
    // Return 1 if the right node is less than
    // Return 0 if they are the same 
    if(left_node->year < right_node->year) return -1;
    else if(right_node->year < left_node->year) return 1;
    if(left_node->month < right_node->month) return -1;
    else if(right_node->month < left_node->month) return 1;
    if(left_node->day < right_node->day) return -1;
    else if(right_node->day < left_node->day) return 1;
    if(left_node->hour < right_node->hour) return -1;
    else if(right_node->hour < left_node->hour) return 1;
    if(left_node->minute < right_node->minute) return -1;
    else if(right_node->minute < left_node->minute) return 1;
    return 0;
}

int compare_node_details(event_node_t * left_node, event_node_t * right_node)
{
    // Return 1 if they are the same
    // Return 0 if they are not the same 
    if(compare_node_date(left_node, right_node) != 0) return 0;
    if(strcmp(left_node->name, right_node->name) != 0) return 0;
    if(strcmp(left_node->location, right_node->location) != 0) return 0;
    return 1;
}

event_node_t * find_node(event_node_t * root_node, event_node_t * search_node)
{
    if(compare_node_details(root_node, search_node) == 1) return root_node;
    else
    {
        if(root_node->next_event == NULL)
        {   
            return NULL;      
        }
        else
        {
            find_node(root_node->next_event, search_node);
        }
    }
}


event_node_t * find_node_date(event_node_t * root_node, event_node_t * search_node)
{
    if(compare_node_date(root_node, search_node) == 0) return root_node;
    else
    {
        if(root_node->next_event == NULL)
        {   
            return NULL;      
        }
        else
        {
            find_node_date(root_node->next_event, search_node);
        }
    }
}

void delete_node(event_node_t * node)
{
    event_node_t * temp_prev_node = node->prev_event;
    event_node_t * temp_next_node = node->next_event;
    if(node->prev_event != NULL) node->prev_event->next_event = temp_next_node;
    if(node->next_event != NULL) node->next_event->prev_event = temp_prev_node;
    free(node);
}

void update_node(event_node_t * root_node, event_node_t * search_node)
{   
    event_node_t * temp_node = find_node_date(root_node, search_node);
    if(temp_node == NULL)
    {
        fprintf(stdout, "Node does not exist\n");
        return;
    }
    strncpy(temp_node->name, search_node->name, sizeof(search_node->name));
    strcpy(temp_node->location, search_node->location);
    temp_node->day = search_node->day;
    temp_node->month = search_node->month;
    temp_node->year = search_node->year;
    temp_node->hour = search_node->hour;
    temp_node->minute = search_node->minute;
}



int main()
{
    char string[STRLEN];
    
    event_node_t * head = NULL;
    event_node_t * temp_head = NULL;
    event_node_t * temp_node = NULL;
    //head = (event_node_t *)malloc(sizeof(event_node_t));
    
    while(1)
    {
        if(fgets(string, STRLEN, stdin) == NULL) break;   
        if(head == NULL)
        {
            head = populateEvent(string, NULL);
        }
        else
        {
            temp_node = populateEvent(string, NULL);
            // If it is to create an event
            if(strcmp(temp_node->type, "C") == 0) 
            {
                if(head == NULL)
                {
                    head = temp_node;
                }
                else
                {
                    insert_node(head, temp_node);
                }
            }
            // If it is to destroy an event
            else if(strcmp(temp_node->type, "D") == 0)
            {
                if(compare_node_details(head, temp_node) == 1)
                {
                    if(head->next_event != NULL)
                    {
                        temp_head = head->next_event;
                        free(head);
                        head = temp_head;
                    }
                    else
                    {
                        head = NULL;
                    }
                }
                else
                {
                    temp_node = find_node(head, temp_node);
                    if(temp_node == NULL) fprintf(stdout, "Node does not exist\n");
                    else 
                    {
                        fprintf(stdout, "Node does exist!\n");
                        delete_node(temp_node);
                    }
                }
            }
            // If it is to change an event
            else if(strcmp(temp_node->type, "X") == 0)
            {   
                fprintf(stdout, "Modifiying calendar\n");
                update_node(head, temp_node);
            }
        }
        fprintf(stdout, "%s", string);
    }
    print_all_nodes(head);    
    return 0;
}