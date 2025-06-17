#include "string_utils.h"
#include <ctype.h>
#include <errno.h>

char* string_trim(char* str) {
    if (!str) return NULL;
    
    // Trim leading whitespace
    while (isspace((unsigned char)*str)) str++;
    
    // If string is all whitespace
    if (*str == 0) return str;
    
    // Trim trailing whitespace
    char* end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    
    // Null terminate
    end[1] = '\0';
    
    return str;
}

char* string_trim_copy(const char* str) {
    if (!str) return NULL;
    
    // Find start of non-whitespace
    while (isspace((unsigned char)*str)) str++;
    
    size_t len = strlen(str);
    if (len == 0) {
        char* result = malloc(1);
        if (result) result[0] = '\0';
        return result;
    }
    
    // Find end of non-whitespace
    const char* end = str + len - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    
    // Calculate trimmed length
    size_t trimmed_len = end - str + 1;
    
    // Allocate and copy
    char* result = malloc(trimmed_len + 1);
    if (result) {
        strncpy(result, str, trimmed_len);
        result[trimmed_len] = '\0';
    }
    
    return result;
}

int string_is_empty(const char* str) {
    if (!str) return 1;
    
    while (*str) {
        if (!isspace((unsigned char)*str)) {
            return 0;
        }
        str++;
    }
    return 1;
}

int string_compare_ignore_case(const char* str1, const char* str2) {
    if (!str1 && !str2) return 0;
    if (!str1) return -1;
    if (!str2) return 1;
    
    while (*str1 && *str2) {
        char c1 = tolower((unsigned char)*str1);
        char c2 = tolower((unsigned char)*str2);
        
        if (c1 != c2) {
            return c1 - c2;
        }
        
        str1++;
        str2++;
    }
    
    return tolower((unsigned char)*str1) - tolower((unsigned char)*str2);
}

int string_split(const char* str, char delimiter, char** tokens, int max_tokens) {
    if (!str || !tokens || max_tokens <= 0) return 0;
    
    int token_count = 0;
    const char* start = str;
    const char* current = str;
    
    while (*current && token_count < max_tokens - 1) {
        if (*current == delimiter) {
            // Found delimiter, create token
            size_t token_len = current - start;
            tokens[token_count] = malloc(token_len + 1);
            if (tokens[token_count]) {
                strncpy(tokens[token_count], start, token_len);
                tokens[token_count][token_len] = '\0';
                string_trim(tokens[token_count]);
                token_count++;
            }
            
            // Skip delimiter and whitespace
            current++;
            while (*current && isspace((unsigned char)*current)) current++;
            start = current;
        } else {
            current++;
        }
    }
    
    // Add last token if there's remaining string
    if (*start && token_count < max_tokens) {
        size_t token_len = strlen(start);
        tokens[token_count] = malloc(token_len + 1);
        if (tokens[token_count]) {
            strcpy(tokens[token_count], start);
            string_trim(tokens[token_count]);
            token_count++;
        }
    }
    
    return token_count;
}

char* string_find_substring(const char* haystack, const char* needle) {
    if (!haystack || !needle) return NULL;
    return strstr(haystack, needle);
}

int string_starts_with(const char* str, const char* prefix) {
    if (!str || !prefix) return 0;
    
    size_t str_len = strlen(str);
    size_t prefix_len = strlen(prefix);
    
    if (prefix_len > str_len) return 0;
    
    return strncmp(str, prefix, prefix_len) == 0;
}

int string_ends_with(const char* str, const char* suffix) {
    if (!str || !suffix) return 0;
    
    size_t str_len = strlen(str);
    size_t suffix_len = strlen(suffix);
    
    if (suffix_len > str_len) return 0;
    
    return strcmp(str + str_len - suffix_len, suffix) == 0;
}

void string_to_upper(char* str) {
    if (!str) return;
    
    while (*str) {
        *str = toupper((unsigned char)*str);
        str++;
    }
}

void string_to_lower(char* str) {
    if (!str) return;
    
    while (*str) {
        *str = tolower((unsigned char)*str);
        str++;
    }
}

