#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#define SIZE 50

// A structure to represent a stack allows push and pop string 
struct Stack
{
	int top;
	unsigned size;
	char **array;
};

// A structure to save variables and their values
typedef struct
{
	int size;
	char *var;
	double *value;
	double notCreated; // If variable not created. This number will return.
}Variable;

struct Stack *createStack(unsigned);// Function to create a Stack of given capacity. Initialize size of stack is 0

void push(struct Stack *, char *);// Add item to Stack

char *pop(struct Stack *);// Remove item from Stack

int isFull(struct Stack *);// If stack is full that means top is equal to last

int isEmpty(struct Stack *);// If stack is empty that means top is -1

char *getAllStack(struct Stack *);// Function to return all stack as a string

Variable *initializeVariables(unsigned);// Function to initialize Variable Struct

char **initializeStrArray(unsigned, unsigned);// Function to initialize StrArray

double getVariable(Variable *, char);// Function to get Variable

void addVariable(Variable *, char, double);// Function to add new Variable

double solvePostfix(struct Stack *, char *, Variable *);// Function to solve the postfix form of arithmatic function

void printVariables(Variable *);// Function to write whole variables with their values

int getPriority(char);// Function to get arithmatic operations priority.

char *createPostfixString(struct Stack *, char *, Variable *, char *);// Function to generate postfix form of arithmetic operation

double operation(double, double, char);// Do arithmetic operation with two double variable





int main() {
	int i = 0, j;// These are for loops
	int lineCount = 0;
	struct Stack *stack = createStack(SIZE);// Stack initialization
	Variable *vars = initializeVariables(SIZE);// Variable initialization
	char **strArray = initializeStrArray(SIZE, SIZE);
	FILE *input = fopen("input.txt", "r");// Opening file	
	if (!input)// If file cannot be opened, program will be end.
	{
		printf("File cannot be open!");
		exit(-1);
	}
	else
	{
		while (!feof(input))// While file isn't over
		{
			char *function = (char *)calloc(100, sizeof(char));// Create a function string which store 100 character
			char variable;
			fgets(function, 100, input);// Read a line from file
			if (function[strlen(function) - 1] == '\n')
				function[strlen(function) - 1] = '\0';
			printf("Reading Line is : %s \n\n \tSTEP 1 : CREATE POSTFIX \n\n", function);
			strArray[lineCount] = createPostfixString(stack, function, vars, &variable);// The function create postfix form and return a char pointer
			addVariable(vars, variable, solvePostfix(stack, strArray[lineCount++], vars));
			printf("\nVariables : ");
			printVariables(vars);//print Variables
			printf("*******************************************\n");

		}
		printf("Created Postfixes are :\n");
		for (i = 0; i < lineCount; i++)// When functions are done postfixes will print
		{
			printf("Postfix Number %d : ", i + 1);
			for (j = 0; j < 100; j++)
				printf("%c", strArray[i][j]);
			printf("\n");
		}
	}
	printf("Created By Bartu");

	return 0;

}
//Stack operations begin
struct Stack *createStack(unsigned capacity)//Create Stack
{
	struct Stack *stack = (struct Stack *) malloc(sizeof(struct Stack));
	stack->size = capacity;
	stack->top = -1;
	stack->array = (char **)calloc(stack->size, sizeof(char *));
	return stack;
}

void push(struct Stack *stack, char *item)// Push item to Stack
{
	if (isFull(stack))
		return;
	stack->array[++stack->top] = (char *)calloc(strlen(item), sizeof(char));
	strcpy(stack->array[stack->top], item);
}

char *pop(struct Stack *stack)// Pop item from Stack
{
	if (isEmpty(stack))
		return NULL;
	return stack->array[stack->top--];
}

int isFull(struct Stack *stack)// Checking the stack 
{
	if (stack->top == SIZE)
	{
		return 1;
	}
	else {
		return 0;
	}
}

int isEmpty(struct Stack *stack)// Checking the stack 

