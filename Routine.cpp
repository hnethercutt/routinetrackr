#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <string>

using namespace std;

const int MAXROUTINES = 100;

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
	bool weekly = true;
	bool daily = true;
	int days = 0;
};

typedef Step ListItemType;

struct Node
{
	ListItemType item;
	Node* next = NULL;
};

struct Routine
{
	string title;
	Node* steps = NULL;
};

Routine* getRoutines(int& numRoutines);
void getSteps(Routine* routines, int& numRoutines);
void newRoutine(Routine* routines, int numRoutines);
void addSteps(Routine* routines, int numRoutines);
void viewTitles(Routine* routines, int numRoutines);
void viewSteps(Routine* routines, int numRoutines, int option);
void viewRoutine(Routine* routines, int numRoutines);
void viewAllRoutines(Routine* routines, int numRoutines);
void deleteRoutine(Routine* routines, int numRoutines);

int main()
{
	int numRoutines = -1;
	Routine* routines = getRoutines(numRoutines);
	getSteps(routines, numRoutines);

	time_t current = time(0);
	tm local;
	localtime_s(&local, &current);
	int day = local.tm_wday;

	int option = 0;

	while (option != 6)
	{
		system("cls");

		cout << "Today's Date is "
			<< daysOfWeek[day] << ", "
			<< 1 + local.tm_mon << "/"
			<< local.tm_mday << "/"
			<< 1900 + local.tm_year << endl
		    << "1. Create a New Routine" << endl
		    << "2. View an Existing Routine" << endl
			<< "3. Delete a Routine" << endl
			<< "4. Mark a Step/Routine as Completed" << endl
		    << "5. View Your Stats" << endl
		    << "6. Exit" << endl
			<< "Choose an Option: ";

		cin >> option;

		switch (option)
		{
		case 1:
			newRoutine(routines, numRoutines);
			break;
		case 2:
			viewRoutine(routines, numRoutines);
			break;
		case 3:
			deleteRoutine(routines, numRoutines);
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		default:
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

	string ignore;
	getline(inFile, ignore);

	Routine* routines = new Routine[MAXROUTINES];

	for (int i = 0; i < numRoutines; i++)
	{
		getline(inFile, routines[i].title);
		routines[i].steps = NULL;
	}

	inFile.close();

	return routines;
}

void getSteps(Routine* routines, int& numRoutines)
{
	ifstream inFile("Steps.txt");

	if (!inFile)
	{
		cerr << "ERROR: File cannot be opened.";
		exit(1);
	}

	string ignore;
	inFile >> ignore;

	for (int i = 0; i < numRoutines; i++)
	{
		string description;

		while (inFile >> description && description != "*")
		{
			// Loads all step information into each routine
			Node* newNode = new Node;

			inFile >> newNode->item.weekly
				   >> newNode->item.daily
				   >> newNode->item.days;
			// Prevents next description from being ignored/Skips to next line
			inFile.ignore(numeric_limits<streamsize>::max(), '\n');

			newNode->item.description = description;
			newNode->next = routines[i].steps;
			routines[i].steps = newNode;
		}
	}

	inFile.close();
}

void newRoutine(Routine* routines, int numRoutines)
{
	ofstream outFile("Routines.txt", ios::app);

	if (!outFile)
	{
		cerr << "ERROR: File cannot be opened.";
		exit(1);
	}

	bool newRoutine = true;
	char decision;
	string title;

	while (newRoutine)
	{
		cout << "Please enter a name for your new routine: ";		
		cin.ignore();
		getline(cin, title);

		outFile << title << endl;
		routines[numRoutines].title = title;

		addSteps(routines, numRoutines);
		
		cout << "Routine created!" << endl
			<< "Create another routine? Enter Y for Yes or N for No: ";
		cin >> decision;

		newRoutine = (decision == 'Y' || decision == 'y');
	}

	outFile.close();
}

void addSteps(Routine* routines, int numRoutines)
{
	ofstream outFile("Steps.txt", ios::app);

	if (!outFile)
	{
		cerr << "ERROR: File cannot be opened.";
		exit(1);
	}

	char input;
	string title, description;
	int weekly, daily, days, numSteps = 0;

	bool newStep = true;

	while (newStep)
	{
		cout << "Add a step to this routine? Enter Y for Yes or N for No: ";
		cin >> input;

		newStep = (input == 'Y' || input == 'y');

		if (!newStep)
		{
			break;
		}
		else
		{
			cout << "Please enter a description for this step: ";
			cin.ignore();
			getline(cin, description);

			cout << "Repeat this step weekly? Enter Y for Yes or N for No: ";
			cin >> input;

			if (input == 'Y' || input == 'y')
			{
				weekly = 1;
			}
			else
			{
				weekly = 0;
			}

			cout << "Repeat this step daily? Enter Y for Yes or N for No: ";
			cin >> input;

			if (input == 'Y' || input == 'y')
			{
				daily = 1;
			}
			else
			{
				daily = 0;
				cout << "One which days should this step be repeated?" << endl
					<< "1. Sunday" << endl
					<< "2. Monday" << endl
					<< "3. Tuesday" << endl
					<< "4. Wednesday" << endl
					<< "5. Thursday" << endl
					<< "6. Friday" << endl
					<< "7. Saturday" << endl
					<< "To choose multiple days, enter the numbers together (ex: 123): ";
				cin >> days;
			}

			Node* newNode = new Node;

			newNode->item.description = description;
			outFile << description << endl;

			newNode->item.weekly = weekly;
			outFile << weekly << endl;

			newNode->item.daily = daily;
			outFile << daily << endl;

			if (daily == 1)
			{
				days = 8;
			}
				
			newNode->item.days = days;
			outFile << days << endl;

			newNode->next = NULL;

			newNode->next = routines[numRoutines].steps;
			routines[numRoutines].steps = newNode;

			numSteps++;
		}
	}

	if (numSteps == 0)
	{
		outFile << "NONE" << endl;
	}
	else
	{
		outFile << "*" << endl;
	}

	outFile.close();
}

void viewRoutine(Routine* routines, int numRoutines)
{
	int option;

	viewTitles(routines, numRoutines);

	cout << (numRoutines + 1) << ". " << "View All" << endl
		 << "Select a Routine: ";
	cin >> option;

	if (option > 0 && option < numRoutines + 1)
	{
		viewSteps(routines, numRoutines, option);
	}
	else if (option = numRoutines + 1)
	{
		viewAllRoutines(routines, numRoutines);
	}

	cout << "Enter 0 to Return to the Main Menu: ";
	cin >> option;
}

void viewAllRoutines(Routine* routines, int numRoutines)
{
	system("cls");

	for (int i = 0; i < numRoutines; i++)
	{
		Node* current = routines[i].steps;
		Node* temp = current;

		cout << "Routine: " << routines[i].title << endl;

		// Displays each step of the routine(s)
		if (current == NULL)
		{
			cout << "This routine has no steps." << endl;
		}
		else
		{
			int stepCount = 1;

			while (current != NULL)
			{
				cout << stepCount << ". " << current->item.description << endl
					<< ((current->item.weekly == 1) ? "Repeats weekly" : "Does not repeat") << endl
					<< ((current->item.daily == 1) ? "Repeats daily" : "Repeats on: ") << endl;

				if (current->item.daily == 0)
				{
					int temp = current->item.days;

					while (temp != 0)
					{
						int day = temp % 10;
						cout << daysOfWeek[day - 1] << endl;
						temp /= 10;
					}
				}
				current = current->next;
				stepCount++;
			}
		}

		cout << endl;
	}
}

void viewTitles(Routine* routines, int numRoutines)
{
	for (int i = 0; i < numRoutines; i++)
	{
		cout << (i + 1) << ". " << routines[i].title << endl;
	}
}

void viewSteps(Routine* routines, int numRoutines, int option)
{
	system("cls");

	Node* current = routines[option - 1].steps;
	Node* temp = current;

	cout << "Routine: " << routines[option - 1].title << endl;

	// Displays each step of the routine(s)
	if (current == NULL)
	{
		cout << "This routine has no steps." << endl;
	}
	else
	{
		int stepCount = 1;

		while (current != NULL)
		{
			cout << stepCount << ". " << current->item.description << endl
				<< ((current->item.weekly == 1) ? "Repeats weekly" : "Does not repeat weekly") << endl
				<< ((current->item.days == 8) ? "Repeats daily" : "Repeats on: ") << endl;

			if (current->item.daily == 0)
			{
				int temp = current->item.days;

				while (temp != 0)
				{
					int day = temp % 10;
					cout << daysOfWeek[day - 1] << endl;
					temp /= 10;
				}
			}
			current = current->next;
			stepCount++;
			cout << endl;
		}
	}
}

void deleteRoutine(Routine* routines, int numRoutines)
{
	ofstream outFile("Routines.txt");

	if (!outFile)
	{
		cerr << "ERROR: File cannot be opened.";
		exit(1);
	}

	viewTitles(routines, numRoutines);

	int option;

	cout << "Choose a Routine to Delete: ";
	cin >> option;

	outFile << "START" << endl;

	for (int i = 0; i < numRoutines; i++)
	{
		if (i == (option - 1))
		{
			i++;
		}

		outFile << routines[i].title << endl;
	}

	outFile.close();

}

