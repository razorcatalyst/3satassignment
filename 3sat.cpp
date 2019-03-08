#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <string>
#include <math.h>
#include <chrono>
#define MIN_VARS 1
#define MAX_VARS 30
#define MIN_CLAUSES 1
#define MAX_CLAUSES 50
using namespace std;
class clause
{
private:
	int variables[MAX_VARS];
	int size; //represents the number of variables in this clause
public:
	clause()
	{
		size = 0;
	}
	void setVariable(int i, int var) //set the variable i in the clause to specified one
	{
		variables[i] = var;
	}
	void setSize(int s) // set the number of variables in this clause
	{
		size = s;
	}

	void print() //prints the clause
	{
		cout << " ( ";
		for (int i = 0; i < size; i++)
		{
			if (i != 0)
				cout << " v ";

			if (variables[i] < 0)
				cout << "~" << "x" << -variables[i];
			else
				cout << "x" << variables[i];
		}
		cout << " ) ";
	}

	int* getVariables() {
		return variables;
	}

	// the truth values for x1, x2 ... 
	//are passed and the clause is evaluated based on these, 
	//the 1st location of truthValues is unused
	bool evaluate(bool truthValues[])
	{
		int idx;
		bool val = false, current;

		for (int i = 0; i < size; i++)
		{
			idx = variables[i];
			if (variables[i] < 0)
			{
				idx = -idx; //conver to +ve and negate the truth value
				current = !truthValues[idx];
			}
			else
				current = truthValues[idx];

			val = val || current;
		}
		return val;
	}
};
//generates variables for this clause and sets its size,. 
//expr_var_used represents the variables that are used in the final expression
//after generating the variables, these variables are set in expr_var_used
void generateVars(clause &c, int expr_var_used[], int numVars);
int *getMaxVariables(int *arr, int array_length, int divider);
void incrementTruthValues(bool *truth_values);
bool getBoolInput(string prompt); //used to prompt the user for a boolean input
//and validate it
int main()
{
	int numClauses, numVars;

	srand(time(0));

	auto start = std::chrono::high_resolution_clock::now(); // Record start time

	int clause_range = MAX_CLAUSES - MIN_CLAUSES + 1;
	numClauses = MAX_CLAUSES;
	//MIN_CLAUSES + (rand() % clause_range); //generates random number between 
	//MIN_CLAUSES to MAX_CLAUSES

	int var_range = MAX_VARS - MIN_VARS + 1;
	numVars = 3;
	//MIN_VARS + (rand() % var_range); //generates random number between MIN_VARS to MAX_VARS

	//numVars is the number of variables that will be in each clause

	bool truth_values[MAX_VARS + 1]; //index 0 is unsed, 1 to MAX_VARS represent 
	//the truth values of x1, x2,.. etc
	int expr_used_vars[MAX_VARS + 1] = { 0 };
	int COMB_VALUE = pow(2, MAX_VARS);
	bool *combination = new bool[COMB_VALUE];

	clause *logicexpr = new clause[numClauses];

	for (int i = 0; i < numClauses; i++)
	{
		generateVars(logicexpr[i], expr_used_vars, numVars); 
		//generate the variables for this clause
	}
	//print the generated logic expression
	for (int i = 0; i < numClauses; i++)
	{
		if (i != 0)
			cout << " ^ ";
		logicexpr[i].print();
	}
	cout << endl << endl;

	//while (true) {
	//	string prompt = "Please enter a value T/F for variable x";
	//	//ask user for values of the variables that were used in the logical expression
	//	for (int i = 1; i < MAX_VARS + 1; i++)
	//	{
	//		if (expr_used_vars[i] == 1) // was this used in any of the clauses ?
	//		{
	//			truth_values[i] = getBoolInput(prompt + to_string(i));
	//		}
	//	}

	//	cout << endl;

	//	//------Exhaustive search----------//
	//	bool expr_value = true;
	//	int j;
	//	for (j = 0; j < numClauses; j++)
	//	{
	//		if (logicexpr[j].evaluate(truth_values) != true) // this clause did not evaluate to true? stop then
	//		{
	//			expr_value = false;
	//			break;
	//		}
	//	}

	//	if (expr_value)
	//		cout << "This assignment is true. " << endl;
	//	else
	//	{
	//		cout << "This assignment is false, due to the clause: " << endl;
	//		logicexpr[j].print();
	//		cout << endl;
	//	}
	//}

	for (int i = 1; i < MAX_VARS + 1; i++) {
		truth_values[i] = 0;
	}

	for (int i = 0; i < COMB_VALUE; i++) {
		bool expr_value = true;
		int j;
		for (j = 0; j < numClauses; j++)
		{
			if (logicexpr[j].evaluate(truth_values) != true)
				//this clause did not evaluate to true? It will stop
			{
				expr_value = false;
				break;
			}
		}

		combination[i] = expr_value;

		incrementTruthValues(truth_values); // Increment the truth values
	}

	string comb = "";

	for (int i = 0; i < COMB_VALUE; i++) {
		// cout << combination[i] << " ";
		if (combination[i] == true)
			comb.append("1 ");
		else
			comb.append("0 ");
	}
	cout << "--------------------Exaustive Search----------------------" << endl;
	cout << comb << endl;
	auto finish = std::chrono::high_resolution_clock::now(); // Record end time
	std::chrono::duration<double> elapsed = finish - start;
	std::cout << "Elapsed time: " << elapsed.count() << endl;

	auto start1 = std::chrono::high_resolution_clock::now(); // Record start time
	const int varCountLength = MAX_VARS * 2 + 1;
	int variableCounters[varCountLength];
	for (int i = 1; i < varCountLength; i++) {
		variableCounters[i] = 0;
	}

	for (int i = 0; i < numClauses; i++) {
		// For every variable in a clause
		for (int j = 0; j < numVars; j++) {
			// The array is divided into two sections, normal and complement variables.
			// The section for complement variables start from the index of MAX_VARS + 1
			int variable_num = logicexpr[i].getVariables()[j];
			if (variable_num > 0) {
				variableCounters[variable_num]++;
			}
			else {
				// MAX_VARS + (-variable_num), variable_num will turn to +ve
				variableCounters[MAX_VARS - variable_num]++;
			}
		}
	}

	cout << endl;
	cout << "--------------------Greedy Search-------------------------" << endl;
	int *mostVariablesDesc = getMaxVariables(variableCounters, varCountLength, MAX_VARS);
	for (int i = 1; i < varCountLength; i++) {
		cout << variableCounters[i] << " ";
	}
	cout << endl;
	for (int i = 0; i < varCountLength - 1; i++) {
		cout << mostVariablesDesc[i] << " ";
	}

	cout << endl;
	auto finish1 = std::chrono::high_resolution_clock::now(); // Record end time
	std::chrono::duration<double> elapsed1 = finish1 - start1;
	std::cout << "Elapsed time: " << elapsed1.count() << endl;

	system("pause");
}

