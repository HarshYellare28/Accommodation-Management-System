#include<stdio.h>
#include<stdlib.h>
#include<string.h>

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
    accommodation data;
    struct Node_tag* next;

}Node;

typedef struct index_node_tag
{
    Node* ptr;
    int key;
    struct index_node_tag* next;

}index_node;


typedef struct accommodation_type_node
{
    boolean isAccommodated;
    struct accommodation_type_node* next;    
}accommodation_type;

index_node* index_head = NULL;

Node* MakeNode(accommodation temp);
Node* InsertAtEnd(Node** ,Node* );
Node* concatenateLists(Node* lptr1, Node* lptr2);
void Insert(Node* ptr);
void InsertRecord();
void removeDuplicates();
void printRecords();
void print(Node* nptr);
void search(char first_name[],char last_name[],int SIZE);
void printSortedRecords();
void indexing();
void deleteRecord();
void updateRecord();
void Insert_acc_type(accommodation_type** head_ptr);
int count_nodes(accommodation_type** head_ptr);
void Delete_acc_type(accommodation_type** head_ptr);
void specialRequestAllocation();


Node* head0 = NULL;
Node* head1 = NULL; //head1 => marks the start of entries with idtype 1
Node* head2 = NULL; 
Node* head = NULL;

int main()
{
    char first_name[50];
    char last_name[50];

    InsertRecord();
    removeDuplicates();
    printf("The records are:\n");
    
    printRecords();
    printf("\n");
    indexing();
    printf("The sorted records are:\n");
    printSortedRecords();
    printf("\n");
    printf("Enter first name you want to search:\n");
    scanf("%s",first_name);
    printf("Enter last name you want to search:\n");
    scanf("%s",last_name);
    search(first_name,last_name,50);
    printf("\n");
    deleteRecord();
    printf("The records are:\n");
    printRecords();
    printf("\n");
    updateRecord();
    printf("The records are:\n");
    printRecords();
    printf("\n");
    specialRequestAllocation();
    printf("The records are:\n");
    printRecords();

    return 0;
}

Node* MakeNode(accommodation temp)
{
    Node* ptr ;
    ptr = (Node* )malloc((sizeof(Node)));
    ptr->data = temp;
    ptr->next = NULL;
    return ptr;
}

Node* InsertAtEnd(Node** lpptr,Node* ptr)
{
    Node* nptr;    
    if (*lpptr == NULL)
    {
        *lpptr = ptr; 
    }  
    else
    {
        nptr = *lpptr;
        while (nptr->next != NULL)
        {   
            nptr = nptr->next;
        }            
        nptr->next = ptr;
    }
    return *lpptr;
}

Node* concatenateLists(Node* lptr1, Node* lptr2)
{
    Node* lptr;
    if (lptr1 == NULL)
        lptr = lptr2;
    else
    {
        lptr = lptr1;
        if (lptr2 != NULL)
        {
            Node *nptr = lptr1;
            while (nptr->next != NULL)
            {
                nptr = nptr->next;
            }
            nptr->next = lptr2;
        }
    }
    return lptr;
}

void Insert(Node* ptr)
{
    if((ptr->data).idtype==0)
    {
        InsertAtEnd(&head0,ptr);        
    }
    else if((ptr->data).idtype==1)
    {
        InsertAtEnd(&head1,ptr);        
    }
    else
    {
        InsertAtEnd(&head2,ptr);        
    }
}

void InsertRecord()
{
	int num_records,i;
	printf("Enter Number of people whose records you want to enter\n");
	scanf("%d",&num_records);
    accommodation temp; // temporary data object
	
	for(i=0;i<num_records;i++)
	{
		printf("Enter First Name (First letter must be capitalized)\n");
		scanf("%s",temp.firstname);
		
		printf("Enter Last Name (First letter must be capitalized)\n");
		scanf("%s",temp.lastname);	
			
		printf("Enter accommodation type\n");
		scanf("%s",temp.accommodationtype);
		
		printf("Enter ID type(0 , 1 or 2)\n");
		scanf("%d",&temp.idtype);
		
		if(temp.idtype==0)
		{
			printf("Enter Aadhaar ID\n");
			scanf("%s",temp.idnumber.aadhaar);
            temp.idnumber.empcode[0]='\0';
            temp.idnumber.passport[0]='\0';
		}
		else if(temp.idtype==1)
		{
			printf("Enter Passport ID\n");
			scanf("%s",temp.idnumber.passport);
            temp.idnumber.empcode[0]='\0';
            temp.idnumber.aadhaar[0]='\0';
		}
		else
		{
			printf("Enter Employee Code\n");
			scanf("%s",temp.idnumber.empcode);
            temp.idnumber.aadhaar[0]='\0';
            temp.idnumber.passport[0]='\0';
		}
			
		printf("Enter Address\n");
		fflush(stdin);
		scanf("%[^\n]%*c",temp.address); // to take multiword input
		printf("\n");

        Node* ptr = MakeNode(temp);
        Insert(ptr);

		printf("Inserted Successfully\n");		
	}
    head = concatenateLists(head0,head1);
    head = concatenateLists(head,head2);
}

