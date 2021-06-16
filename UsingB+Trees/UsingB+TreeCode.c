/* 
    NAME :- HARSH YELLARE 
    ROLL NO. :- BT19CSE124
    BATCH R4
    DSPD-II ASSIGNMENT-2 
    ACCOMMODATION MANAGEMENT SYSTEM IMPLEMENTED USING B+ TREES.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_CHILDREN 4 // a non leaf node (internal node other than root) can have maximum MAX_CHILDREN children and MAX_CHILDREN-1 keys
#define MAX_KEYS 3 // MAX_CHILDREN -1 , any node can hold maximum MAX_KEYS keys.
// Assumption : Defined it in such a way that all nodes (internal + leaf) can hold MAX_KEYS number of keys . We could have kept different numbers for internal and leaf

typedef enum{FALSE,TRUE}boolean;
typedef enum{FAILURE,SUCCESS}status;

typedef struct accommodation_tag
{
    char firstname[50];
    char lastname[50];
    char accommodationtype[20];
    int idtype;
    struct idnum
    {
        char aadhaar [15];
        char passport [15];
        char empcode [15];
    }idnumber;
    char address [100];

}accommodation;

typedef struct Node_tag
{
    char keys[MAX_KEYS][16]; //keys[i] stores idtype+idnum concatenated
    int numOfKeys;
    boolean isLeaf;
    struct Node_tag* nextPtr;//For leaf nodes
    union pointer 
    {
        struct Node_tag* ptrToTree[MAX_CHILDREN];
        accommodation* ptrToData[MAX_KEYS];        
    }ptrToTreeOrData;
    
}Node;

int posnFinder(Node* nptr,int lo,const char* keyStr,int maxKeySize);
void InitializeLeafNode(Node** NodePtrPtr);
void InitializeInternalNode(Node** NodePtrPtr);
void findParent(Node** parentPtrPtr,Node* nptr, Node* child);
void InsertInInternal(Node** rootPtrPtr,const char* keyStr,int maxKeySize,Node** parentPtrPtr,Node** childPtrPtr);
void InsertInLeaf(Node** rootPtrPtr,const char* keyStr,int maxKeySize,accommodation* dataPtr);
void InsertRecord(Node** rootPtrPtr,Node** typeArootPtr,Node** typeBrootPtr,Node** typeCrootPtr,Node** typeDrootPtr);
void display(Node* nptr) ;
Node* firstLeftNode(Node* nptr);
void abortfunc(Node** rootPtrPtr);
void Search(Node* nptr);
accommodation** searchKey(Node* nptr,const char* keyStr,int maxKeySize);
void UpdateRecord(Node** rootPtrPtr);
void DeleteInInternal(Node** rootPtrPtr,const char* keyStr,int maxKeySize,Node** parentPtrPtr ,Node* childPtr);
void DeleteInLeaf(Node** rootPtrPtr,const char* keyStr, int maxKeySize);
void DeleteRecord(Node** rootPtrPtr,Node** typeArootPtr,Node** typeBrootPtr,Node** typeCrootPtr,Node** typeDrootPtr);
void removeDuplicates(Node** rootPtrPtr);
void rangeSearch(Node* nptr,char* startKey,char* endKey,int maxKeySize);
void addressSearch(Node* typeAroot,Node* typeBroot,Node* typeCroot,Node* typeDroot);
void deallocateBlock(Node** typeArootPtr,Node** typeBrootPtr,Node** typeCrootPtr,Node** typeDrootPtr,accommodation* dataPtr);
void searchDataPtr(Node* nptr,accommodation* dataPtr,char keyStr[],int maxKeySize);
void allocateBlock(Node** typeArootPtr,Node** typeBrootPtr,Node** typeCrootPtr,Node** typeDrootPtr,accommodation* dataPtr);
int countNodes(Node** rootPtrPtr);
void rangeSearch(Node* nptr,char* startKey,char* endKey,int maxKeySize);
void addressSearch(Node* typeAroot,Node* typeBroot,Node* typeCroot,Node* typeDroot);

//display =>printRecords() function
void main()
{
    Node* rootPtr = NULL;// BPtree declaration

    //accommodation type trees
    Node* typeAroot = NULL;
    Node* typeBroot = NULL;
    Node* typeCroot = NULL;
    Node* typeDroot = NULL; 
    
    InsertRecord(&rootPtr,&typeAroot,&typeBroot,&typeCroot,&typeDroot);
    display(rootPtr);
    removeDuplicates(&rootPtr);
    display(rootPtr);
    Search(rootPtr);
    DeleteRecord(&rootPtr,typeAroot,typeBroot,typeCroot,typeDroot);
    display(rootPtr);
    UpdateRecord(&rootPtr);
    display(rootPtr);
    //allocateBlock and deallocateBlock are maintained internally
    addressSearch(typeAroot,typeBroot,typeCroot,typeDroot);      
}
// void abortfunc(Node** rootPtrPtr)
// {
//     free(*rootPtrPtr);
// }
Node* firstLeftNode(Node* nptr)
{
    if (nptr->isLeaf)
    {
        return nptr;
    }
    int i;
    for (i = 0; i < nptr->numOfKeys+1 ; i++)
    {
        if (nptr->ptrToTreeOrData.ptrToTree[i] != NULL)
        {
            return firstLeftNode(nptr->ptrToTreeOrData.ptrToTree[i]);
        }            
    }
        
    return NULL;
}

void print(accommodation* printPtr)
{
    if(printPtr == NULL)
    {
        printf("No person exists at this location\n");
    }
    else
    {
        if (printPtr->idtype==0)
        {
            printf("%s %s \t %s \t %d \t %s \t %s \n",printPtr->firstname,printPtr->lastname,printPtr->accommodationtype,printPtr->idtype,printPtr->idnumber.aadhaar,printPtr->address);
        }
        else if(printPtr->idtype==1)
        {
            printf("%s %s \t %s \t %d \t %s \t %s \n",printPtr->firstname,printPtr->lastname,printPtr->accommodationtype,printPtr->idtype,printPtr->idnumber.passport,printPtr->address);
        }
        else
        {
            printf("%s %s \t %s \t %d \t %s \t %s \n",printPtr->firstname,printPtr->lastname,printPtr->accommodationtype,printPtr->idtype,printPtr->idnumber.empcode,printPtr->address);
        }
    }    
}

void display(Node* nptr) 
{
    Node* firstLeft = firstLeftNode(nptr);
    if (firstLeft == NULL) 
    {
        printf("No Data in the Database yet!\n");
        return;
    }
    while (firstLeft != NULL)
    {
        int i;
        // printf("%d\n",firstLeft->numOfKeys);
        for ( i = 0; i < firstLeft->numOfKeys; i++) 
        {
            print(firstLeft->ptrToTreeOrData.ptrToData[i]);            
        }
        
        firstLeft = firstLeft->nextPtr;
    }
    printf("\n");
    return;
}

int posnFinder(Node* nptr,int lo,const char* keyStr,int maxKeySize)
{
    int temp = lo;
    while ((temp<(MAX_KEYS)) && (strcmp(nptr->keys[temp],keyStr)<=0))
    {
        temp++;
    }
    return temp;  
}

void InitializeLeafNode(Node** NodePtrPtr) //note: CHECK
{
    (*NodePtrPtr)->isLeaf = TRUE;
    int i;
    for ( i = 0; i < MAX_KEYS; i++)
    {
        (*NodePtrPtr)->keys[i][0]='3'; // Initializing to 3 so that it is greater than all other valid keys
        (*NodePtrPtr)->ptrToTreeOrData.ptrToData[i]=NULL;
    }   
    (*NodePtrPtr)->numOfKeys = 0;
    (*NodePtrPtr)->nextPtr = NULL;   
}
void InitializeInternalNode(Node** NodePtrPtr) //note: CHECK
{
    (*NodePtrPtr)->isLeaf = FALSE;
    int i;
    for ( i = 0; i < MAX_KEYS; i++)
    {
        (*NodePtrPtr)->keys[i][0] = '3';
    }
    (*NodePtrPtr)->numOfKeys = 0;
    (*NodePtrPtr)->nextPtr = NULL;//Always set to NULL for non leaf
    for ( i = 0; i < MAX_CHILDREN; i++)
    {
        (*NodePtrPtr)->ptrToTreeOrData.ptrToTree[i]=NULL;
    }    
}

void findParent(Node** parentPtrPtr,Node* nptr, Node* child)
{
    if(nptr->isLeaf || nptr->ptrToTreeOrData.ptrToTree[0]->isLeaf)
    {
        return;
    }
    int i=0;
    for ( i = 0; i < (nptr->numOfKeys+1); i++)
    {
        if (nptr->ptrToTreeOrData.ptrToTree[i] == child)
        {
            (*parentPtrPtr) = nptr;
        }
        else
        {
            Node* tempNptr = nptr->ptrToTreeOrData.ptrToTree[i];
            findParent(parentPtrPtr,tempNptr,child);
        }        
    }    
}

void InsertInInternal(Node** rootPtrPtr,const char* keyStr,int maxKeySize,Node** parentPtrPtr,Node** childPtrPtr)
{
    if ((*parentPtrPtr)->numOfKeys < MAX_KEYS)
    {
        int posn = posnFinder((*parentPtrPtr),0,keyStr,maxKeySize);
        int i = (*parentPtrPtr)->numOfKeys;
        while (i>posn) //shifting keys
        {
            strcpy((*parentPtrPtr)->keys[i],(*parentPtrPtr)->keys[i-1]);
            i--;
        }
        strcpy((*parentPtrPtr)->keys[posn],keyStr);
        i = (*parentPtrPtr)->numOfKeys + 1;
        while (i>posn+1) //shifting ptrTotrees
        {
            (*parentPtrPtr)->ptrToTreeOrData.ptrToTree[i] = (*parentPtrPtr)->ptrToTreeOrData.ptrToTree[i-1]; 
            i--;
        }
        (*parentPtrPtr)->ptrToTreeOrData.ptrToTree[posn+1] = (*childPtrPtr);
        
        ((*parentPtrPtr)->numOfKeys)++;
    }
    else
    {
        //splitting parent
        char tempKeys[MAX_KEYS+1][16];
        Node* tempChildPtr[MAX_CHILDREN+1];
        int i;
        for ( i = 0; i < MAX_KEYS ; i++)
        {
            strcpy(tempKeys[i],(*parentPtrPtr)->keys[i]);
        }
        for ( i = 0; i < MAX_CHILDREN; i++)
        {
            tempChildPtr[i] = (*parentPtrPtr)->ptrToTreeOrData.ptrToTree[i];
        }
        int j = MAX_KEYS;
        int posn = posnFinder((*parentPtrPtr),0,keyStr,maxKeySize);
        while (j>posn)
        {
            strcpy(tempKeys[j],tempKeys[j-1]);
            j--;
        }
        strcpy(tempKeys[posn],keyStr);
        j = MAX_CHILDREN;
        while (j>posn+1)
        {
            tempChildPtr[j]=tempChildPtr[j-1];
            j--;
        }
        tempChildPtr[posn+1] = (*childPtrPtr); 

        char partitionKey[16];
        strcpy(partitionKey,tempKeys[(MAX_KEYS+1)/2]);
        int partitionIndex = (MAX_KEYS+1)/2;
        (*parentPtrPtr)->numOfKeys = partitionIndex;
        i=0;
        for ( i = 0; i < partitionIndex ; i++)
        {
            strcpy((*parentPtrPtr)->keys[i],tempKeys[i]);
        }
        for ( i = 0; i < partitionIndex + 1; i++)
        {
            (*parentPtrPtr)->ptrToTreeOrData.ptrToTree[i]=tempChildPtr[i];
        }
        
        Node* newInternalNode = (Node*)malloc(sizeof(Node));
        InitializeInternalNode(&newInternalNode);
        j=0;
        for ( i = partitionIndex; i < MAX_KEYS+1 ; i++)
        {
            strcpy(newInternalNode->keys[j],tempKeys[i]);
        }
        (newInternalNode->numOfKeys) = j;
        j=0;
        for ( i = partitionIndex+1; i < MAX_CHILDREN+1; i++)
        {
            newInternalNode->ptrToTreeOrData.ptrToTree[j] = tempChildPtr[i];
        }
        if ((*parentPtrPtr) == (*rootPtrPtr))
        {
            //If parent is a root we create a new Node
			Node* newRoot = (Node*)malloc(sizeof(Node));
            InitializeInternalNode(&newRoot);
            strcpy(newRoot->keys[0],partitionKey);
            newRoot->ptrToTreeOrData.ptrToTree[0] = (*parentPtrPtr);
            newRoot->ptrToTreeOrData.ptrToTree[1] = (newInternalNode);
            (*rootPtrPtr) = newRoot;  
        }
        else
        {
            //Recursion
            Node* parentPtr ;
            findParent(&parentPtr,(*rootPtrPtr),(*parentPtrPtr));
            InsertInInternal(rootPtrPtr,partitionKey,maxKeySize,&parentPtr,&newInternalNode);
        }              
    }    
}
 
void InsertInLeaf(Node** rootPtrPtr,const char* keyStr,int maxKeySize,accommodation* dataPtr) //in leaf
{
    if (*rootPtrPtr == NULL)
    {
        *rootPtrPtr = (Node* )malloc(sizeof(Node));
        InitializeLeafNode(rootPtrPtr);  
        printf("Entered key :");
        puts(keyStr);
        printf("\n");      
        strcpy((*rootPtrPtr)->keys[0],keyStr);
        (*rootPtrPtr)->ptrToTreeOrData.ptrToData[0] = dataPtr;
        // print((*rootPtrPtr)->ptrToTreeOrData.ptrToData[0]);
        ((*rootPtrPtr)->numOfKeys)++;
    }
    else
    {
        Node* nptr = *rootPtrPtr; // Traversing pointer
        Node* parentPtr = NULL;
        // Searching 
        while (nptr->isLeaf == FALSE)
        {
            parentPtr = nptr;
            int posn = posnFinder(nptr,0,keyStr,maxKeySize);
            nptr = nptr->ptrToTreeOrData.ptrToTree[posn];
        }
        // Now nptr has reached to apt leaf node
        if (nptr->numOfKeys < MAX_KEYS)
        {
            //if current leaf is not full find correct posn and insert
            if(nptr == (*rootPtrPtr))
            {
                int posn = posnFinder((*rootPtrPtr),0,keyStr,maxKeySize);
                int i = (*rootPtrPtr)->numOfKeys;
                while (i>posn)
                {
                    strcpy((*rootPtrPtr)->keys[i],(*rootPtrPtr)->keys[i-1]);
                    (*rootPtrPtr)->ptrToTreeOrData.ptrToData[i] = (*rootPtrPtr)->ptrToTreeOrData.ptrToData[i-1]; 
                    i--;
                }
                printf("Entered key :");
                puts(keyStr);
                printf("\n");
                strcpy((*rootPtrPtr)->keys[posn],keyStr);
                (*rootPtrPtr)->ptrToTreeOrData.ptrToData[posn] = dataPtr;  
                // print((*rootPtrPtr)->ptrToTreeOrData.ptrToData[posn]);       
                ((*rootPtrPtr)->numOfKeys)++;
            }
            else
            {
                int posn = posnFinder(nptr,0,keyStr,maxKeySize);
                int i = nptr->numOfKeys;
                while (i>posn)
                {
                    strcpy(nptr->keys[i],nptr->keys[i-1]);
                    nptr->ptrToTreeOrData.ptrToData[i] = nptr->ptrToTreeOrData.ptrToData[i-1]; 
                    i--;
                }
                printf("Entered key :");
                puts(keyStr);
                printf("\n");
                strcpy(nptr->keys[posn],keyStr);
                nptr->ptrToTreeOrData.ptrToData[posn] = dataPtr;  
                // print((nptr)->ptrToTreeOrData.ptrToData[posn]);        
                (nptr->numOfKeys)++;
            }            
        }
        else
        {
            // Node overflow condition
            char tempKeys[MAX_KEYS+1][16];
            accommodation* tempDataPtr[MAX_KEYS+1];
            int i;
            for ( i = 0; i < MAX_KEYS ; i++)
            {
                strcpy(tempKeys[i],nptr->keys[i]);
                tempDataPtr[i] = nptr->ptrToTreeOrData.ptrToData[i];
            }
            int j = MAX_KEYS;
            int posn = posnFinder(nptr,0,keyStr,maxKeySize);
            while (j>posn)
            {
                strcpy(tempKeys[j],tempKeys[j-1]);
                tempDataPtr[j]=tempDataPtr[j-1];
                j--;
            }
            strcpy(tempKeys[posn],keyStr);
            tempDataPtr[posn] = dataPtr;
            // Creating New leaf
            Node* newLeaf = (Node* )malloc(sizeof(Node));
            printf("Creating new leaf\n");
            InitializeLeafNode(&newLeaf);
            printf("Initialized new leaf\n");
            // Swapping next Pointers
            
            Node* temp = nptr->nextPtr;
            if(nptr == (*rootPtrPtr))//special case
            {
                (*rootPtrPtr)->nextPtr = newLeaf;
            }
            else
            {
                nptr->nextPtr = newLeaf;
            }            
            newLeaf->nextPtr = temp;
            

            //note: check 
            nptr->numOfKeys = (MAX_KEYS/2 + 1); // set to 2 , initially they were 3
            i=0;
            for ( i = 0; i < nptr->numOfKeys; i++)
            {
                strcpy(nptr->keys[i],tempKeys[i]);
                (nptr->ptrToTreeOrData).ptrToData[i] = tempDataPtr[i];
            }
            // inserting remaining stuff to new leaf Node we created
            j=0;
            for ( i = nptr->numOfKeys; i < (MAX_KEYS+1) ; i++)
            {
                strcpy(newLeaf->keys[j],tempKeys[i]);
                newLeaf->ptrToTreeOrData.ptrToData[j]=tempDataPtr[i];
                j++;
            }
            newLeaf->numOfKeys = j;

            if(nptr == (*rootPtrPtr))
            {
                // We create new Root in this case
                Node* newRoot = (Node*)malloc(sizeof(Node));
                InitializeInternalNode(&newRoot);
                (newRoot->numOfKeys)++;
                strcpy(newRoot->keys[0],newLeaf->keys[0]);
                newRoot->ptrToTreeOrData.ptrToTree[0] = nptr;
                newRoot->ptrToTreeOrData.ptrToTree[1] = newLeaf;                
                (*rootPtrPtr) = newRoot ;
                printf("Created New Root \n");  
            }
            else
            {
                InsertInInternal(rootPtrPtr,newLeaf->keys[0],maxKeySize,&parentPtr,&newLeaf);
            }
        }        
    }    
}

void InsertRecord(Node** rootPtrPtr,Node** typeArootPtr,Node** typeBrootPtr,Node** typeCrootPtr,Node** typeDrootPtr)
{
    int num_records,i;
	printf("Enter Number of people whose records you want to enter\n");
	scanf("%d",&num_records);
    accommodation* temp = (accommodation*)malloc(sizeof(accommodation)*num_records);
	for(i=0;i<num_records;i++)
	{
        printf("Enter First Name (First letter must be capitalized)\n");
		scanf("%s",temp[i].firstname);
		
		printf("Enter Last Name (First letter must be capitalized)\n");
		scanf("%s",temp[i].lastname);	
			
		printf("Enter accommodation type(type-A, type-B, type-C, or type-D)\n");
		scanf("%s",temp[i].accommodationtype);
		
		printf("Enter ID type(0 , 1 or 2)\n");
		scanf("%d",&temp[i].idtype);
		
		if(temp[i].idtype==0)
		{
			printf("Enter Aadhaar ID\n");
			scanf("%s",temp[i].idnumber.aadhaar);
            temp[i].idnumber.empcode[0]='\0';
            temp[i].idnumber.passport[0]='\0';
		}
		else if(temp[i].idtype==1)
		{
			printf("Enter Passport ID\n");
			scanf("%s",temp[i].idnumber.passport);
            temp[i].idnumber.empcode[0]='\0';
            temp[i].idnumber.aadhaar[0]='\0';
		}
		else
		{
			printf("Enter Employee Code\n");
			scanf("%s",temp[i].idnumber.empcode);
            temp[i].idnumber.aadhaar[0]='\0';
            temp[i].idnumber.passport[0]='\0';
		}
			
		printf("Enter Address\n");
		fflush(stdin);
		scanf("%[^\n]%*c",temp[i].address);// to take multiword input
		printf("\n");
        accommodation* tempDataPtr ;
        tempDataPtr = &temp[i];

        int maxKeySize = 16; //1+15
        char tempKeyStr[maxKeySize];
        switch (temp[i].idtype)
        {
        case 0:
            tempKeyStr[0] = '0';
            strcat(tempKeyStr,temp[i].idnumber.aadhaar);
            break;
        case 1:
            tempKeyStr[0] = '1';
            strcat(tempKeyStr,temp[i].idnumber.passport);
            break;
        default:
            tempKeyStr[0] = '2';
            strcat(tempKeyStr,temp[i].idnumber.empcode);
            break;
        }
        print(&temp[i]);
        InsertInLeaf(rootPtrPtr,tempKeyStr,maxKeySize,tempDataPtr);
        // print(&temp[i]);
        //allocate blocks part
        // allocateBlock(&typeArootPtr,&typeBrootPtr,&typeCrootPtr,&typeDrootPtr,tempDataPtr);
    }
}

void Search(Node* nptr)
{
    char first_name[50];
    char last_name[50];
    printf("Enter first name you want to search:\n");
    scanf("%s",first_name);
    printf("Enter last name you want to search:\n");
    scanf("%s",last_name);
    Node* firstLeft = firstLeftNode(nptr);
    if (firstLeft == NULL) 
    {
        printf("Key not found\n");
    }
    else
    {
        int flag=0;
        while ((!flag) && (firstLeft != NULL))
        {
            int i;
            // printf("%d\n",firstLeft->numOfKeys);
            for ( i = 0; i < (firstLeft->numOfKeys) && (!flag) ; i++) 
            {
                if(strcmp(firstLeft->ptrToTreeOrData.ptrToData[i]->firstname,first_name)==0 && strcmp(firstLeft->ptrToTreeOrData.ptrToData[i]->lastname,last_name)==0)
                {
                    flag=1;
                    printf("The result of your search is:\n");
                    print(firstLeft->ptrToTreeOrData.ptrToData[i]);
                }         
            }
            firstLeft = firstLeft->nextPtr;
        }
        if(flag==0)
        {
            printf("Key Not found!\n");
        }
        printf("\n");
    }
}

accommodation** searchKey(Node* nptr,const char* keyStr,int maxKeySize)
{
    accommodation** retPtr;
    if(nptr == NULL)
    {
        printf("No record inserted yet:\n");
    }
    else
    {
        while (nptr->isLeaf == FALSE)
        {
            int posn = posnFinder(nptr,0,keyStr,maxKeySize);
            nptr=nptr->ptrToTreeOrData.ptrToTree[posn];
        }
        //reached leaf
        int temp = 0;
        int found = 0;
        while ((temp<MAX_KEYS) && (!found))
        {
            // printf("Comparing: ");
            // puts(nptr->keys[temp]);
            // printf(",\n");
            // puts(keyStr);
            // printf("\n");
            if(strcmp(nptr->keys[temp],keyStr)==0)
            {
                found=1;
            }
            else
            {
                temp++;
            }
        }
        if (found==1)
        {
            retPtr = &(nptr->ptrToTreeOrData.ptrToData[temp]);
            printf("%s\n",(*retPtr)->firstname);
        }
        else
        {
            retPtr = NULL;
        }       
    }
    return retPtr;
}

void UpdateRecord(Node** rootPtrPtr)
{
    int id_type;
	char id_aadhaar[15];
	char id_passport[15];
	char id_empcode[15];
    char first_name[50],last_name[50],accommodation_type[20],person_address[100];
    int maxKeySize = 16; //1+15
    char* tempKeyStr = (char*) malloc(sizeof(char));	
	int found = 0;
	printf("Enter the ID-Type(0,1 or 2) of the person whose record you wish to update\n");
	scanf("%d",&id_type);
	if(id_type==0)
	{  
        tempKeyStr[0] = '0';     
        printf("Enter the Aadhaar number :\n");
		scanf("%s",id_aadhaar);        
        for(int i=0;i<strlen(id_aadhaar);i++)
        {
            tempKeyStr[i+1]=id_aadhaar[i];
        }
	}
	else if(id_type==1)
	{
        tempKeyStr[0]='1';  
		printf("Enter the Passport number :\n");
		scanf("%s",id_passport);
        for(int i=0;i<strlen(id_passport);i++)
        {
            tempKeyStr[i+1]=id_passport[i];
        }
        puts(tempKeyStr);
	}
	else if(id_type==2)
	{
        tempKeyStr[0] = '2';  
		printf("Enter the employee code :\n");
		scanf("%s",id_empcode);
        for(int i=0;i<strlen(id_empcode);i++)
        {
            tempKeyStr[i+1]=id_empcode[i];
        }
	}
    accommodation** UpdateNodePtr = searchKey((*rootPtrPtr),tempKeyStr,maxKeySize); 
    if (UpdateNodePtr == NULL)
    {
        printf("Key not found !!");
    }
    else
    {
        int decider;
		printf("The record is found to exist:\nPlease enter the field name which you wish to update\n");
		printf("The field names are as follows:\n");
		printf("Enter 1 to update first name\nEnter 2 to update last name\nEnter 3 to update accommodation type\nEnter 4 to update address\n");
		scanf("%d",&decider);
		if(decider==1)
		{
			printf("Enter First name (First letter must be capitalized) :\n");
			scanf("%s",first_name);
			strcpy((*UpdateNodePtr)->firstname,first_name);
            //Indexing(); 
		}
		else if(decider==2)
		{
			printf("Enter last name (First letter must be capitalized) :\n");
			scanf("%s",last_name);
			strcpy((*UpdateNodePtr)->lastname,last_name);
            //Indexing(); 
		}
		else if(decider==3)
		{
			printf("Enter accommodation type:\n");
			scanf("%s",accommodation_type);
			strcpy((*UpdateNodePtr)->accommodationtype,accommodation_type);
		}
		else if(decider==4)
		{
			printf("Enter person address :\n");
            fflush(stdin);
			scanf("%[^\n]%*c",person_address);
			strcpy((*UpdateNodePtr)->address,person_address);
		}
        printf("Updated successfully !\n");
    }  
    free(tempKeyStr);  
}

void DeleteRecord(Node** rootPtrPtr,Node** typeArootPtr,Node** typeBrootPtr,Node** typeCrootPtr,Node** typeDrootPtr)
{
    int id_type;
	char id_aadhaar[15];
	char id_passport[15];
	char id_empcode[15];
    int maxKeySize = 16; //1+15
    char* tempKeyStr = (char*) malloc(sizeof(char));
	int found = 0;
	printf("Enter the ID-Type(0,1 or 2) of the person whose record you wish to delete\n");
	scanf("%d",&id_type);
	if(id_type==0)
	{  
        tempKeyStr[0] = '0';     
        printf("Enter the Aadhaar number :\n");
		scanf("%s",id_aadhaar);        
        for(int i=0;i<strlen(id_aadhaar);i++)
        {
            tempKeyStr[i+1]=id_aadhaar[i];
        }
	}
	else if(id_type==1)
	{
        tempKeyStr[0]='1';  
		printf("Enter the Passport number :\n");
		scanf("%s",id_passport);
        for(int i=0;i<strlen(id_passport);i++)
        {
            tempKeyStr[i+1]=id_passport[i];
        }
	}
	else if(id_type==2)
	{
        tempKeyStr[0] = '2';  
		printf("Enter the employee code :\n");
		scanf("%s",id_empcode);
        for(int i=0;i<strlen(id_empcode);i++)
        {
            tempKeyStr[i+1]=id_empcode[i];
        }
	}
    DeleteInLeaf(rootPtrPtr,tempKeyStr,maxKeySize);
    free(tempKeyStr);
    // accommodation** tempDataPtrPtr = searchKey(rootPtrPtr,tempKeyStr,maxKeySize);
    // deallocateBlock(typeArootPtr,typeBrootPtr,typeCrootPtr,typeDrootPtr,(*tempDataPtrPtr));
}

void DeleteInLeaf(Node** rootPtrPtr,const char* keyStr, int maxKeySize)
{
    if((*rootPtrPtr)==NULL)
    {
        printf("The Database is empty\n");
    }
    else
    {
        Node* nptr = (*rootPtrPtr);
        Node* parentPtr;
        int leftSibling , rightSibling;
        int flag = 0;
        // Binary Search to find the keyStr
        while (nptr->isLeaf == FALSE)
        {
            int i;
            flag=0;
            for ( i = 0; (!flag) && (i < nptr->numOfKeys) ; i++)
            {
                parentPtr = nptr;
                leftSibling = i-1; // left side of the parent node
                rightSibling = i+1; // right 
                if ((!flag) && (strcmp(keyStr,nptr->keys[i])<0))
                {
                    nptr = nptr->ptrToTreeOrData.ptrToTree[i];
                    flag = 1;
                }
                if((!flag) && (i == (nptr->numOfKeys-1)))
                {
                    leftSibling = i;
                    rightSibling = i+2;//note: check
                    nptr = nptr->ptrToTreeOrData.ptrToTree[i+1];
                    flag = 1;
                }                 
            }            
        }
        int posn = 0;
        int found = 0;
        while ((posn<MAX_KEYS) && (!found))
        {
            if(strcmp(nptr->keys[posn],keyStr)==0)
            {
                found=1;
            }
            else
            {
                posn++;
            }
        }
        if (found==0)
        {
            printf("KEY NOT FOUND !!\n");
        } //(MAX_KEYS+1)/2
        else
        {
            printf("Deleting the record: ");
            print(nptr->ptrToTreeOrData.ptrToData[posn]);
            // free(nptr->ptrToTreeOrData.ptrToData[posn]);//free ing the heap memory //note: causing issues
            nptr->ptrToTreeOrData.ptrToData[posn]=NULL;
            
            int i;
            for (i=posn;i<nptr->numOfKeys;i++)
            {
                strcpy(nptr->keys[i],nptr->keys[i+1]);
                nptr->ptrToTreeOrData.ptrToData[i]=nptr->ptrToTreeOrData.ptrToData[i+1];
            }
            (nptr->numOfKeys)--;
            nptr->keys[(nptr->numOfKeys)][0] = '3'; // Mini Initialization
            nptr->ptrToTreeOrData.ptrToData[(nptr->numOfKeys)] = NULL;
            if(nptr==(*rootPtrPtr))
            {
                if ((*rootPtrPtr)->numOfKeys == 0)
                {
                    (*rootPtrPtr) = NULL;
                    printf("Database has become empty !");
                }                
            }
            else
            {
                // Insufficient nodes, borrowing required
                if(nptr->numOfKeys < (MAX_KEYS+1)/2)
                {
                    if (leftSibling>=0)
                    {
                        Node* leftNode = parentPtr->ptrToTreeOrData.ptrToTree[leftSibling];
                        if (leftNode->numOfKeys >= (((MAX_KEYS+1)/2) + 1))
                        {
                            //transfering the maximum key from leftNode
                            int maxIndex = leftNode->numOfKeys-1;
                            char tempKeyStr[maxKeySize];
                            accommodation* tempDataPtr;
                            strcpy(tempKeyStr,leftNode->keys[maxIndex]);
                            tempDataPtr = leftNode->ptrToTreeOrData.ptrToData[maxIndex];

                            (leftNode->numOfKeys)--;
                            leftNode->keys[maxIndex][0] = '3';//for safety
                            leftNode->ptrToTreeOrData.ptrToData[maxIndex] = NULL; // for safety

                            //shifting right in nptr
                            int i = (nptr->numOfKeys - 1);
                            while (i>0)
                            {
                                strcpy(nptr->keys[i],nptr->keys[i-1]);
                                nptr->ptrToTreeOrData.ptrToData[i] = nptr->ptrToTreeOrData.ptrToData[i-1];
                                i--;
                            }
                            // can store at 0th index now no data loss
                            strcpy(nptr->keys[0],tempKeyStr);
                            nptr->ptrToTreeOrData.ptrToData[0]=tempDataPtr;  
                            (nptr->numOfKeys)++; 

                            //updating the parent
                            strcpy(parentPtr->keys[leftSibling],nptr->keys[0]);
                            printf("Borrowed from left sibling during deletion of %s \n",keyStr);
                        }                        
                    }
                    else if (rightSibling < (parentPtr->numOfKeys + 1)) // to borrow from right sibling
                    {
                        Node* rightNode = parentPtr->ptrToTreeOrData.ptrToTree[rightSibling];
                        if(rightNode->numOfKeys >= (((MAX_KEYS+1)/2) + 1))
                        {
                            //transfering the minimum key from rightNode
                            int minIndex = 0;
                            char tempKeyStr[maxKeySize];
                            accommodation* tempDataPtr;
                            strcpy(tempKeyStr,rightNode->keys[minIndex]);
                            tempDataPtr = rightNode->ptrToTreeOrData.ptrToData[minIndex];

                            rightNode->keys[minIndex][0] = '3';//for safety
                            rightNode->ptrToTreeOrData.ptrToData[minIndex] = NULL; // for safety

                            //shifting leftwards in rightNode
                            int i = 1;
                            while (i<(rightNode->numOfKeys))
                            {
                                strcpy(rightNode->keys[i-1],rightNode->keys[i]);
                                rightNode->ptrToTreeOrData.ptrToData[i-1] = rightNode->ptrToTreeOrData.ptrToData[i];
                                i++;
                            }

                            (rightNode->numOfKeys)--;
                            
                            //storing in maxIndex of nptr
                            strcpy(nptr->keys[(nptr->numOfKeys)],tempKeyStr);
                            nptr->ptrToTreeOrData.ptrToData[(nptr->numOfKeys)] = tempDataPtr;
                            (nptr->numOfKeys)++;   

                            //updating the parent
                            strcpy(parentPtr->keys[rightSibling],rightNode->keys[0]);
                            printf("Borrowed from right sibling during deletion of %s \n",keyStr);                            
                        }
                    }// Case when Siblings doesnt have enough                      
                    else if(leftSibling>=0)
                    {
                        Node* leftNode = parentPtr->ptrToTreeOrData.ptrToTree[leftSibling];
                        //Transfer keys of nptr to leftNode also eventually number of keys in leftNode wont surpass MAX_KEYS bcz 
                        // they are already less than the minimum threshold in both
                        int j = 0;
                        int i = leftNode->numOfKeys;
                        while ((i<MAX_KEYS) && (j < nptr->numOfKeys))
                        {
                            strcpy(leftNode->keys[i],nptr->keys[j]);
                            leftNode->ptrToTreeOrData.ptrToData[i] = nptr->ptrToTreeOrData.ptrToData[j];
                            i++;
                            j++;
                        }
                        leftNode->numOfKeys = leftNode->numOfKeys + nptr->numOfKeys ;
                        leftNode->nextPtr = nptr->nextPtr;
                        printf("Merging two leafs\n");
                        DeleteInInternal(rootPtrPtr,parentPtr->keys[leftSibling],maxKeySize,&parentPtr,nptr);
                        // InitializeLeafNode(nptr); note: free(nptr) ?
                    }
                    else if (rightSibling <= parentPtr->numOfKeys)
                    {
                        Node* rightNode = parentPtr->ptrToTreeOrData.ptrToTree[rightSibling];
                        //Transfer keys of rightNode to nptr also eventually number of keys in nptr wont surpass MAX_KEYS bcz 
                        // they are already less than the minimum threshold in both
                        int j = 0;
                        int i = nptr->numOfKeys;
                        while ((i<MAX_KEYS) && (j < rightNode->numOfKeys))
                        {
                            strcpy(nptr->keys[i],rightNode->keys[j]);
                            nptr->ptrToTreeOrData.ptrToData[i] = rightNode->ptrToTreeOrData.ptrToData[j];
                            i++;
                            j++;
                        }
                        nptr->numOfKeys = nptr->numOfKeys + rightNode->numOfKeys ;
                        nptr->nextPtr = rightNode->nextPtr;
                        printf("Merging two leafs\n");
                        DeleteInInternal(rootPtrPtr,parentPtr->keys[rightSibling-1],maxKeySize,&parentPtr,rightNode);
                        // InitializeLeafNode(rightNode);  note: free(rightNode) ?
                    }                                        
                }
            }                        
        }        
    }
}

void DeleteInInternal(Node** rootPtrPtr,const char* keyStr,int maxKeySize,Node** parentPtrPtr ,Node* childPtr)
{
    if((*parentPtrPtr)==(*rootPtrPtr))
    {
        if((*parentPtrPtr)->numOfKeys == 1)
        {
            if((*parentPtrPtr)->ptrToTreeOrData.ptrToTree[1] == childPtr)
            {
                (*rootPtrPtr) = ((*parentPtrPtr)->ptrToTreeOrData).ptrToTree[0];
                //note: delete (*parentPtrPtr)??
                printf("New Changed Root\n");
            }
            else if ((*parentPtrPtr)->ptrToTreeOrData.ptrToTree[0] == childPtr)
            {
                (*rootPtrPtr) = (*parentPtrPtr)->ptrToTreeOrData.ptrToTree[1];
                //note: delete (*parentPtrPtr)??
                printf("New Changed Root\n");
            }            
        }
    }
    else // Deleting keyStr from parent
    {
        int posn;
        int found = 0;
        while ((!found) && (posn < (*parentPtrPtr)->numOfKeys))
        {
            if (strcmp((*parentPtrPtr)->keys[posn],keyStr) == 0)
            {
                found = 1;
            }
            else
            {
                posn++;
            }
        }
        while (posn < ((*parentPtrPtr)->numOfKeys - 1))
        {
            strcpy((*parentPtrPtr)->keys[posn],(*parentPtrPtr)->keys[posn+1]);
        }
        
        // deleting ptrTotree
        found = 0;
        posn = 0;
        while ((!found) && (posn < (*parentPtrPtr)->numOfKeys + 1))
        {
            if ((*parentPtrPtr)->ptrToTreeOrData.ptrToTree[posn] == childPtr)
            {
                found = 1;
            }
            else
            {
                posn++;
            }
        }
        while (posn < ((*parentPtrPtr)->numOfKeys))
        {
            (*parentPtrPtr)->ptrToTreeOrData.ptrToTree[posn] = (*parentPtrPtr)->ptrToTreeOrData.ptrToTree[posn+1];
        }
        ((*parentPtrPtr)->keys[(*parentPtrPtr)->numOfKeys-1][0]) = '3'; // For safety
        (*parentPtrPtr)->ptrToTreeOrData.ptrToTree[(*parentPtrPtr)->numOfKeys] = NULL ; // For safety 
        ((*parentPtrPtr)->numOfKeys)--;


        if((*parentPtrPtr) != (*rootPtrPtr))
        {
            if ((*parentPtrPtr)->numOfKeys >= (MAX_KEYS+1)/2 - 1)
            {
                printf("Deleted %s from internal node successfully\n",keyStr);
            }
            else
            {
                printf("Underflow in internal node\n");
                Node* newParent; //new Parent will be parent node of (*parentPtrPtr)
                findParent(&newParent,(*rootPtrPtr),(*parentPtrPtr));
                int leftSibling , rightSibling;

                int posn = 0;
                int flag = 0;
                while ((!flag) && (posn < newParent->numOfKeys+1))
                {
                    if(newParent->ptrToTreeOrData.ptrToTree[posn] == (*parentPtrPtr))
                    {
                        leftSibling = posn-1;
                        rightSibling = posn + 1;
                        flag = 1;
                    }
                    else
                    {
                        posn++;
                    }
                }
                
                // Transfer from left sibling if possible 
                if (leftSibling>=0)
                {
                    Node* leftNode = newParent->ptrToTreeOrData.ptrToTree[leftSibling];
                    //check for extra keys 
                    if(leftNode->numOfKeys >= (MAX_KEYS+1)/2)
                    {
                        //transfer from left sibling through parent
                        int maxIndexKey = leftNode->numOfKeys-1;
                        int maxIndexPtr = leftNode->numOfKeys;
                        char tempKeyStr[maxKeySize];
                        Node* tempTreePtr;
                        strcpy(tempKeyStr,leftNode->keys[maxIndexKey]);
                        tempTreePtr = leftNode->ptrToTreeOrData.ptrToTree[maxIndexPtr];
                        (leftNode->numOfKeys)--;
                        leftNode->keys[maxIndexKey][0] = '3';//for safety
                        leftNode->ptrToTreeOrData.ptrToTree[maxIndexPtr] = NULL; // for safety
                        //shifting right in (*parentPtrPtr)
                        int i = ((*parentPtrPtr)->numOfKeys - 1);
                        while (i>0)
                        {
                            strcpy((*parentPtrPtr)->keys[i],(*parentPtrPtr)->keys[i-1]);
                            i--;
                        }
                        i = ((*parentPtrPtr)->numOfKeys);
                        while (i>0)
                        {
                            (*parentPtrPtr)->ptrToTreeOrData.ptrToTree[i]=(*parentPtrPtr)->ptrToTreeOrData.ptrToTree[i-1];
                            i--;
                        }
                        
                        // can store at 0th index now no data loss
                        strcpy((*parentPtrPtr)->keys[0],tempKeyStr);
                        (*parentPtrPtr)->ptrToTreeOrData.ptrToTree[0]=tempTreePtr;  
                        ((*parentPtrPtr)->numOfKeys)++; 
                        //updating the parent
                        strcpy(newParent->keys[leftSibling],(*parentPtrPtr)->keys[0]);
                        printf("Borrowed from left sibling during deletion of %s in internal node\n",keyStr);
                    }                    
                } 
                else if (rightSibling < (newParent->numOfKeys + 1))
                {   
                    Node* rightNode = newParent->ptrToTreeOrData.ptrToTree[rightSibling];
                    //check if right sibling has extra keys
                    if(rightNode->numOfKeys >= (((MAX_KEYS+1)/2) + 1))
                    {
                        //transfering the minimum key from rightNode
                        int minIndexKey = 0;
                        int minIndexPtr = 0;
                        char tempKeyStr[maxKeySize];
                        Node* tempTreePtr;
                        strcpy(tempKeyStr,rightNode->keys[minIndexKey]);
                        tempTreePtr = rightNode->ptrToTreeOrData.ptrToTree[minIndexPtr];
                        rightNode->keys[minIndexKey][0] = '3';//for safety
                        rightNode->ptrToTreeOrData.ptrToTree[minIndexPtr] = NULL; // for safety
                        //shifting leftwards in rightNode
                        int i = 1;
                        while (i<(rightNode->numOfKeys))
                        {
                            strcpy(rightNode->keys[i-1],rightNode->keys[i]);
                            i++;
                        }
                        //shifting treeptrs leftwards
                        i = 1;
                        while (i<(rightNode->numOfKeys + 1))
                        {
                            rightNode->ptrToTreeOrData.ptrToTree[i-1] = rightNode->ptrToTreeOrData.ptrToTree[i];
                            i++;
                        }
                        
                        (rightNode->numOfKeys)--;
                        
                        //storing in maxIndex of (*parentPtrPtr)

                        strcpy((*parentPtrPtr)->keys[((*parentPtrPtr)->numOfKeys)],tempKeyStr);
                        (*parentPtrPtr)->ptrToTreeOrData.ptrToTree[((*parentPtrPtr)->numOfKeys)+1] = tempTreePtr;
                        ((*parentPtrPtr)->numOfKeys)++;   

                        //updating the parent
                        strcpy(newParent->keys[rightSibling],rightNode->keys[0]);
                        printf("Borrowed from right sibling during deletion of %s in internal Node\n",keyStr);
                    }
                }
                else if (leftSibling>=0)//start to merge if all above false
                {
                    Node* leftNode = newParent->ptrToTreeOrData.ptrToTree[leftSibling];
                    //note: something missing ??
                    //Transfer keys of nptr to leftNode also eventually number of keys in leftNode wont surpass MAX_KEYS bcz 
                    // they are already less than the minimum threshold in both
                    int j = 0;
                    int i = leftNode->numOfKeys;
                    while ((i<MAX_KEYS) && (j < (*parentPtrPtr)->numOfKeys))
                    {
                        strcpy(leftNode->keys[i],(*parentPtrPtr)->keys[j]);
                        i++;
                        j++;
                    }

                    j = 0;
                    i = leftNode->numOfKeys+1;
                    while ((i<MAX_CHILDREN) && ( j < (*parentPtrPtr)->numOfKeys + 1 ))
                    {   
                        leftNode->ptrToTreeOrData.ptrToTree[i] = (*parentPtrPtr)->ptrToTreeOrData.ptrToTree[j];
                        i++;
                        j++;
                    }
                        
                    leftNode->numOfKeys = leftNode->numOfKeys + (*parentPtrPtr)->numOfKeys ;
                    leftNode->nextPtr = (*parentPtrPtr)->nextPtr;
                    printf("Merging two Internal nodes\n");
                    DeleteInInternal(rootPtrPtr,newParent->keys[leftSibling],maxKeySize,&newParent,(*parentPtrPtr));
                    // InitializeLeafNode((*parentPtrPtr)); note: free((*parentPtrPtr)) ?
                }
                else if (rightSibling <= newParent->numOfKeys)
                {
                    Node* rightNode = newParent->ptrToTreeOrData.ptrToTree[rightSibling];
                    //note: something missing ??
                    int j = 0;
                    int i = (*parentPtrPtr)->numOfKeys;
                    while ((i<MAX_KEYS) && (j < rightNode->numOfKeys))
                    {
                        strcpy((*parentPtrPtr)->keys[i],rightNode->keys[j]);
                        i++;
                        j++;
                    }

                    j = 0;
                    i = (*parentPtrPtr)->numOfKeys+1;
                    while ((i<MAX_CHILDREN) && ( j < rightNode->numOfKeys + 1 ))
                    {   
                        (*parentPtrPtr)->ptrToTreeOrData.ptrToTree[i] = rightNode->ptrToTreeOrData.ptrToTree[j];
                        i++;
                        j++;
                    }
                        
                    (*parentPtrPtr)->numOfKeys = (*parentPtrPtr)->numOfKeys + rightNode->numOfKeys ;
                    (*parentPtrPtr)->nextPtr = rightNode->nextPtr;
                    printf("Merging two Internal nodes\n");
                    DeleteInInternal(rootPtrPtr,newParent->keys[rightSibling-1],maxKeySize,&newParent,rightNode); 
                }              
            }
        }                
    }
}

void removeDuplicates(Node** rootPtrPtr)
{
    int flag = 0;
    Node* firstLeft = firstLeftNode((*rootPtrPtr));
    if (firstLeft != NULL) 
    {
        while ((firstLeft != NULL))
        {
            int i=0;
            while(i < (firstLeft->numOfKeys - 1)) 
            {
                if(strcmp(firstLeft->keys[i+1],firstLeft->keys[i])==0)
                {
                    printf("Duplicate found for ID type %d and ID number %s\n",firstLeft->ptrToTreeOrData.ptrToData[i]->idtype,firstLeft->ptrToTreeOrData.ptrToData[i]->idnumber);
                    DeleteInLeaf(rootPtrPtr,firstLeft->keys[i+1],16); // DeleteInLeaf takes care of shifting business
                    printf("The duplicate found was deleted successfully\n");
                    flag = 1;
                }
                else
                {
                    i++;
                }        
            }
            firstLeft = firstLeft->nextPtr;
        }
        if (flag == 0)
        {
            printf("No duplicate found\n");
        }
    }
    else
    {
        printf("Cannot remove duplicates\n");
    }        
}

int countNodes(Node** rootPtrPtr)
{
    int count=0;
    Node* firstLeft = firstLeftNode(rootPtrPtr);
    if (firstLeft == NULL) 
    {
        count=0;
    }
    else
    {
        while (firstLeft != NULL)
        {
            int i;
            for ( i = 0; i < firstLeft->numOfKeys; i++) 
            {
                count++;            
            }
            firstLeft = firstLeft->nextPtr;
        }    
    }
    return count;
}

void allocateBlock(Node** typeArootPtr,Node** typeBrootPtr,Node** typeCrootPtr,Node** typeDrootPtr,accommodation* dataPtr)
{
    
    char tempKeyStr[3];

    if (strcmp("type-A",dataPtr->accommodationtype)==0)
    {
        tempKeyStr[0] = 'A';
        int currA = countNodes(typeArootPtr) + 1;
        if (currA != 10)
        {
            char current = currA + '0';
            strcat(tempKeyStr,current);        
        }
        else if(currA == 10)
        {
            char current = 1 + '0';
            strcat(tempKeyStr,current);
            current = 0 + '0';
            strcat(tempKeyStr,current);
        }
        
        InsertInLeaf(typeArootPtr,tempKeyStr,3,dataPtr);
    }
    else if (strcmp("type-B",dataPtr->accommodationtype)==0)
    {
        tempKeyStr[0] = 'B';
        int currB = countNodes(typeBrootPtr) + 1;
        if (currB != 10)
        {
            char current = currB + '0';
            strcat(tempKeyStr,current);        
        }
        else if(currB == 10)
        {
            char current = 1 + '0';
            
            strcat(tempKeyStr,current);
            current = 0 + '0';
            strcat(tempKeyStr,current);
        }
        
        InsertInLeaf(typeBrootPtr,tempKeyStr,3,dataPtr);
    }
    else if (strcmp("type-C",dataPtr->accommodationtype)==0)
    {
        tempKeyStr[0] = 'C';
        int currC = countNodes(typeCrootPtr) + 1;
        if (currC != 10)
        {
            char current = currC + '0';
            strcat(tempKeyStr,current);        
        }
        else if(currC == 10)
        {
            char current = 1 + '0';
            strcat(tempKeyStr,current);
            current = 0 + '0';
            strcat(tempKeyStr,current);
        }
        
        InsertInLeaf(typeCrootPtr,tempKeyStr,3,dataPtr);
    }
    else if (strcmp("type-D",dataPtr->accommodationtype)==0)
    {
        tempKeyStr[0] = 'D';
        int currD = countNodes(typeDrootPtr) + 1;
        if (currD != 10)
        {
            char current = currD + '0';
            strcat(tempKeyStr,current);        
        }
        else if(currD == 10)
        {
            char current = 1 + '0';
            strcat(tempKeyStr,current);
            current = 0 + '0';
            strcat(tempKeyStr,current);
        }
        
        InsertInLeaf(typeDrootPtr,tempKeyStr,3,dataPtr);
    }    
}

void searchDataPtr(Node* nptr,accommodation* dataPtr,char keyStr[],int maxKeySize)
{    
    Node* firstLeft = firstLeftNode(nptr);
    if (firstLeft == NULL) 
    {
        printf("All blocks are free\n");
    }
    else
    {
        int flag=0;
        while ((!flag) && (firstLeft != NULL))
        {
            int i;
            for ( i = 0; i < (firstLeft->numOfKeys) && (!flag) ; i++) 
            {
                if((firstLeft->ptrToTreeOrData.ptrToData[i] == dataPtr))
                {
                    flag=1;
                    strcpy(keyStr,firstLeft->keys[i]);//putting key containing dataPtr in tempKeyStr
                }         
            }
            firstLeft = firstLeft->nextPtr;
        }
        if(flag==0)
        {
            printf("No such data found\n");
        }
    }   
}

void deallocateBlock(Node** typeArootPtr,Node** typeBrootPtr,Node** typeCrootPtr,Node** typeDrootPtr,accommodation* dataPtr)
{
    char tempKeyStr[3];

    if (strcmp("type-A",dataPtr->accommodationtype)==0)
    {
        searchDataPtr((*typeArootPtr),dataPtr,tempKeyStr,3);//key value is updated in tempKeyStr
        DeleteInLeaf(typeArootPtr,tempKeyStr,3);
    }
    else if (strcmp("type-B",dataPtr->accommodationtype)==0)
    {
        searchDataPtr((*typeBrootPtr),dataPtr,tempKeyStr,3);
        DeleteInLeaf(typeBrootPtr,tempKeyStr,3);
    }
    else if (strcmp("type-C",dataPtr->accommodationtype)==0)
    {
        searchDataPtr((*typeCrootPtr),dataPtr,tempKeyStr,3);
        DeleteInLeaf(typeCrootPtr,tempKeyStr,3);
    }
    else if (strcmp("type-D",dataPtr->accommodationtype)==0)
    {
        searchDataPtr((*typeDrootPtr),dataPtr,tempKeyStr,3);
        DeleteInLeaf(typeDrootPtr,tempKeyStr,3);
    }
}

void addressSearch(Node* typeAroot,Node* typeBroot,Node* typeCroot,Node* typeDroot)
{
    printf("Enter the accommodation type name (type-A, type-B, type-C, or type-D) in which you wish to perform range search \n");
    char accommodation_type[20];
    scanf("%s",accommodation_type);
    int start_block;
    printf("Enter starting block address of the range you want(1-10)\n");
    scanf("%d",&start_block);
    int end_block;
    printf("Enter ending block address of the range you want(1-10)\n");
    scanf("%d",&end_block);

    char startKeyStr[3];
    char endKeyStr[3];

    if (strcmp("type-A",accommodation_type)==0)
    {
        startKeyStr[0] = 'A';
        endKeyStr[0] = 'A';

        if (start_block != 10)
        {
            char start = start_block + '0';
            strcat(startKeyStr,start);        
        }
        else if(start_block == 10)
        {
            char start = 1 + '0';
            strcat(startKeyStr,start);
            start = 0 + '0';
            strcat(startKeyStr,start);
        }
        if (end_block != 10)
        {
            char end = end + '0';
            strcat(endKeyStr,end);        
        }
        else if(end_block == 10)
        {
            char end = 1 + '0';
            strcat(endKeyStr,end);
            end = 0 + '0';
            strcat(endKeyStr,end);
        }        
        rangeSearch(typeAroot,startKeyStr,endKeyStr,3);
    }
    else if (strcmp("type-B",accommodation_type)==0)
    {
        startKeyStr[0] = 'B';
        endKeyStr[0] = 'B';

        if (start_block != 10)
        {
            char start = start_block + '0';
            strcat(startKeyStr,start);        
        }
        else if(start_block == 10)
        {
            char start = 1 + '0';
            strcat(startKeyStr,start);
            start = 0 + '0';
            strcat(startKeyStr,start);
        }
        if (end_block != 10)
        {
            char end = end + '0';
            strcat(endKeyStr,end);        
        }
        else if(end_block == 10)
        {
            char end = 1 + '0';
            strcat(endKeyStr,end);
            end = 0 + '0';
            strcat(endKeyStr,end);
        }        
        rangeSearch(typeBroot,startKeyStr,endKeyStr,3);
    }
    else if (strcmp("type-C",accommodation_type)==0)
    {
        startKeyStr[0] = 'C';
        endKeyStr[0] = 'C';

        if (start_block != 10)
        {
            char start = start_block + '0';
            strcat(startKeyStr,start);        
        }
        else if(start_block == 10)
        {
            char start = 1 + '0';
            strcat(startKeyStr,start);
            start = 0 + '0';
            strcat(startKeyStr,start);
        }
        if (end_block != 10)
        {
            char end = end + '0';
            strcat(endKeyStr,end);        
        }
        else if(end_block == 10)
        {
            char end = 1 + '0';
            strcat(endKeyStr,end);
            end = 0 + '0';
            strcat(endKeyStr,end);
        }        
        rangeSearch(typeCroot,startKeyStr,endKeyStr,3);
    }
    else if (strcmp("type-D",accommodation_type)==0)
    {
        startKeyStr[0] = 'D';
        endKeyStr[0] = 'D';

        if (start_block != 10)
        {
            char start = start_block + '0';
            strcat(startKeyStr,start);        
        }
        else if(start_block == 10)
        {
            char start = 1 + '0';
            strcat(startKeyStr,start);
            start = 0 + '0';
            strcat(startKeyStr,start);
        }
        if (end_block != 10)
        {
            char end = end + '0';
            strcat(endKeyStr,end);        
        }
        else if(end_block == 10)
        {
            char end = 1 + '0';
            strcat(endKeyStr,end);
            end = 0 + '0';
            strcat(endKeyStr,end);
        }        
        rangeSearch(typeDroot,startKeyStr,endKeyStr,3);
    }   
}

void rangeSearch(Node* nptr,char* startKey,char* endKey,int maxKeySize)
{
    Node* firstLeft = firstLeftNode(nptr);
    if (firstLeft == NULL) 
    {
        printf("Blocks are empty\n");
    }
    else
    {
        int flag = 0;
        int startFound = 0;
        int endFound = 0;
        while ((!flag) && (firstLeft != NULL))
        {
            int i;
            for ( i = 0; (!flag) &&  i < (firstLeft->numOfKeys); i++) 
            {
                if(strcmp(firstLeft->keys[i],startKey)==0)
                {
                    printf("The data in the given range %s to %s are :\n",startKey,endKey);
                    startFound = 1;
                }
                if((startFound == 1) && (endFound==0))
                {
                    printf("%s: ",firstLeft->keys[i]);
                    print(firstLeft->ptrToTreeOrData.ptrToData[i]);
                } 
                if(strcmp(firstLeft->keys[i],endKey)==0)
                {
                    endFound = 1;
                    flag = 1;
                }                     
            }
            firstLeft = firstLeft->nextPtr;
        }
        printf("\n");
    }
}