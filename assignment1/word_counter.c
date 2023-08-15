#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

typedef enum
{
    START,
    CHAR,
    SPACE,
    OPEN_SLASH,
    COMMENT,
    END_STAR,
    SLASH_AFTER_SPACE
} State;

int act_in_start(char c, int *line, int *word, int *cnt)
{
    if (c == '/')
    {
        (*cnt)++;
        (*line)++;
        return SLASH_AFTER_SPACE;
    }
    else if (isspace(c))
    {
        if (c == '\n')
        {
            (*line)++;
        }
        (*cnt)++;
        (*line)++;
        return SPACE;
    }
    else
    {
        (*cnt)++;
        (*line)++;
        (*word)++;
        return CHAR;
    }
}

int act_in_character(char c, int *line, int *word, int *cnt)
{
    if (c == '/')
    {
        (*cnt)++;
        return OPEN_SLASH;
    }
    else if (isspace(c))
    {
        if (c == '\n')
            (*line)++;
        (*cnt)++;
        return SPACE;
    }
    else
    {
        (*cnt)++;
        return CHAR;
    }
}

int act_in_space(char c, int *line, int *word, int *cnt)
{
    if (c == '/')
    {
        (*cnt)++;
        return SLASH_AFTER_SPACE;
    }
    else if (isspace(c))
    {
        if (c == '\n')
            (*line)++;
        (*cnt)++;
        return SPACE;
    }
    else
    {
        (*cnt)++;
        (*word)++;
        return CHAR;
    }
}

int act_in_open_slash(char c, int *line, int *word, int *cnt, int *error_line)
{
    if (c == '/')
    {
        (*cnt)++;
        return OPEN_SLASH;
    }
    else if (c == '*')
    {
        *error_line = *line;
        return COMMENT;
    }
    else if (isspace(c))
    {
        if (c == '\n')
            (*line)++;
        (*cnt)++;
        return SPACE;
    }
    else
    {
        (*cnt)++;
        return CHAR;
    }
}

int act_in_comment(char c, int *line, int *word, int *cnt)
{
    if (c == '*')
        return END_STAR;
    else
    {
        if (c == '\n')
        {
            (*line)++;
            (*cnt)++;
        }
        return COMMENT;
    }
}

int act_in_end_star(char c, int *line, int *word, int *cnt)
{
    if (c == '/')
        return SPACE;
    else if (c == '*')
        return END_STAR;
    else
    {
        if (c == '\n')
        {
            (*line)++;
            (*cnt)++;
        }
        return COMMENT;
    }
}

int act_in_slash_after_space(char c, int *line, int *word, int *cnt, int *error_line)
{
    if (c == '/')
    {
        (*cnt)++;
        (*word)++;
        return OPEN_SLASH;
    }
    else if (c == '*')
    {
        *error_line = *line;
        return COMMENT;
    }
    else if (isspace(c))
    {
        if (c == '\n')
            (*line)++;
        (*cnt)++;
        (*word)++;
        return SPACE;
    }
    else
    {
        (*cnt)++;
        (*word)++;
        return CHAR;
    }
}

int main()
{
    char c;
    State state = START;
    int line = 0;
    int word = 0;
    int cnt = 0;
    int error_line = 0;

    while (scanf("%c", &c) != EOF)
    {
        switch (state)
        {
        case START:
            state = (State)act_in_start(c, &line, &word, &cnt);
            break;
        case CHAR:
            state = (State)act_in_character(c, &line, &word, &cnt);
            break;
        case SPACE:
            state = (State)act_in_space(c, &line, &word, &cnt);
            break;
        case COMMENT:
            state = (State)act_in_comment(c, &line, &word, &cnt);
            break;
        case OPEN_SLASH:
            state = (State)act_in_open_slash(c, &line, &word, &cnt, &error_line);
            break;
        case END_STAR:
            state = (State)act_in_end_star(c, &line, &word, &cnt);
            break;
        case SLASH_AFTER_SPACE:
            state = (State)act_in_slash_after_space(c, &line, &word, &cnt, &error_line);
            break;
        }
    }

    if (state == COMMENT || state == END_STAR)
    {
        printf("Error: line %d: unterminated comment\n", error_line);
        return EXIT_FAILURE;
    }

    printf("%d %d %d\n", line, word, cnt);
    return EXIT_SUCCESS;
}