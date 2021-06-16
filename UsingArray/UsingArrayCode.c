#include<stdio.h>
#include<string.h>
#define SIZE 500
#define SUCCESS 1 
#define FAILURE 0
void Initialize(struct accommodation B[], int );
void InsertRecord(struct accommodation B[],int );
void RemoveDuplicates(struct accommodation B[],int );
void printRecords(struct accommodation B[],int );
void search (struct accommodation B[],int );
// void print(struct accommodation B[],int  );
//void printSortedRecords(struct accommodation B[],int ,int index[]);
int deleteRecord(struct accommodation B[],int ,int index[]);
int updateRecord(struct accommodation B[],int ,int index[]);
void more_on_accommodation_type (struct accommodation B[],int );
void printSortedRecords(struct accommodation B[],int ,int index[]);
void indexing(struct accommodation B[],int ,int index[]);

struct accommodation
{ 
	char firstname[50]; 
	char lastname[50]; 
	char accommodationtype[20];
	int idtype; 
	struct idnum
	{ 
		char aadhaar[15]; 
		char passport[15]; 
		char empcode[15];
	}idnumber; 	
	char address[100]; 
};
int main()
{
	struct accommodation B[SIZE];	
	Initialize(B,SIZE);
	InsertRecord(B,SIZE);
	RemoveDuplicates(B,SIZE);
	printRecords(B,SIZE);
	search(B,SIZE);
	int index[SIZE];
	for(int i=0;i<SIZE;i++)
	{
		index[i]=0;
	}
	indexing(B,SIZE,index);
	int flag1 = deleteRecord(B,SIZE,index);
	if(flag1)
	{
		printf("Record deleted successfully\n");
	}
	else
	{
		printf("Record not found\n");
	}
	int flag2 = updateRecord(B,SIZE,index);
	if(flag2)
	{
		printf("Record updated successfully\n");
	}
	else
	{
		printf("Record not found\n");
	}
	more_on_accommodation_type (B,SIZE);

	
}
void Initialize(struct accommodation B[], int size)
{
	int i;
	for(i=0;i<size;i++)
	{
		B[i].firstname[0]='\0';
		B[i].lastname[0]='\0';
		B[i].accommodationtype[0]='\0';	
		B[i].idtype=-1;
		B[i].idnumber.aadhaar[0]='\0';
		B[i].idnumber.passport[0]='\0';
		B[i].idnumber.empcode[0]='\0';
		B[i].address[0]='\0';		
	}	
}
void InsertRecord(struct accommodation B[],int N)
{
	int num_records,i;
	printf("Enter Number of people whose records you want to enter\n");
	scanf("%d",&num_records);
	char first_name[50],last_name[50],accommodation_type[20],person_address[100],id_aadhaar[15],id_passport[15],id_empcode[15];
	int id_type;
	for(i=0;i<num_records;i++)
	{
		printf("Enter First Name\n");
		scanf("%s",first_name);
		
		printf("Enter Last Name\n");
		scanf("%s",last_name);	
			
		printf("Enter accommodation type\n");
		scanf("%s",accommodation_type);
		
		printf("Enter ID type(0 , 1 or 2)\n");
		scanf("%d",&id_type);
		
		if(id_type==0)
		{
			printf("Enter Aadhaar ID\n");
			scanf("%s",id_aadhaar);
		}
		else if(id_type==1)
		{
			printf("Enter Passport ID\n");
			scanf("%s",id_passport);
		}
		else
		{
			printf("Enter Employee Code\n");
			scanf("%s",id_empcode);
		}
			
		printf("Enter Address\n");
		fflush(stdin);
		scanf("%[^\n]%*c",person_address); // to take multiword input
		printf("\n");
		
		strcpy(B[i].firstname,first_name);
		strcpy(B[i].lastname,last_name);
		strcpy(B[i].accommodationtype,accommodation_type);
		B[i].idtype=id_type;
		if(B[i].idtype==0)
		{
			strcpy(B[i].idnumber.aadhaar,id_aadhaar);
		}
		else if(B[i].idtype==1)
		{
			strcpy(B[i].idnumber.passport,id_passport);
		}
		else
		{
			strcpy(B[i].idnumber.empcode,id_empcode);
		}
		strcpy(B[i].address,person_address);
		printf("Inserted Successfully\n");		
	}
}

