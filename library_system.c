#include <stdio.h>
#include <string.h>
#include <time.h>
#define MAX_BOOKS 100
#define MAX_USERS 50
#define MAX_LOANS 200
#define LOAN_SECONDS (14*24*60*60)
#define FINE_PER_DAY 5

int main()
{
    int bookId[MAX_BOOKS];
    char bookTitle[MAX_BOOKS][50];
    int bookStock[MAX_BOOKS];
    int totalBooks=0;
    int userId[MAX_USERS];
    char userName[MAX_USERS][50];
    int totalUsers=0;
    int loanUser[MAX_LOANS]={0};
    int loanBook[MAX_LOANS]={0};
    time_t loanDue[MAX_LOANS];
    int totalLoans=0;
    int choice,r,ch,i,j;
    int uid,bid,stock;
    int u,b,slot;
    int exists,count,days,fine;
    char text[50];
    double late;
    time_t now;

    while(1){
        printf("\n===== LIBRARY MANAGEMENT SYSTEM =====\n");
        printf("1. Add a new book\n");
        printf("2. Register a user\n");
        printf("3. Show all books\n");
        printf("4. Search book by ID\n");
        printf("5. Issue a book\n");
        printf("6. Return a book\n");
        printf("7. User dashboard\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        r=scanf("%d",&choice);
        if(r==EOF){printf("\nGoodbye!\n");break;}
        while((ch=getchar())!='\n' && ch!=EOF);
        if(r!=1){printf("Please enter a number from 1 to 8.\n");continue;}

        switch(choice){
        case 1:
            if(totalBooks>=MAX_BOOKS){printf("Catalog is full.\n");break;}
            printf("Enter book ID: ");
            r=scanf("%d",&bid);
            while((ch=getchar())!='\n' && ch!=EOF);
            if(r!=1){printf("Book ID has to be a number.\n");break;}
            printf("Enter book title: ");
            r=scanf(" %49[^\n]",text);
            while((ch=getchar())!='\n' && ch!=EOF);
            if(r!=1){printf("Invalid title.\n");break;}
            printf("Enter stock: ");
            r=scanf("%d",&stock);
            while((ch=getchar())!='\n' && ch!=EOF);
            if(r!=1){printf("Stock has to be a number.\n");break;}
            if(bid<=0){printf("Book ID must be positive.\n");break;}
            exists=0;
            for(i=0;i<totalBooks;i++) if(bookId[i]==bid) exists=1;
            if(exists){printf("A book with ID %d already exists.\n",bid);break;}
            if(stock<0){printf("Stock can't be negative.\n");break;}
            bookId[totalBooks]=bid;
            strcpy(bookTitle[totalBooks],text);
            bookStock[totalBooks]=stock;
            totalBooks++;
            printf("Book added: ID %d, \"%s\", stock %d.\n",bid,text,stock);
            break;

        case 2:
            if(totalUsers>=MAX_USERS){printf("User list is full.\n");break;}
            printf("Enter user ID: ");
            r=scanf("%d",&uid);
            while((ch=getchar())!='\n' && ch!=EOF);
            if(r!=1){printf("User ID has to be a number.\n");break;}
            printf("Enter user name: ");
            r=scanf(" %49[^\n]",text);
            while((ch=getchar())!='\n' && ch!=EOF);
            if(r!=1){printf("Invalid name.\n");break;}
            if(uid<=0){printf("User ID must be positive.\n");break;}
            exists=0;
            for(i=0;i<totalUsers;i++) if(userId[i]==uid) exists=1;
            if(exists){printf("A user with ID %d already exists.\n",uid);break;}
            userId[totalUsers]=uid;
            strcpy(userName[totalUsers],text);
            totalUsers++;
            printf("User registered: ID %d, %s.\n",uid,text);
            break;

        case 3:
            if(totalBooks==0){printf("No books in the catalog yet.\n");break;}
            printf("%-8s %-49s %6s\n","ID","Title","Stock");
            printf("-----------------------------------------------------------------\n");
            for(i=0;i<totalBooks;i++){
                printf("%-8d %-49s %6d\n",bookId[i],bookTitle[i],bookStock[i]);
            }
            break;

        case 4:
            printf("Enter book ID to search: ");
            r=scanf("%d",&bid);
            while((ch=getchar())!='\n' && ch!=EOF);
            if(r!=1){printf("Book ID has to be a number.\n");break;}
            b=-1;
            for(i=0;i<totalBooks;i++){
                if(bookId[i]==bid){b=i;break;}
            }
            if(b==-1) printf("Book not found.\n");
            else{
                printf("Title: %s\n",bookTitle[b]);
                printf("Stock: %d\n",bookStock[b]);
            }
            break;

        case 5:
            printf("Enter user ID: ");
            r=scanf("%d",&uid);
            while((ch=getchar())!='\n' && ch!=EOF);
            if(r!=1){printf("User ID has to be a number.\n");break;}
            printf("Enter book ID: ");
            r=scanf("%d",&bid);
            while((ch=getchar())!='\n' && ch!=EOF);
            if(r!=1){printf("Book ID has to be a number.\n");break;}
            u=-1;
            for(i=0;i<totalUsers;i++){
                if(userId[i]==uid){u=i;break;}
            }
            if(u==-1){printf("User ID %d is not registered.\n",uid);break;}
            b=-1;
            for(i=0;i<totalBooks;i++){
                if(bookId[i]==bid){b=i;break;}
            }
            if(b==-1){printf("Book ID %d does not exist.\n",bid);break;}
            if(bookStock[b]<=0){printf("\"%s\" is out of stock.\n",bookTitle[b]);break;}
            exists=0;
            for(i=0;i<totalLoans;i++) if(loanUser[i]==uid && loanBook[i]==bid) exists=1;
            if(exists){printf("%s already has this book.\n",userName[u]);break;}
            slot=-1;
            for(i=0;i<totalLoans;i++){
                if(loanUser[i]==0){slot=i;break;}
            }
            if(slot==-1){
                if(totalLoans==MAX_LOANS){printf("Loan records are full, can't issue more books right now.\n");break;}
                slot=totalLoans;
                totalLoans++;
            }
            loanUser[slot]=uid;
            loanBook[slot]=bid;
            loanDue[slot]=time(NULL)+LOAN_SECONDS;
            bookStock[b]--;
            printf("Issued \"%s\" to %s.\n",bookTitle[b],userName[u]);
            printf("Due date: %s",ctime(&loanDue[slot]));
            break;

        case 6:
            printf("Enter user ID: ");
            r=scanf("%d",&uid);
            while((ch=getchar())!='\n' && ch!=EOF);
            if(r!=1){printf("User ID has to be a number.\n");break;}
            printf("Enter book ID: ");
            r=scanf("%d",&bid);
            while((ch=getchar())!='\n' && ch!=EOF);
            if(r!=1){printf("Book ID has to be a number.\n");break;}
            if(uid<=0 || bid<=0){printf("No active loan found for that user and book.\n");break;}
            slot=-1;
            for(i=0;i<totalLoans;i++){
                if(loanUser[i]==uid && loanBook[i]==bid){slot=i;break;}
            }
            if(slot==-1){printf("No active loan found for that user and book.\n");break;}
            now=time(NULL);
            loanUser[slot]=0;
            loanBook[slot]=0;
            for(i=0;i<totalBooks;i++){
                if(bookId[i]==bid){bookStock[i]++;break;}
            }
            printf("Book %d returned by user %d.\n",bid,uid);
            if(now>loanDue[slot]){
                late=difftime(now,loanDue[slot]);
                days=(int)(late/86400);
                if(late>days*86400) days++;
                fine=days*FINE_PER_DAY;
                printf("Days overdue: %d\n",days);
                printf("Fine: %d\n",fine);
            }
            else printf("Returned on time, no fine.\n");
            break;

        case 7:
            printf("Enter user ID: ");
            r=scanf("%d",&uid);
            while((ch=getchar())!='\n' && ch!=EOF);
            if(r!=1){printf("User ID has to be a number.\n");break;}
            u=-1;
            for(i=0;i<totalUsers;i++){
                if(userId[i]==uid){u=i;break;}
            }
            if(u==-1){printf("User ID %d is not registered.\n",uid);break;}
            printf("\nDashboard for %s (ID %d)\n",userName[u],uid);
            printf("------------------------------------------\n");
            now=time(NULL);
            count=0;
            for(i=0;i<totalLoans;i++){
                if(loanUser[i]!=uid) continue;
                b=-1;
                for(j=0;j<totalBooks;j++){
                    if(bookId[j]==loanBook[i]){b=j;break;}
                }
                if(now>loanDue[i]) printf("[OVERDUE] ");
                else printf("[ACTIVE]  ");
                printf("Book ID: %d | Title: %s | Due: %s",loanBook[i],bookTitle[b],ctime(&loanDue[i]));
                count++;
            }
            if(count==0) printf("No active loans.\n");
            break;

        case 8:
            printf("Thanks for using the Library Management System. Goodbye!\n");
            return 0;

        default:
            printf("Invalid choice, pick a number from 1 to 8.\n");
        }
    }
    return 0;
}
