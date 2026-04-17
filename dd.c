#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma pack(4)
typedef struct Student
{
	unsigned int regNo;
	unsigned short groupNo;
	char* name;
} Student, * PStudent;
typedef struct DoubleLinkedList
{
	Student* info;
	struct DoubleLinkedList* next, * prev;
}DoubleLinkedList;
typedef struct HashTable
{
	DoubleLinkedList** items;//vector alocat dinamic de liste
	unsigned int size;
}HashTable;

//typedef struct Student Student;
//typedef struct Student* PStudent;
#define LINE_SIZE 256
#define HT_InitialSize 3

PStudent createStudent(unsigned int reg, unsigned short group, const char* name);
void deleteStudent(Student* stud);
void printStudent(Student* stud);
HashTable putStudent(HashTable, Student*);
void printHashTable(HashTable);

int main()
{
	//Student stud = {.regNo=12300, .groupNo=1055, .name="Popescu Ioan"};
	//printf("sizeof(Student)=%d\n", sizeof(Student));
	HashTable hashTable = { .items = NULL , .size = 0 };

	FILE* pFile = fopen("Data.txt", "r");
	if (pFile != NULL)
	{
		char line[LINE_SIZE];
		//char* delimiter = ",";
		char delimiter[] = { ',','\n','\0' };
		char* token = NULL, * context = NULL;
		unsigned int reg;
		unsigned short group;
		char buffer[LINE_SIZE];
		while (fgets(line, LINE_SIZE, pFile))
		{
			token = strtok_s(line, delimiter, &context);
			reg = atoi(token);
			//printf("Remaining string: %s\n", context);

			token = strtok_s(NULL, delimiter, &context);
			strcpy_s(buffer, strlen(token) + 1, token);

			token = strtok_s(NULL, delimiter, &context);
			group = atoi(token);
			//printf("Remaining string: %s\n", context);

			Student* stud = createStudent(reg, group, buffer);

			hashTable = putStudent(hashTable, stud);
		}

		printHashTable(hashTable);

		//deleteStudentByKey(hashTable, "Popescu Ion");
	}
	return 0;
}


DoubleLinkedList* insertLDIC(DoubleLinkedList* list, Student* stud)
{
	DoubleLinkedList* nod = (DoubleLinkedList*)malloc(sizeof(DoubleLinkedList));
	nod->prev = NULL;
	nod->next = NULL;
	nod->info = stud;
	if (list == NULL) {
		nod->next = nod;
		nod->prev = nod;
		/*list = nod;*/
		return nod;
	}
	else {
		nod->prev = list->prev;
		nod->next = list;
		list->prev = nod;
		nod->prev->next = nod;
	}
	return list;

}

void printHashTable(HashTable hashTable)
{
	for (unsigned int i = 0; i < hashTable.size; i++)
	{
		printf("Bucket: %d\n", i);
		DoubleLinkedList* iterator = hashTable.items[i];
		do
		{
			printStudent(iterator->info);
			iterator = iterator->next;
		} while (iterator != hashTable.items[i]);
	}
}

int fhash(char* name, int size)
{
	int m = 0;
	for (unsigned int i = 0; i < strlen(name); i++)
	{
		m += name[i];
	}
	return m % size;
}

HashTable putStudent(HashTable hashTable, Student* stud)
{
	if (hashTable.items == NULL || hashTable.size == 0)
	{
		hashTable.items = (DoubleLinkedList**)malloc(HT_InitialSize * sizeof(DoubleLinkedList*));
		hashTable.size = HT_InitialSize;
		memset(hashTable.items, 0, HT_InitialSize * sizeof(DoubleLinkedList*));
	}
	int index = fhash(stud->name, hashTable.size);

	//insertLDIC(&hashTable.items[index], stud );

	hashTable.items[index] = insertLDIC(hashTable.items[index], stud);
	return hashTable;

}

PStudent createStudent(unsigned int reg, unsigned short group, const char* name)
{
	Student* stud = (Student*)malloc(sizeof(Student));
	if (stud != NULL)
	{
		stud->groupNo = group;
		stud->regNo = reg;
		stud->name = (char*)malloc(strlen(name) + 1);
		if (stud->name != NULL)
		{
			strcpy_s(stud->name, strlen(name) + 1, name);
		}
		else
		{
			free(stud);
			stud = NULL;
		}
	}
	return stud;
}
void deleteStudent(Student* stud)
{
	if (stud != NULL)
	{
		if (stud->name != NULL)
			free(stud->name);
		free(stud);
	}
}
void printStudent(Student* stud)
{
	if (stud != NULL && stud->name != NULL)
	{
		printf("RegNo:%d, Name:%s, group:%d\n",
			stud->regNo, stud->name, stud->groupNo);
	}
}