void removeDuplicates()
{
    Node* optr = head; // optr=>outer pointer
    Node* iptr = NULL; // inner pointer
    Node* trail_iptr;
    int found = 0;
    while (optr != NULL)
    {
        trail_iptr = optr;
        iptr = optr->next;        
        while (iptr != NULL)
        {
            found = 0;
            if (((iptr->data).idtype == (iptr->data).idtype))
            {
                if ((iptr->data).idtype == 0)
                {
                    if (!strcmp((iptr->data).idnumber.aadhaar,(optr->data).idnumber.aadhaar))
                    {
                        found = 1;
                        trail_iptr->next = iptr->next;
                        free(iptr);
                        iptr = NULL;
                    }                    
                }
                else if ((iptr->data).idtype == 1)
                {
                    if (!strcmp((iptr->data).idnumber.passport,(optr->data).idnumber.passport))
                    {
                        found = 1;
                        trail_iptr->next = iptr->next;
                        free(iptr);
                        iptr = NULL;
                    }                    
                }
                else
                {
                    if (!strcmp((iptr->data).idnumber.empcode,(optr->data).idnumber.empcode))
                    {
                        found = 1;
                        trail_iptr->next = iptr->next;
                        free(iptr);
                        iptr = NULL;
                    }                    
                }                              
            } 
            if (found == 1)
            {
                iptr = trail_iptr->next;
            }
            
            else
            {
                trail_iptr = iptr; 
                iptr = iptr->next;  
            }           
        }
        optr = optr->next;
    }   
}

void print(Node* nptr)
{
    if ((nptr->data).idtype==0)
    {
        printf("%s %s\t%s\t%d\t%s\t%s \n",(nptr->data).firstname,(nptr->data).lastname,(nptr->data).accommodationtype,(nptr->data).idtype,(nptr->data).idnumber.aadhaar,(nptr->data).address);
    }
    else if((nptr->data).idtype==1)
    {
        printf("%s %s\t%s\t%d\t%s\t%s \n",(nptr->data).firstname,(nptr->data).lastname,(nptr->data).accommodationtype,(nptr->data).idtype,(nptr->data).idnumber.passport,(nptr->data).address);
    }
    else
    {
        printf("%s %s\t%s\t%d\t%s\t%s \n",(nptr->data).firstname,(nptr->data).lastname,(nptr->data).accommodationtype,(nptr->data).idtype,(nptr->data).idnumber.empcode,(nptr->data).address);
    }
}

void printRecords()
{
    Node* nptr = head;
    while (nptr!=NULL)
    {
        print(nptr);
        nptr=nptr->next;
    }    
}

void search(char first_name[],char last_name[],int SIZE)
{
    int i ;
    int found=0;
    Node* nptr = head;
    while(nptr != NULL)
    {
        if((!strcmp((nptr->data).firstname,first_name)) && (!strcmp((nptr->data).lastname,last_name)))
        {
            print(nptr);
            found=1;
        }	
        nptr = nptr->next;	        
    }
    if(!found)
    {
    	printf("Not Found\n");
	} 
}

// char fullname_generator(Node* nptr)
// {    
//     char fullname[100];
    
//     strcpy(fullname,(nptr->data).firstname);
// 	strcat(fullname,(nptr->data).lastname);
    
//     return fullname;
// }


void printSortedRecords()
{
    index_node* nptr = index_head;
    while (nptr != NULL)
    {
        print(nptr->ptr); // definition of print is given earlier
        nptr = nptr->next;
    }    
}

