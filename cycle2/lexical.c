#include <stdio.h> 
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LEN 100

const char *keywords[] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "inline", "int", "long", "register", "restrict", "return", "short",
    "signed", "sizeof", "static", "struct", "switch", "typedef", "union",
    "unsigned", "void", "volatile", "while", "bool", "true", "false",
    "class", "public", "private", "protected", "namespace", "using"
};

int isKeyword(char *str) 
{
    if (str == NULL) 
        return 0;

    int arraySize = sizeof(keywords) / sizeof(keywords[0]);

    for (int i = 0; i < arraySize; i++) 
    {
        if (strcmp(str, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

int isDelimiter(char ch) 
{
    return (ch == ';' || ch == ',' || ch == '(' || ch == ')' ||
            ch == '{' || ch == '}' || ch == '[' || ch == ']');
}

int isOperator(char ch) 
{
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/' ||
            ch == '=' || ch == '<' || ch == '>' || ch == '!' ||
            ch == '&' || ch == '|' || ch == '%' || ch == '^' ||
            ch == '~' || ch == '?' || ch == ':');
}

void skipSingleLineComment(FILE *fp) 
{
    int ch;
    while ((ch = fgetc(fp)) != EOF && ch != '\n') 
    {
        // Skip until end of line
    }
    if (ch == '\n') 
    {
        ungetc(ch, fp);
    }
}

void skipMultiLineComment(FILE *fp) 
{
    int ch, prev = 0;
    while ((ch = fgetc(fp)) != EOF) 
    {
        if (prev == '*' && ch == '/') 
        {
            break; // End of comment
        }
        prev = ch;
    }
}

void lexicalAnalyzer(FILE *fp) 
{
    int c;
    char buffer[MAX_LEN];
    int i;
    int tokenCount = 0;

    while ((c = fgetc(fp)) != EOF) 
    {
        char ch = (char)c;

        if (isspace(ch)) 
        {
            continue;
        }

        if (isalpha(ch) || ch == '_') 
        {
            i = 0;
            buffer[i++] = ch; // Start of identifier or keyword

            int next_c;
            while (i < MAX_LEN - 1 && (next_c = fgetc(fp)) != EOF) 
            {
                char next_ch = (char)next_c;

                if (isalnum(next_ch) || next_ch == '_') 
                {
                    buffer[i++] = next_ch;
                } 
                else 
                {
                    ungetc(next_c, fp); // Put back the non-identifier character
                    break;
                }
            }

            buffer[i] = '\0';

            if (isKeyword(buffer)) 
            {
                printf("<KEYWORD, %s>\n", buffer);
            } 
            else 
            {
                printf("<IDENTIFIER, %s>\n", buffer);
            }

            tokenCount++;
        }

        else if (isdigit(ch)) // Number literal
        {
            i = 0;
            buffer[i++] = ch;
            int hasDot = 0;

            int next_c;
            while (i < MAX_LEN - 1 && (next_c = fgetc(fp)) != EOF) 
            {
                char next_ch = (char)next_c;

                if (isdigit(next_ch)) 
                {
                    buffer[i++] = next_ch;
                } 
                else if (next_ch == '.' && !hasDot) // Handle decimal point
                {
                    hasDot = 1;
                    buffer[i++] = next_ch;
                } 
                else 
                {
                    ungetc(next_c, fp);
                    break;
                }
            }

            buffer[i] = '\0';
            printf("<NUMBER, %s>\n", buffer);
            tokenCount++;
        }

        else if (ch == '"') // String literal
        {
            i = 0;
            buffer[i++] = ch;

            int next_c;
            while (i < MAX_LEN - 1 && (next_c = fgetc(fp)) != EOF) 
            {
                char next_ch = (char)next_c;
                buffer[i++] = next_ch;

                if (next_ch == '"') // End of string
                {
                    break;
                } 
                else if (next_ch == '\\') // Escape sequence
                {
                    if (i < MAX_LEN - 1 && (next_c = fgetc(fp)) != EOF) 
                    {
                        buffer[i++] = (char)next_c;
                    }
                }
            }

            buffer[i] = '\0';
            printf("<STRING, %s>\n", buffer);
            tokenCount++;
        }

        else if (ch == '\'') // Character literal
        {
            i = 0;
            buffer[i++] = ch;

            int next_c;
            while (i < MAX_LEN - 1 && (next_c = fgetc(fp)) != EOF) 
            {
                char next_ch = (char)next_c;
                buffer[i++] = next_ch;

                if (next_ch == '\'') // End of character
                {
                    break;
                } 
                else if (next_ch == '\\') // Escape sequence
                {
                    if (i < MAX_LEN - 1 && (next_c = fgetc(fp)) != EOF) 
                    {
                        buffer[i++] = (char)next_c;
                    }
                }
            }

            buffer[i] = '\0';
            printf("<CHARACTER, %s>\n", buffer);
            tokenCount++;
        }
        else if (ch == '/') 
        {
            int next_c = fgetc(fp);
            if (next_c == '/') 
            {
                skipSingleLineComment(fp);   // skip until newline
            } 
            else if (next_c == '*') 
            {
                skipMultiLineComment(fp);    // skip until */
            } 
            else 
            {
                ungetc(next_c, fp);          // not a comment, so push back
                printf("<OPERATOR, %c>\n", ch);
            }
            tokenCount++;
        }

        else if (isOperator(ch)) // Operator
        {
            int next_c = fgetc(fp);
            if (next_c != EOF) 
            {
                char next = (char)next_c;

                if ((ch == '=' && next == '=') || (ch == '!' && next == '=') || (ch == '<' && next == '=') || (ch == '>' && next == '=') || (ch == '&' && next == '&') || (ch == '|' && next == '|') || (ch == '+' && next == '+') || (ch == '-' && next == '-') || (ch == '+' && next == '=') || (ch == '-' && next == '=') || (ch == '*' && next == '=') || (ch == '/' && next == '=') || (ch == '%' && next == '=') || (ch == '<' && next == '<') || (ch == '>' && next == '>') || (ch == '-' && next == '>'))
                {
                    printf("<OPERATOR, %c%c>\n", ch, next);
                } 
                else 
                {
                    ungetc(next_c, fp);
                    printf("<OPERATOR, %c>\n", ch);
                }
            } 
            else 
            {
                printf("<OPERATOR, %c>\n", ch);
            }
            tokenCount++;
        }

        else if (isDelimiter(ch)) // Delimiter
        {
            printf("<DELIMITER, %c>\n", ch);
            tokenCount++;
        }

        else if (ch == '#') // Preprocessor directive
        {
            i = 0;
            buffer[i++] = ch;

            int next_c;
            while (i < MAX_LEN - 1 && (next_c = fgetc(fp)) != EOF) 
            {
                char next_ch = (char)next_c;
                if (next_ch == '\n') 
                {
                    ungetc(next_c, fp);
                    break;
                }
                buffer[i++] = next_ch;
            }

            buffer[i] = '\0';
            printf("<PREPROCESSOR, %s>\n", buffer);
            tokenCount++;
        }
    }

    printf("\nTotal tokens processed: %d\n", tokenCount);
}

int main() 
{
    FILE *fp;
    char filename[100];
    fp = fopen("source.txt", "r");
    printf("Tokens identified:\n");
    lexicalAnalyzer(fp);
    fclose(fp);
    return 0;
}