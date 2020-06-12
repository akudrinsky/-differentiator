//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Sorts lines of a russian masterpiece Evgeny Onegin written by Alexandr Pushkin
//!
//! @param [in] input.txt - text file
//! @param [out] output.txt - tet file, in which lines of input.txt are sorted
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐

#ifndef ADD_BETTER_ONEGIN
#define ADD_BETTER_ONEGIN

#define DEBUG
#ifdef DEBUG
#define ASSERT(cond)\
    if (!(cond)) {\
        printf ("Assert failure: < %s > in file %s, line %d, function %s\n", #cond, __FILE__, __LINE__, __PRETTY_FUNCTION__);\
        abort();\
}
#else
#define ASSERT(cond) ;
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
struct str {
    char* letters;
    int len;
};

//using namespace std;

void err_info (const char* str, const char* file = "errors_info.txt");
void say_it (const char* message, bool need_print = true, const char* voice = "Yuri");

char* read_text (FILE* pFile);
void make_orig_txt (str* first_line, str* last_line);
void replace_c (char* line, int n_symbols, char prev, char now);
int Strcmp (const str s1, const str s2);
void qsort_index (str* first_line, str* last_line, int (*cmp)(const str, const str));
void output_text (str *text);
void drop_empty_lines (str *text, int *nlines);
void drop_line (str *text, int num_line, int* nlines);
int count_symbols (FILE* file);
int count_lines (const char* line, int n_symbols);
str* read_txt (char* *line, FILE* pFile, int* nlines, bool with_empty = true);
str* remake_text (char* line, int nlines);
void Swap (str* ptr1, str* ptr2);
bool is_letter (char ch);
bool is_num (const char* str);
int back_cmp (const str s1, const str s2);
void print (char* format, ...);

void test_back_cmp (void);
void test_Strcmp (void);
void test_is_letter (void);
void test_qsort_index (void);
void test_make_orig_txt (void);
void test_count_symbols (void);
void test_count_lines (void);
void test_replace_c (void);

int find_bool_length (int path, const int max_depth = 10) {
    int num = 0;

    for (num = max_depth; num > 0; num--) {
        if ((path >> num) == 1) {
            break;
        }
    }

    if (num == max_depth)
        err_info ("maybe you had overwritten max_depth while finding digits number\n");

    return num + 1;
}

void err_info (const char* str, const char* file) {
    ASSERT (file != nullptr)
    ASSERT (str != nullptr)
    FILE* pFile = fopen (file, "a");
    ASSERT (pFile != nullptr)

    fprintf (pFile, "%s", str);

    fclose (pFile);
}

void say_it (const char* message, bool need_print, const char* voice) {
    if (need_print) {
        printf (message);
    }

    int str_len = strnlen (message, 1000) + 20;
    char* command = (char* )calloc (str_len, sizeof (char));

    strncat (command, "say -v ", str_len);
    strncat (command, voice, str_len);
    strncat (command, " '", str_len);
    strncat (command, message, str_len);
    strncat (command, "'", str_len);

    //execute command with the help of terminal
    system (command);
    free (command);
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Prints text console
//!
//! @param [in] format - pointer to line with a format of output
//! @param [in] pFile - pointer to file where the text exists
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
#include <stdarg.h>
char for_print[100] = {};
void print (char* format, ...) {
    va_list argptr = {};
    va_start (argptr, format);

    char* format_c = nullptr;                                         //pointer to character in format line

    for (format_c = format; *format_c; ++format_c) {
        if (*format_c != '%') {
            sprintf (for_print, "%c", *format_c);
            continue;                                               //go to next character, this was printed
        }
        switch (*(++format_c)) {
            case 'd': {
                int ival = va_arg (argptr, int);
                sprintf (for_print, "%d", ival);
            }   break;

            case 'b': {
                int ival = va_arg (argptr, int);
                for (int i = sizeof(int) * 8; i >= 0; --i) {
//problem           sprintf (for_print, "%d", bit_return (ival, i));
                }
            } break;

            case 'f': {
                double dval = va_arg (argptr, double);
                sprintf (for_print, "%f", dval);
            } break;

            case 's': {
                for (char* sval = va_arg (argptr, char *); *sval; ++sval)
                    sprintf (for_print, "%c", *sval);
            }   break;

            case 'c': {
                char sval = va_arg (argptr, char);
                sprintf (for_print, "%c", sval);
            }   break;

            case '%': {
                sprintf (for_print, "%%");
                sprintf (for_print, "%%");
            }   break;

            default: {
                sprintf (for_print, "%c", *format_c);
            }
                break;
        }
    }
    va_end (argptr); /* очистка, когда все сделано */

}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Reads text to line
//!
//! @param [in] line - pointer to line, where to store file
//! @param [in] pFile - pointer to file where the text exists
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
char* read_text (FILE* pFile) {
    ASSERT (pFile != nullptr);
    rewind (pFile);

    int n = count_symbols (pFile);

    char* line = (char* )calloc (n, sizeof (char));
    fread (line, sizeof(char), n, pFile);

    return line;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Tries to make the order of lines as it was in the beginning
//!
//! @param [in] line - pointer to line, where to store file
//! @param [in] text - pointer to array of lines * characters
//! @param [in] pFile - pointer to file where the text exists
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
str* read_txt (char* *line, FILE* pFile, int* nlines, bool with_empty) {
    ASSERT (line != nullptr);
    ASSERT (pFile != nullptr);
    rewind (pFile);

    int n = count_symbols (pFile);

    *line = (char* )calloc (n+1, sizeof (char));
    fread (*line, sizeof(char), n, pFile);
    (*line)[n] = '\n';

    replace_c (*line, n+1, '\n', '\0');                   //change all \n to \0

    *nlines = count_lines (*line, n+1);
    str* text = (str* )calloc (*nlines, sizeof (str));
    ASSERT (text != nullptr);

    char c_prev = '\0';
    char* buf = *line;
    str *ptr = text;

    while ((buf - *line) < n + 1) {
        if (c_prev == '\0') {                       //big flow of conditions to better understand the code
            if (with_empty) {
                (*ptr).letters = buf;
                (*ptr).len = strlen (buf);
                ++ptr;
                ASSERT (ptr != nullptr);
            }
            else {
                if (*(buf) != '\0') {
                    (*ptr).letters = buf;
                    (*ptr).len = strlen (buf);
                    ++ptr;
                }
            }
        }
        c_prev = *buf;
        ++buf;
        ASSERT (buf != nullptr);
    }

    *nlines = (int)(ptr - text);

    return text;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Makes array of pointers to strings (str) of lines of a text
//!
//! @param [in] line where text is stosred
//! @param [in] nlines - number of lines in text
//! @return - array of pointers to lines of a text (as a str)
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
str* remake_text (char* line, int nlines) {
    str* text = (str* )calloc (nlines, sizeof (str));
    ASSERT (text != nullptr);

    char c_prev = '\0';
    char* buf = line;
    str *ptr = text;

    while ((ptr - text) < nlines + 1) {
        if (c_prev == '\0') {                       //big flow of conditions to better understand the code
            (*ptr).letters = buf;
            (*ptr).len = strlen(buf);
            ++ptr;
            ASSERT (ptr != nullptr);
        }
        c_prev = *buf;
        ++buf;
        ASSERT (buf != nullptr);
    }

    return text;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Tries to make the order of lines as it was in the beginning
//!
//! @param [in] first_line - pointer to first line in the text
//! @param [in] ast_line - pointer to last line in the text
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void make_orig_txt (str* first_line, str* last_line) {
    ASSERT (first_line != nullptr);
    ASSERT (last_line != nullptr);

    str pivot = *first_line;
    str* l_hold = first_line;
    str* r_hold = last_line;

    while (first_line <= last_line) {
        while ((*first_line).letters < pivot.letters && first_line < r_hold)
            ++first_line;
        while ((*last_line).letters > pivot.letters && last_line > l_hold)
            --last_line;

        if (first_line <= last_line) {
            std::swap (*first_line, *last_line);
            ++first_line;
            --last_line;
        }
    }

    // recursive if there is something to sort
    if (last_line > l_hold)
        make_orig_txt (l_hold, last_line);
    if (r_hold > first_line)
        make_orig_txt (first_line, r_hold);
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Compares two lines
//!
//! @param [in] s1 - first one
//! @param [in] s2 - second one
//!
//! @param [out] 1 if first is bigger, 0 if they are equal, else -1
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
int Strcmp (const str s1, const str s2) {
    ASSERT (s1.letters != nullptr);
    ASSERT (s2.letters != nullptr);

    int min_len_s = (s1.len, s2.len);               //min length of s1 and s2

    for (int i = 0; i < min_len_s; ++i) {
        if (s1.letters[i] > s2.letters[i])
            return 1;
        if (s1.letters[i] < s2.letters[i])
            return -1;
    }

    if (s1.len < s2.len)
        return -1;
    if (s1.len > s2.len)
        return 1;

    return 0;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Compares two lines from the end (ignoring all non-letter characters)
//!
//! @param [in] s1 - first one
//! @param [in] s2 - second one
//!
//! @param [out] 1 if first is bigger, 0 if they are equal, else -1
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
int back_cmp (const str s1, const str s2) {
    ASSERT (s1.letters != nullptr);
    ASSERT (s2.letters != nullptr);

    int i = s1.len , j = s2.len;

    while (i >= 0 && j >= 0) {
        while (!is_letter (s1.letters[i]) && i >= 0)
            --i;
        while (!is_letter (s2.letters[j]) && j >= 0)
            --j;


        if (s1.letters[i] > s2.letters[j])
            return 1;
        if (s1.letters[i] < s2.letters[j])
            return -1;
        --i;
        --j;
    }

    if (i == -1 && j == -1)
        return 0;
    if (i == -1)
        return -1;
    else
        return 1;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Writes if the character is a letter or not
//!
//! @param [in] ch - character
//! @param [out] true for letter, else false
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
bool is_letter (char ch) {
    if (ch >= 'a' && ch <= 'z')
        return true;
    return (ch >= 'A' && ch <= 'Z');
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Writes if the character is a number or not
//!
//! @param [in] ch - character
//! @param [out] true for number, else false
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
bool is_num (const char* str) {
    if ((str[0] < '0' || str[0] > '9') && str[0] != '-') {
        //printf ("first\n");
        return false;
    }
    if (strncmp (str, "-", 3) == 0) {
        return false;
    }
    bool was_dot = false;
    for (int i = 1; i < strlen (str); ++i) {
        if ((str[i] < '0' || str[i] > '9') && str[i] != '.') {
            //printf ("not first\n");
            return false;
        }
        if (str[i] == '.' && was_dot) {
            return false;
        }
        if (str[i] == '.') {
            was_dot = true;
        }
    }

    return true;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Sorts index array so as to get the alphabetic order of coresponding array of strings and writes them to output.txt
//!
//! @param [in] first_line - pointer to first line in text to sort
//! @param [in] last_line - pointer to last line in text to sort
//! @param [in] cmp - way to sort
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void qsort_index (str* first_line, str* last_line, int (*cmp)(const str, const str)) {
    ASSERT (first_line != nullptr);
    ASSERT (last_line != nullptr);

    str pivot = *first_line;
    str* l_hold = first_line;
    str* r_hold = last_line;

    while (first_line <= last_line) {
        while (cmp (*first_line, pivot) == -1 && first_line < r_hold)
            ++first_line;
        ASSERT (first_line != nullptr);

        while (cmp (*last_line, pivot) == 1 && last_line > l_hold)
            --last_line;
        ASSERT (last_line != nullptr);

        if (first_line <= last_line) {
            Swap (first_line, last_line);
            ++first_line;
            --last_line;
            ASSERT (first_line != nullptr);
            ASSERT (last_line != nullptr);
        }
    }

    // recursive if there is something to sort
    if (last_line > l_hold)
        qsort_index (l_hold, last_line, cmp);
    if (r_hold > first_line)
        qsort_index (first_line, r_hold, cmp);
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Swaps two pointers on strings
//!
//! @param [in] ptr1 - first pointer
//! @param [in] ptr2 - second pointer
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void Swap (str* ptr1, str* ptr2) {
    ASSERT (ptr1 != nullptr);
    ASSERT (ptr2 != nullptr);

    str tmp = *ptr1;
    *ptr1 = *ptr2;
    *ptr2 = tmp;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Writes text to output.txt
//!
//! @param [in] text - array of strings
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void output_text (str *text) {
    FILE* pFile = fopen ("output.txt", "ab");
    ASSERT (pFile != nullptr);
    ASSERT (text != nullptr);

    while ((*text).letters != nullptr) {
        fputs ((*text).letters, pFile);
        fputs ("\n", pFile);
        ++text;
    }
    fputs ("\n___________________________End____________________________\n\n", pFile);

    fclose (pFile);
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Deletes line from array of strings
//!
//! @param [in] text - array of strings
//! @param [in] num_line - line to drop out (starts with 0)
//! @param [in] nlines - number of lines in array
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void drop_line (str *text, int num_line, int* nlines) {
    ASSERT (text != nullptr);
    ASSERT (nlines != nullptr);

    for (int i = num_line; i < *nlines-1; ++i) {
        text[i] = text[i+1];
    }
    *nlines -= 1;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Deletes empty ("") lines from array of strings
//!
//! @param [in] text - array of strings
//! @param [in] nlines - number of lines in array
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void drop_empty_lines (str *text, int *nlines) {
    ASSERT (text != nullptr);
    ASSERT (nlines != nullptr);

    int i = 0;
    while (i < *nlines){
        if ((text[i]).letters[0] == '\0') {
            drop_line (text, i, nlines);
            --i;
        }
        ++i;
    }
    text[*nlines].letters = nullptr;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Counts all symbols in text file
//!
//! @param [in] file - pointer to text file
//!
//! @param [out] number of symbols in a file
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
int count_symbols (FILE* file) {
    ASSERT (file != nullptr);

    fseek (file, 0, SEEK_END);
    int n_symbols = ftell (file);
    rewind (file);
    return n_symbols;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Reads file to array of lines with no free space (non-rectangular form)
//!
//! @param [in] line - text file with in one line (where '\n' -> '\0' by read_txt())
//! @param [in] n_symbols - number of symbols in line (corresponding text file)
//!
//! @param [out] number of lines in a file
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
int count_lines (const char* line, int n_symbols) {
    ASSERT (line != nullptr);

    int number = 1;
    for (int i = 0; i < n_symbols; ++i) {
        if (line[i] == '\0' || line[i] == '\n') {
            ++number;
        }
    }
    return number;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Change all prev symbols to now
//! @param [in] line - text file wittn in one line
//! @param [in] prev - symbol to change
//! @param [in] now - symbol to write instead of prev
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void replace_c (char* line, int n_symbols, char prev, char now) {
    ASSERT (line != nullptr);

    for (int i = 0; i < n_symbols; ++i) {
        if (line[i] == prev) {
            line[i] = now;
        }
    }
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Tests back comparator
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void test_back_cmp (void) {
    str str1 = {"a",1};
    str str2 = {"a", 1};
    if (back_cmp (str1, str2) != 0)
        printf ("test_back_cmp error 1\n");

    str1 = {"abc", 3};
    str2 = {"abb", 3};
    if (back_cmp (str1, str2) != 1)
        printf ("test_back_cmp error 2\n");

    str1 = {"abcd", 4};
    str2 = {"bcd", 3};
    if (back_cmp (str1, str2) != 1)
        printf ("test_back_cmp error 3\n");

    str1 = {"bcd", 3};
    str2 = {"abcd", 4};
    if (back_cmp (str1, str2) != -1)
        printf ("test_back_cmp error 4\n");
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Tests strcmp_
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void test_Strcmp (void) {
    str str1 = {"a",1};
    str str2 = {"a", 1};
    if (Strcmp (str1, str2) != 0)
        printf ("test_Strcmp error 1\n");

    str1 = {"abc", 3};
    str2 = {"abb", 3};
    if (Strcmp (str1, str2) != 1)
        printf ("test_Strcmp error 2\n");

    str1 = {"abcd", 4};
    str2 = {"bcd", 3};
    if (Strcmp (str1, str2) != -1)
        printf ("test_Strcmp error 3\n");

    str1 = {"bcd", 3};
    str2 = {"abcd", 4};
    if (Strcmp (str1, str2) != 1)
        printf ("test_Strcmp error 4\n");
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Tests is_letter
//! @param [in] cmp - comparator to test
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void test_is_letter(void) {
    char c = 'a';
    if (is_letter (c) != true)
        printf ("test_is_letter  error 1\n");

    c = 'A';
    if (is_letter (c) != true)
        printf ("test_is_letter  error 1\n");

    c = 'z';
    if (is_letter (c) != true)
        printf ("test_is_letter  error 1\n");

    c = 'Z';
    if (is_letter (c) != true)
        printf ("test_is_letter  error 1\n");

    c = 'h';
    if (is_letter (c) != true)
        printf ("test_is_letter  error 1\n");

    c = 'H';
    if (is_letter (c) != true)
        printf ("test_is_letter  error 1\n");

    c = ' ';
    if (is_letter (c) == true)
        printf ("test_is_letter  error 1\n");

    c = '\n';
    if (is_letter (c) == true)
        printf ("test_is_letter  error 1\n");

    c = '1';
    if (is_letter (c) == true)
        printf ("test_is_letter  error 1\n");

    c = '9';
    if (is_letter (c) == true)
        printf ("test_is_letter  error 1\n");

    c = '*';
    if (is_letter (c) == true)
        printf ("test_is_letter  error 1\n");

    c = '#';
    if (is_letter (c) == true)
        printf ("test_is_letter  error 1\n");

    c = ';';
    if (is_letter (c) == true)
        printf ("test_is_letter  error 1\n");

    c = ',';
    if (is_letter (c) == true)
        printf ("test_is_letter  error 1\n");

    c = '.';
    if (is_letter (c) == true)
        printf ("test_is_letter  error 1\n");

    c = '?';
    if (is_letter (c) == true)
        printf ("test_is_letter  error 1\n");

    c = '!';
    if (is_letter (c) == true)
        printf ("test_is_letter  error 1\n");
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Tests qsort_index
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void test_qsort_index (void) {
    str text[6] = {{"    Once upon a midnight dreary, while I pondered, weak and weary,", 62},
                   {"  Over many a quaint and curious volume of forgotten lore,", 56},
                   {"    While I nodded, nearly napping, suddenly there came a tapping,", 62},
                   {"   As of some one gently rapping, rapping at my chamber door.", 58},
                   {"  'Tis some visitor, I muttered, tapping at my chamber door-", 57},
                   {"                Only this, and nothing more.", 28}};
    qsort_index (text, text + 5, Strcmp);

    if (Strcmp(text[0], {"                Only this, and nothing more.", 28}) != 0)
        printf ("test_qsort_index error 1\n");

    if (Strcmp(text[1], {"    Once upon a midnight dreary, while I pondered, weak and weary,", 62}) != 0)
        printf ("test_qsort_index error 2\n");

    if (Strcmp(text[2], {"    While I nodded, nearly napping, suddenly there came a tapping,", 62}) != 0)
        printf ("test_qsort_index error 3\n");

    if (Strcmp(text[3], {"   As of some one gently rapping, rapping at my chamber door.", 58}) != 0)
        printf ("test_qsort_index error 4\n");

    if (Strcmp(text[4], {"  'Tis some visitor, I muttered, tapping at my chamber door-", 57}) != 0)
        printf ("test_qsort_index error 5\n");

    if (Strcmp(text[5], {"  Over many a quaint and curious volume of forgotten lore,", 56}) != 0)
        printf ("test_qsort_index error 6\n");

    str text2[6] = {{"abcd", 4},
                    {"fva", 3},
                    {"sad", 3},
                    {"sadsd", 5},
                    {"s", 1},
                    {"sd", 2}};

    qsort_index (text2, text2 + 5, Strcmp);

    if (Strcmp(text2[0], {"abcd", 4}) != 0)
        printf ("test_qsort_index error 13\n");

    if (Strcmp(text2[1], {"fva", 3}) != 0)
        printf ("test_qsort_index error 14\n");

    if (Strcmp(text2[2], {"s", 1}) != 0)
        printf ("test_qsort_index error 15\n");

    if (Strcmp(text2[3], {"sad", 3}) != 0)
        printf ("test_qsort_index error 16\n");

    if (Strcmp(text2[4], {"sadsd", 5}) != 0)
        printf ("test_qsort_index error 17\n");

    if (Strcmp(text2[5], {"sd", 2}) != 0)
        printf ("test_qsort_index error 18\n");

    qsort_index (text2, text2 + 5, back_cmp);

    if (Strcmp(text2[0], {"fva", 3}) != 0)
        printf ("test_qsort_index error 19\n");

    if (Strcmp(text2[1], {"sad", 3}) != 0)
        printf ("test_qsort_index error 20\n");

    if (Strcmp(text2[2], {"abcd", 4}) != 0)
        printf ("test_qsort_index error 21\n");

    if (Strcmp(text2[3], {"sd", 2}) != 0)
        printf ("test_qsort_index error 22\n");

    if (Strcmp(text2[4], {"sadsd", 5}) != 0)
        printf ("test_qsort_index error 23\n");

    if (Strcmp(text2[5], {"s", 1}) != 0)
        printf ("test_qsort_index error 24\n");
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Tests make_orig_txt
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void test_make_orig_txt (void) {
    str text[6] = {{"    Once upon a midnight dreary, while I pondered, weak and weary,", 62},
                   {"  Over many a quaint and curious volume of forgotten lore,", 56},
                   {"    While I nodded, nearly napping, suddenly there came a tapping,", 62},
                   {"   As of some one gently rapping, rapping at my chamber door.", 58},
                   {"  'Tis some visitor, I muttered, tapping at my chamber door-", 57},
                   {"                Only this, and nothing more.", 28}};
    qsort_index (text, text + 5, Strcmp);
    make_orig_txt (text, text+5);

    if (Strcmp(text[0], {"    Once upon a midnight dreary, while I pondered, weak and weary,", 62}) != 0)
        printf ("test_make_orig_txt error 1\n");

    if (Strcmp(text[1], {"  Over many a quaint and curious volume of forgotten lore,", 56}) != 0)
        printf ("test_make_orig_txt error 2\n");

    if (Strcmp(text[2], {"    While I nodded, nearly napping, suddenly there came a tapping,", 62}) != 0)
        printf ("test_make_orig_txt error 3\n");

    if (Strcmp(text[3], {"   As of some one gently rapping, rapping at my chamber door.", 58}) != 0)
        printf ("test_make_orig_txt error 4\n");

    if (Strcmp(text[4], {"  'Tis some visitor, I muttered, tapping at my chamber door-", 57}) != 0)
        printf ("test_make_orig_txt error 5\n");

    if (Strcmp(text[5], {"                Only this, and nothing more.", 28}) != 0)
        printf ("test_make_orig_txt error 6\n");
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Tests count_symbols
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void test_count_symbols (void) {
    FILE* pFile = fopen ("test1.txt", "r");
    ASSERT (pFile != nullptr);

    int n = count_symbols (pFile);
    if (n != 23)
        printf ("test_count_symbols error 1\n");
    fclose (pFile);

    pFile = fopen ("test2.txt", "r");
    ASSERT (pFile != nullptr);

    n = count_symbols (pFile);
    if (n != 32)
        printf ("test_count_symbols error 2\n");
    fclose (pFile);

    pFile = fopen ("test3.txt", "r");
    ASSERT (pFile != nullptr);

    n = count_symbols (pFile);
    if (n != 77)
        printf ("test_count_symbols error 3\n");
    fclose (pFile);
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Tests count_lines
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void test_count_lines (void) {
    FILE* pFile = fopen ("test1.txt", "r");
    ASSERT (pFile != nullptr);
    int n1 = count_symbols (pFile);
    char* line1 = (char*) calloc (n1+1, sizeof(char));
    fread (line1, sizeof(char), n1, pFile);
    int nlines1 = count_lines (line1, n1);

    if (nlines1 != 6)
        printf ("test_count_lines error 1\n");
    fclose (pFile);

    pFile = fopen ("test2.txt", "r");
    ASSERT (pFile != nullptr);
    int n2 = count_symbols (pFile);
    char* line2 = (char*) calloc (n2+1, sizeof(char));
    fread (line2, sizeof(char), n2, pFile);
    int nlines2 = count_lines (line2, n2);

    if (nlines2 != 11)
        printf ("test_count_lines error 2\n");
    fclose (pFile);

    pFile = fopen ("test3.txt", "r");
    ASSERT (pFile != nullptr);
    int n3 = count_symbols (pFile);
    char* line3 = (char*) calloc (n3+1, sizeof(char));
    fread (line3, sizeof(char), n3, pFile);
    int nlines3 = count_lines (line3, n3);

    if (nlines3 != 18)
        printf ("test_count_lines error 3\n");
    fclose (pFile);
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Tests replace_c
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void test_replace_c (void) {
    FILE *pFile = fopen("test1.txt", "r");
    ASSERT (pFile != nullptr);
    int n1 = count_symbols(pFile);
    str line1;
    line1.letters = (char *) calloc(n1 + 1, sizeof(char));
    line1.len = n1+1;
    fread(line1.letters, sizeof(char), n1, pFile);
    replace_c (line1.letters, n1, 'a', '1');

    if (Strcmp(line1, {"1bcd\nfv1\ns1d\ns1dsd\ns\nsd", 24}) != 0)
        printf("test_count_lines error 1");

    pFile = fopen("test2.txt", "r");
    ASSERT (pFile != nullptr);
    int n2 = count_symbols(pFile);
    str line2;
    line2.letters = (char *) calloc(n2 + 1, sizeof(char));
    line2.len = n1+1;
    fread(line2.letters, sizeof(char), n2, pFile);
    replace_c (line2.letters, n2, 's', '1');

    if (Strcmp(line2, {"fad1f1d\n\n1d1d\n\n1\n\n1\n11\n1\n1\n1ad11", 24}) != 0)
        printf("test_count_lines error 2");
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! puts line into file
//!
//! @param [in] string - line to put
//! @param [in] file - pointer to file where to put
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void fputs_ (char* string, FILE* file) {
    ASSERT (string != nullptr);
    ASSERT (file != nullptr);

    char c = 0;
    int i = 0;

    while ((c = string[i]) != '\0') {
        fputc (c, file);
    }
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Puts line to the screen
//!
//! @param [in] string - pointer to line to put out
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void puts_ (char* string) {
    ASSERT (string != nullptr);
    printf ("%s", string);
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! deletes empty lines from text file
//!
//! @param [in] file - pointer to text file
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void delete_empty_strings (FILE* file) {
    ASSERT (file != nullptr);
    FILE* can;
    can = fopen ("can.txt", "w");
    ASSERT (can != nullptr);

    char c = '0', cprev = '0';
    while ((c = fgetc (file)) != EOF) {
        if (!(cprev == '\n' && c == '\n')) {
            fputc (c, can);
            cprev = c;
        }
    }
    fclose (file);
    fclose (can);

    file = fopen ("input.txt", "w");
    can = fopen ("can.txt", "r");
    while ((c = fgetc (can)) != EOF) {
        fputc (c, file);
    }
    fclose (file);
    fclose (can);

    file = fopen ("input.txt", "r");
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! counts lines in file
//!
//! @param [in] file - pointer to text file
//!
//! @param [out] number - number of lines in a file
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
int count_lines_txt (FILE* file){
    ASSERT (file != nullptr);

    rewind (file);
    int number = 1;
    char c = '0';
    while ((c = fgetc(file)) != EOF){
        if (c == '\n')
            ++number;
    }
    rewind (file);
    return number;
}


//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Gets line from file; returns the way line was absorbed
//!
//! @param [in] str[] - line to get
//! @param [in] max_size - max size of line s[]
//! @param [in] file - pointer to text file to get line from
//! @param [out] 0 for the last line in text, 1 for the normal end, 2 for trying to overwrite s[]
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
int fgets_ (char* str, int max_size, FILE* file)
{
    ASSERT (str != nullptr);
    ASSERT (file != nullptr);

    char c = '0';
    bool is_space = true;           //is there enough space?
    bool not_end = true;            //is it the end of line?
    int i = 0;

    for (i = 0; i < max_size; ++i) {     //fill every element with \0
        str[i] = '\0';
    }
    i = 0;

    while (is_space && not_end){
        c = fgetc(file);
        if (c == EOF){
            str[i] = '\n';
            break;
        }
        if (i<max_size){
            str[i] = c;
            ++i;
        }
        else {
            is_space = false;
        }
        if (c=='\n') {
            not_end = false;
        }
    }
    if (c == EOF)
        return 0;
    if (c == '\n')
        return 1;
    if (!is_space)
        return 2;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Gets minimum if two integer elements
//!
//! @param [in] a - integer value
//! @param [in] b - integer value
//!
//! @param [out] minimum of them
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
int min_ (int a, int b) {
    return (b > a) ? a : b;
}

#endif