/*void InsertRecord(struct accommodation B[],int N)
{
	int num_records,i;
	int status=SUCCESS ; 
	printf("Enter Number of people whose records you want to enter:\n");
	scanf("%d",&num_records);
	
	char first_name[50],last_name[50],accommodation_type[20],person_address[100],id_aadhaar[15],id_passport[15],id_empcode[15];
	int id_type;
	
	
	for(i=0;i<num_records;i++)
	{
		printf("Enter First Name\n");
		scanf("%s",first_name);
		
		printf("Enter Last Name\n");
		scanf("%s",last_name);	
			
		printf("Enter accommodation type\n");
		scanf("%s",accommodation_type);
		
		printf("Enter Address\n");
		gets(person_address);
		//scanf("%s",person_address);
		
		printf("Enter ID type\n");
		scanf("%s",&id_type);
		
		printf("\n");
		
		if(id_type==0)
		{
			printf("Enter Aadhaar ID\n");
			scanf("%s",id_aadhaar);
		}
		else if(id_type==1)
		{
			printf("Enter Passport ID\n");
			scanf("%s",id_passport);
		}
		else if(id_type==2)
		{
			printf("Enter Employee Code\n");
			scanf("%s",id_empcode);
		}
			
	
		
		int j , k , found , is_free_loc ;
		j=0;
		found=0;
		while((j<N) && (!found))
		{
			if(id_type==0)
			{
				if(strcmp(B[j].idnumber.aadhaar,id_aadhaar)==0)
				{
					found=1;
				}
				else
				{
					j++;
				}				
			}
			else if(id_type==1)
			{
				if(strcmp(B[j].idnumber.passport,id_passport)==0)
				{
					found=1;
				}
				else
				{
					j++;
				}
			}
			else
			{
				if(strcmp(B[j].idnumber.empcode,id_empcode)==0)
				{
					found=1;
				}
				else
				{
					j++;
				}
			}			
		}
		if(found==1)
		{
			strcpy(B[j].firstname,first_name);
			strcpy(B[j].lastname,last_name);
			strcpy(B[j].accommodationtype,accommodation_type);
			strcpy(B[j].address,person_address);			
		}
		else
		{
			k=0;
			is_free_loc=0;
			while((k<N) && (!is_free_loc))
			{
				if((B[k].idnumber.aadhaar[0]='\0') && (B[k].idnumber.passport[0]='\0') && (B[k].idnumber.empcode[0]='\0')) 
				{
					is_free_loc=1;
				}
				else
				{
					k++;
				}
			}
			if(is_free_loc==1)
			{
				strcpy(B[k].firstname,first_name);
				strcpy(B[k].lastname,last_name);
				strcpy(B[k].accommodationtype,accommodation_type);
				B[k].idtype=id_type;
				if(B[k].idtype==0)
				{
					strcpy(B[k].idnumber.aadhaar,id_aadhaar);
				}
				else if(B[k].idtype==1)
				{
					strcpy(B[k].idnumber.passport,id_passport);
				}
				else
				{
					strcpy(B[k].idnumber.empcode,id_empcode);
				}
				strcpy(B[k].address,person_address);
			}
			else
			{
				status = FAILURE ;
			}			
		}
		printf("%d\n",status);						
	}	
}*/
void RemoveDuplicates(struct accommodation B[],int N)
{
	int i , j ; 
	for(i=0;i<N;i++)
	{
		if((B[i].idtype!=-1))
		{
			for(j=i+1;j<N;j++)
			{
				if((B[i].idtype==0) && (B[j].idtype==0) && (!strcmp(B[i].idnumber.aadhaar,B[j].idnumber.aadhaar)))
				{
					B[j].firstname[0]='\0';
					B[j].lastname[0]='\0';
					B[j].accommodationtype[0]='\0';	
					B[j].idtype=-1;
					B[j].idnumber.aadhaar[0]='\0';					
					B[j].address[0]='\0';
				}
				if((B[i].idtype==1) && (B[j].idtype==1) && (!strcmp(B[i].idnumber.passport,B[j].idnumber.passport)))
				{
					B[j].firstname[0]='\0';
					B[j].lastname[0]='\0';
					B[j].accommodationtype[0]='\0';	
					B[j].idtype=-1;
					B[j].idnumber.passport[0]='\0';					
					B[j].address[0]='\0';
				}
				if((B[i].idtype==2) && (B[j].idtype==2) && (!strcmp(B[i].idnumber.empcode,B[j].idnumber.empcode)))
				{
					B[j].firstname[0]='\0';
					B[j].lastname[0]='\0';
					B[j].accommodationtype[0]='\0';	
					B[j].idtype=-1;
					B[j].idnumber.empcode[0]='\0';					
					B[j].address[0]='\0';
				}
			}
		}
		else
		{
			i++;
		}		
	}
}