void string_capitalize(char* str) {
    if (!str || *str == '\0') return;
    
    // Capitalize first character
    *str = toupper((unsigned char)*str);
    str++;
    
    // Lowercase the rest
    while (*str) {
        *str = tolower((unsigned char)*str);
        str++;
    }
}

int string_to_int(const char* str, int* result) {
    if (!str || !result) return 0;
    
    char* endptr;
    errno = 0;
    long val = strtol(str, &endptr, 10);
    
    // Check for conversion errors
    if (errno != 0 || endptr == str || *endptr != '\0') {
        return 0;
    }
    
    // Check for overflow
    if (val > INT_MAX || val < INT_MIN) {
        return 0;
    }
    
    *result = (int)val;
    return 1;
}

int string_to_double(const char* str, double* result) {
    if (!str || !result) return 0;
    
    char* endptr;
    errno = 0;
    double val = strtod(str, &endptr);
    
    // Check for conversion errors
    if (errno != 0 || endptr == str || *endptr != '\0') {
        return 0;
    }
    
    *result = val;
    return 1;
}

int string_is_numeric(const char* str) {
    if (!str || *str == '\0') return 0;
    
    // Allow leading sign
    if (*str == '+' || *str == '-') str++;
    
    if (*str == '\0') return 0;
    
    int has_digit = 0;
    int has_decimal = 0;
    
    while (*str) {
        if (isdigit((unsigned char)*str)) {
            has_digit = 1;
        } else if (*str == '.' && !has_decimal) {
            has_decimal = 1;
        } else {
            return 0;
        }
        str++;
    }
    
    return has_digit;
}

CalendarResult string_parse_date_components(const char* date_str, int* day, int* month, int* year) {
    if (!date_str || !day || !month || !year) {
        return CALENDAR_ERROR_NULL_POINTER;
    }
    
    // Try different date formats
    int parsed = 0;
    
    // Format: YYYY-MM-DD
    parsed = sscanf(date_str, "%d-%d-%d", year, month, day);
    if (parsed == 3) return CALENDAR_SUCCESS;
    
    // Format: DD/MM/YYYY
    parsed = sscanf(date_str, "%d/%d/%d", day, month, year);
    if (parsed == 3) return CALENDAR_SUCCESS;
    
    // Format: MM/DD/YYYY (American style)
    parsed = sscanf(date_str, "%d/%d/%d", month, day, year);
    if (parsed == 3) return CALENDAR_SUCCESS;
    
    // Format: DD.MM.YYYY
    parsed = sscanf(date_str, "%d.%d.%d", day, month, year);
    if (parsed == 3) return CALENDAR_SUCCESS;
    
    return CALENDAR_ERROR_CONVERSION_FAILED;
}

void string_format_date_iso(int day, int month, int year, char* buffer, size_t buffer_size) {
    if (!buffer) return;
    
    snprintf(buffer, buffer_size, "%04d-%02d-%02d", year, month, day);
}

void string_format_date_readable(int day, int month, int year, const char* month_names[], char* buffer, size_t buffer_size) {
    if (!buffer) return;
    
    if (month_names && month >= 1 && month <= 12) {
        snprintf(buffer, buffer_size, "%d %s %d", day, month_names[month - 1], year);
    } else {
        snprintf(buffer, buffer_size, "%d/%d/%d", day, month, year);
    }
}

CalendarResult string_parse_islamic_date(const char* date_str, int* day, int* month, int* year) {
    if (!date_str || !day || !month || !year) {
        return CALENDAR_ERROR_NULL_POINTER;
    }
    
    // Try to parse Islamic date format: "DD Month YYYY AH"
    char month_str[50];
    char ah_str[10];
    
    int parsed = sscanf(date_str, "%d %s %d %s", day, month_str, year, ah_str);
    if (parsed >= 3) {
        // Try to match month name to number
        const char* islamic_months[] = {
            "Muharram", "Safar", "Rabi'", "Jumada", "Rajab", "Sha'ban",
            "Ramadan", "Shawwal", "Dhu", "Dhul"
        };
        
        *month = 1;  // Default
        for (int i = 0; i < 12; i++) {
            if (string_starts_with(month_str, islamic_months[i % 10])) {
                *month = i + 1;
                break;
            }
        }
        
        return CALENDAR_SUCCESS;
    }
    
    // Try numeric format
    return string_parse_date_components(date_str, day, month, year);
}