void indexing()
{
    Node* optr = head->next;
    index_node* tptr = NULL;
    index_node* iptr ;
    index_node* traverser;
    index_node* temp ;
    index_head = (index_node* )malloc(sizeof(index_node));
    index_head->ptr = head;
    index_head->key = 1;
    index_head->next = NULL;
    int found = 0;
    while (optr!=NULL)
    {
        found = 0;
        tptr = NULL;
        iptr = index_head;
        while ((!found))
        {
            if((iptr != NULL) && ((strcmp(((iptr->ptr)->data).firstname,(optr->data).firstname)<0) || ((strcmp(((iptr->ptr)->data).firstname,(optr->data).firstname)==0) && (strcmp(((iptr->ptr)->data).lastname,(optr->data).lastname)<0))))
            {
                tptr = iptr;
                iptr=iptr->next;
            }            
            else
            {
                found = 1;
                if (iptr == NULL)
                {
                    temp = (index_node* )malloc(sizeof(index_node));
                    tptr->next = temp;
                    temp->ptr = optr;
                    temp->key = tptr->key+1;
                    temp->next = NULL;
                }
                else if (iptr == index_head)
                {
                    temp = (index_node* )malloc(sizeof(index_node));
                    temp->ptr = optr;
                    temp->key = 1;
                    temp->next = index_head;
                    index_head = temp;
                    traverser = iptr;
                    while (traverser != NULL)
                    {
                        traverser->key = traverser->key + 1;
                        traverser = traverser->next;
                    }
                }
                else
                {
                    temp = (index_node* )malloc(sizeof(index_node));
                    tptr->next = temp;
                    temp->ptr = optr;
                    temp->key = iptr->key;
                    temp->next = iptr;
                    traverser = iptr;
                    while (traverser != NULL)
                    {
                        traverser->key = traverser->key + 1;
                        traverser = traverser->next;
                    }                    
                }                
            }
            
        }        
        optr = optr->next;
    } 
}

void deleteRecord()
{
	int id_type;
	char id_aadhaar[15];
	char id_passport[15];
	char id_empcode[15];

	int found=0 ;
	printf("Enter the ID-Type(0,1 or 2) of the person whose record you wish to delete\n");
	scanf("%d",&id_type);
	if(id_type==0)
	{
		printf("Enter the Aadhaar number :\n");
		scanf("%s",id_aadhaar);
	}
	else if(id_type==1)
	{
		printf("Enter the Passport number :\n");
		scanf("%s",id_passport);
	}
	else if(id_type==2)
	{
		printf("Enter the employee code :\n");
		scanf("%s",id_empcode);
	}

    Node* nptr = head;
    Node* tptr = NULL;
    index_node* iptr;
    index_node* tiptr;
    index_node* traverser;
	while((nptr != NULL) && !(found))
	{
		if((id_type==0) && (!strcmp((nptr->data).idnumber.aadhaar,id_aadhaar)))
		{
			found=1;
		}
		else if((id_type==1) && (!strcmp((nptr->data).idnumber.passport,id_passport)))
		{
			found=1;
		}
		else if((id_type==2) && (!strcmp((nptr->data).idnumber.empcode,id_empcode)))
		{
			found=1;
		}
		else
		{
            tptr = nptr;
			nptr = nptr->next;
		}		
	}
    
	if(found==1)
	{
        if (nptr == head)
        {
            head = nptr->next;            
        }
        else
        {
            tptr->next = nptr->next;
        }  
        // index updation
        iptr = index_head;
        while ((iptr->ptr) != nptr)
        {
            tiptr = iptr;
            iptr = iptr->next;
        }
        if(iptr == index_head)
        {
            index_head = index_head->next;
        }
        else
        {
            tiptr->next = iptr->next;
        }        
        traverser = iptr->next;
        while (traverser != NULL)
        {
            traverser->key = traverser->key - 1;
            traverser = traverser->next;
        }
        
        free(iptr);
        iptr = NULL;
        free(nptr);
        nptr = NULL;         	
	}
	else
	{
		printf("Entry not found\n");
	}
}

