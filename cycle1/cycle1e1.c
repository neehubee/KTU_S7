#include <stdio.h>
#include <stdlib.h>

struct node {
    int st;
    struct node *link;
};

void findclosure(int, int);
void insert_trantbl(int, char, int);
int findalpha(char);
void print_e_closure(int);

static int set[20], nostate, noalpha, s, notransition, c, r, buffer[20];
char alphabet[20];
static int e_closure[20][20] = {0};
struct node *transition[20][20] = {NULL};

int main() {
    int i, j;
    printf("Enter the number of alphabets?\n");
    scanf("%d", &noalpha);
    getchar();

    printf("NOTE:- [ use letter e as epsilon]\n");
    printf("NOTE:- [e must be last character, if it is present]\n");
    printf("\nEnter alphabets?\n");
    for (i = 0; i < noalpha; i++) {
        alphabet[i] = getchar();
        getchar();
    }

    printf("\nEnter the number of states?\n");
    scanf("%d", &nostate);

    printf("\nEnter no of transitions?\n");
    scanf("%d", &notransition);

    printf("NOTE:- [Transition is in the form --> qno alphabet qno]\n");
    printf("NOTE:- [States number must be greater than zero]\n");
    printf("\nEnter transitions?\n");

    for (i = 0; i < notransition; i++) {
        scanf("%d %c%d", &r, &c, &s);
        insert_trantbl(r, c, s);
    }

    printf("\n");
    printf("ε-closure of states\n");
    printf("----------------------\n");

    for (i = 1; i <= nostate; i++) {
        c = 0;
        for (j = 0; j < 20; j++) {
            buffer[j] = 0;
            e_closure[i][j] = 0;
        }
        findclosure(i, i);
        printf("\nε-closure(q%d): ", i);
        print_e_closure(i);
    }

    return 0;
}

void insert_trantbl(int r, char c, int s) {
    int j;
    struct node *temp;
    j = findalpha(c);
    temp = (struct node *)malloc(sizeof(struct node));
    temp->st = s;
    temp->link = transition[r][j];
    transition[r][j] = temp;
}

int findalpha(char c) {
    int i;
    for (i = 0; i < noalpha; i++) {
        if (alphabet[i] == c)
            return i;
    }
    return -1;
}

void findclosure(int x, int sta) {
    struct node *temp;
    int i, j;
    j = findalpha('e');  // epsilon is 'e'
    if (j == -1)
        return;

    temp = transition[x][j];
    while (temp != NULL) {
        if (buffer[temp->st] == 0) {
            e_closure[sta][++c] = temp->st;
            buffer[temp->st] = 1;
            findclosure(temp->st, sta);
        }
        temp = temp->link;
    }
}

void print_e_closure(int i) {
    int j;
    printf(" { q%d", i);
    for (j = 1; e_closure[i][j] != 0; j++)
        printf(", q%d", e_closure[i][j]);
    printf(" }");
}
