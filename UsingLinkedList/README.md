
# Accommodation Management System using Linked List

    A C Project for maintaining Resident Record.
  
The following structure defines the accommodation records maintained for all residents of VNIT campus.
  
struct accommodation  
{  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;char firstname[50];  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;char lastname[50];  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;char accommodation_type [20];  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;int idtype;  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;struct idnum  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;char aadhaar[15];  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;char passport[15];  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;char empcode [15];  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;};  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;char address[100];  
}  

The field idtype takes a value 0, 1, or 2, indicating whether the ID is Aadhaar Card number, Passport number, or Employee code respectively.

(a) Write a function, InsertRecord(), which reads a record from user and insert into a linked list of structures. The records with idtype 0 must be stored first, followed by records with idtype 1 and 2. This order must be preserved throughout.  
Example records:  

&nbsp;&nbsp;Suman Dey Type-II 2 2022513 Formal methods lab, VNIT, Nagpur.
&nbsp;&nbsp;Pramod Gupta Type-I 1 ZF351076 Dept. of CSE, VNIT, Nagpur.

&nbsp;&nbsp;After reading idnum you must treat the rest of the line as the address. The other fields of idnum should be null.

(b) Write a function removeDuplicates() to remove the duplicate entries from the records.

(c) Write a function, printRecords(), which prints the records in linked list so that all those who have provided Aadhar number are listed first, all those who have provided Passport number are listed next, and all those who have provided Employee code are listed at the end.

(d) We wish to print the records in LL in alphabetic order of names (sorted by firstname and ties resolved by lastname). Instead of sorting the records in LL, we will create a thing called index. For this purpose, create a LL, index, of N nodes, and then for each k, 0 ≤ k < N, populate kth node with the index of the record containing the kth name in alphabetic order. Write a function printSortedRecords() that uses the index LL, to print the records in alphabetic order of names.

(e) Write a search() function that prints the whole information for a given Employee name. If there are multiple records with same name then print them in alphabetic order.

(f) Write a function deleteRecord() to delete the record of a given employee, the input to this function should be idtype and idnum. Make sure to change the index LL after deletion.

(g) Write the updateRecord() function to update the record of an employee. The field to update should be the user input. Make sure to change the index LL after updating firstname or lastname.

(h) Create an additional structure having 10 blocks of each type of accommodation. The types of accommodation are type-I, type-II, type-III, and type-IV. Mark the blocks as accommodated/not accommodated. Write a function specialRequestAllocation() to change a type of block requested by an employee depending on the availability of that type of block.