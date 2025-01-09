#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
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

Routine* getRoutines(int& numRoutines);
void getSteps(Routine* routines, int numRoutines);
void newRoutine();

int main()
{
	int numRoutines = 0;

	Routine* routines = getRoutines(numRoutines);
	getSteps(routines, numRoutines);

	time_t current = time(0);
	tm local;
	localtime_s(&local, &current);
	int day = local.tm_wday;

	int option = 0;

	while (option != 5)
	{
		cout << "Today's Date is "
			<< daysOfWeek[day] << ", "
			<< 1 + local.tm_mon << "/"
			<< local.tm_mday << "/"
			<< 1900 + local.tm_year << endl;
		cout << "1. Create a New Routine" << endl;
		cout << "2. View an Existing Routine" << endl;
		cout << "3. Edit an Existing Routine" << endl;
		cout << "4. View Your Stats" << endl;
		cout << "5. Exit" << endl;
		cout << "Choose an Option: ";

		cin >> option;

		switch (option)
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

Routine* getRoutines(int& numRoutines)
{
	ifstream inFile("Routines.txt");

	if (!inFile)
	{
		cerr << "ERROR: File cannot be opened.";
		exit(1);
	}

	string line;

	while (getline(inFile, line))
	{
		numRoutines++;
	}

	inFile.clear();
	inFile.seekg(0, ios::beg);

	Routine* routines = new Routine[numRoutines];

	for (int i = 0; getline(inFile, line); i++)
	{
		routines[i].title = line;
	}

	inFile.close();

	return routines;
}

void getSteps(Routine* routines, int numRoutines)
{
	ifstream inFile("Steps.txt");

	if (!inFile)
	{
		cerr << "ERROR: File cannot be opened.";
		exit(1);
	}

	for (int i = 0; i < numRoutines; i++)
	{
		string description;

		while (getline(inFile, description) && description != "*")
		{
			Node* newNode = new Node;

			inFile >> newNode->item.frequency
				>> newNode->item.daily
				>> newNode->item.weekly
				>> newNode->item.days;
			inFile.ignore(numeric_limits<streamsize>::max(), '\n');

			newNode->item.description = description;
			newNode->next = routines[i].steps;
			routines[i].steps = newNode;
		}
	}
}

void newRoutine()
{
	ofstream outFile("Routines.txt", ios::app);

	if (!outFile)
	{
		cerr << "ERROR: File cannot be opened.";
	}

	string name;
	char choice;
	bool newRoutine = true;
	while (newRoutine)

	{
		cout << "Please enter a name for your new routine: ";
		cin >> name;

		outFile << name << "\n";

		cout << "Create another routine? Enter Y for Yes or N for No: ";
		cin >> choice;

		if (choice == 'N' || choice == 'n')
		{
			newRoutine = false;
		}
	}
}