void updateRecord()
{
	int id_type;
	char id_aadhaar[15];
	char id_passport[15];
	char id_empcode[15];
	char first_name[50],last_name[50],accommodation_type[20],person_address[100];
	
	int found=0 ;
	printf("Enter the ID-Type(0,1 or 2) of the person whose record you wish to update\n");
	scanf("%d",&id_type);
	if(id_type==0)
	{
		printf("Enter the Aadhaar number :\n");
		scanf("%s",id_aadhaar);
	}
	else if(id_type==1)
	{
		printf("Enter the Passport number :\n");
		scanf("%s",id_passport);
	}
	else if(id_type==2)
	{
		printf("Enter the employee code :\n");
		scanf("%s",id_empcode);
	}
	Node* nptr = head;
    Node* tptr = NULL;
    // index_node* iptr;
    // index_node* tiptr;
    // index_node* traverser;
	while((nptr != NULL) && !(found))
	{
		if((id_type==0) && (!strcmp((nptr->data).idnumber.aadhaar,id_aadhaar)))
		{
			found=1;
		}
		else if((id_type==1) && (!strcmp((nptr->data).idnumber.passport,id_passport)))
		{
			found=1;
		}
		else if((id_type==2) && (!strcmp((nptr->data).idnumber.empcode,id_empcode)))
		{
			found=1;
		}
		else
		{
            tptr = nptr;
			nptr = nptr->next;
		}		
	}
	if(found==1)
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
			strcpy((nptr->data).firstname,first_name);
		}
		else if(decider==2)
		{
			printf("Enter last name (First letter must be capitalized) :\n");
			scanf("%s",last_name);
			strcpy((nptr->data).lastname,last_name);
		}
		else if(decider==3)
		{
			printf("Enter accommodation type:\n");
			scanf("%s",accommodation_type);
			strcpy((nptr->data).accommodationtype,accommodation_type);
		}
		else if(decider==4)
		{
			printf("Enter person address :\n");
			scanf("%s",person_address);
			strcpy((nptr->data).address,person_address);
		}
        indexing();
	}
	else
	{
		printf("Entry Not Found\n");
	}
}
accommodation_type* type1head = NULL;
accommodation_type* type2head = NULL;
accommodation_type* type3head = NULL;
accommodation_type* type4head = NULL;


void Insert_acc_type(accommodation_type** head_ptr)
{
    accommodation_type* aptr = *head_ptr;
    accommodation_type* temp;
    
    if (aptr == NULL)
    {
        temp = (accommodation_type* )malloc(sizeof(accommodation_type));
        temp->isAccommodated = TRUE;
        temp->next = NULL;
        *head_ptr = temp;
        
    }
    else
    {
        while (aptr->next!=NULL)
        {
            aptr = aptr->next;
        }        
        temp = (accommodation_type* )malloc(sizeof(accommodation_type));
        temp->isAccommodated = TRUE;
        aptr->next = temp;
        temp->next = NULL; 
             
    }    
}

int count_nodes(accommodation_type** head_ptr)
{
    accommodation_type* aptr = *head_ptr;
    int count = 0;
    
    while (aptr != NULL)
    {
        count++;
        aptr = aptr->next;
    }
    
    return count;
}

void Delete_acc_type(accommodation_type** head_ptr)
{
    
    accommodation_type* temp;
    temp = *head_ptr;
    if (*head_ptr != NULL)
    {
        *head_ptr = (*head_ptr)->next;
    }    
    
    free(temp);
}

