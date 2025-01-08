#include <iostream>
#include <fstream>
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

struct Step
{
    string description;
    int frequency; 
    bool daily = true; 
    bool weekly = true;  
    int days; 
};

typedef Step ListItemType;

struct Node
{
    ListItemType item;
    Node* next;
};

struct Routine
{
    string title; 
    Node* steps;
};

Routine* getRoutines();
void newRoutine();

int main()
{
    Routine* routines = getRoutines();

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
                newRoutine();
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

Routine* getRoutines()
{
    ifstream inFile("../output/Routines.txt");

    if(!inFile)
    {
        cerr << "ERROR: File cannot be opened.";
        exit(1);
    }

    int numRoutines = 0;
    string line;

    while(getline(inFile, line, '\0'))
    {
        numRoutines++;
    }

    inFile.clear();
    inFile.seekg(0, ios::beg);

    Routine* routines = new Routine[numRoutines];

    for(int i = 0; getline(inFile, line, '\0'); i++)
    {
        routines[i].title = line;
    }

    inFile.close();

    return routines;
}

void newRoutine()
{
    ofstream outFile("Routines.txt", ios::app);

    if(!outFile)
    {
        cerr << "ERROR: File cannot be opened.";
    }

    string name;
    char choice;
    bool newRoutine = true;
    while(newRoutine)
    
    {
        cout << "Please enter a name for your new routine: ";
        cin >> name;

        outFile << name << "\n";

        cout << "Create another routine? Enter Y for Yes or N for No: ";
        cin >> choice;

        if(choice == 'N' || choice == 'n')
        {
            newRoutine = false;
        }
    }
}