void printRecords(struct accommodation B[],int N)
{
	int i;
	for(i=0;i<N;i++)
	{
		if(B[i].idtype==1)
		{
			printf("The current records are: %s %s\t%s\t%d\t%s\t%s \n",B[i].firstname,B[i].lastname,B[i].accommodationtype,B[i].idtype,B[i].idnumber.passport,B[i].address);
		}		
	}
	for(i=0;i<N;i++)
	{
		if(B[i].idtype==0)
		{
			printf("The current records are: %s %s\t%s\t%d\t%s\t%s \n",B[i].firstname,B[i].lastname,B[i].accommodationtype,B[i].idtype,B[i].idnumber.aadhaar,B[i].address);
		}		
	}
	for(i=0;i<N;i++)
	{
		if(B[i].idtype==2)
		{
			printf("The current records are: %s %s\t%s\t%d\t%s\t%s \n",B[i].firstname,B[i].lastname,B[i].accommodationtype,B[i].idtype,B[i].idnumber.empcode,B[i].address);
		}		
	}
}
void search (struct accommodation B[],int N)
{
    char first_name[50], last_name[50];
    printf("Enter first name you want to search for: \n");
    scanf("%s",first_name);
    printf("Enter last name you want to search for: \n");
    scanf("%s",last_name);
    int i ;
    int found=0;
    for(i=0;i<N;i++)
    {
        if((!strcmp(B[i].firstname,first_name)) && (!strcmp(B[i].lastname,last_name)))
        {
            if(B[i].idtype==0)
			{
				printf("The result of your search is : %s %s\t%s\t%d\t%s\t%s \n",B[i].firstname,B[i].lastname,B[i].accommodationtype,B[i].idtype,B[i].idnumber.aadhaar,B[i].address);
				found=1;
			}					
			else if(B[i].idtype==1)
			{
				printf("The result of your search is : %s %s\t%s\t%d\t%s\t%s \n",B[i].firstname,B[i].lastname,B[i].accommodationtype,B[i].idtype,B[i].idnumber.passport,B[i].address);
				found=1;
			}
			else if(B[i].idtype==2)
			{	
				printf("The result of your search is : %s %s\t%s\t%d\t%s\t%s \n",B[i].firstname,B[i].lastname,B[i].accommodationtype,B[i].idtype,B[i].idnumber.empcode,B[i].address);
				found=1;
			}
        }		        
    }
    if(!found)
    {
    	printf("Not Found\n");
	}
}
void indexing(struct accommodation B[],int N,int index[])
{
	struct fullname 
	{
		char full_name[100];
    }first_last[N];
	
	for(int i=0;i<N;i++)
	{
		strcpy(first_last[i].full_name,B[i].firstname);
		strcat(first_last[i].full_name,B[i].lastname);
	}
	int i , j ,temp;
	for(i=1;i<N;i++)
	{ 
		temp=i;
		j=i-1;
		
		while((strcmp(first_last[i].full_name,first_last[j].full_name)<0) && (j>=0))
		{
			index[j+1]=index[j];
			j=j-1;			
		}
		index[j+1]=temp;		
	}
	printSortedRecords(B,N,index);	
}

void printSortedRecords(struct accommodation B[],int N,int index[])
{
	int temp;
	for(int i=0;i<N;i++)
	{
		temp=index[i];
		if(B[temp].idtype==0)
		{
			printf("%s %s\t%s\t%d\t%s\t%s \n",B[temp].firstname,B[temp].lastname,B[temp].accommodationtype,B[temp].idtype,B[temp].idnumber.aadhaar,B[temp].address);
		}
		else if(B[temp].idtype==1)
		{
			printf("%s %s\t%s\t%d\t%s\t%s \n",B[temp].firstname,B[temp].lastname,B[temp].accommodationtype,B[temp].idtype,B[temp].idnumber.passport,B[temp].address);
		}
		else if(B[temp].idtype==2)
		{
			printf("%s %s\t%s\t%d\t%s\t%s \n",B[temp].firstname,B[temp].lastname,B[temp].accommodationtype,B[temp].idtype,B[temp].idnumber.empcode,B[temp].address);
		}		
	}
}


int deleteRecord(struct accommodation B[],int N,int index[])
{
	int id_type;
	char id_aadhaar[15];
	char id_passport[15];
	char id_empcode[15];
	int status = SUCCESS;
	int i=0 , found=0 ;
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
	while((i<N) && !found)
	{
		if((id_type==0) && (!strcmp(B[i].idnumber.aadhaar,id_aadhaar)))
		{
			found=1;
		}
		else if((id_type==1) && (!strcmp(B[i].idnumber.passport,id_passport)))
		{
			found=1;
		}
		else if((id_type==2) && (!strcmp(B[i].idnumber.empcode,id_empcode)))
		{
			found=1;
		}
		else
		{
			i++;
		}		
	}
	if(found==1)
	{
		B[i].firstname[0]='\0';
		B[i].lastname[0]='\0';
		B[i].accommodationtype[0]='\0';	
		B[i].idtype=-1;
		B[i].idnumber.aadhaar[0]='\0';
		B[i].idnumber.passport[0]='\0';
		B[i].idnumber.empcode[0]='\0';
		B[i].address[0]='\0';
		indexing(B,SIZE,index);	
	}
	else
	{
		status=FAILURE;
	}
	return status;	
}

