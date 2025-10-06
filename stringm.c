/*
** -----------------------------------------------------------------
** IMPORTANT NOTE: For this assignment, you are not allowed to use
** string.h or any header file other than what is defined below
** -----------------------------------------------------------------
*/
#include "stringm.h"

/*
** strlen_m calculates the length of a string
** const char *string - string to calculate length of
** return the size of the string
**
** note: you can assume string is not NULL
*/
size_t strlen_m(const char *string)
{
    size_t length = 0;
    while (string[length] != '\0') {
        length++;
    }
    return length;
}

/*
** strncpy_m copies n characters of string and returns it
** const char *string - string to copy
** size_t n - number of characters to copy (not including null character)
** return a copy of first n characters of string
**
** note: you can assume string is not NULL
** hint: you will need to malloc a size n + 1 string to accomodate the null character
*/
char *strncpy_m(const char *string, size_t n)
{
    size_t src_len = strlen_m(string);
    size_t count = (src_len < n) ? src_len : n;

    char *copy = (char *)malloc(count + 1);
    if (!copy) return NULL;

    for (size_t i = 0; i < count; i++) {
        copy[i] = string[i];
    }
    copy[count] = '\0';
    return copy;
}


/*
** join_m joins an array of strings separated by a delimiter
** Strings strings - structure that stores an array of strings
** const char *delimiter - delimiter string which joins each string
** return the string created by joining all strings with the delimiter
**
** note: you can assume delimiter is not NULL
** hint: return NULL if strings.num_strings is 0
*/
char *join_m(Strings strings, const char *delimiter)
{
    if (strings.num_strings == 0) return NULL;

    size_t delimiter_len = strlen_m(delimiter);
    size_t total_length = 0;

    for (int i = 0; i < strings.num_strings; i++) {
        total_length += strlen_m(strings.strings[i]);
        if (i < strings.num_strings - 1) {
            total_length += delimiter_len;
        }
    }

    char *joined = (char *)malloc(total_length + 1);
    if (!joined) return NULL;

    size_t pos = 0;
    for (int i = 0; i < strings.num_strings; i++) {
        const char *s = strings.strings[i];
        for (size_t j = 0; s[j] != '\0'; j++) {
            joined[pos++] = s[j];
        }
        if (i < strings.num_strings - 1) {
            for (size_t k = 0; k < delimiter_len; k++) {
                joined[pos++] = delimiter[k];
            }
        }
    }
    joined[pos] = '\0';
    return joined;
}


/*
** free_strings frees all allocated elements in strings
** String strings - free each string in strings.strings and strings.strings itself
*/
void free_strings(Strings strings)
{
    for (int i = 0; i < strings.num_strings; i++) {
        free(strings.strings[i]);
    }
    free(strings.strings);
}

/*
** split_m splits a string at any occurence of pattern
** const char *string - string that is searched for the pattern
** const char *pattern - pattern which string should be split
** return a Strings structure which contains an array of each string
**
** note: you may assume string and pattern are not NULL
**       If pattern is "", split_m performs NO split and returns a Strings with
**       an array of one element: a malloc'd copy of 'string'
** hint 1: TA solution uses strlen_m, strstr_m, and strncpy_m
** hint 2: first calculate how many strings are needed, which is:
**         (the number of times the delimiter appears + 1)
** hint 3: when trying to store a substring, think about how the length of
**         that substring might be calculated in terms of pointer arithmetic
**         - what is the outcome of adding or subtract pointers?
** hint 3.5: strstr_m will return a pointer to the first character of the next occurence
**           or NULL if not found
**
*/
Strings split_m(const char *string, const char *pattern)
{
    Strings result;
    result.strings = NULL;
    result.num_strings = 0;
    return result;
}

/*
** find_and_replace_all_m finds each occurence of the pattern in the string and replaces it
** const char *string - string to search through
** const char *pattern - pattern to search for in the string
** const char *replacement - replacement string for each occurence of the pattern in the string
** return a string in which every occurence of pattern is replaced replacement
**
** note: you may assume string, pattern, and replacement are all not NULL
**       If pattern is "", return a malloc'd copy of 'string' unchanged (no insertions of 'replacement').
** hint: there are two main ways of implementing this function, one involves many lines, one involves four
*/
char *find_and_replace_all_m(const char *string, const char *pattern, const char *replacement)
{
    size_t string_len = strlen_m(string);
    size_t pattern_len = strlen_m(pattern);
    size_t replacement_len = strlen_m(replacement);

    if (pattern_len == 0) {
        char *copy = (char *)malloc(string_len + 1);
        if (!copy) return NULL;
        for (size_t i = 0; i < string_len; i++) {
            copy[i] = string[i];
        }
        copy[string_len] = '\0';
        return copy;
    }

    size_t count = 0;
    const char *p = string;
    const char *match = NULL;
    while ((match = strstr_m(p, pattern)) != NULL) {
        count++;
        p = match + pattern_len;
    }

    size_t new_len = string_len + count * (replacement_len - pattern_len);
    char *out = (char *)malloc(new_len + 1);
    if (!out) return NULL;

    const char *src = string;
    size_t pos = 0;
    while ((match = strstr_m(src, pattern)) != NULL) {
        const char *q = src;
        while (q < match) {
            out[pos++] = *q++;
        }
        for (size_t i = 0; i < replacement_len; i++) {
            out[pos++] = replacement[i];
        }
        src = match + pattern_len;
    }
    while (*src) {
        out[pos++] = *src++;
    }
    out[pos] = '\0';
    return out;
}


/*
** The strstr function is implemented for you to use -- DO NOT MODIFY
** If you are curious about the algorithm used, look up the Knuth-Morris-Pratt (KMP)
** algorithm that can find a substring inside another string 'blazingly fast'
*/
const char *strstr_m(const char *haystack, const char *needle)
{
    size_t haystack_len = 0, needle_len = 0;
    for (const char *h = haystack; *h; h++)
        haystack_len++;
    for (const char *n = needle; *n; n++)
        needle_len++;

    if (needle_len > haystack_len)
        return NULL;

    char *lps_str = malloc(haystack_len + needle_len + 1);
    size_t i = 0;
    for (const char *n = needle; *n; n++, i++)
        lps_str[i] = *n;
    lps_str[i++] = '\1';
    for (const char *h = haystack; *h; h++, i++)
        lps_str[i] = *h;

    int *lps_arr = calloc((haystack_len + needle_len + 1), sizeof *lps_arr);
    size_t l = 0, r = 1;
    bool success = false;

    while (r < haystack_len + needle_len + 1)
    {
        if (lps_str[l] == lps_str[r])
        {
            l++;
            lps_arr[r] = l;
            r++;
        }
        else if (l)
            l = lps_arr[l - 1];
        else
        {
            lps_arr[r] = 0;
            r++;
        }

        if (l == needle_len)
        {
            success = true;
            break;
        }
    }

    free(lps_arr);
    free(lps_str);
    if (success)
        return haystack + (r - l - needle_len - 1);
    return NULL;
}
