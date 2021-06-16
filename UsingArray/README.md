
# Accommodation Management System Using 1-D Arrays

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
## Problem Statement:- 

The field idtype takes a value 0, 1, or 2, indicating whether the ID is Aadhaar Card number, Passport number, or Employee code. 

(a) Write a function, InsertRecord(), which reads a record from user.
Example records:  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Suman Dey Type-II 2 2022513 Formal methods lab, VNIT, Nagpur.  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Pramod Gupta Type-I 1 ZF351076 Dept. of CSE, VNIT, Nagpur.  
After reading idnum you must treat the rest of the line as the address.  
The other fields of idnum should be null.  

(b) Write a function removeDuplicates() to remove the duplicate entries from the records. Duplicates are identified on the basis of similar idtypes and idnums .  

(c) Write a function, printRecords(), which prints the records in B so that all those who have provided passport numbers are listed first, all those who have provided Aadhaar card numbers are listed next, and all those who have provided Employee code are listed at the end.

(d) We wish to print the records in B in alphabetic order of names (sorted by firstname and ties resolved by lastname). Instead of sorting the records in B, we will create a thing called index. For this purpose, create an array, index, of N integers, and then for each k, 0 ≤ k < N, populate index[k] with the index of the record containing the kth name in alphabetic order. For example, the index for the records of part (a) will be as follows: index [0] = 1 index [1] = 0. Write a function printSortedRecords() that uses the arrayindex[], to print the records in alphabetic order of names.

(e) Write a search() function that prints the whole information for a given Employee name. If there are multiple records with same name then print them in alphabetic order.

(f) Write a function deleteRecord() to delete the record of a given employee, the input to this function should be idtype and idnum. Make sure to change the index array after deletion.

(g) Write the updateRecord() function to update the record of an employee. The field to update should be the user input. Make sure to change the index array after updating firstname or lastname.

(h) Create an additional structure having 10 blocks of each type of accommodation. The types of accommodation are type-I, type-II, type-III, and type-IV. Mark the blocks as accommodated/not accommodated. Write a function specialRequestAllocation() to change a type of block requested by an employee depending on the availability of that type of block.
  