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
	int frequency = 0;
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
//void viewTitles(Routine* routines, int numRoutines);
//void viewSteps(Routine* routines, int numRoutines, int option);
//void viewRoutine(Routine* routines, int numRoutines);
//void viewAllRoutines(Routine* routines, int numRoutines);
//void editRoutine(Routine* routines, int numRoutines);
//void editStepDescription(Routine* routines, int stepOption, string newDes);
//void editStepFrequency(Routine* routines, int stepOption, int newFreq);
//void editStepWeekly(Routine* routines, int stepOption, int newWeekly);
//void editStepDaily(Routine* routines, int stepOption, int newDaily);
//void editStepDays(Routine* routines, int stepOption, int newDays);

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
		    << "3. Edit an Existing Routine" << endl
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
			//viewRoutine(routines, numRoutines);
			break;
		case 3:
			//editRoutine(routines, numRoutines);
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
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

	for (int i = 0; i < numRoutines; i++)
	{
		string description;
		int routineIndex = 0;

		while (inFile >> description && description != "*")
		{
			// Loads all step information into each routine
			Node* newNode = new Node;

			inFile >> newNode->item.frequency
				>> newNode->item.weekly
				>> newNode->item.daily
				>> newNode->item.days;
			// Prevents next description from being ignored/Skips to next line
			inFile.ignore(numeric_limits<streamsize>::max(), '\n');

			newNode->item.description = description;
			newNode->next = routines[routineIndex].steps;
			routines[routineIndex].steps = newNode;

			// Moves to the next step if there is one
			if (!inFile.eof() && routines[routineIndex].steps->next != NULL)
			{
				routineIndex++;
			}
		}
	}

	inFile.close();
}

void newRoutine(Routine* routines, int numRoutines)
{
	bool newRoutine = true;
	char input;
	string title, description;
	int frequency, weekly, daily, days;

	while (newRoutine)
	{
		cout << "Please enter a name for your new routine: ";
		// Ensures any input with whitespace will be read completely
		
		cin.ignore();
		getline(cin, title);

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

				cout << "How many times per day should this step be completed? Please enter a number: ";
				cin >> frequency;

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
			}
		}

		Node* newNode = new Node;
		newNode->item.description = description;
		newNode->item.frequency = frequency;
		newNode->item.weekly = weekly;
		newNode->item.daily = daily;
		newNode->item.days = days;
		newNode->next = NULL;

		newNode->next = routines[numRoutines].steps;
		routines[numRoutines].steps = newNode;

		cout << "Routine created!" << endl
			<< "Create another routine? Enter Y for Yes or N for No: ";
		cin >> input;

		newRoutine = (input == 'Y' || input == 'y');

		if (!newRoutine)
		{
			break;
		}
	}
}

