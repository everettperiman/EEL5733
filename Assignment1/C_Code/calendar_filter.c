#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Set a constant int to that of the expected STRLEN
const int STRLEN = 42;

// Create a CalendarEvent structure to track details
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

// Create a new event based on input text
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

    return new_event;
}

// Insert a new node at the end of the linked list
void insert_node(event_node_t * root_node, event_node_t * temp_node)
{
    if(root_node->next_event != NULL) 
        insert_node(root_node->next_event, temp_node);
    else
    {
        root_node->next_event = temp_node;
        root_node->next_event->prev_event = root_node;
    }
}

// Print the node details in the desired format
void print_node(event_node_t * node)
{
    fprintf(stdout, "%02d/%02d/%d,%02d:%02d,%s\n", 
    node->month, node->day, node->year, node->hour, node->minute, node->location);
}

// Print the node details on a day that will die
void print_dead_node(event_node_t * node)
{
    fprintf(stdout, "%02d/%02d/%d,--:--,NA\n", node->month, node->day, node->year);
}

// Compare if the two nodes share the same time
int compare_node_time(event_node_t * left_node, event_node_t * right_node)
{
    // Return -1 if the left node is less than
    // Return 1 if the right node is less than
    // Return 0 if they are the same     
    if(left_node->hour < right_node->hour) return -1;
    if(right_node->hour < left_node->hour) return 1;
    if(left_node->minute < right_node->minute) return -1;
    if(right_node->minute < left_node->minute) return 1;
    return 0;
}

// Compare if the two nodes share the same day, 0 indicates the same, -1 indicates the left is earlier
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
    return 0;
}

void delete_node(event_node_t * node)
{
    event_node_t * temp_prev_node = node->prev_event;
    event_node_t * temp_next_node = node->next_event;
    if(node->prev_event != NULL) node->prev_event->next_event = temp_next_node;
    if(node->next_event != NULL) node->next_event->prev_event = temp_prev_node;
    free(node);
}

// Returns 1 if the new node is the earliest or equal to the earliest
int check_if_earliest(event_node_t * traversal_node, event_node_t * new_node)
{
    int comparision_val;
    // Check if both nodes share the same date
    if(compare_node_date(traversal_node, new_node) == 0 && strcmp(traversal_node->name, new_node->name) != 0)
    {
        // Check if the current node is less than the new node, if it is then return 0
        // This demonstrates that the new node is not the earliest
        if(compare_node_time(traversal_node, new_node) == -1)
        {   
            return 0;
        }
    }
    // If the current node is not less that the new node continue of the next event is not NULL
    // If the next event is NULL set the output to 1 denoting that the new node is the earliest
    if(traversal_node->next_event == NULL)
        return 1;
    else
        return check_if_earliest(traversal_node->next_event, new_node);
}

event_node_t * find_next_earliest(event_node_t * traversal_node, event_node_t * old_node, event_node_t * orignal_node, int first_flag)
{
    // If the dates match compare times
    if(compare_node_date(traversal_node, old_node) == 0 && strcmp(traversal_node->name, orignal_node->name) != 0)
    {
        if(first_flag == 1 || compare_node_time(traversal_node, old_node) == -1)
        {
            first_flag = 0;
            if(traversal_node->next_event != NULL)
                return find_next_earliest(traversal_node->next_event, traversal_node, orignal_node, first_flag);
            else
                return traversal_node;
        }
    }
    if(traversal_node->next_event != NULL) return find_next_earliest(traversal_node->next_event, old_node, orignal_node, first_flag);
    else return old_node;

        
}

event_node_t * update_node(event_node_t * current_node, event_node_t * modifiying_node)
{   
    strcpy(current_node->location, modifiying_node->location);
    current_node->day = modifiying_node->day;
    current_node->month = modifiying_node->month;
    current_node->year = modifiying_node->year;
    current_node->hour = modifiying_node->hour;
    current_node->minute = modifiying_node->minute;
    return current_node;
}

event_node_t * find_node_by_name(event_node_t * root_node, event_node_t * search_node)
{
    if(strcmp(root_node->name, search_node->name) == 0) return root_node;
    else
    {
        if(root_node->next_event == NULL)
            return NULL;
        else
            return find_node_by_name(root_node->next_event, search_node);
    }
}

// Returns 1 if it is the last event on that date
int check_if_last(event_node_t * root_node, event_node_t * search_node)
{
    if(compare_node_date(root_node, search_node) == 0 && root_node != search_node) return 0;
    else
    {
        if(root_node->next_event == NULL)
            return 1;
        else
            return check_if_last(root_node->next_event, search_node);
    } 
}


int main()
{
    // Setup variables
    char string[STRLEN];
    // Temporarily store information about the day, month and year
    int temp_day = 0, temp_month = 0, temp_year = 0;
    int print_node_flag = 0;
    // Create temporary event node variables
    event_node_t * head = NULL;
    event_node_t * temp_head = NULL;
    event_node_t * temp_node = NULL;
    event_node_t * modified_node = NULL;
    event_node_t * earliest_node = NULL;
    event_node_t * new_earliest_node = NULL;
    

    // So long as fgets is not NULL keep reading
    while(fgets(string, STRLEN, stdin) != NULL)
    {
        // Create a new temporary node based on the input data
        temp_node = populateEvent(string, NULL);

        // If the head is NULL and string input is not NULL
        if(head == NULL)
        {   
            // Create a new event and assign it to the head pointer
            head = temp_node;
            
            // Print the node value
            print_node(head); 
        }
        else
        {
            // If it is desired to create an event
            if(strcmp(temp_node->type, "C") == 0) 
            {
                // Insert the new node 
                insert_node(head, temp_node);
                // Check if the new node is the earliest on that date
                if(check_if_earliest(head, temp_node) == 1)
                    print_node(temp_node); 
            }
            
            // If it is desired to change an event
            else if(strcmp(temp_node->type, "X") == 0)
            {
                print_node_flag = 0;
                // Find and retrieve the node with the matching name
                modified_node = find_node_by_name(head, temp_node);
                
                // Check if the node is the last of its data and if the new date is different than the current one
                if(check_if_last(head, modified_node) == 1 && compare_node_date(modified_node, temp_node) != 0)
                {
                    print_dead_node(modified_node);
                }

                // Check if the name is getting updated
                //if(strcmp(modified_node->location, temp_node->location) != 0)
                //{
                //    print_node_flag = 1;
                //}

                // If that node exists exchange the temp node data in
                if(modified_node != NULL)
                    modified_node = update_node(modified_node, temp_node);

                // Check if the new node is the earliest on that date
                if(check_if_earliest(head, modified_node) == 1)
                {
                    print_node_flag = 1;
                }
                
                // If it has been determined that the newest event is different 
                // or the location changed and it is the earliest event print the node data
                if(print_node_flag)
                    print_node(modified_node);
            }
            
            // If it is desired to delete an event
            else if(strcmp(temp_node->type, "D") == 0)
            {   
                modified_node = find_node_by_name(head, temp_node);
                // Check if the node that will be deleted is the last event on that date
                if(check_if_last(head, modified_node) == 1)
                {
                    // Print dead node message
                    print_dead_node(modified_node);
                }
                else if(check_if_earliest(head, modified_node) == 1)
                {
                    print_node(find_next_earliest(head, modified_node, modified_node, 1));
                }

                // If the node to delete is the head
                if(find_node_by_name(head, temp_node) == head)
                {
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
                }
                else
                {
                    if(temp_node != NULL) 
                    {
                        delete_node(modified_node);
                    }
                }
            }
        }
    }
    return 0;
}