#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STUDENTS 100
#define SUBJECTS 6

typedef struct {
    char id[20];
    char name[50];
    int marks[SUBJECTS];
    int total;
    float percentage;
    float cgpa;
    char grade[3];
} Student;

int isValidID(char id[], Student students[], int count);
int isValidName(char name[]);
int isValidMarks(int marks[]);
void computeResult(Student *s);
void assignGrade(Student *s);
void displayReport(Student students[], int n);
void Statistics(Student students[], int n);

int isValidID(char id[], Student students[], int count) {
    for (int i = 0; id[i] != '\0'; i++) {
        if (!isalnum(id[i]))
            return 0;
    }

    for (int j = 0; j < count; j++) {
        if (strcmp(id, students[j].id) == 0)
            return 0;
    }
    return 1;
}

/* Validate Student Name */
int isValidName(char name[]) {
    for (int i = 0; name[i] != '\0'; i++) {
        if (!isalpha(name[i]) && name[i] != ' ')
            return 0;
    }
    return 1;
}

/* Validate Marks */
int isValidMarks(int marks[]) {
    for (int i = 0; i < SUBJECTS; i++) {
        if (marks[i] < 0 || marks[i] > 100)
            return 0;
    }
    return 1;
}

void computeResult(Student *s) {
    s->total = 0;
    int pass = 1;

    for (int i = 0; i < SUBJECTS; i++) {
        s->total += s->marks[i];
        if (s->marks[i] < 50)
            pass = 0;
    }

    s->percentage = (s->total * 100.0) / (SUBJECTS * 100);
    s->cgpa = s->percentage / 10.0;

    if (!pass)
        strcpy(s->grade, "F");
    else
        assignGrade(s);
}


void assignGrade(Student *s) {
    float p = s->percentage;

    if (p >= 90) strcpy(s->grade, "O");
    else if (p >= 85) strcpy(s->grade, "A+");
    else if (p >= 75) strcpy(s->grade, "A");
    else if (p >= 65) strcpy(s->grade, "B+");
    else if (p >= 60) strcpy(s->grade, "B");
    else if (p >= 55) strcpy(s->grade, "C");
    else if (p >= 50) strcpy(s->grade, "D");
    else strcpy(s->grade, "F");
}


void Report(Student students[], int n) {
    printf("\n================ STUDENT REPORT ================\n");
    printf("ID\tName\t\tMarks\t\t\tTotal\t%%\tGrade\tCGPA\n");
    printf("--------------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        printf("%s\t%-10s\t", students[i].id, students[i].name);

        for (int j = 0; j < SUBJECTS; j++)
            printf("%d ", students[i].marks[j]);

        printf("\t%d\t%.2f\t%s\t%.2f\n",
               students[i].total,
               students[i].percentage,
               students[i].grade,
               students[i].cgpa);
    }
}

void Statistics(Student students[], int n) {
    float sum = 0;
    float highest = students[0].percentage;
    float lowest = students[0].percentage;

    int highIndex = 0;
    int lowIndex = 0;

    int gradeCount[8] = {0}; /* O, A+, A, B+, B, C, D, F */

    for (int i = 0; i < n; i++) {
        sum += students[i].percentage;

        if (students[i].percentage > highest) {
            highest = students[i].percentage;
            highIndex = i;
        }

        if (students[i].percentage < lowest) {
            lowest = students[i].percentage;
            lowIndex = i;
        }

        if (strcmp(students[i].grade, "O") == 0) gradeCount[0]++;
        else if (strcmp(students[i].grade, "A+") == 0) gradeCount[1]++;
        else if (strcmp(students[i].grade, "A") == 0) gradeCount[2]++;
        else if (strcmp(students[i].grade, "B+") == 0) gradeCount[3]++;
        else if (strcmp(students[i].grade, "B") == 0) gradeCount[4]++;
        else if (strcmp(students[i].grade, "C") == 0) gradeCount[5]++;
        else if (strcmp(students[i].grade, "D") == 0) gradeCount[6]++;
        else gradeCount[7]++;
    }

    printf("\n================ STATISTICS =================\n");
    printf("Class Average Percentage: %.2f\n", sum / n);
    printf("Highest Percentage: %.2f (ID: %s)\n",
           highest, students[highIndex].id);
    printf("Lowest Percentage : %.2f (ID: %s)\n",
           lowest, students[lowIndex].id);

    printf("\nGrade Distribution:\n");
    printf("O  : %d\nA+ : %d\nA  : %d\nB+ : %d\nB  : %d\nC  : %d\nD  : %d\nF  : %d\n",
           gradeCount[0], gradeCount[1], gradeCount[2],
           gradeCount[3], gradeCount[4], gradeCount[5],
           gradeCount[6], gradeCount[7]);
}


/* Main Function */
int main() {
    FILE *fp;
    Student students[MAX_STUDENTS];
    int count = 0, n;

    fp = fopen("students.txt", "r");
    if (fp == NULL) {
        printf("Error: Unable to open input file.\n");
        return 1;
    }

    fscanf(fp, "%d", &n);

    if (n <= 0 || n > MAX_STUDENTS) {
        printf("Invalid number of students.\n");
        fclose(fp);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        Student temp;

        fscanf(fp, "%s %s", temp.id, temp.name);
        for (int j = 0; j < SUBJECTS; j++)
            fscanf(fp, "%d", &temp.marks[j]);

        if (!isValidID(temp.id, students, count)) continue;
        if (!isValidName(temp.name)) continue;
        if (!isValidMarks(temp.marks)) continue;

        computeResult(&temp);
        students[count++] = temp;
    }

    fclose(fp);

    Report(students, count);
    Statistics(students, count);

    return 0;
}
