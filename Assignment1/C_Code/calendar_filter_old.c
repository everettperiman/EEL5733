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
event_node_t * update_node(event_node_t * root_node, event_node_t * search_node);
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
    new_event->month = atoi(strncpy(temp, &(event_str[13]), 2));
    new_event->day = atoi(strncpy(temp, &(event_str[16]), 2));
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
        insert_node(root_node->next_event, temp_node);
    }
}

void debug_print_node(event_node_t * node)
{
    fprintf(stdout, "Title: %s Location: %s Type: %s Day: %d Month: %d Year: %d Hour: %d Minute: %d\n", 
    node->name, node->location, node->type, node->day, node->month, node->year, node->hour, node->minute);
}

void print_node(event_node_t * node)
{
    fprintf(stdout, "%02d/%02d/%d,%d:%d,%s\n", 
    node->month, node->day, node->year, node->hour, node->minute, node->location);
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

event_node_t * find_node_name(event_node_t * root_node, event_node_t * search_node)
{
    if(strcmp(root_node->name, search_node->name) == 0) return root_node;
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

event_node_t * update_node(event_node_t * root_node, event_node_t * search_node)
{   
    event_node_t * temp_node = find_node_name(root_node, search_node);
    if(temp_node == NULL)
    {
        //fprintf(stdout, "Node does not exist\n");
        return NULL;
    }
    strncpy(temp_node->name, search_node->name, sizeof(search_node->name));
    strcpy(temp_node->location, search_node->location);
    temp_node->day = search_node->day;
    temp_node->month = search_node->month;
    temp_node->year = search_node->year;
    temp_node->hour = search_node->hour;
    temp_node->minute = search_node->minute;
}

event_node_t * find_earliest_node(event_node_t * root_node, event_node_t * earliest_node)
{
    if(root_node->day == earliest_node->day && earliest_node->month == earliest_node->month && root_node->year == earliest_node->year){
        if(compare_node_date(root_node, earliest_node) <= 0 && root_node != earliest_node) return root_node;
    }
    else
    {
        if(root_node->next_event != NULL)
        {
            return find_earliest_node(root_node->next_event, earliest_node);
        }
        else
        {
            return earliest_node;
        }
    }
}

int find_matching_date(event_node_t * node, int day, int month, int year)
{
    if(node->day == day && node->month == month && node->year == year)
    {
        return 1;
    }
    else
    {
        if(node->next_event == NULL)
            return 0;
        else
            return find_matching_date(node->next_event, day, month, year);
    }
}


int main()
{
    char string[STRLEN];
    int temp_day = 0, temp_month = 0, temp_year = 0;
    int is_earliest = 0;
    event_node_t * head = NULL;
    event_node_t * temp_head = NULL;
    event_node_t * temp_node = NULL;
    event_node_t * earliest_node = NULL;
    event_node_t * new_earliest_node = NULL;
    
    while(1)
    {
        is_earliest = 0;
        if(fgets(string, STRLEN, stdin) == NULL) break;   
        if(head == NULL)
        {
            head = populateEvent(string, NULL);
            earliest_node = head;
            is_earliest = 1;
            print_node(earliest_node);
        }
        else
        {
            // Convert the string to an event
            temp_node = populateEvent(string, NULL);

            // Record the current earliest event on the new date
            if(head != NULL && find_matching_date(head, temp_node->day, temp_node->month, temp_node->year) != 1)
                is_earliest = 1;
            else
                earliest_node = find_earliest_node(head, head);

            // If it is to create an event
            if(strcmp(temp_node->type, "C") == 0) 
            {
                if(head != NULL) insert_node(head, temp_node);
            }
            // If it is to destroy an event
            else if(strcmp(temp_node->type, "D") == 0)
            {
                if(compare_node_details(head, temp_node) == 1)
                {
                    temp_day = temp_node->day;
                    temp_month = temp_node->month;
                    temp_year = temp_node->year;
                    if(head->next_event != NULL)
                    {
                        temp_head = head->next_event;
                        free(head);
                        head = temp_head;
                    }
                    else
                    {
                        free(head);
                        head = NULL;
                    }
                    if(head == NULL || find_matching_date(head, temp_day, temp_month, temp_year) != 1)
                            fprintf(stdout, "%02d/%02d/%d--:--,NA        \n", temp_month, temp_day, temp_year);
                }
                else
                {
                    temp_day = temp_node->day;
                    temp_month = temp_node->month;
                    temp_year = temp_node->year;
                    temp_node = find_node(head, temp_node);
                    if(temp_node != NULL) 
                    {
                        delete_node(temp_node);
                        if(find_matching_date(head, temp_day, temp_month, temp_year) != 1)
                                fprintf(stdout, "%02d/%02d/%d--:--,NA        \n", temp_month, temp_day, temp_year);
                    }
                }
            }
            // If it is to change an event
            else if(strcmp(temp_node->type, "X") == 0)
            {   
                temp_day = temp_node->day;
                temp_month = temp_node->month;
                temp_year = temp_node->year;
                event_node_t * updated_node =  update_node(head, temp_node);
                if(updated_node != NULL)
                    print_node(temp_node);
                    if(find_matching_date(head, temp_day, temp_month, temp_year) != 1)
                            fprintf(stdout, "%02d/%02d/%d--:--,NA        \n", temp_month, temp_day, temp_year);
            }

            // Determine if the earliest node has changed from the earlier check
            if(is_earliest)
            {
                print_node(new_earliest_node);
            }
            else{
                new_earliest_node = find_earliest_node(head, earliest_node);
                if(new_earliest_node != earliest_node) 
                {
                    print_node(new_earliest_node);
                }
            }
        }
        //fprintf(stdout, "%s", string);
    }
    //print_all_nodes(head);    
    return 0;
}