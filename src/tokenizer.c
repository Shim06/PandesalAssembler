#include "tokenizer.h"

char peek(Tokenizer* tz)
{
    return tz->source[tz->pos];
}

char advance(Tokenizer* tz)
{
    char c = tz->source[tz->pos++];
    if (c == '\n') tz->line++;
    return c;
}

void init_token_list(TokenList* list)
{
    list->tokens = NULL;
    list->count = 0;
    list->capacity = 0;
}

void free_token_list(TokenList* list)
{
    free(list->tokens);
    list->tokens = NULL;
    list->count = 0;
    list->capacity = 0;
}   

int add_token(TokenList* list, Token token)
{
    if (list->count >= list->capacity)
    {
        size_t new_capacity = (list->capacity == 0) ? 64 : list->capacity * 2;
        Token *new_tokens = realloc(list->tokens, new_capacity * sizeof(Token));
        if (new_tokens == NULL)
        {
            fprintf(stderr, "Memory Allocation failed");
            return -1;
        }

        list->tokens = new_tokens;
        list->capacity = new_capacity;
    }
    list->tokens[list->count++] = token;
    return 0;
}

void skip_whitespace(Tokenizer* tz)
{
    while (isspace(peek(tz))) advance(tz);
}

Token read_token(Tokenizer* tz, HashTable* mnemonic_table)
{
    skip_whitespace(tz);
    Token token = { .line = tz->line };
    char c = peek(tz);

    switch (c)
    {
    int i = 0;
    case '\0':
        token.type = TOKEN_END;
        return token;   

    case ';':
        advance(tz);
        i = 0;
        while (peek(tz) != '\n' && peek(tz) != '\0') { advance(tz); }
        return read_token(tz, mnemonic_table);

    case '#':
        advance(tz);
        i = 0;
        token.text[i++] = '#';
        while (isxdigit(peek(tz)) || peek(tz) == '$')
        {
            if (i < sizeof(token.text) - 1) token.text[i++] = advance(tz);
            else advance(tz);
        }
        token.text[i] = '\0';
        token.type = TOKEN_IMMEDIATE;
        return token;

    case '$':
        i = 0;
        while (isxdigit(peek(tz)) || peek(tz) == '$')
        {
            if (i < sizeof(token.text) - 1) token.text[i++] = advance(tz);
            else advance(tz);
        }
        token.text[i] = '\0';
        token.type = TOKEN_ADDRESS;
        return token;

    case '(':
        i = 0;
        while (peek(tz) != ')' && peek(tz) != '\0')
        {
            if (i < sizeof(token.text) - 1) token.text[i++] = advance(tz);
            else advance(tz);
        }
        if (peek(tz) == ')') token.text[i++] = advance(tz);
        token.text[i] = '\0';
        token.type = TOKEN_INDIRECT;
        return token;

    case ',':
        token.text[0] = advance(tz);
        token.text[1] = '\0';
        token.type = TOKEN_COMMA;
        return token;

    case '.':
        i = 0;
        while (peek(tz) != '\0' && !isspace(peek(tz)))
        {
            if (i < sizeof(token.text) - 1) token.text[i++] = advance(tz);
            else advance(tz);
        }
        token.text[i] = '\0';
        token.type = TOKEN_DIRECTIVE;
        return token;
    }

    if (isalpha(c) || c == '_')
    {
        int i = 0;
        while (isalnum(peek(tz)) || peek(tz) == '_')
        {
            if (i < sizeof(token.text) - 1) token.text[i++] = advance(tz);
            else advance(tz);
        }
        token.text[i] = '\0';
        
        if (is_register(token.text))
        {
            token.type = TOKEN_REGISTER;
        }
        else if (peek(tz) == ':')
        {
            advance(tz);
            token.type = TOKEN_LABEL;
        }
        else if (search(mnemonic_table, token.text))
        {
            token.type = TOKEN_MNEMONIC;
        }
        else
        {
            token.type = TOKEN_IDENTIFIER;
        }
        return token;
    }

    if (isdigit(c))
    {
        int i = 0;
        while (isdigit(peek(tz)))
        {
            if (i < sizeof(token.text) - 1) token.text[i++] = advance(tz);
            else advance(tz);
        }
        token.text[i] = '\0';

        token.type = TOKEN_ADDRESS;
        return token;
    }

    // Unknown character
    token.text[0] = advance(tz);
    token.text[1] = '\0';
    token.type = TOKEN_UNKNOWN;
    return token;
}

void tokenize(const char* source, TokenList* list, HashTable* mnemonic_table)
{
    if (list == NULL)
        return;

    Tokenizer tz = { .source = source, .pos = 0, .line = 1 };
    Token tok;
    while ((tok = read_token(&tz, mnemonic_table)).type != TOKEN_END)
    {
        add_token(list, tok);
        //printf("Line %d: [%d] %s\n", tok.line, tok.type, tok.text);
    }
    return;
}



