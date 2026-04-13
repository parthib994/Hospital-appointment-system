 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



struct node {
    int token;
    char name[50];
    int type;
    struct node *next;
};

struct node *front = NULL, *rear = NULL;

struct stack {
    int token;
    char name[50];
    struct stack *next;
};

struct stack *top = NULL;

struct doctor {
    int token;
    char patientName[50];
    char doctorName[50];
    struct doctor *next;
};

struct doctor *dhead = NULL;



void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int isDuplicate(int token) {
    struct node *ptr = front;
    while(ptr != NULL) {
        if(ptr->token == token) return 1;
        ptr = ptr->next;
    }
    return 0;
}



void pushHistory(int token, char name[]) {
    struct stack *temp = (struct stack*)malloc(sizeof(struct stack));
    temp->token = token;
    strcpy(temp->name, name);
    temp->next = top;
    top = temp;
}

void showLastServed() {
    if(top == NULL) {
        printf("\n  No history available.\n");
        return;
    }
    printf("\n--- LAST SERVED PATIENT ---\n");
    printf("Name: %s | Token: %d\n", top->name, top->token);
}



void addPatient() {
    struct node *temp = (struct node*)malloc(sizeof(struct node));

    printf("Enter Patient Name: ");
    clearBuffer();
    fgets(temp->name, sizeof(temp->name), stdin);
    temp->name[strcspn(temp->name, "\n")] = '\0';

    printf("Enter Token Number: ");
    if (scanf("%d", &temp->token) != 1) {
        printf("\n  ERROR: Invalid input! Token must be a NUMBER.\n");
        clearBuffer();
        free(temp);
        return;
    }

    if(isDuplicate(temp->token)) {
        printf("  ERROR: Token %d already exists!\n", temp->token);
        free(temp);
        return;
    }


    printf("Type (1 = Emergency, 2 = Old, 3 = Normal): ");
    if (scanf("%d", &temp->type) != 1) {
        printf("\n  ERROR: Invalid type! Use numbers (1, 2, or 3).\n");
        clearBuffer();
        free(temp);
        return;
    }

    if(temp->type < 1 || temp->type > 3) {
    printf("  ERROR: Invalid Type! Patient not added.\n");
    free(temp);
    return;
}
    temp->next = NULL;

    //  Priority Insertion Logic
    if (front == NULL || temp->type < front->type) {
        temp->next = front;
        front = temp;
        if (rear == NULL) rear = temp;
    }
    else {
        struct node *ptr = front;
        while (ptr->next != NULL && ptr->next->type <= temp->type) {
            ptr = ptr->next;
        }
        temp->next = ptr->next;
        ptr->next = temp;
        if (temp->next == NULL) rear = temp;
    }

    printf("Patient '%s' Added Successfully!\n", temp->name);
}

void servePatient() {
    if(front == NULL) {
        printf("\n  List is empty.\n");
        return;
    }

    struct node *temp = front;
    printf("\n>>> SERVING: [%s] %s (Token %d)\n",
           (temp->type == 1 ? "EMERGENCY" : (temp->type == 2 ? "OLD" : "NORMAL")),
           temp->name, temp->token);

    pushHistory(temp->token, temp->name);

    front = front->next;
    if(front == NULL) rear = NULL;
    free(temp);
}

void displayPatients() {
    if(front == NULL) {
        printf("\n  No patients in the queue.\n");
        return;
    }

    struct node *ptr = front;
    printf("\n--- CURRENT PATIENT LIST ---\n");
    while(ptr != NULL) {
        printf("[%s] Name: %-15s | Token: %d\n",
               (ptr->type == 1 ? "EMERGENCY" : (ptr->type == 2 ? "OLD" : "NORMAL")),
               ptr->name, ptr->token);
        ptr = ptr->next;
    }
}



void assignDoctor() {
    int token;
    printf("Enter Patient Token to assign doctor: ");
    if (scanf("%d", &token) != 1) {
        printf("  Invalid Input!\n");
        clearBuffer();
        return;
    }

    struct node *p = front;
    int found = 0;
    char pName[50];

    while(p != NULL) {
        if(p->token == token) {
            found = 1;
            strcpy(pName, p->name);
            break;
        }
        p = p->next;
    }

    if(!found) {
        printf("  ERROR: Patient with Token %d not found!\n", token);
        return;
    }

    struct doctor *temp = (struct doctor*)malloc(sizeof(struct doctor));
    temp->token = token;
    strcpy(temp->patientName, pName);

    printf("Assigning Doctor for Patient: %s\n", pName);
    printf("Enter Doctor Name: ");
    clearBuffer();
    fgets(temp->doctorName, sizeof(temp->doctorName), stdin);
    temp->doctorName[strcspn(temp->doctorName, "\n")] = '\0';

    temp->next = dhead;
    dhead = temp;

    printf("Doctor Assigned Successfully!\n");
}

void displayDoctorList() {
    if(dhead == NULL) {
        printf("\n  No doctor assignments found.\n");
        return;
    }

    struct doctor *ptr = dhead;
    printf("\n--- ASSIGNED DOCTORS ---\n");
    while(ptr != NULL) {
        printf("Patient: %-15s (Token %d) -> Doctor: %s\n",
               ptr->patientName, ptr->token, ptr->doctorName);
        ptr = ptr->next;
    }
}



int main() {
    int choice;

    while(1) {
        printf("\n====================================");
        printf("\n  HOSPITAL APPOINTMENT SYSTEM");
        printf("\n====================================");
        printf("\n1. Add Patient");
        printf("\n2. Serve Next Patient");
        printf("\n3. View All Patients");
        printf("\n4. Assign Doctor");
        printf("\n5. View Assigned Doctor List");
        printf("\n6. Show Last Served  ");
        printf("\n7. Exit");
        printf("\n------------------------------------");
        printf("\nEnter Choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("\n  Invalid Choice! Please enter a number.\n");
            clearBuffer();
            continue;
        }

        switch(choice) {
            case 1: addPatient(); break;
            case 2: servePatient(); break;
            case 3: displayPatients(); break;
            case 4: assignDoctor(); break;
            case 5: displayDoctorList(); break;
            case 6: showLastServed(); break;
            case 7:printf("System closing. Stay healthy!\n");
                exit(0);
            default: printf(" Invalid Choice! Try again.\n");
        }
    }
    return 0;
}