int updateRecord(struct accommodation B[],int N,int index[])
{
	int id_type;
	char id_aadhaar[15];
	char id_passport[15];
	char id_empcode[15];
	char first_name[50],last_name[50],accommodation_type[20],person_address[100];
	int status = SUCCESS;
	int i=0 , found=0 ;
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
	while((i<N) && !found)
	{
		if((id_type==0) && (!strcmp(B[i].idnumber.aadhaar,id_aadhaar)))
		{
			found=1;
		}
		else if((id_type==1) && (!strcmp(B[i].idnumber.passport,id_passport)))
		{
			found=1;
		}
		else if((id_type==2) && (!strcmp(B[i].idnumber.empcode,id_empcode)))
		{
			found=1;
		}
		else
		{
			i++;
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
			printf("Enter First name :\n");
			scanf("%s",first_name);
			strcpy(B[i].firstname,first_name);
		}
		else if(decider==2)
		{
			printf("Enter last name :\n");
			scanf("%s",last_name);
			strcpy(B[i].lastname,last_name);
		}
		else if(decider==3)
		{
			printf("Enter accommodation type:\n");
			scanf("%s",accommodation_type);
			strcpy(B[i].accommodationtype,accommodation_type);
		}
		else if(decider==4)
		{
			printf("Enter person address :\n");
			scanf("%s",person_address);
			strcpy(B[i].address,person_address);
		}
	}
	else
	{
		status=FAILURE;
		indexing(B,SIZE,index);	
	}
	
	return status;
}

void more_on_accommodation_type (struct accommodation B[],int N)
{
	struct fill_accommodation_type
	{
		char typeI[10];
		char typeII[10];
		char typeIII[10];
		char typeIV[10];
	}accommodation_type;
	int i;
	//Initialising with zero.
	char str[11]="0000000000";
	for(i=0;i<N;i++)
	{
		strcpy(accommodation_type.typeI,str);
		strcpy(accommodation_type.typeII,str);	
		strcpy(accommodation_type.typeIII,str);
		strcpy(accommodation_type.typeIV,str);	
	}
	char str1[10]="Type-I";
	char str2[10]="Type-II";
	char str3[10]="Type-III";
	char str4[10]="Type-IV";
	
	int j=0,k=0,l=0,m=0;
	for(i=0;i<N;i++)
	{
		if(!strcmp(B[i].accommodationtype,str1))
		{
			accommodation_type.typeI[j++]==1;
		}
		else if(!strcmp(B[i].accommodationtype,str2))
		{
			accommodation_type.typeII[k++]==1;
		}
		else if(!strcmp(B[i].accommodationtype,str3))
		{
			accommodation_type.typeIII[l++]==1;
		}
		else if(!strcmp(B[i].accommodationtype,str4))
		{
			accommodation_type.typeIV[m++]==1;
		}
	}
	char requested_accommo_type[20];
	printf("Enter the accommodation type which you wish to request for allocation :\n");
	scanf("%s",requested_accommo_type);
	
	int status=SUCCESS;
	
	if(!strcmp(requested_accommo_type,str1))
	{
		int i=0,found=0;
		while((i>=0) && (!found))
		{
			if(accommodation_type.typeI[i]==0)
			{
				found=1;
			}
			else
			{
				i++;
			}
		}
		if(found==1)
		{
			strcpy(B[i].accommodationtype,str1);
		}
		else
		{
			status=FAILURE;
		}
	}
	else if(!strcmp(requested_accommo_type,str2))
	{
		int i=0,found=0;
		while((i<10) && (!found))
		{
			if(accommodation_type.typeII[i]==0)
			{
				found=1;
			}
			else
			{
				i++;
			}
		}
		if(found==1)
		{
			strcpy(B[i].accommodationtype,str2);
		}
		else
		{
			status=FAILURE;
		}
	}
	else if(!strcmp(requested_accommo_type,str3))
	{
		int i=0,found=0;
		while((i<10) && (!found))
		{
			if(accommodation_type.typeIII[i]==0)
			{
				found=1;
			}
			else
			{
				i++;
			}
		}
		if(found==1)
		{
			strcpy(B[i].accommodationtype,str3);
		}
		else
		{
			status=FAILURE;
		}
	}
	else if(!strcmp(requested_accommo_type,str4))
	{
		int i=0,found=0;
		while((i<10) && (!found))
		{
			if(accommodation_type.typeIV[i]==0)
			{
				found=1;
			}
			else
			{
				i++;
			}
		}
		if(found==1)
		{
			strcpy(B[i].accommodationtype,str4);			
		}
		else
		{
			status=FAILURE;
		}
			
	}
	int flag3 = status ;
	if(flag3)
	{
		printf("This type is available to be allocated\n");
	}
	else
	{
		printf("Not Available\n");
	}
}
	

