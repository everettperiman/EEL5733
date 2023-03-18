#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/mman.h>

/**************************
NEW Code
***************************/
// New structures to track items in the dynamic circular buffer
typedef struct BufferItem{
    struct BufferItem * next;
    struct BufferItem * prev;
    char* buffervalue;
} buffer_item_t;

typedef struct CircularBuffer{
    struct BufferItem * first;
    struct BufferItem * last;
    int count;
    int size;
} circular_buffer_t;

// Declarations of the new varaibles that are used
const int CALENDAR_FILTER_STRLEN = 42;
circular_buffer_t* circ_buffer;

// Declarations of the new functions that are used
// They are placed like this for better code readability 
// These two functions are placed after the main code
void* producer(void* args);
void* consumer(void* args);


buffer_item_t* create_buffer_item(char* string_text, buffer_item_t* head)
{
    buffer_item_t* buffpointer;
    buffpointer = (buffer_item_t*)malloc(sizeof(buffer_item_t));
    buffpointer->next = NULL;
    buffpointer->prev = head;
    if(buffpointer->prev != NULL) buffpointer->prev->next = buffpointer;
    buffpointer->buffervalue = (char *)malloc(sizeof(char) * (CALENDAR_FILTER_STRLEN + 1));
    strcpy(buffpointer->buffervalue, string_text);
    return buffpointer;
}

buffer_item_t* delete_buffer_item(buffer_item_t* buffer_item)
{
    //printf("%s", buffer_item->buffervalue)
    buffer_item_t* temp = NULL;
    if(buffer_item->next != NULL)
    {
        temp = buffer_item->next;
        temp->prev = NULL;
    }
    free(buffer_item->buffervalue);
    //free(buffer_item);
    return temp;

}

void free_buffer_items(buffer_item_t* buffer_item)
{
    buffer_item_t* temp_next;
    temp_next = buffer_item->next;
    if(delete_buffer_item(buffer_item) != NULL)
        free_buffer_items(temp_next);
    else
        return;
}

/**********************************************************
Email Filter Code
***********************************************************/
const int EMAIL_FILTER_STRLEN = 51;

int checkFilter(char* input_string)
{   
    int compare_result = 0;
    char wild_card = 42;
    char *valid_string = "Subject:**,**********,**/**/****,**:**,**********";
    char *valid_subjects = "CXD";
    int valid_subject_count = 3;
    int subject_position = 9;
    char a, b;

    // Check each character in the string
    for(int i = 0; i < (EMAIL_FILTER_STRLEN-2); i++)
    {
        a = valid_string[i];
        b = input_string[i];
        // Check if the valid string is using a wildcard in that space
        if(a != wild_card)
        {
            // Compare the two chars to determine if they are the same
            if(a != b) return 0;
        }
    }

    // Check if the newline is at the end of the string after the last 10 chars
    a = input_string[EMAIL_FILTER_STRLEN-2];
    if(a != '\n') return 0;

    // Check if there is a valid subject type
    for(int i = 0; i < valid_subject_count; i++)
    {
        a = input_string[subject_position];
        b = valid_subjects[i];
        if(a == b) return 1;
    }  
    return 0;
}

/**********************************************************
Calendar Filter Code
***********************************************************/
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
event_node_t * populateEvent(char event_str[CALENDAR_FILTER_STRLEN], event_node_t * previous_event)
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


/**********************************************************
New Code
***********************************************************/
typedef struct IPC_Data{
    pthread_cond_t bufferEmpty;
    pthread_cond_t bufferFull;
    pthread_mutex_t sharedMutex;
    circular_buffer_t circ_buffer;
    int kill_flag;
} ipc_data_t;

ipc_data_t *IPC;

/**********************************************************
Main Code
***********************************************************/

int main(int argc, char* argv[])
{
    int buf_size;
    // Check if a number has been passed through
    if(argc == 2)
    {
        buf_size = atoi(argv[1]);
        if(buf_size < 1)
        {
            printf("Invalid buffer size of %d\n", buf_size);
            return 0;
        }
    }
    else
    {
        return 0;
    }

    // Create new shared memory region
    IPC = mmap(NULL, sysconf(_SC_PAGESIZE), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    pthread_cond_init(&IPC->bufferEmpty, NULL);
    pthread_cond_init(&IPC->bufferFull, NULL);
    pthread_mutex_init(&IPC->sharedMutex, NULL);
    IPC->kill_flag = 0;
    IPC->circ_buffer.first = NULL;
    IPC->circ_buffer.last = NULL;
    IPC->circ_buffer.count = 0;
    IPC->circ_buffer.size = buf_size;

    circ_buffer = &IPC->circ_buffer;

    // Create pthread types for both threads
    pthread_t t1;
    pthread_t t2;
    pthread_create(&t1, NULL, &producer, NULL);
    pthread_create(&t2, NULL, &consumer, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    //free(circ_buffer);
    return 0;
}


/**********************************************************
Producer Consumer Code
***********************************************************/
void* producer(void* args)
{
    char string[EMAIL_FILTER_STRLEN];
    while(1)
    {
        // Obtain the lock
        pthread_mutex_lock(&IPC->sharedMutex);
        // While the buffer is full
        while(circ_buffer->count == circ_buffer->size)
        {
            pthread_cond_wait(&IPC->bufferFull, &IPC->sharedMutex);
        }
        
        // If there is no more information to ingest unlock, flag, and return
        if(fgets(string, EMAIL_FILTER_STRLEN, stdin) == NULL)
        {
            IPC->kill_flag = 1;
            pthread_mutex_unlock(&IPC->sharedMutex);
            pthread_cond_signal(&IPC->bufferEmpty);
            return NULL;
        }
        // Process the string
        // Create a new buffer item
        else
        {
            if(checkFilter(string))
            {
                circ_buffer->first = create_buffer_item(&string[9], circ_buffer->first);
                if(circ_buffer->count == 0) circ_buffer->last = circ_buffer->first;
                circ_buffer->count++;
            }
        }
        pthread_mutex_unlock(&IPC->sharedMutex);
        pthread_cond_signal(&IPC->bufferEmpty);  
    }
    return NULL;
}

void* consumer(void* args)
{
    // Setup variables
    char string[CALENDAR_FILTER_STRLEN];
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
    
    buffer_item_t* temp_last;

    while(1)
    {
        pthread_mutex_lock(&IPC->sharedMutex);
        // Wait until the buffer is no longer empty
        while(circ_buffer->count == 0)
        {
            if(IPC->kill_flag == 1)
            {
                pthread_mutex_unlock(&IPC->sharedMutex);
                return NULL;
            }
            pthread_cond_wait(&IPC->bufferEmpty, &IPC->sharedMutex);
        }
        /**********************************************
        Beginning of the imported code
        **********************************************/
        if(circ_buffer->last != NULL)
        {
            //printf("%s", string);
            // Create a new temporary node based on the input data
            temp_node = populateEvent(circ_buffer->last->buffervalue, NULL);

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
            /**********************************************
            End of the imported code
            **********************************************/
            circ_buffer->last = delete_buffer_item(circ_buffer->last);
            circ_buffer->count--;
        }
        pthread_mutex_unlock(&IPC->sharedMutex);
        pthread_cond_signal(&IPC->bufferFull);
    }
    
    return NULL;
}