#include <stdio.h>
#include <string.h>
#include <time.h>

int main(void)
{
    /* ---------- Database: books (parallel arrays) ---------- */
    int  book_ids[100] = {0};
    char book_titles[100][50] = {{0}};
    int  book_stock[100] = {0};

    /* ---------- Database: users ---------- */
    int  user_ids[50] = {0};
    char user_names[50][50] = {{0}};

    /* ---------- Database: transactions (ledger) ---------- */
    int    ledger_user_ids[200] = {0};   /* -1 means the slot is free / returned */
    int    ledger_book_ids[200] = {0};
    time_t ledger_issue_dates[200] = {0};
    time_t ledger_due_dates[200] = {0};

    /* ---------- Counters ---------- */
    int total_books = 0, total_users = 0, total_transactions = 0;

    /* ---------- Limits and configuration (keep in sync with array sizes) ---------- */
    int max_books = 100, max_users = 50, max_transactions = 200;
    int loan_period_seconds = 1209600;   /* 14 days; change temporarily to test fines */
    int fine_per_day = 5;                /* fixed fine rate per overdue day */

    /* ---------- Working variables ---------- */
    int menu_choice = 0;          /* option picked from the menu */
    int scan_result = 0;          /* return value of the latest scanf */
    int c = 0;                    /* used to discard the rest of an input line */
    int i = 0, j = 0;             /* loop counters */
    int input_user_id = 0;        /* user ID typed by the person */
    int input_book_id = 0;        /* book ID typed by the person */
    int input_stock = 0;          /* stock quantity typed by the person */
    char input_text[50] = {0};    /* temporary buffer for titles and names */
    int found_flag = 0;           /* 1 when a search succeeded */
    int found_index = -1;         /* index returned by a search */
    int user_index = -1;          /* array index of the matching user */
    int book_index = -1;          /* array index of the matching book */
    int title_index = -1;         /* array index used for title lookups */
    int ledger_slot = -1;         /* ledger slot chosen or matched */
    int loan_count = 0;           /* number of active loans on the dashboard */
    int days_overdue = 0;         /* whole days late, rounded up */
    int fine = 0;                 /* fine amount for a late return */
    double overdue_seconds = 0.0; /* seconds past the due date */
    time_t now_time = 0;          /* current time snapshot */
    time_t return_time = 0;       /* time at which a book is returned */

    /* Mark every ledger slot as free at start-up */
    for (i = 0; i < max_transactions; i++) {
        ledger_user_ids[i] = -1;
        ledger_book_ids[i] = -1;
    }

    /* ---------- Main menu loop ---------- */
    while (1) {
        printf("\n");
        printf("==========================================\n");
        printf("        LIBRARY MANAGEMENT SYSTEM\n");
        printf("==========================================\n");
        printf(" 1. Add a New Book\n");
        printf(" 2. Register a User\n");
        printf(" 3. Display Catalog\n");
        printf(" 4. Search Book by ID\n");
        printf(" 5. Issue a Book\n");
        printf(" 6. Return a Book\n");
        printf(" 7. User Dashboard\n");
        printf(" 8. Exit\n");
        printf("------------------------------------------\n");
        printf("Enter your choice: ");

        scan_result = scanf("%d", &menu_choice);
        if (scan_result == EOF) {
            printf("\n[INFO] End of input reached. Goodbye!\n");
            break;
        }
        if (scan_result != 1) {
            printf("[ERROR] Invalid input. Please enter a number from 1 to 8.\n");
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        while ((c = getchar()) != '\n' && c != EOF);

        if (menu_choice == 1) {
            /* ===== Feature 1: Add a New Book ===== */
            if (total_books >= max_books) {
                printf("[ERROR] Catalog full. Cannot add more books.\n");
                continue;
            }

            printf("Enter book ID (positive integer): ");
            scan_result = scanf("%d", &input_book_id);
            if (scan_result == EOF) {
                printf("\n[INFO] End of input reached. Goodbye!\n");
                break;
            }
            if (scan_result != 1) {
                printf("[ERROR] Invalid input. A number was expected.\n");
                while ((c = getchar()) != '\n' && c != EOF);
                continue;
            }
            while ((c = getchar()) != '\n' && c != EOF);

            printf("Enter book title: ");
            scan_result = scanf(" %49[^\n]", input_text);
            if (scan_result == EOF) {
                printf("\n[INFO] End of input reached. Goodbye!\n");
                break;
            }
            if (scan_result != 1) {
                printf("[ERROR] Invalid title.\n");
                while ((c = getchar()) != '\n' && c != EOF);
                continue;
            }
            while ((c = getchar()) != '\n' && c != EOF);

            printf("Enter stock quantity: ");
            scan_result = scanf("%d", &input_stock);
            if (scan_result == EOF) {
                printf("\n[INFO] End of input reached. Goodbye!\n");
                break;
            }
            if (scan_result != 1) {
                printf("[ERROR] Invalid input. A number was expected.\n");
                while ((c = getchar()) != '\n' && c != EOF);
                continue;
            }
            while ((c = getchar()) != '\n' && c != EOF);

            if (input_book_id <= 0) {
                printf("[ERROR] Book ID must be a positive integer.\n");
                continue;
            }

            /* Reject duplicate book IDs */
            found_flag = 0;
            for (i = 0; i < total_books; i++) {
                if (book_ids[i] == input_book_id) {
                    found_flag = 1;
                    break;
                }
            }
            if (found_flag == 1) {
                printf("[ERROR] A book with ID %d already exists.\n", input_book_id);
                continue;
            }

            if (input_stock < 0) {
                printf("[ERROR] Stock cannot be negative.\n");
                continue;
            }

            book_ids[total_books] = input_book_id;
            strcpy(book_titles[total_books], input_text);
            book_stock[total_books] = input_stock;
            total_books++;
            printf("[OK] Book added: ID %d, \"%s\", stock %d.\n", input_book_id, input_text, input_stock);

        } else if (menu_choice == 2) {
            /* ===== Feature 2: Register a User ===== */
            if (total_users >= max_users) {
                printf("[ERROR] User list full. Cannot register more users.\n");
                continue;
            }

            printf("Enter user ID (positive integer): ");
            scan_result = scanf("%d", &input_user_id);
            if (scan_result == EOF) {
                printf("\n[INFO] End of input reached. Goodbye!\n");
                break;
            }
            if (scan_result != 1) {
                printf("[ERROR] Invalid input. A number was expected.\n");
                while ((c = getchar()) != '\n' && c != EOF);
                continue;
            }
            while ((c = getchar()) != '\n' && c != EOF);

            printf("Enter user name: ");
            scan_result = scanf(" %49[^\n]", input_text);
            if (scan_result == EOF) {
                printf("\n[INFO] End of input reached. Goodbye!\n");
                break;
            }
            if (scan_result != 1) {
                printf("[ERROR] Invalid name.\n");
                while ((c = getchar()) != '\n' && c != EOF);
                continue;
            }
            while ((c = getchar()) != '\n' && c != EOF);

            if (input_user_id <= 0) {
                printf("[ERROR] User ID must be a positive integer.\n");
                continue;
            }

            /* Reject duplicate user IDs */
            found_flag = 0;
            for (i = 0; i < total_users; i++) {
                if (user_ids[i] == input_user_id) {
                    found_flag = 1;
                    break;
                }
            }
            if (found_flag == 1) {
                printf("[ERROR] A user with ID %d already exists.\n", input_user_id);
                continue;
            }

            user_ids[total_users] = input_user_id;
            strcpy(user_names[total_users], input_text);
            total_users++;
            printf("[OK] User registered: ID %d, %s.\n", input_user_id, input_text);

        } else if (menu_choice == 3) {
            /* ===== Feature 3: Display Catalog ===== */
            if (total_books == 0) {
                printf("[INFO] No books in the catalog.\n");
                continue;
            }
            printf("-----------------------------------------------------------------\n");
            printf("%-8s %-49s %6s\n", "ID", "Title", "Stock");
            printf("-----------------------------------------------------------------\n");
            for (i = 0; i < total_books; i++) {
                printf("%-8d %-49s %6d\n", book_ids[i], book_titles[i], book_stock[i]);
            }
            printf("-----------------------------------------------------------------\n");

        } else if (menu_choice == 4) {
            /* ===== Feature 4: Search Book by ID ===== */
            printf("Enter book ID to search: ");
            scan_result = scanf("%d", &input_book_id);
            if (scan_result == EOF) {
                printf("\n[INFO] End of input reached. Goodbye!\n");
                break;
            }
            if (scan_result != 1) {
                printf("[ERROR] Invalid input. A number was expected.\n");
                while ((c = getchar()) != '\n' && c != EOF);
                continue;
            }
            while ((c = getchar()) != '\n' && c != EOF);

            found_flag = 0;
            found_index = -1;
            for (i = 0; i < total_books; i++) {
                if (book_ids[i] == input_book_id) {
                    found_flag = 1;
                    found_index = i;
                    break;
                }
            }
            if (found_flag == 1) {
                printf("[OK] Book found.\n");
                printf("     Title: %s\n", book_titles[found_index]);
                printf("     Stock: %d\n", book_stock[found_index]);
            } else {
                printf("[INFO] Book not found.\n");
            }

        } else if (menu_choice == 5) {
            /* ===== Feature 5: Issue a Book (Checkout) ===== */
            printf("Enter user ID: ");
            scan_result = scanf("%d", &input_user_id);
            if (scan_result == EOF) {
                printf("\n[INFO] End of input reached. Goodbye!\n");
                break;
            }
            if (scan_result != 1) {
                printf("[ERROR] Invalid input. A number was expected.\n");
                while ((c = getchar()) != '\n' && c != EOF);
                continue;
            }
            while ((c = getchar()) != '\n' && c != EOF);

            printf("Enter book ID: ");
            scan_result = scanf("%d", &input_book_id);
            if (scan_result == EOF) {
                printf("\n[INFO] End of input reached. Goodbye!\n");
                break;
            }
            if (scan_result != 1) {
                printf("[ERROR] Invalid input. A number was expected.\n");
                while ((c = getchar()) != '\n' && c != EOF);
                continue;
            }
            while ((c = getchar()) != '\n' && c != EOF);

            /* Find the user */
            found_flag = 0;
            user_index = -1;
            for (i = 0; i < total_users; i++) {
                if (user_ids[i] == input_user_id) {
                    found_flag = 1;
                    user_index = i;
                    break;
                }
            }
            if (found_flag == 0) {
                printf("[ERROR] User ID %d is not registered.\n", input_user_id);
                continue;
            }

            /* Find the book */
            found_flag = 0;
            book_index = -1;
            for (i = 0; i < total_books; i++) {
                if (book_ids[i] == input_book_id) {
                    found_flag = 1;
                    book_index = i;
                    break;
                }
            }
            if (found_flag == 0) {
                printf("[ERROR] Book ID %d does not exist.\n", input_book_id);
                continue;
            }

            if (book_stock[book_index] <= 0) {
                printf("[ERROR] \"%s\" is out of stock.\n", book_titles[book_index]);
                continue;
            }

            /* Reject if this user already holds an active loan of this book */
            found_flag = 0;
            for (i = 0; i < total_transactions; i++) {
                if (ledger_user_ids[i] == input_user_id && ledger_book_ids[i] == input_book_id) {
                    found_flag = 1;
                    break;
                }
            }
            if (found_flag == 1) {
                printf("[ERROR] %s already has an active loan of this book.\n", user_names[user_index]);
                continue;
            }

            /* Choose a ledger slot: reuse a free one, otherwise append */
            ledger_slot = -1;
            for (i = 0; i < total_transactions; i++) {
                if (ledger_user_ids[i] == -1) {
                    ledger_slot = i;
                    break;
                }
            }
            if (ledger_slot == -1) {
                if (total_transactions < max_transactions) {
                    ledger_slot = total_transactions;
                    total_transactions++;
                } else {
                    printf("[ERROR] Ledger full. Cannot issue more books right now.\n");
                    continue;
                }
            }

            /* Record the loan */
            ledger_user_ids[ledger_slot] = input_user_id;
            ledger_book_ids[ledger_slot] = input_book_id;
            ledger_issue_dates[ledger_slot] = time(NULL);
            ledger_due_dates[ledger_slot] = ledger_issue_dates[ledger_slot] + loan_period_seconds;
            book_stock[book_index]--;

            printf("[OK] Issued \"%s\" to %s.\n", book_titles[book_index], user_names[user_index]);
            printf("     Due date: %s", ctime(&ledger_due_dates[ledger_slot]));

        } else if (menu_choice == 6) {
            /* ===== Feature 6: Return a Book ===== */
            printf("Enter user ID: ");
            scan_result = scanf("%d", &input_user_id);
            if (scan_result == EOF) {
                printf("\n[INFO] End of input reached. Goodbye!\n");
                break;
            }
            if (scan_result != 1) {
                printf("[ERROR] Invalid input. A number was expected.\n");
                while ((c = getchar()) != '\n' && c != EOF);
                continue;
            }
            while ((c = getchar()) != '\n' && c != EOF);

            printf("Enter book ID: ");
            scan_result = scanf("%d", &input_book_id);
            if (scan_result == EOF) {
                printf("\n[INFO] End of input reached. Goodbye!\n");
                break;
            }
            if (scan_result != 1) {
                printf("[ERROR] Invalid input. A number was expected.\n");
                while ((c = getchar()) != '\n' && c != EOF);
                continue;
            }
            while ((c = getchar()) != '\n' && c != EOF);

            /* IDs are always positive, so -1 must never match a free slot */
            if (input_user_id <= 0 || input_book_id <= 0) {
                printf("[ERROR] No active loan found for this user and book.\n");
                continue;
            }

            /* Find the first active matching loan */
            found_flag = 0;
            ledger_slot = -1;
            for (i = 0; i < total_transactions; i++) {
                if (ledger_user_ids[i] == input_user_id && ledger_book_ids[i] == input_book_id) {
                    found_flag = 1;
                    ledger_slot = i;
                    break;
                }
            }
            if (found_flag == 0) {
                printf("[ERROR] No active loan found for this user and book.\n");
                continue;
            }

            return_time = time(NULL);

            /* Free the ledger slot */
            ledger_user_ids[ledger_slot] = -1;
            ledger_book_ids[ledger_slot] = -1;

            /* Put the book back in stock */
            for (i = 0; i < total_books; i++) {
                if (book_ids[i] == input_book_id) {
                    book_stock[i]++;
                    book_index = i;
                    break;
                }
            }
            printf("[OK] Book ID %d returned by user ID %d.\n", input_book_id, input_user_id);

            /* Fine calculation: every started day counts */
            if (return_time > ledger_due_dates[ledger_slot]) {
                overdue_seconds = difftime(return_time, ledger_due_dates[ledger_slot]);
                days_overdue = (int)(overdue_seconds / 86400);
                if (overdue_seconds - days_overdue * 86400.0 > 0) {
                    days_overdue = days_overdue + 1;
                }
                fine = days_overdue * fine_per_day;
                printf("[INFO] Days overdue: %d\n", days_overdue);
                printf("[INFO] Fine due: %d\n", fine);
            } else {
                printf("[INFO] Returned on time. No fine.\n");
            }

        } else if (menu_choice == 7) {
            /* ===== Feature 7: User Dashboard ===== */
            printf("Enter user ID: ");
            scan_result = scanf("%d", &input_user_id);
            if (scan_result == EOF) {
                printf("\n[INFO] End of input reached. Goodbye!\n");
                break;
            }
            if (scan_result != 1) {
                printf("[ERROR] Invalid input. A number was expected.\n");
                while ((c = getchar()) != '\n' && c != EOF);
                continue;
            }
            while ((c = getchar()) != '\n' && c != EOF);

            found_flag = 0;
            user_index = -1;
            for (i = 0; i < total_users; i++) {
                if (user_ids[i] == input_user_id) {
                    found_flag = 1;
                    user_index = i;
                    break;
                }
            }
            if (found_flag == 0) {
                printf("[ERROR] User ID %d is not registered.\n", input_user_id);
                continue;
            }

            printf("------------------------------------------\n");
            printf("Dashboard for: %s (ID %d)\n", user_names[user_index], input_user_id);
            printf("------------------------------------------\n");

            now_time = time(NULL);
            loan_count = 0;
            for (i = 0; i < total_transactions; i++) {
                if (ledger_user_ids[i] == input_user_id) {
                    /* Look up the title of the borrowed book */
                    title_index = -1;
                    for (j = 0; j < total_books; j++) {
                        if (book_ids[j] == ledger_book_ids[i]) {
                            title_index = j;
                            break;
                        }
                    }
                    if (now_time > ledger_due_dates[i]) {
                        printf("[OVERDUE] ");
                    } else {
                        printf("[ACTIVE]  ");
                    }
                    if (title_index >= 0) {
                        printf("Book ID: %d | Title: %s | Due: %s", ledger_book_ids[i],
                               book_titles[title_index], ctime(&ledger_due_dates[i]));
                    } else {
                        printf("Book ID: %d | Title: (unknown) | Due: %s", ledger_book_ids[i],
                               ctime(&ledger_due_dates[i]));
                    }
                    loan_count++;
                }
            }
            if (loan_count == 0) {
                printf("[INFO] No active loans.\n");
            }

        } else if (menu_choice == 8) {
            /* ===== Feature 8: Exit ===== */
            printf("[INFO] Thank you for using the Library Management System. Goodbye!\n");
            break;

        } else {
            /* ===== Invalid menu choice ===== */
            printf("[ERROR] Invalid choice. Please select a number from 1 to 8.\n");
        }
    }

    return 0;
}