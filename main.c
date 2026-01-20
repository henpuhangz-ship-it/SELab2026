#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 100
#define SUBJECTS 6


struct Student {
    char id[20];
    char name[50];
    int marks[SUBJECTS];
    int total;
    float percentage;
    char grade[3];
};


int isValidID(char id[], struct Student s[], int count);
int isValidName(char name[]);
int isValidMarks(int m);
void calculateResult(struct Student *s);
void assignGrade(struct Student *s);
void display(struct Student s[], int n);
void classStatistics(struct Student s[], int n);


int main() {
    struct Student s[MAX];
    int n;
    FILE *fp;

    fp = fopen("students.txt", "r");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    fscanf(fp, "%d", &n);

    for (int i = 0; i < n; i++) {

        fscanf(fp, "%s", s[i].id);
        if (!isValidID(s[i].id, s, i)) {
            printf("Invalid or Duplicate ID: %s\n", s[i].id);
            return 1;
        }

        fscanf(fp, "%s", s[i].name);
        if (!isValidName(s[i].name)) {
            printf("Invalid Name: %s\n", s[i].name);
            return 1;
        }

        for (int j = 0; j < SUBJECTS; j++) {
            fscanf(fp, "%d", &s[i].marks[j]);
            if (!isValidMarks(s[i].marks[j])) {
                printf("Invalid Marks for %s\n", s[i].name);
                return 1;
            }
        }

        calculateResult(&s[i]);
    }

    fclose(fp);

    display(s, n);
    classStatistics(s, n);

    return 0;
}

int isValidID(char id[], struct Student s[], int count) {
    for (int i = 0; id[i]; i++)
        if (!isalnum(id[i]))
            return 0;

    for (int i = 0; i < count; i++)
        if (strcmp(id, s[i].id) == 0)
            return 0;

    return 1;
}

int isValidName(char name[]) {
    for (int i = 0; name[i]; i++)
        if (!isalpha(name[i]))
            return 0;
    return 1;
}


int isValidMarks(int m) {
    return (m >= 0 && m <= 100);
}


void calculateResult(struct Student *s) {
    s->total = 0;
    for (int i = 0; i < SUBJECTS; i++)
        s->total += s->marks[i];

    s->percentage = s->total / 5.0;
    assignGrade(s);
}


void assignGrade(struct Student *s) {
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

void display(struct Student s[], int n) {
    printf("\nID\tName\tTotal\tPercentage\tGrade\n");
    printf("-------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("%s\t%s\t%d\t%.2f\t\t%s\n",
               s[i].id, s[i].name, s[i].total,
               s[i].percentage, s[i].grade);
    }
}


void classStatistics(struct Student s[], int n) {
    float sum = 0, max = s[0].percentage, min = s[0].percentage;
    int gradeCount[8] = {0};

    for (int i = 0; i < n; i++) {
        sum += s[i].percentage;
        if (s[i].percentage > max) max = s[i].percentage;
        if (s[i].percentage < min) min = s[i].percentage;

        if (!strcmp(s[i].grade, "O")) gradeCount[0]++;
        else if (!strcmp(s[i].grade, "A+")) gradeCount[1]++;
        else if (!strcmp(s[i].grade, "A")) gradeCount[2]++;
        else if (!strcmp(s[i].grade, "B+")) gradeCount[3]++;
        else if (!strcmp(s[i].grade, "B")) gradeCount[4]++;
        else if (!strcmp(s[i].grade, "C")) gradeCount[5]++;
        else if (!strcmp(s[i].grade, "D")) gradeCount[6]++;
        else gradeCount[7]++;
    }

    printf("\nClass Average: %.2f\n", sum / n);
    printf("Highest Percentage: %.2f\n", max);
    printf("Lowest Percentage: %.2f\n", min);

    printf("\nGrade Distribution:\n");
    printf("O: %d\nA+: %d\nA: %d\nB+: %d\nB: %d\nC: %d\nD: %d\nF: %d\n",
           gradeCount[0], gradeCount[1], gradeCount[2], gradeCount[3],
           gradeCount[4], gradeCount[5], gradeCount[6], gradeCount[7]);
}