void specialRequestAllocation()
{
    char str1[20]="type-I";
	char str2[20]="type-II";
	char str3[20]="type-III";
	char str4[20]="type-IV";
    Node* nptr = head;
    while (nptr!=NULL)
    {
        if(!strcmp((nptr->data).accommodationtype,str1))
        {
            Insert_acc_type(&type1head);
        }
        else if(!strcmp((nptr->data).accommodationtype,str2))
        {
            Insert_acc_type(&type2head);
        }
        else if(!strcmp((nptr->data).accommodationtype,str3))
        {
            Insert_acc_type(&type3head);
        }
        else
        {
            Insert_acc_type(&type4head);
        }
        nptr=nptr->next;
    } 
    
    char requested_accommo_type[20];
	printf("Enter the accommodation type which you wish to request for allocation :\n");
    fflush(stdin);
	scanf("%s",requested_accommo_type);
    int id_type;
    char id_aadhaar[15];
    char id_passport[15];
    char id_empcode[15];
	if(!strcmp(requested_accommo_type,str1))
    {
        if(count_nodes(&type1head)<10)
        {
            printf("Enter the ID-Type(0,1 or 2) of the person whose accommodation type you wish to change\n");
	        scanf("%d",&id_type);
	        if(id_type==0)
	        {
	        	printf("Enter the Aadhaar number :\n");
	        	scanf("%s",id_aadhaar);
	        }
	        else if(id_type==1)
	        {
	        	printf("Enter the Passport number :\n");
	        	scanf("%s",id_passport);
	        }
	        else if(id_type==2)
	        {
	        	printf("Enter the employee code :\n");
	        	scanf("%s",id_empcode);
	        }
            int found=0;
	        Node* nptr = head;
            Node* tptr = NULL;
	        while((nptr != NULL) && !(found))
	        {
	        	if((id_type==0) && (!strcmp((nptr->data).idnumber.aadhaar,id_aadhaar)))
	        	{
	        		found=1;
	        	}
	        	else if((id_type==1) && (!strcmp((nptr->data).idnumber.passport,id_passport)))
	        	{
	        		found=1;
	        	}
	        	else if((id_type==2) && (!strcmp((nptr->data).idnumber.empcode,id_empcode)))
	        	{
	        		found=1;
	        	}
	        	else
	        	{
                    tptr = nptr;
	        		nptr = nptr->next;
	        	}		
	        }
            if (found == 1)
            {
                char temp_str[20];
                strcpy(temp_str,(nptr->data).accommodationtype);
                strcpy((nptr->data).accommodationtype,str1);
                printf("%s \n",(nptr->data).accommodationtype);
                Insert_acc_type(&type1head);
                if(!strcmp(temp_str,str1))
                {                    
                    Delete_acc_type(&type1head);                    
                }
                else if(!strcmp(temp_str,str2))
                {
                    Delete_acc_type(&type2head);
                }
                else if(!strcmp(temp_str,str3))
                {
                    Delete_acc_type(&type3head);
                }
                else
                {
                    Delete_acc_type(&type4head);
                }                
            }
            else
            {
                printf("No such resident found\n");
            }
        }
        else
        {
            printf("This Accommodation type is already full, cannot allocate\n");
        }
    }
    else if(!strcmp(requested_accommo_type,str2))
    {
        if(count_nodes(&type2head)<10)
        {
            printf("Enter the ID-Type(0,1 or 2) of the person whose accommodation type you wish to change\n");
	        scanf("%d",&id_type);
	        if(id_type==0)
	        {
	        	printf("Enter the Aadhaar number :\n");
	        	scanf("%s",id_aadhaar);
	        }
	        else if(id_type==1)
	        {
	        	printf("Enter the Passport number :\n");
	        	scanf("%s",id_passport);
	        }
	        else if(id_type==2)
	        {
	        	printf("Enter the employee code :\n");
	        	scanf("%s",id_empcode);
	        }
            int found=0;
	        Node* nptr = head;
            Node* tptr = NULL;
	        while((nptr != NULL) && !(found))
	        {
	        	if((id_type==0) && (!strcmp((nptr->data).idnumber.aadhaar,id_aadhaar)))
	        	{
	        		found=1;
	        	}
	        	else if((id_type==1) && (!strcmp((nptr->data).idnumber.passport,id_passport)))
	        	{
	        		found=1;
	        	}
	        	else if((id_type==2) && (!strcmp((nptr->data).idnumber.empcode,id_empcode)))
	        	{
	        		found=1;
	        	}
	        	else
	        	{
                    tptr = nptr;
	        		nptr = nptr->next;
	        	}		
	        }
            if (found == 1)
            {
                char temp_str[20];
                strcpy(temp_str,(nptr->data).accommodationtype);
                strcpy((nptr->data).accommodationtype,str2);
                Insert_acc_type(&type1head);
                if(!strcmp(temp_str,str1))
                {                    
                    Delete_acc_type(&type1head);                    
                }
                else if(!strcmp(temp_str,str2))
                {
                    Delete_acc_type(&type2head);
                }
                else if(!strcmp(temp_str,str3))
                {
                    Delete_acc_type(&type3head);
                }
                else
                {
                    Delete_acc_type(&type4head);
                }                
            }
            else
            {
                printf("No such resident found\n");
            }
        }
        else
        {
            printf("This Accommodation type is already full, cannot allocate\n");
        }
    }
    else if(!strcmp(requested_accommo_type,str3))
    {
        if(count_nodes(&type3head)<10)
        {
            printf("Enter the ID-Type(0,1 or 2) of the person whose accommodation type you wish to change\n");
	        scanf("%d",&id_type);
	        if(id_type==0)
	        {
	        	printf("Enter the Aadhaar number :\n");
	        	scanf("%s",id_aadhaar);
	        }
	        else if(id_type==1)
	        {
	        	printf("Enter the Passport number :\n");
	        	scanf("%s",id_passport);
	        }
	        else if(id_type==2)
	        {
	        	printf("Enter the employee code :\n");
	        	scanf("%s",id_empcode);
	        }
            int found=0;
	        Node* nptr = head;
            Node* tptr = NULL;
	        while((nptr != NULL) && !(found))
	        {
	        	if((id_type==0) && (!strcmp((nptr->data).idnumber.aadhaar,id_aadhaar)))
	        	{
	        		found=1;
	        	}
	        	else if((id_type==1) && (!strcmp((nptr->data).idnumber.passport,id_passport)))
	        	{
	        		found=1;
	        	}
	        	else if((id_type==2) && (!strcmp((nptr->data).idnumber.empcode,id_empcode)))
	        	{
	        		found=1;
	        	}
	        	else
	        	{
                    tptr = nptr;
	        		nptr = nptr->next;
	        	}		
	        }
            if (found == 1)
            {
                char temp_str[20];
                strcpy(temp_str,(nptr->data).accommodationtype);
                strcpy((nptr->data).accommodationtype,str3);
                Insert_acc_type(&type1head);
                if(!strcmp(temp_str,str1))
                {                    
                    Delete_acc_type(&type1head);                    
                }
                else if(!strcmp(temp_str,str2))
                {
                    Delete_acc_type(&type2head);
                }
                else if(!strcmp(temp_str,str3))
                {
                    Delete_acc_type(&type3head);
                }
                else
                {
                    Delete_acc_type(&type4head);
                }                
            }
            else
            {
                printf("No such resident found\n");
            }
        }
        else
        {
            printf("This Accommodation type is already full, cannot allocate\n");
        }
    }
    else
    {
        if(count_nodes(&type4head)<10)
        {
            printf("Enter the ID-Type(0,1 or 2) of the person whose accommodation type you wish to change\n");
	        scanf("%d",&id_type);
	        if(id_type==0)
	        {
	        	printf("Enter the Aadhaar number :\n");
	        	scanf("%s",id_aadhaar);
	        }
	        else if(id_type==1)
	        {
	        	printf("Enter the Passport number :\n");
	        	scanf("%s",id_passport);
	        }
	        else if(id_type==2)
	        {
	        	printf("Enter the employee code :\n");
	        	scanf("%s",id_empcode);
	        }
            int found=0;
	        Node* nptr = head;
            Node* tptr = NULL;
	        while((nptr != NULL) && !(found))
	        {
	        	if((id_type==0) && (!strcmp((nptr->data).idnumber.aadhaar,id_aadhaar)))
	        	{
	        		found=1;
	        	}
	        	else if((id_type==1) && (!strcmp((nptr->data).idnumber.passport,id_passport)))
	        	{
	        		found=1;
	        	}
	        	else if((id_type==2) && (!strcmp((nptr->data).idnumber.empcode,id_empcode)))
	        	{
	        		found=1;
	        	}
	        	else
	        	{
                    tptr = nptr;
	        		nptr = nptr->next;
	        	}		
	        }
            if (found == 1)
            {
                char temp_str[20];
                strcpy(temp_str,(nptr->data).accommodationtype);
                strcpy((nptr->data).accommodationtype,str4);
                Insert_acc_type(&type1head);
                if(!strcmp(temp_str,str1))
                {                    
                    Delete_acc_type(&type1head);                    
                }
                else if(!strcmp(temp_str,str2))
                {
                    Delete_acc_type(&type2head);
                }
                else if(!strcmp(temp_str,str3))
                {
                    Delete_acc_type(&type3head);
                }
                else
                {
                    Delete_acc_type(&type4head);
                }                
            }
            else
            {
                printf("No such resident found\n");
            }
        }
        else
        {
            printf("This Accommodation type is already full, cannot allocate\n");
        }
    }      
}





