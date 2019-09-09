/*===========================
Library Book Finder / Tracker
===========================*/

/*=================================================
=================== DIRECTIVES ====================
=================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define booksFile "books.txt"			// These files are tab-delimited
#define borrowersFile "borrowers.txt"	// and can be opened in Excel





/*=================================================
==================== VARIABLES ====================
=================================================*/

/*** For choosing an action ***/
int choice;


/*** For Loops ***/
int i = 0;


/*** For student's details ***/
struct Student_Details {
	char name[50], date[20];
	int idnum;
};

struct Student_Details student;


/*** For book's details ***/
struct Book_Details {
	char title[50], genres[50], author[50], pubDate[20];
	int quantity;
};

struct Book_Details books[100];		// Stores the books listed on the books.txt file


/*** For file I/O ***/
FILE *fp; 							// Declare file pointer


/*** For searching books ***/
char books_found[100]; 				// Stores the index of the books that match the search
char *ptr; 							// Used in comparing strings
int num_found = 0, books_size = 0;	// Number of books that match the search and the size of the books struct





/*=================================================
===================== FUNCTIONS ===================
=================================================*/

/*==================
CAPITALIZE A STRING
==================*/
void upper_string( char s[] ) {
   int c = 0;	// Character index/position in string
   
   while (s[c] != '\0') {				// Loop while it's not the end of the string
      if (s[c] >= 'a' && s[c] <= 'z') {	// Check if character is in lower case using decimal ASCII code
         s[c] = s[c] - 32;				// Capitalize character by subtracting 32 to its decimal ASCII code
      }
      c++;		// Increment to go to next character
   }
}



/*===================
DISPLAY BOOK DETAILS
===================*/
void displayBook( struct Book_Details book ) {
	printf(" | Title: %s\n", book.title);
	printf(" | Genres: %s\n", book.genres);
	printf(" | Author: %s\n", book.author);
	printf(" | Publication Date: %s\n", book.pubDate);
	printf(" | Quantity: %d\n", book.quantity);
}



/*==============================
STORE READ LINE IN BOOKS STRUCT
==============================*/
void lineToStruct( int lineNum, char line[] ) {
	
	/*** Get the book details by splitting the line based on delimiter "\t" (tab) ***/
	char *token;	// to be used for strtok() to split the line
					// strtok()
	
	/*** First split/row: Title ***/
    token = strtok(line, "\t"); 
    upper_string(token);
    strcpy(books[lineNum].title, token);
    
    /*** Second split/row: Genres ***/
    token = strtok(NULL, "\t"); 
    upper_string(token);
    strcpy(books[lineNum].genres, token);
    
    /*** Third split/row: Author ***/
	token = strtok(NULL, "\t");
    upper_string(token);
    strcpy(books[lineNum].author, token);
    
    /*** Fourth split/row: Publication Date ***/
	token = strtok(NULL, "\t");
    strcpy(books[lineNum].pubDate, token);
    
    /*** Fifth split/row: Quantity ***/
	token = strtok(NULL, "\t"); 
    books[lineNum].quantity = atoi(token);
       
    //displayBook(books[lineNum]);
}



/*==============
READ BOOKS FILE
==============*/
void readFile() {
	printf("Reading database... \n");
	
	int lineNum = 0; 	// indicates the row number of data, also called index
	char line[1000];	// holds the line read from file

	fp = fopen(booksFile, "r"); 		// open and read("r") file
	fgets ( line, sizeof line, fp ); 	// read first line and do not store because it contains the headers
	
	/*** Read the consequent lines to store the book details in the books[] struct ***/
	while ( fgets ( line, sizeof line, fp ) != NULL ) {
		lineToStruct(lineNum, line); 	// Calls the function to store read line to struct
	    lineNum++;						// Increment to be able to read the next line of data in file
	}
		
	fclose(fp);		// Close file
	
	books_size = sizeof(books)/sizeof(books[0]);	// Compute for the size of books[] struct to be used for loops in seaarching part
}



