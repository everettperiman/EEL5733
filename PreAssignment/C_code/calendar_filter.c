#include <stdio.h>
#include <string.h>

struct CalendarEvent
{
    char type;
    char name[10];
    char location[10];
    int day;
    int month;
    int year;
    int hour;
    int minute;
    struct CalendarEvent *next_event;

};




int main()
{



}