// Divider is the index between the normal variables and complement variables
int* getMaxVariables(int *arr, int array_length, int divider) {
	// The list of variables ordered descendingly by its frequency in the clauses
	int *variable_count_desc = new int[array_length];

	for (int vcd = 0; vcd < array_length; vcd++)
	{
		int max_index = 0, max_num = 0;
		for (int i = 1; i < array_length; i++) {
			// Check if number variable already exist in the max variable list
			int j = 0;
			for (; j < array_length; j++) {
				int v_num = (i > divider) ? -((i - 1) % divider + 1) : i;
				if (variable_count_desc[j] == v_num)
					break;
			}
			// If j is not more than array_length that means the loop is broken
			// Which means the variable exist in the list
			if (j < array_length) continue;

			if (arr[i] > max_num) {
				max_index = i;
				max_num = arr[i];
			}
		}
		variable_count_desc[vcd] = (max_index > divider) ? -((max_index - 1) % divider + 1) : max_index;
	}

	return variable_count_desc;
}

void incrementTruthValues(bool *truth_values) {
	bool addOne = false;
	for (int i = 1; i < MAX_VARS + 1; i++) {
		if (i == 1) {
			if (truth_values[i] == true)
				addOne = true;
			truth_values[i] = !truth_values[i];
		}
		else if (addOne == true) {
			if (truth_values[i] == false)
				addOne = false;
			truth_values[i] = !truth_values[i];
		}
	}
}

// bool getBoolInput(string prompt)
// {
// 	char val;
// 	while (true)
// 	{
// 		cout << prompt << ": ";
// 		cin >> val;
// 		val = toupper(val);
// 		if (val != 'T' && val != 'F')
// 			cout << "invalid input. please try again." << endl;
// 		else
// 			break;
// 	}
// 	if (val == 'T')
// 		return true;
// 	else
// 		return false;
// }

void generateVars(clause &c, int expr_used_vars[], int numVars)
{
	int usedVars[MAX_VARS + 1] = { 0 }; 
	//if 0, corresponding var was not used in the clause, if 1 
	//the corresponindg var was used in the clause

	int var;

	c.setSize(numVars);
	for (int i = 0; i < numVars; )
	{
		var = 1 + rand() % MAX_VARS; // generate a random variable
		if (usedVars[var] == 1)//if the variable is already used, then continut to generate again
			continue;
		else
		{
			int negate = rand() % 2;

			if (negate == 0)
				c.setVariable(i, var);
			else
				c.setVariable(i, -var);

			usedVars[var] = 1; 
			//mark the variable number as used, so that we don't reuse 
			//the same variable in the clause
			expr_used_vars[var] = 1; 
			//mark the variable number that was used, so we can later ask user's input
			i++;
		}
	}
}