{
	if (stack->top == -1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

char *getAllStack(struct Stack *stack)// Return all stack 
{
	char *AllStack = (char *)calloc(100, sizeof(char));
	int i;
	strcat(AllStack, "Stack: ");
	for (i = 0; i <= stack->top; i++)
	{
		strcat(AllStack, stack->array[i]);
		strcat(AllStack, " ");
	}
	if (stack->top == -1)
		strcat(AllStack, "Empty");
	return AllStack;

}
//Stack operations end

//Variable operations begin
Variable *initializeVariables(unsigned size)
{
	Variable *vars = (Variable *)malloc(sizeof(Variable));
	vars->value = (double *)calloc(size, sizeof(double));
	vars->var = (char *)calloc(size, sizeof(char));
	vars->notCreated = -99999;
	vars->size = 0;
	return vars;
}

double getVariable(Variable *vars, char variable)
{
	int i = 0;
	while (i < vars->size && vars->var[i] != variable)// Looking for variable is created before
		i++;
	if (i == vars->size)// If variable cannot found return notCreated value
		return vars->notCreated;
	return vars->value[i];
}

void addVariable(Variable *vars, char variable, double value)
{
	int i = 0;
	while (i < vars->size && vars->var[i] != variable)// Look for this variable created before
		i++;
	if (i == vars->size)// If this variable is not created before, create a new variable
	{
		vars->var[vars->size] = variable;
		vars->value[vars->size++] = value;
	}
	else// If this variable created before, only update this value
		vars->value[i] = value;
	if (vars->notCreated == value)
		vars->notCreated++;
}
//Variable operations end

char **initializeStrArray(unsigned strlong, unsigned strnumb)
{
	char **strarr = (char **)calloc(strnumb, sizeof(char *));
	int i;
	for (i = 0; i < strnumb; i++)
		strarr[i] = (char *)calloc(strlong, sizeof(char));
	return strarr;
}

char *createPostfixString(struct Stack *stack, char *function, Variable *vars, char *var)
{
	char *postfixString = (char *)calloc(500, sizeof(char));// Initialize a char array
	char *ptok = strtok(function, " ");// Parse function string by ' ' character
	*var = ptok[0];
	ptok = strtok(NULL, " ");// This is for jump to '=' character
	while (ptok != NULL && ptok[0] != ';')// While a ptok can be created and ptok is not ';' character
	{
		if (isdigit(ptok[0]))// If ptok is a number
		{
			strcat(postfixString, ptok);// Numbers added to postfix directly
			strcat(postfixString, " ");
		}
		else if (getVariable(vars, ptok[0]) != vars->notCreated)// If ptok is a variable
		{
			strcat(postfixString, ptok);// Variables added to postfix directly
			strcat(postfixString, " ");
		}
		else if (getPriority(ptok[0]) <= 4)// If ptok is a arithmatic operation
		{
			char *operationInStack = pop(stack);
			while (operationInStack != NULL && getPriority(operationInStack[0]) < getPriority(ptok[0]))// While there is higher priority operations in the stack
			{
				strcat(postfixString, operationInStack);// Operations -which are higher priority- popped stack and added to postfix
				strcat(postfixString, " ");
				operationInStack = pop(stack);
			}
			if (operationInStack != NULL)// If last poped operation is not null its must be push again
				push(stack, operationInStack);
			push(stack, ptok);// New arithmatic operation is pushed
		}
		else if (ptok[0] == '(')// If ptok is '(' it's pushed directly
			push(stack, ptok);
		else if (ptok[0] == ')')// If ptok is ')' while poped operation is '(' all operations poped and added to postfix expression
		{
			char *operationInStack = pop(stack);
			while (operationInStack[0] != '(')
			{
				strcat(postfixString, operationInStack);
				strcat(postfixString, " ");
				operationInStack = pop(stack);
			}
		}
		ptok = strtok(NULL, " ");
		printf("Postfix: %-40s %s\n", postfixString, getAllStack(stack));
	}
	char *operationInStack = pop(stack);
	while (operationInStack != NULL)// Operations left at stack must be added to postfix too
	{
		strcat(postfixString, operationInStack);
		strcat(postfixString, " ");
		printf("Postfix: %-40s %s\n", postfixString, getAllStack(stack));
		operationInStack = pop(stack);
	}
	printf("\nPostfix version of function: %-40s \n\n", postfixString);
	return postfixString;
}

double solvePostfix(struct Stack *stack, char *postfix, Variable *vars)
{
	printf("\t Step 2: SOLVING POSTFIX \n\n");
	char *ptok = strtok(postfix, " ");// Parse function string by ' ' character
	while (ptok != NULL)
	{
		if (isdigit(ptok[0]))// If ptok is number, it's push to stack
		{
			push(stack, ptok);
		}
		else if (getVariable(vars, ptok[0]) != vars->notCreated)// If ptok is a variable, it's value push to stack
		{
			char *string = (char *)calloc(100, sizeof(char));
			_snprintf(string, 10, "%.2f", getVariable(vars, ptok[0]));
			push(stack, string);
		}
		else if (getPriority(ptok[0]) <= 4)// If ptok is a arithmatic operation, pop two values from stack and do the operation and return value pushed to stack
		{
			char *string = (char *)calloc(100, sizeof(char));
			_snprintf(string, 10, "%.2f", operation(atof(pop(stack)), atof(pop(stack)), ptok[0]));
			push(stack, string);
		}
		printf("%s\n", getAllStack(stack));
		ptok = strtok(NULL, " ");
	}
	return atof(pop(stack));// When all postfix solved. One value remained in stack. It's the result of our function
}

void printVariables(Variable *vars)
{
	int i = 0;
	for (i = 0; i < vars->size; i++)
		printf("%c=%.2f ", vars->var[i], vars->value[i]);
	printf("\n");

}

double operation(double value1, double value2, char operation)
{
	if (operation == '+')
		return value1 + value2;

	if (operation == '-')
		return value1 - value2;

	if (operation == '*')
		return value1 * value2;

	if (operation == '/')
		return value1 / value2;

}

int getPriority(char operation)
{
	int i = 0;
	char *priority = "*/+-";// The priortiy of arithmatic operations. When added a new operation it's must be added to operation function too
	while (priority[i++] != operation);
	return i;

}
