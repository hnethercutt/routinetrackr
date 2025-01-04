#include <iostream>
#include <ctime>
#include <string>
using namespace std;

const string daysOfWeek[] =
{
    "Sunday",
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday"
};

const int MAXCHARS = 150;

struct Step
{
    bool daily = true; 
    bool repeat = true; 
    char description[MAXCHARS]; 
    char* days; 
    int frequency; 
};

typedef Step ListItemType;

struct Node
{
    ListItemType item;
    Node* next;
};

struct Routine
{
    string title[MAXCHARS]; 
    Node* steps;
};

int main()
{
    time_t current = time(0);
    tm *local = localtime(&current);
    int day = local->tm_wday;
    
    int option = 0;

    while(option != 5)
    {
        cout << "Today's Date Is: " 
             << daysOfWeek[day] << ", "
             << 1 + local->tm_mon << "/" 
             << local->tm_mday << "/" 
             << 1900 + local->tm_year << endl;
        cout << "1. Create a New Routine" << endl;
        cout << "2. View an Existing Routine" << endl;
        cout << "3. Edit an Existing Routine" << endl;
        cout << "4. View Your Stats" << endl;
        cout << "5. Exit" << endl;
        cout << "Choose an Option: ";

        cin >> option;

        switch(option)
        {
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            default:
                cout << "Invalid input\n";
                break;
        }
    }

    return 0;
}