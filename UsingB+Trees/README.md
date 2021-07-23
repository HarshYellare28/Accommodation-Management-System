

# Accommodation Management System using B+ Trees

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
 
Create a database of all residents of VNIT campus using B+ tree.

The field idtype takes a value 0, 1, or 2, indicating whether the ID is Aadhaar Card number,
Passport number, or Employee code respectively.

(a) Write a function, InsertRecord(), which reads a record from user and insert into tree of
structures. The records with idtype 0 must be stored first, followed by records with idtype
1 and 2. This order must be preserved throughout.  

Example records:  

  Suman Dey Type-II 2 2022513 Formal methods lab, VNIT, Nagpur.
  Pramod Gupta Type-I 1 ZF351076 Dept. of CSE, VNIT, Nagpur.

After reading idnum you must treat the rest of the line as the address.
The other fields of idnum should be null.

(b) Write a function removeDuplicates() to remove the duplicate entries from the records.

(c) Write a function, printRecords(), which prints the records in tree so that all those who
have provided Aadhar number are listed first, all those who have provided Passport
number are listed next, and all those who have provided Employee code are listed at the
end.

(d) Write a search() function that prints the whole information for a given Employee name. If
there are multiple records with same name then print them in alphabetic order.

(e) Write a function deleteRecord() to delete the record of a given employee, the input to this
function should be idtype and idnum. Make sure to change the index tree after deletion.

(f) Write the updateRecord() function to update the record of an employee. The field to
update should be the user input. Make sure to change the index tree after updating
firstname or lastname.

(g) Create an additional structure having 10 blocks of each type of accommodation. The
types of accommodation are type-A, type-B, type-C, and type-D. These blocks should be
addressed with type, followed by serial number. For example, blocks in type-A will be
named as A1, A2, A3, A4, … & so on for other blocks also.Mark the blocks as
accommodated/not accommodated.

1. Write a function allocateBlock() to allocate block whenever the new resident record is
inserted.

2. Write a function deallocateBlock() to deallocate the block whenever the resident
record is deleted & mark that block as “not accommodated”.

3. Write a function addressSearch() to print the list of residents living in the given range
of addresses. (For e.g., B5 to B9, C1 to C8).

4. Write a function specialRequestAllocation() to change a type of block requested by an
employee depending on the availability of that type of block.
