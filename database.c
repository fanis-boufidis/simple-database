#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define years 4

typedef struct{
    int id;
    char name[40];
    int year;
    int is_deleted;
    int subjects;
}studentT;

bool AddStudent(FILE *fDatabase, studentT students[], int *N);
bool DeleteStudent(FILE *fDatabase, studentT students[], int *N);
int FindStudent(FILE *fDatabase, studentT students[], int *N,int *index_found);
bool AddGPA(FILE *fDatabase, studentT students[]);

int main()
{
    int N = 0, answer;
    int ch, i, index_found, found; 
    char junk[256], subjects[100][30];
    bool added;

    FILE *fDatabase, *fSubjects;
    studentT students[1000];

    fDatabase = fopen("StudentsInformations.txt", "r");
    if(fDatabase != NULL)
    {  
        if (fgets(junk, sizeof(junk), fDatabase) != NULL) {
        // 2. Διαβάζουμε και "πετάμε" τη 2η γραμμή (Παύλες)
        fgets(junk, sizeof(junk), fDatabase);
        }

        i=0;
        //Get in the struct students the txt file
        while(fscanf(fDatabase, "%d %s %d %d", &students[i].id, students[i].name, &students[i].year, &students[i].is_deleted) == 4)
        {
            i++;
        }
        N = i;

        fclose(fDatabase);   
    }

    
    if(N == 0){
        fDatabase = fopen("StudentsInformations.txt", "w");
        fprintf(fDatabase,"%-15s %-15s %-15s %-15s\n", "ID", "NAME", "ENTRY-YEAR", "IS DELETED(YES=1, NO=0)");
        fprintf(fDatabase,"-----------------------------------------------------------------------------------\n");
        fclose(fDatabase); 
    }                                 

    printf("Τι θέλετε να κάνεται;\n1.Προσθήκη φοιτητή\n2.Διαγραφή φοιτητή\n3.Αναζήτηση φοιτητή\n4.Εισαγωγή βαθμού\n5.Τέλος διεργασίας ");
    scanf("%d", &answer);
    while (answer != 5)
    {
        if(answer == 1){
            added = AddStudent(fDatabase,students, &N);
            if(added==true){
                printf("Ο φοιτητής προστέθηκε με επιτυχία\n\n");
            }
            else{
                printf("Σφάλμα, αποτυχία προσθήκης φοιτητή\n\n");
            }
        }
        else if (answer == 2){
            bool deleted = DeleteStudent(fDatabase,students, &N);
            if(deleted==true){
                printf("Ο φοιτητής διαγράφηκε με επιτυχία\n\n");
            }
            else{
                printf("Σφάλμα, αποτυχία διαγραφής φοιτητή ή ήδη διεγραμμένος φοιτητής\n\n");
            }
        }
        else if(answer == 3){
            found = FindStudent(fDatabase,students, &N, &index_found);
            if(found == 0){
                printf("Ο φοιτητής δεν βρέθηκε \n\n");
            }
            else if(found == 1){
                printf("Ο φοιτητής βρέθηκε: \n");
                printf("%-10s %-10s %-10s\n", "ID", "NAME", "YEAR");
                printf("--------------------------\n");
                printf("%-10d %-10s %-10d\n", students[index_found].id, students[index_found].name, students[index_found].year);
            }
            else if(found == 2){
                printf("Ο φοιτητής διαγράφηκε: \n\n");
            }else{
                printf("Σφάλμα\n\n");
            }
        }
        else if(answer == 4){
            AddGPA(fDatabase, students);
        }
        else{
            break;
        }
        printf("Τι θέλετε να κάνεται;\n1.Προσθήκη φοιτητή\n2.Διαγραφή φοιτητή\n3.Αναζήτηση φοιτητή\n4.Τέλος διεργασίας ");
        scanf("%d", &answer);
    }
    return 0;
}

//ADD STUDENT
bool AddStudent(FILE *fDatabase, studentT students[], int *N){
    bool added = true;
    int i, targetIndex = *N;

    for(i=0; i<*N; i++)
    {
        if(students[i].is_deleted == 1){
                targetIndex = i;
                break;
        }
    }

    printf("Παρακαλώ δώστε τα εξής στοιχεία:\n");

    printf("Επίθετο: ");
    scanf("%s", students[targetIndex].name);
    

    printf("Id: ");
    scanf("%d", &students[targetIndex].id);

    printf("Year: ");
    scanf("%d", &students[targetIndex].year);

    students[targetIndex].is_deleted = 0;
    if(targetIndex == *N){
        (*N)++; 
    }

    fDatabase = fopen("StudentsInformations.txt", "w");
    if(fDatabase == NULL) return false;
    fprintf(fDatabase,"%-15s %-15s %-15s %-15s\n", "ID", "NAME", "ENTRY-YEAR", "IS DELETED(YES=1, NO=0)");
    fprintf(fDatabase,"--------------------------------------------------------------------------------------\n");

    for(i=0; i<*N; i++)
    {
        fprintf(fDatabase, "%-15d %-15s %-15d %-15d\n", students[i].id, students[i].name, students[i].year, students[i].is_deleted);
    }
    fclose(fDatabase);
    return true;
        
}

//DELETE STUDENT
bool DeleteStudent(FILE *fDatabase, studentT students[],int *N){
    bool deleted = false;
    int possibleID, i;
    if(fDatabase == NULL) return false;
    printf("Ποιόν φοιτητή θέλετε να διαγάψατε; (δώστε το id του)");
    scanf("%d", &possibleID);
    for(i=0; i<*N; i++)
    {
        if(possibleID == students[i].id){
            students[i].is_deleted = 1;
            deleted = true;
        }
    }
    

    fDatabase = fopen("StudentsInformations.txt", "w");
    if(fDatabase == NULL) return false;
    fprintf(fDatabase,"%-15s %-15s %-15s %-15s\n", "ID", "NAME", "ENTRY-YEAR", "IS DELETED(YES=1, NO=0)");
    fprintf(fDatabase, "------------------------------------------------------------------------\n");

    for(i=0; i<*N; i++)
    {
        fprintf(fDatabase, "%-15d %-15s %-15d %-15d\n", students[i].id, students[i].name, students[i].year, students[i].is_deleted);
    }
    fclose(fDatabase);
    return deleted;
}


//FIND STUDENT
int FindStudent(FILE *fDatabase, studentT students[], int *N, int *index_found){
    int found = 0; //0 δεν βρεθηκε, 1 βρεθηκε, 2 διαγραφηκε
    int i,possibleID;
    printf("Ποιόν φοιτητή θέλετε να αναζητήσετε; (δώστε το id του) ");
    scanf("%d", &possibleID);
    
    for(i=0; i<*N; i++){
        if(possibleID == students[i].id) {
            if(students[i].is_deleted == 0){
                *index_found = i;
                found = 1;
                break;
            }
            if(students[i].is_deleted == 1){
                found = 2;
                break;
            }
        }
    }
    return found;
}

//ADD GPA
bool AddGPA(FILE *fDatabase, studentT students[])
{
    int possibleID;
    printf("Σε ποιόν φοιτητή θελετε να προσθέσετε βαθμό; (δώστε το id)");
    scanf("%d", &possibleID);
    //ΣΥΝΕΧΕΙΑ ΜΕΤΑ

}

