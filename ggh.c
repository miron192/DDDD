#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma pack(4)
typedef struct Student
{
    unsigned int regNo;
    unsigned short groupNo;
    char* name;
} Student, *PStudent;

#define LINE_SIZE 256

typedef struct ListNode
{
    Student* data;
    struct ListNode* next;
} ListNode, *PListNode;

PStudent createStudent(unsigned int, unsigned short, const char*);
void deleteStudent(Student*);
void printStudent(Student*);

PListNode createNode(PStudent);
PListNode insertIntoHeadCircularList(PListNode, Student*);
PListNode insertIntoEndCircularList(PListNode, Student*);
void printCircularList(PListNode);
void deleteCircularList(PListNode*);

int main()
{
    ListNode* circularList = NULL;

    printf("sizeof(Student)=%d\n", sizeof(Student));

    FILE* pFile = fopen("Data.txt", "r");
    if (pFile != NULL)
    {
        char line[LINE_SIZE];
        char delimiter[] = { ',','\n','\0' };
        char* token = NULL, * context = NULL;
        unsigned int reg;
        unsigned short group;
        char buffer[LINE_SIZE];

        while (fgets(line, LINE_SIZE, pFile))
        {
            token = strtok_s(line, delimiter, &context);
            reg = atoi(token);

            token = strtok_s(NULL, delimiter, &context);
            strcpy_s(buffer, strlen(token) + 1, token);

            token = strtok_s(NULL, delimiter, &context);
            group = atoi(token);

            Student* stud = createStudent(reg, group, buffer);

            // la alegere: inserție la cap sau la coadă
            circularList = insertIntoEndCircularList(circularList, stud);
            // circularList = insertIntoHeadCircularList(circularList, stud);

            printCircularList(circularList);
            printf("\n-------------------------------\n");
        }

        fclose(pFile);
        deleteCircularList(&circularList);
        printCircularList(circularList);
    }

    return 0;
}

PListNode createNode(PStudent stud)
{
    ListNode* node = (ListNode*)malloc(sizeof(ListNode));
    if (node != NULL)
    {
        node->data = stud;
        node->next = NULL;
    }
    return node;
}

// Inserție la cap - head-ul devine nodul nou, ultimul nod pointează la noul head
PListNode insertIntoHeadCircularList(PListNode headList, Student* pStud)
{
    ListNode* node = createNode(pStud);
    if (node == NULL)
        return headList;

    if (headList == NULL)
    {
        node->next = node;       // singurul nod pointează la el însuși
        return node;
    }

    // găsim ultimul nod (cel care pointează la head)
    PListNode last = headList;
    while (last->next != headList)
        last = last->next;

    node->next = headList;       // nodul nou pointează la vechiul head
    last->next = node;           // ultimul pointează la noul head
    return node;                 // noul head
}

// Inserție la coadă - head-ul rămâne, nodul nou devine ultimul și pointează la head
PListNode insertIntoEndCircularList(PListNode headList, Student* pStud)
{
    ListNode* node = createNode(pStud);
    if (node == NULL)
        return headList;

    if (headList == NULL)
    {
        node->next = node;
        return node;
    }

    PListNode last = headList;
    while (last->next != headList)
        last = last->next;

    last->next = node;
    node->next = headList;       // închidem cercul
    return headList;
}

// Parcurgere cu do...while ca să nu ratăm head-ul și să nu intrăm în buclă infinită
void printCircularList(PListNode headList)
{
    if (headList == NULL)
        return;

    PListNode aux = headList;
    do
    {
        printStudent(aux->data);
        aux = aux->next;
    } while (aux != headList);
}

// Rupem cercul înainte de a șterge, altfel intrăm în while infinit
void deleteCircularList(PListNode* headList)
{
    if (*headList == NULL)
        return;

    // găsim ultimul și rupem legătura spre head
    PListNode last = *headList;
    while (last->next != *headList)
        last = last->next;
    last->next = NULL;           // acum e listă simplă liniară

    while (*headList)
    {
        ListNode* tmp = *headList;
        *headList = (*headList)->next;
        free(tmp->data->name);
        free(tmp->data);
        free(tmp);
    }
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
