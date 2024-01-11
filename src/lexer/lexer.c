#include "lexer.h"

// #include <fnmatch.h>

// init an empty token
struct Token init_token()
{
    struct Token token;
    token.condition = EMPTY;
    token.word = NULL;
    return token;
}

// Fonction pour libérer la mémoire de la liste de tokens
void free_list_tokens(struct Token* tokenList, int numTokens) {
    for (int i = 0; i < numTokens; ++i) {
        free(tokenList[i].word);
    }
    free(tokenList);
}

// Fonction pour afficher la liste de tokens
void print_list_tokens(struct Token* tokens, size_t length)
{
    for (size_t i = 0; i < length; ++i) {
        const char *condition_name = condition_names[tokens[i].condition];

        if (tokens[i].condition == WORD) {
            printf("%s: %s\n", condition_name, tokens[i].word);
        } else {
            printf("%s\n", condition_name);
        }
    }
}

void fnmatch_for_tokens(struct Token* tokenList, int *numTokens, char *str)
{
    // printf("str =     %.5s\n", str);
    if (strstr(str, "if") == str)
        tokenList[*numTokens].condition = IF;
    else if (strstr(str, "then") == str)
        tokenList[*numTokens].condition = THEN;
    else if (strstr(str, "elif") == str)
        tokenList[*numTokens].condition = ELIF;
    else if (strstr(str, "else") == str)
        tokenList[*numTokens].condition = ELSE;
    else if (strstr(str, "fi") == str)
        tokenList[*numTokens].condition = FI;
    else if (strstr(str, ";") == str)
        tokenList[*numTokens].condition = PVIRG;
    else if (strstr(str, "\n") == str)
        tokenList[*numTokens].condition = NEWLINE;
    else if (strstr(str, "'") == str)
        tokenList[*numTokens].condition = QUOTE;
    // if (!fnmatch("if[' ';\\n]?*", str, 0))
    //         tokenList[*numTokens].condition = IF;
    //     else if (!fnmatch("then[' ';\\n\\0]+*", str, 0))
    //         tokenList[*numTokens].condition = THEN;
    //     else if (!fnmatch("elif[' ';\\n\\0]+*", str, 0))
    //         tokenList[*numTokens].condition = ELIF;
    //     else if (!fnmatch("else[' ';\\n\\0]+*", str, 0))
    //         tokenList[*numTokens].condition = ELSE;
    //     else if (!fnmatch("fi[' ';\\n\\0]+*", str, 0))
    //         tokenList[*numTokens].condition = FI;
    //     else if (!fnmatch(";[' ';\\n\\0]+*", str, 0))
    //         tokenList[*numTokens].condition = PVIRG;
    //     else if (!fnmatch("\\n[' ';\\n\\0]+*", str, 0))
    //         tokenList[*numTokens].condition = NEWLINE;
    //     else if (!fnmatch("'[' ';\\n\\0]+*", str, 0))
    //         tokenList[*numTokens].condition = QUOTE;
    else
    {
        tokenList[*numTokens].condition = WORD;
        int word_len = strcspn(str, " ;\n\0");
        tokenList[*numTokens].word = malloc(word_len + 1);
        strncpy(tokenList[*numTokens].word, str, word_len);
    }
}

struct Token* char_to_token(char* str, int* numTokens)
{
    int capacity = 10;
    struct Token* tokenList = malloc(capacity * sizeof(struct Token));
    if (tokenList == NULL)
    {
        fprintf(stderr, "Allocation failed");
        exit(EXIT_FAILURE);
    }
    *numTokens = 0;

    while (str[0] != 0)
    {
        // printf("str[0] = %.5s\n ; numtokens = %d\n\n",str, *numTokens);
        if (*numTokens >= capacity)
        {
            capacity *= 2;
            tokenList = realloc(tokenList, capacity * sizeof(struct Token));
            if (tokenList == NULL)
            {
                fprintf(stderr, "Reallocation failed");
                exit(EXIT_FAILURE);
            }
        }
        tokenList[*numTokens] = init_token();
        fnmatch_for_tokens(tokenList, numTokens, str);
        int test = str[0] == '\n';
        str++;
        while (!test && ((*str >= 'a' && *str <= 'z') || (*str >= 'A' && *str <= 'Z')))
        {
            str++;
            // printf("str[0] = '%c' ; numtokens = %d\n",str[0], *numTokens);
        }
        while (*str == ' ')
        {
            str++;
            // printf("str[0] = '%c' ; numtokens = %d\n",str[0], *numTokens);
        }
        // printf("cpt = %d\n", cpt);
        (*numTokens)++;
    }
    return tokenList;
}

// Fonction pour libérer la mémoire allouée à la liste de tokens
void free_tokens(struct Token* tokenList, int numTokens) {
    for (int i = 0; i < numTokens; ++i) {
        if (tokenList[i].condition == WORD) {
            free(tokenList[i].word); // Libérer la mémoire du mot si nécessaire
        }
    }
    free(tokenList); // Libérer la mémoire de la liste de tokens
}

int main() {
    char input[] = "if false; true; then\necho a\necho b; echo c;\nfi"; // Exemple de chaîne d'entrée

    int numTokens;
    struct Token* tokens = char_to_token(input, &numTokens);

    // Afficher la liste de tokens
    printf("len = %d ;; Liste de tokens :\n",numTokens);
    print_list_tokens(tokens, numTokens);

    // Libérer la mémoire allouée pour la liste de tokens
    // free_list_tokens(tokens, numTokens);

    return 0;
}
