#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <string>

using namespace std;

// This will be used later on to convert digits in routine file
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
	int frequency = 0;
	bool daily = true;
	bool weekly = true;
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
void getSteps(Routine* routines, int numRoutines);
void newRoutine();

int main()
{
	int numRoutines = 0;

	Routine* routines = getRoutines(numRoutines);
	getSteps(routines, numRoutines);

	// For display
	time_t current = time(0);
	tm local;
	localtime_s(&local, &current);
	int day = local.tm_wday;

	int option = 0;

	while (option != 5)
	{
		system("cls");

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

	for (int i = 0; i < numRoutines; i++)
	{
		getline(inFile, routines[i].title);
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

		// Loads all step information into each routine
		while (getline(inFile, description) && description != "*")
		{
			Node* newNode = new Node;

			inFile >> newNode->item.frequency
				>> newNode->item.daily
				>> newNode->item.weekly
				>> newNode->item.days;
			// Prevents next description from being ignored
			inFile.ignore(numeric_limits<streamsize>::max(), '\n');

			newNode->item.description = description;
			newNode->next = routines[i].steps;
			routines[i].steps = newNode;
		}
	}
}

void newRoutine()
{
	// Text added to the file will be appended 
	ofstream outFile("Routines.txt", ios::app);

	if (!outFile)
	{
		cerr << "ERROR: File cannot be opened.";
	}

	bool newRoutine = true;
	string input;

	while (newRoutine)
	{
		cout << "Please enter a name for your new routine: ";
		// Ensures any input with whitespace will be read completely
		cin.ignore();
		getline(cin, input);

		outFile << input << "\n";

		bool newStep = true;

		// Gets all details for the current step from the user
		while (newStep)
		{
			cout << "Add a step to this routine? Enter Y for Yes or N for No: ";
			cin >> input;

			if (input == "Y" || input == "y")
			{
				cout << "Please enter a description for this step: ";

				cin.ignore();
				getline(cin, input);

				outFile << input << "\n";

				int numInput = 0;

				// Error handling. Maintains an infinite loop until the user gives valid input
				while (true)
				{
					cout << "How many times per day should this step be completed? Please enter a number: ";

					// Valid input written to file
					if (cin >> numInput && numInput > 0)
					{
						outFile << numInput << "\n";
						break;
					}
					else
					{
						// Clears the buffer for next input attempt
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						cout << "Invalid input. Please try again.\n";
					}
				}

				while (true)
				{
					cout << "Repeat this step weekly? Enter Y for Yes or N for No: ";
					cin >> input;

					if (input == "Y" || input == "y")
					{
						outFile << "1\n";
						break;
					}
					else if (input == "N" || input == "n")
					{
						outFile << "0\n";
						break;
					}
					else
					{
						cout << "Invalid input. Please try again.\n";
					}
				}

				while (true)
				{
					cout << "Repeat this step daily? Enter Y for Yes or N for No: ";
					cin >> input;

					if (input == "Y" || input == "y")
					{
						outFile << "1\n";
						break;
					}
					else if (input == "N" || input == "n")
					{
						outFile << "0\n";

						cout << "One which days should this step be repeated?" << endl
							<< "1. Sunday" << endl
							<< "2. Monday" << endl
							<< "3. Tuesday" << endl
							<< "4. Wednesday" << endl
							<< "5. Thursday" << endl
							<< "6. Friday" << endl
							<< "7. Saturday" << endl
							<< "To choose multiple days, enter the numbers together (ex: 123): ";
						cin >> numInput;

						outFile << numInput;
						break;
					}
				}
			}
			else if (input == "N" || input == "n")
			{
				newStep = false;
			}
			else
			{
				cout << "Invalid input. Please try again.\n";
				continue;
			}
		}
		cout << "Routine created!" << endl
			 << "Create another routine? Enter Y for Yes or N for No: ";
		cin >> input;

		newRoutine = (input == "Y" || input == "y");
	}
}