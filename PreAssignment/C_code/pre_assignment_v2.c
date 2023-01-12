#include <stdio.h>
#include <string.h>

const int STRLEN = 50;

int checkFilter(char* input_string);

int main()
{
    // Create variable to assert if the meeting is valid
    int good_meeting;

    // The string has a max size of 48
    char string[STRLEN];

    // Get the current string from stdin
    while(1)
    {
        if(fgets(string, STRLEN, stdin) == NULL) return 0;    

        // Check if the meeting string is good
        good_meeting = checkFilter(string);

        // Print if the meeting string was good or bad
        if(good_meeting) fprintf(stdout, "%s\n", &string[9]);
    }
    return 0;
}

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
    for(int i = 0; i < STRLEN; i++)
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
    for(int i = 0; i < valid_subject_count; i++)
    {
        a = input_string[subject_position];
        b = valid_subjects[i];
        if(a == b) return 1;
    }  
    return 0;
}