/*==============
UPDATE BOOKS FILE
==============*/
void updateFile(int book_chose){
	printf("\nUpdating database...\n");
	
	/*** Update the BOOKS file ***/
	fp = fopen(booksFile, "w"); 										// Open to write ("w") on file
	fprintf(fp, "Title\tGenres\tAuthor\tPublication Date\tQuantity\n");	// Write the headers on the first line
	
	/* Loop through the elements books[] struct to append it in the file */
	i = 0;
	while ( (i < books_size) && 				// Enables looping on the books[] struct
			(strlen(books[i].title) > 0) ) {	// Checks if the current books[] item is not null 
		fprintf(fp, "%s\t%s\t%s\t%s\t%d\n", 	// Add line containing the book details
					books[i].title, books[i].genres, books[i].author, books[i].pubDate, books[i].quantity);	
		i++;	// Increment to get the next item in the books[] struct
	}
		
	fclose(fp);	// Close the books file
	
	
	/*** Create/Update the BORROWERS file ***/
	if (!(fp = fopen(borrowersFile, "r"))) { // Checks if file does not exist. If it does not, create the file
		fclose(fp);		
		
		printf("Creating a file named %s...\n", borrowersFile);
		fp = fopen(borrowersFile, "w"); 	// Create the file, then write("w") the header on it
		fprintf(fp, "Title\tGenres\tAuthor\tPublication Date\tBorrower's Name\tBorrower's ID Number\tBorrow Date\n"); // Add header
		fclose(fp);							// Close file 
	}
	
	/* If the borrowers file exists or is created, add the borrow details */
	fp = fopen(borrowersFile, "a"); 				// Open file to append("a") entry
    
    upper_string(books[book_chose].title); 			// Calls the upper_string() function to capitalize the characters within the string
    upper_string(books[book_chose].genres);
    upper_string(books[book_chose].author);
    upper_string(student.name);
    
    fprintf(fp, "%s\t%s\t%s\t%s\t%s\t%d\t%s\n",  				// Finally, add/append the borrow details to the file
			books[book_chose].title, books[book_chose].genres, 	// book_chose is the index of the book chose based on the books_found[]
			books[book_chose].author, books[book_chose].pubDate,
			student.name, student.idnum, student.date
			);
    fclose(fp);										// Close the file
}



/*================
SEARCH FOR BOOK/S
================*/
void genreMenu();		// Initialize function for genre menu 
void searchBook( const char* genre ){
	readFile(); 		// Call function to read the books.txt file first and store its contents in the books struct
	
	printf("\nSearching for %s books...\n", genre);
	
	num_found = 0; 		// Initialize the number of books found to 0 before searching
	
	/*** Search for the book in the books struct ***/
	for (i = 0; i < books_size; i++) {
		ptr = strstr(books[i].genres, genre);// strstr('a', 'b') compares strings 'a' & 'b'; NULL if string 'a' does not contain string 'b
		if ( (ptr != NULL) && 				// If book[i].genres contains the search term,
			 (books[i].quantity > 0) ) { 	// and if there are available copies, 
			books_found[num_found] = i;		// add the book's index in the books_found array
			num_found++;					// Increment num_found to track how many matching books are found
		}
	}
	
	/*** If there are available books that match the search, display those books and let user choose; else, go back to genre menu ***/
	if (num_found > 0) { 					
		printf("\n-----------------------------------------------------\n");
		printf("Book/s available: %d", num_found);	// Print how many available boosk found
		printf("\n-----------------------------------------------------\n");
		
		/* Display the results by looping on the books_found array and using displayBook() function */
		for (i = 0; i < num_found; i++) {
			printf("[%d]\n", i);
			displayBook(books[ books_found[i] ]); 	// Call the displayBook() function to print the book details
			printf("\n-----------------------------------------------------\n");
		}		
		
		/* Ask user to enter which book to borrow among the available books */
		printf("|~ What book will you borrow? ");
		scanf("%d", &choice);
		
		/* Check if the entered number is among the books found */
		if ( (choice >= 0) && (choice < num_found) ) {	// If the entered number is within the length of the books_found array 
			int index = books_found[choice];			// Get the index of the chosen book
			
			printf("\nThe book you want to borrow: %s by %s\n", books[index].title,  books[index].author);
						
			books[index].quantity--; 	// Decrement the quantity of the chosen book
			updateFile(index); 			// Call function to update the books file
			
		/* If invalid input or not in books found, go back to genre menu */
		} else { 
			printf("\nInvalid input. \nGoing back to Genre menu...\n");
			system("pause"); 	// "Press any key to continue..."
			genreMenu(); 		// Return to genre menu
		}
	
	/* If there are no available books that match the search */
	} else {
		printf("\nNo more available copies. \nGoing back to Genre menu...\n");
		system("pause"); 	// "Press any key to continue..."
		genreMenu(); 		// Return to genre menu
	}
}