CalendarResult string_parse_chinese_date(const char* date_str, int* day, int* month, int* year, int* is_leap) {
    if (!date_str || !day || !month || !year || !is_leap) {
        return CALENDAR_ERROR_NULL_POINTER;
    }
    
    *is_leap = 0;  // Default
    
    // Check for leap month indicator
    if (strstr(date_str, "leap") || strstr(date_str, "闰")) {
        *is_leap = 1;
    }
    
    // Parse basic date components
    return string_parse_date_components(date_str, day, month, year);
}

CalendarResult string_read_calendar_data_line(const char* line, char* type, char* data[], int max_fields) {
    if (!line || !type || !data || max_fields <= 0) {
        return CALENDAR_ERROR_NULL_POINTER;
    }
    
    // Skip comment lines
    if (string_is_comment_line(line)) {
        return CALENDAR_ERROR_CONVERSION_FAILED;
    }
    
    // Create a copy to work with
    char* line_copy = string_trim_copy(line);
    if (!line_copy) {
        return CALENDAR_ERROR_MEMORY_ALLOCATION;
    }
    
    // Split line by commas
    char* tokens[20];  // Maximum fields
    int token_count = string_split(line_copy, ',', tokens, 20);
    
    if (token_count > 0) {
        // First field is the type
        string_safe_copy(type, tokens[0], 50);
        
        // Copy remaining fields
        for (int i = 1; i < token_count && i <= max_fields; i++) {
            data[i-1] = malloc(strlen(tokens[i]) + 1);
            if (data[i-1]) {
                strcpy(data[i-1], tokens[i]);
            }
        }
    }
    
    // Clean up tokens
    for (int i = 0; i < token_count; i++) {
        free(tokens[i]);
    }
    free(line_copy);
    
    return token_count > 0 ? CALENDAR_SUCCESS : CALENDAR_ERROR_CONVERSION_FAILED;
}

int string_is_comment_line(const char* line) {
    if (!line) return 1;
    
    char* trimmed = string_trim_copy(line);
    int is_comment = (trimmed[0] == '#' || trimmed[0] == '\0');
    free(trimmed);
    
    return is_comment;
}

int string_utf8_length(const char* str) {
    if (!str) return 0;
    
    int length = 0;
    while (*str) {
        if ((*str & 0x80) == 0) {
            // ASCII character
            str++;
        } else if ((*str & 0xE0) == 0xC0) {
            // 2-byte character
            str += 2;
        } else if ((*str & 0xF0) == 0xE0) {
            // 3-byte character
            str += 3;
        } else if ((*str & 0xF8) == 0xF0) {
            // 4-byte character
            str += 4;
        } else {
            // Invalid UTF-8
            return -1;
        }
        length++;
    }
    
    return length;
}

int string_is_valid_utf8(const char* str) {
    return string_utf8_length(str) >= 0;
}

size_t string_safe_copy(char* dest, const char* src, size_t dest_size) {
    if (!dest || !src || dest_size == 0) return 0;
    
    size_t src_len = strlen(src);
    size_t copy_len = (src_len < dest_size - 1) ? src_len : dest_size - 1;
    
    strncpy(dest, src, copy_len);
    dest[copy_len] = '\0';
    
    return copy_len;
}

size_t string_safe_concat(char* dest, const char* src, size_t dest_size) {
    if (!dest || !src || dest_size == 0) return 0;
    
    size_t dest_len = strlen(dest);
    if (dest_len >= dest_size - 1) return dest_len;
    
    size_t remaining = dest_size - dest_len - 1;
    size_t src_len = strlen(src);
    size_t copy_len = (src_len < remaining) ? src_len : remaining;
    
    strncat(dest, src, copy_len);
    
    return dest_len + copy_len;
}