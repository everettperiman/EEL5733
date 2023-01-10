#include <stdio.h>

// Subject: X,Meeting ,01/12/2019,15:45,Larsen239
// Read the first 9 characters they should equal "Subject: "
// Read the next 1 char, they should be either C, X, or D
// Read the next 1 char it will be a comma
// Read the next 10 chars, the are the title of the meeting
// Read the next 1 char it will be a comma
// Read the next 10 char it will be the date
// Read the next 1 char it will be a comma
// Read the next 5 chars it will be the time
// Read the next 10 chars it will be the location
// Total length of the string should be 48 chars

int checkFilter(char* input_string);

int main()
{
    int good_meeting = 0;

    char string[48];
    fgets(string, 48, stdin);    

    good_meeting = checkFilter(string);
    if(good_meeting) printf("That meeting was good!\n");
    else printf("That meeting was bad\n");

    return 0;
}

int checkFilter(char* input_string)
{   
    printf("%s", input_string);
    int comparison_results = 0;
    char *subject_string = "Subject: ";
    char comparision_string[1 + sizeof(subject_string)/sizeof(char)];
    memcpy(comparision_string, &input_string[0], 9);
    comparison_results = strcmp(subject_string, comparision_string);
    if (comparison_results == 0) return 1;
    
    return 0;
}