/*===========
ENTER GENRE
===========*/
int main();
void genreMenu() {
	
	system("cls"); 	// clearscreen
	
	printf("\n\n");
	printf("=======================================================================\n");
	printf("=================    LIBRARY BOOK FINDER / TRACKER    =================\n");
	printf("=======================================================================\n");
	printf("\n\n");
		
	/*** Ask the user to enter preferred genre of book ***/
	printf("GENRES:\n");
	printf("| [1] Adventure\n");
	printf("| [2] Fantasy\n");
	printf("| [3] Mystery\n");
	printf("| [4] Romance\n");
	printf("| [5] Young Adult\n");
	printf("|\n|~ Enter the genre you are interested in: ");
	scanf("%d", &choice);
	
	system("cls"); 	// clearscreen
	
	printf("\n\n");
	printf("=======================================================================\n");
	printf("=================    LIBRARY BOOK FINDER / TRACKER    =================\n");
	printf("=======================================================================\n");
	printf("\n\n");
	
	switch(choice) {
		/*** Adventure ***/
		case 1:						
			searchBook("ADVENTURE"); 	// Call searchBook() function to search book with the chosen genre
			break;			
		
		/*** Fantasy ***/
		case 2:			
			searchBook("FANTASY"); 		// Call searchBook() function to search book with the chosen genre
			break;			
		
		/*** Mystery ***/
		case 3:			
			searchBook("MYSTERY"); 		// Call searchBook function to search book with the chosen genre
			break;			
		
		/*** Romance ***/
		case 4:			
			searchBook("ROMANCE"); 		// Call searchBook() function to search book with the chosen genre
			break;			
		
		/*** Young Adult ***/
		case 5:			
			searchBook("YOUNG ADULT"); 	// Call searchBook() function to search book with the chosen genre
			break;			
		
		/*** Invalid Input ***/
		default:			
			printf("Invalid input. Try again.\n");
			system("pause");			// "Press any key to continue..."
			genreMenu();				// Go back to genre menu
			break;
	}
	
	printf("\nGoing back to main...\n");
	system("pause"); 	// "Press any key to continue..."
	main(); 			// Reinitialize int main()
}
			




/*=================================================
======================= MAIN ======================
=================================================*/

int main() {
	
	system("cls"); // clearscreen
	
	printf("\n\n");
	printf("=======================================================================\n");
	printf("=================    LIBRARY BOOK FINDER / TRACKER    =================\n");
	printf("=======================================================================\n");
	printf("\n\n");
	
	
	/*** Ask user to enter name, ID number and current date ***/
	printf("|~ Enter your name: ");
	scanf("%s", student.name);
	printf("|~ Enter your ID number: ");
	scanf("%d", &student.idnum);
	printf("|~ Enter the date [dd-mm-yyyy]: ");
	scanf("%s", student.date);
			
	genreMenu();	// Go to the genre menu
		
	return 0;
}