//void viewRoutine(Routine* routines, int numRoutines)
//{
//	int option;
//
//	viewTitles(routines, numRoutines);
//
//	cout << (numRoutines + 1) << ". " << "View All" << endl
//		 << "Select a Routine: ";
//	cin >> option;
//
//	if (option > 0 && option < numRoutines + 1)
//	{
//		viewSteps(routines, numRoutines, option);
//	}
//	else if (option = numRoutines + 1)
//	{
//		viewAllRoutines(routines, numRoutines);
//	}
//
//	cout << "Enter 0 to Return to the Main Menu: ";
//	cin >> option;
//}
//
//void viewAllRoutines(Routine* routines, int numRoutines)
//{
//	system("cls");
//
//	for (int i = 0; i < numRoutines; i++)
//	{
//		Node* current = routines[i].steps;
//		Node* temp = current;
//
//		cout << "Routine: " << routines[i].title << endl;
//
//		// Displays each step of the routine(s)
//		if (current == NULL)
//		{
//			cout << "This routine has no steps." << endl;
//		}
//		else
//		{
//			int stepCount = 1;
//
//			while (current != NULL)
//			{
//				cout << stepCount << ". " << current->item.description << endl
//					<< "To be completed " << current->item.frequency << " times per day." << endl
//					<< ((current->item.weekly == 1) ? "Repeats weekly" : "Does not repeat") << endl
//					<< ((current->item.daily == 1) ? "Repeats daily" : "Repeats on: ") << endl;
//
//				if (current->item.daily == 0)
//				{
//					int temp = current->item.days;
//
//					while (temp != 0)
//					{
//						int day = temp % 10;
//						cout << daysOfWeek[day - 1] << endl;
//						temp /= 10;
//					}
//				}
//				current = current->next;
//				stepCount++;
//			}
//		}
//
//		cout << endl;
//	}
//}
//
//void viewTitles(Routine* routines, int numRoutines)
//{
//	for (int i = 0; i < numRoutines; i++)
//	{
//		cout << (i + 1) << ". " << routines[i].title << endl;
//	}
//}
//
//void viewSteps(Routine* routines, int numRoutines, int option)
//{
//	system("cls");
//
//	Node* current = routines[option - 1].steps;
//	Node* temp = current;
//
//	cout << "Routine: " << routines[option - 1].title << endl;
//
//	// Displays each step of the routine(s)
//	if (current == NULL)
//	{
//		cout << "This routine has no steps." << endl;
//	}
//	else
//	{
//		int stepCount = 1;
//
//		while (current != NULL)
//		{
//			cout << stepCount << ". " << current->item.description << endl
//				<< "To be completed " << current->item.frequency << " times per day." << endl
//				<< ((current->item.weekly == 1) ? "Repeats weekly" : "Does not repeat") << endl
//				<< ((current->item.daily == 1) ? "Repeats daily" : "Repeats on: ") << endl;
//
//			if (current->item.daily == 0)
//			{
//				int temp = current->item.days;
//
//				while (temp != 0)
//				{
//					int day = temp % 10;
//					cout << daysOfWeek[day - 1] << endl;
//					temp /= 10;
//				}
//			}
//			current = current->next;
//			stepCount++;
//			cout << endl;
//		}
//	}
//}
//
//void editRoutine(Routine* routines, int numRoutines)
//{
//	char newRep;
//	int option;
//	int editOption;
//	int stepOption;
//	int newFreq;
//	string newTitle;
//	string newDes;
//
//	cout << "Which routine would you like to edit?" << endl;
//	viewTitles(routines, numRoutines);
//
//	cout << "Choose an Option: ";
//	cin >> option;
//
//	system("cls");
//	
//	cout << "1. Routine Title" << endl
//		<< "2. Step Description" << endl
//		<< "3. Step Frequency" << endl
//		<< "4. Weekly Repetition" << endl
//		<< "5. Daily Repetition" << endl
//		<< "6. Days to Repeat" << endl
//		<< "Choose an Option to Edit: ";
//	cin >> editOption;
//
//	if (editOption != 7)
//	{
//		switch (editOption)
//		{
//		case 1:
//			cout << "Enter a New Title: ";
//			cin.ignore();
//			getline(cin, routines[option - 1].title);
//
//			cout << endl << "Title Updated" << endl;
//			break;
//		case 2:
//			viewSteps(routines, numRoutines, option);
//
//			cout << "Which step would you like to edit?: ";
//			cin >> stepOption;
//
//			cout << "Enter the New Step Description: ";
//			cin.ignore();
//			getline(cin, newDes);
//
//			editStepDescription(routines, stepOption, newDes);
//			cout << endl << "Step Description Updated" << endl;
//			break;
//		case 3:
//			viewSteps(routines, numRoutines, option);
//
//			cout << "Which step would you like to edit?: ";
//			cin >> stepOption;
//
//			cout << "Enter the New Step Frequency: ";
//			cin.ignore();
//			cin >> newFreq;
//
//			editStepFrequency(routines, stepOption, newFreq);
//			cout << endl << "Step Frequency Updated" << endl;
//			break;
//		case 4:
//			viewSteps(routines, numRoutines, option);
//
//			cout << "Which step would you like to edit?: ";
//			cin >> stepOption;
//
//			cout << "Repeat this step weekly? Enter Y for Yes or N for No: ";
//			cin >> newRep;
//
//			if (newRep == 'Y' || newRep == 'y')
//			{
//				editStepWeekly(routines, stepOption, 1);
//			}
//			if (newRep == 'N' || newRep == 'n')
//			{
//				editStepWeekly(routines, stepOption, 0);
//			}
//
//			cout << endl << "Weekly Repeat Updated" << endl;
//			break;
//		case 5:
//			viewSteps(routines, numRoutines, option);
//
//			cout << "Which step would you like to edit?: ";
//			cin >> stepOption;
//
//			cout << "Repeat this step daily? Enter Y for Yes or N for No: ";
//			cin >> newRep;
//
//			if (newRep == 'Y' || newRep == 'y')
//			{
//				editStepDaily(routines, stepOption, 1);
//			}
//			if (newRep == 'N' || newRep == 'n')
//			{
//				editStepDaily(routines, stepOption, 0);
//				editStepDays(routines, stepOption, 0);
//			}
//
//			cout << endl << "Weekly Repeat Updated" << endl;
//			break;
//		case 6: break;
//
//		}
//
//		cout << "Enter 0 to Return to Main Menu: ";
//		cin >> option;
//	}
//}
//
//void editStepDescription(Routine* routines, int stepOption, string newDes)
//{
//	Node* current = routines[stepOption - 1].steps;
//	
//	string oldDes = current->item.description;
//
//	for (int i = 1; i < stepOption; i++)
//	{
//		current->next;
//	}
//
//	current->item.description = newDes;
//}
//
//void editStepFrequency(Routine* routines, int stepOption, int newFreq)
//{
//	Node* current = routines[stepOption - 1].steps;
//
//	int oldFreq = current->item.frequency;
//
//	for (int i = 1; i < stepOption; i++)
//	{
//		current->next;
//	}
//
//	current->item.frequency = newFreq;
//}
//
//void editStepWeekly(Routine* routines, int stepOption, int newWeekly)
//{
//	Node* current = routines[stepOption - 1].steps;
//
//	int oldWeekly = current->item.weekly;
//
//	for (int i = 1; i < stepOption; i++)
//	{
//		current->next;
//	}
//
//	current->item.weekly = newWeekly;
//}
//
//void editStepDaily(Routine* routines, int stepOption, int newDaily)
//{
//	Node* current = routines[stepOption - 1].steps;
//
//	int oldWeekly = current->item.daily;
//
//	for (int i = 1; i < stepOption; i++)
//	{
//		current->next;
//	}
//
//	current->item.daily = newDaily;
//}
//
//void editStepDays(Routine* routines, int stepOption, int newDays)
//{
//	int numInput;
//
//	Node* current = routines[stepOption - 1].steps;
//
//	int oldDays = current->item.days;
//
//	cout << "One which days should this step be repeated?" << endl
//		<< "1. Sunday" << endl
//		<< "2. Monday" << endl
//		<< "3. Tuesday" << endl
//		<< "4. Wednesday" << endl
//		<< "5. Thursday" << endl
//		<< "6. Friday" << endl
//		<< "7. Saturday" << endl
//		<< "To choose multiple days, enter the numbers together (ex: 123): ";
//	cin >> numInput;
//
//	for (int i = 1; i < stepOption; i++)
//	{
//		current->next;
//	}
//
//	current->item.days = newDays;
//}
