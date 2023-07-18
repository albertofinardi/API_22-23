#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void reverse(char s[])
{
    int i, j;
    char c;

    for (i = 0, j = strlen(s) - 1; i < j; i++, j--)
    {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

void itoa(int n, char s[])
{
    int i, sign;

    if ((sign = n) < 0) /* record sign */
        n = -n;         /* make n positive */
    i = 0;
    do
    {                          /* generate digits in reverse order */
        s[i++] = n % 10 + '0'; /* get next digit */
    } while ((n /= 10) > 0);   /* delete it */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}

int main()
{
    char path1[100];
    char path2[100];
    char command[200];
    char command2[200];
    char num[4];
    //system("gcc complete.c -o complete");
    //system("gcc compare_out.c -o compare");
    for (int i = 1; i <= 111; i++)
    {
        itoa(i, num);
        strcpy(path1, "archivio_test_aperti/open_");
        strcat(path1, num);
        strcat(path1, ".txt");
        strcpy(path2, "res/open_");
        strcat(path2, num);
        strcat(path2, ".txt");
        strcpy(command, "./complete < ");
        strcat(command, path1);
        strcat(command, " > ");
        strcat(command, path2);
        system(command);
        /*strcpy(command2, "diff ");
        strcat(command2, "archivio_test_aperti/open_");
        strcat(command2, num);
        strcat(command2, ".output.txt ");
        strcat(command2, "res/open_");
        strcat(command2, num);
        strcat(command2, ".txt > diff.txt");
        system(command2);*/
    }
}