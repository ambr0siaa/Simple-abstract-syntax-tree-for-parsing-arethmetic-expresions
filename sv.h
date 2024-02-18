#ifndef SV_H_
#define SV_H_

#include <ctype.h>
#include <string.h>

typedef struct {
    char *data;
    size_t count;
} String_View;

String_View sv_div_by_next_symbol(String_View *sv);
String_View sv_from_cstr(char *cstr);
String_View sv_trim_left(String_View sv);
String_View sv_trim_right(String_View sv);
String_View sv_trim(String_View sv);
String_View sv_div_by_delim(String_View *sv, char delim);
String_View separate_by_operator(String_View *sv);
int sv_cmp(String_View sv1, String_View sv2);
int sv_to_int(String_View sv);
int sv_is_float(String_View sv);

#endif // SV_H_

#ifdef SV_IMPLEMENTAION

#define SV_Fmt "%.*s"
#define SV_Args(sv) (int) (sv).count, (sv).data

String_View sv_from_cstr(char *cstr)
{
    return (String_View) {
        .count = strlen(cstr),
        .data = cstr
    };
}

String_View sv_trim_left(String_View sv)
{
    size_t i = 0;
    while (i < sv.count && isspace(sv.data[i])) {
        i += 1;
    }

    return (String_View) {
        .count = sv.count - i,
        .data = sv.data + i
    };
}

String_View sv_trim_right(String_View sv)
{
    size_t i = 0;
    while (i < sv.count && isspace(sv.data[sv.count - i - 1])) {
        i += 1;
    }

    return (String_View) {
        .count = sv.count - i,
        .data = sv.data
    };
}

String_View sv_trim(String_View sv)
{
    return sv_trim_right(sv_trim_left(sv));
}

String_View sv_div_by_delim(String_View *sv, char delim)
{
    size_t i = 0;
    while (i < sv->count && sv->data[i] != delim) {
        i += 1;
    }

    String_View result = {
        .count = i,
        .data = sv->data
    };

    if (i < sv->count) {
        sv->count -= i + 1;
        sv->data += i + 1;
    } else {
        sv->count -= i;
        sv->data += i;
    }

    return result;
}

int sv_cmp(String_View sv1, String_View sv2)
{
    if (sv1.count != sv2.count) {
        return 0;
    } else {
        return memcmp(sv1.data, sv2.data, sv1.count) == 0;
    }
}

int sv_to_int(String_View sv)
{
    int result = 0;
    for (size_t i = 0; i < sv.count && isdigit(sv.data[i]); ++i) {
        result = result * 10 + sv.data[i] - '0'; 
    }
    
    return result;
}

int sv_is_float(String_View sv)
{
    int res = 0;
    for (size_t i = 0; i < sv.count; ++i) {
        if (sv.data[i] == '.') {
            res = 1;
            break;
        }
    }
    return res;
}

String_View sv_div_by_next_symbol(String_View *sv)
{
    String_View result;

    size_t i = 0;
    size_t count = 0;
    while (i < sv->count) {
        if (!isspace(sv->data[i])) {
            count++;
        }

        if (count == 2) { 
            break;
        }

        ++i;
    }

    if (i == 1) result.count = i;
    else result.count = (i - 1); 

    result.data = sv->data;
    
    if (i < sv->count) {
        sv->count -= (i - 1);
        sv->data += i;
    } else {
        sv->count -= i;
        sv->data += i;
    }

    return result;
}

String_View separate_by_operator(String_View *sv)
{
    size_t i = 0;
    int brk = 0;
    String_View result;

    while (i < sv->count) {
        if (sv->data[i] == '+' ||
            sv->data[i] == '-' ||
            sv->data[i] == '*' ||
            sv->data[i] == '/' ||
            sv->data[i] == ')' ||
            sv->data[i] == '('  ) 
            brk = 1;
        
        if (brk) break;
        i += 1;
    }

    if (i == 1) result.count = i;
    else result.count = i; 

    result.data = sv->data;

    sv->count -= i;
    sv->data += i;

    return result;
}

#endif // SV_IMPLEMENTAION