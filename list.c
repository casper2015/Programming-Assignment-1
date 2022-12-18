#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define stdin stdin
#define INITIAL_SIZE 2
#define MAX_NUM_OF_CMDS 100

typedef struct entry
{
	char *name, *lastname;
	float height;
	int age;
} Entry;

void freeEntry(Entry *entry)
{
	free(entry->name);
	free(entry->lastname);
	free(entry);
}

Entry *initializeEntry(char *name,char *lastname, float height, int age)
{
    Entry *newEntry = malloc(sizeof(Entry));
    newEntry->name = malloc(sizeof(char)*(strlen(name)+1));
	strcpy(newEntry->name,name);
    newEntry->lastname = malloc(sizeof(char)*(strlen(lastname)+1));
	strcpy(newEntry->lastname,lastname);
    newEntry->height = height;
    newEntry->age = age;
    return newEntry;
}

typedef struct arrayList 
{
	int capacity;
	int size;
	Entry **data;
} List;


List *initializeList();
void deleteList(List *myList);
void doubleCapacity(List *myList);
void halveCapacity(List *myList);
void insertToHead(List *myList, char *name,char *lastname, float height, int age);
void insertToTail(List *myList, char *name,char *lastname, float height, int age);
void insertToPosition(List *myList, int position, char *name,char *lastname, float height, int age);
int findPosition(List *myList,char *name);
void deleteFromHead(List *myList);
void deleteFromTail(List *myList);
void deleteFromPosition(List *myList, int position);
void printList(List *myList);
void printListInfo(List *myList);


List *initializeList() 
{
	List *newList = malloc(sizeof(List));
	newList->capacity = INITIAL_SIZE; //to be user defined
	newList->size = 0; //list is initially empty
	newList->data = malloc(sizeof(Entry *)*INITIAL_SIZE);
    for(int i=0;i<newList->capacity;i++)
        newList->data[i] = NULL;
	return newList;
}

void deleteList(List *myList)
{
	for(int i=0;i<myList->size;i++)
		freeEntry(myList->data[i]);

    free(myList->data);
    free(myList);
}

void doubleCapacity(List *myList) 
{
	Entry **newData = malloc(sizeof(Entry *)*myList->capacity*2);
	for(int i=0;i<myList->capacity*2;i++)
        newData[i] = NULL;
    
	for(int i=0;i<myList->size;i++)
		newData[i] = myList->data[i];

	Entry **temp = myList->data;
	myList->data = newData;
	myList->capacity *= 2;

	free(temp);
}

void halveCapacity(List *myList) 
{
	Entry **newData = malloc(sizeof(Entry *)*(myList->capacity/2));
	for(int i=0;i<myList->capacity/2;i++)
        newData[i] = NULL;

	for(int i=0;i<myList->size;i++)
        newData[i] = myList->data[i];

	Entry **temp = myList->data;
	myList->data = newData;
	myList->capacity /= 2;

	free(temp);
}


void insertToHead(List *myList, char *name,char *lastname, float height, int age) 
{
	if(myList->size==myList->capacity)
		doubleCapacity(myList);
	
	for(int i=myList->size; i>0;i--)
        myList->data[i] = myList->data[i-1];
	
    myList->data[0] = initializeEntry(name,lastname,height,age);
	myList->size++;
}

void insertToTail(List *myList, char *name,char *lastname, float height, int age) 
{
	if(myList->size==myList->capacity)
		doubleCapacity(myList);
	
    myList->data[myList->size] = initializeEntry(name,lastname,height,age);
	myList->size++;
}

void insertToPosition(List *myList, int position, char *name,char *lastname, float height, int age)
{
	if(position>myList->size)
	{
		printf("Invalid insert!\n");
		return;
	}
	
	if(myList->size==myList->capacity)
		doubleCapacity(myList);
	
	for(int i=myList->size; i>position;i--)
        myList->data[i] = myList->data[i-1];
	
    myList->data[position] = initializeEntry(name,lastname,height,age);
	myList->size++;
}

int findPosition(List *myList,char *name)
{
	for(int i=0; i<myList->size;i++)
	{
		if(strcmp(myList->data[i]->name,name)==0)
			return i;	
	}
	return -1;
}

void deleteFromHead(List *myList)
{
	if(myList->size<=0)
    {
        printf("Invalid delete!\n");
        return;
    }
    
	else if(myList->size>1)
	{
        freeEntry(myList->data[0]);
		for(int i=1; i<myList->size;i++)
			myList->data[i-1] = myList->data[i];
	}

	myList->size--;

	if(myList->size < myList->capacity/2)
		halveCapacity(myList);
}

void deleteFromTail(List *myList)
{
	if(myList->size<=0)
	{
        	printf("Invalid delete!\n");
        	return;
	}

	freeEntry(myList->data[myList->size-1]);
	myList->size--;
	
	if(myList->size < myList->capacity/2)
		halveCapacity(myList);
}

void deleteFromPosition(List *myList, int position)
{
	if(position>myList->size-1)
	{
		printf("Invalid delete!\n");
		return;
	}

    freeEntry(myList->data[position]);
    
	for(int i=position;i<myList->size-1;i++)
		myList->data[i] = myList->data[i+1];

	
	myList->size--;
	
	if(myList->size < myList->capacity/2)
		halveCapacity(myList);
}

void printList(List *myList)
{
	if(myList->size==0)
		printf("List is empty!!\n");
	else
	{
		for(int i=0;i<myList->size;i++)
			printf("[%d]\t%s\t%s\t%0.2f\t%d\n",i,myList->data[i]->name,myList->data[i]->lastname,myList->data[i]->height,myList->data[i]->age);
	}
}

void printListInfo(List *myList)
{
	printf("size:%d, capacity:%d\n",myList->size,myList->capacity);
}


int main(int argc, char** argv) 
{
	char* line;
	size_t lineBuffSize;
	ssize_t lineSize;
	char** listOfCommands;
	List* myList;

	line = NULL;
	lineBuffSize = 0;
	listOfCommands = malloc(sizeof(char*) * MAX_NUM_OF_CMDS);
	myList = initializeList();

	for (int i = 0; i < MAX_NUM_OF_CMDS; i++)
	{
		listOfCommands[i] = NULL;
	}

	int command_number = 0;
	while ((lineSize = getline(&line, &lineBuffSize, stdin)) != -1)
	{
		listOfCommands[command_number] = malloc(sizeof(char) * (strlen(line) + 1));
		strcpy(listOfCommands[command_number], line);
		command_number++;
	}

	for (int j = 0; j < command_number; j++)
	{
		char* command_line;
		char* token;
		const char delimiter[2] = " ";

		command_line = listOfCommands[j];
		
		//Since each line ends with a newline character, let's replace it with the null-terminator character
		//otherwise, it can cause us trouble below when we make string comparisons
		if (command_line[strlen(command_line) - 1] == '\n')
		{
			command_line[strlen(command_line) - 1] = '\0';
		}

		//split the buffer by space character and get the first part written into variable "token"
		token = strtok(command_line, delimiter); 
					
		//In these nested if statements, read the command from file and execute it
		if (strcmp(token, "insertToHead") == 0)
		{
			char *name, *lastname;
			float height;
			int age;
			name = strtok(NULL,delimiter);
			lastname = strtok(NULL,delimiter);
			height = atof(strtok(NULL,delimiter));
			age = atoi(strtok(NULL,delimiter));
			//change the print statement below with a call to insert to list
			//printf("insertToHead(listPointer,%s,%s,%0.2f,%d)\n",name,lastname,height,age);
			insertToHead(myList,name,lastname,height,age);
		}
		else if(strcmp(token,"insertToTail")==0)
		{
			char *name, *lastname;
			float height;
			int age;
			name = strtok(NULL,delimiter);
			lastname = strtok(NULL,delimiter);
			height = atof(strtok(NULL,delimiter));
			age = atoi(strtok(NULL,delimiter));
			//change the print statement below with a call to insert to list
			//printf("insertToTail(listPointer,%s,%s,%0.2f,%d)\n",name, lastname,height,age);
			insertToTail(myList,name,lastname,height,age);
		}
		else if(strcmp(token,"insertToPosition")==0)
		{
			char *name, *lastname;
			float height;
			int age,position;
			position = atoi(strtok(NULL,delimiter));
			name = strtok(NULL,delimiter);
			lastname = strtok(NULL,delimiter);
			height = atof(strtok(NULL,delimiter));
			age = atoi(strtok(NULL,delimiter));
			//change the print statement below with a call to insert to list
			//printf("insertToPosition(listPointer,%d,%s,%s,%0.2f,%d)\n",position,name, lastname,height,age);
			insertToPosition(myList,position,name,lastname,height,age);
		}
		else if(strcmp(token,"findPosition")==0)
		{
			char *name;
			name = strtok(NULL,delimiter);
			//change the print statement below with a call to search to list
			//printf("findPosition(listPointer,%s)\n",name);
			printf("%d\n",findPosition(myList,name));
		}
		else if(strcmp(token,"deleteFromHead")==0)
		{
			//change the print statement below with a call to search to list
			//printf("deleteFromHead(listPointer)\n");
			deleteFromHead(myList);
		}
		else if(strcmp(token,"deleteFromTail")==0)
		{
			//change the print statement below with a call to search to list
			//printf("deleteFromTail(listPointer)\n");
			deleteFromTail(myList);
		}
		else if(strcmp(token,"deleteFromPosition")==0)
		{
			int position;
			position = atoi(strtok(NULL,delimiter));
			//change the print statement below with a call to search to list
			//printf("deleteFromPosition(listPointer,%d)\n",position);
			deleteFromPosition(myList,position);
		}
		else if(strcmp(token,"printList")==0)
		{
			//change the print statement below with a call to print function that prints the contents of the list
			//printf("printList(listPointer)\n");
			printList(myList);
		}
		else if(strcmp(token,"printListInfo")==0)
		{
			//change the print statement below with a call to print function that prints size information about the list
			//printf("printListInfo(listPointer)\n");
			printListInfo(myList);
		}
		else if(strcmp(token,"deleteList")==0)
		{
			//change the print statement below with a call to the function that deallocates the memory for the list
			//printf("deleteList(listPointer)\n");
			deleteList(myList);
		}
		else
		{
			printf("Invalid command: <%s>\n",token);
		}
	}

	for (int k = 0; k < command_number; k++)
	{
		free(listOfCommands[k]);
	}
	free(listOfCommands);

	return 0;
} 
