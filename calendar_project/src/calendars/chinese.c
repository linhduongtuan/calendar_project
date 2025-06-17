#include "chinese.h"
#include "../utils/date_utils.h"

// Chinese zodiac animals
const char* chinese_zodiac[CHINESE_ZODIAC_COUNT] = {
    "Rat", "Ox", "Tiger", "Rabbit", "Dragon", "Snake",
    "Horse", "Goat", "Monkey", "Rooster", "Dog", "Pig"
};

// Chinese heavenly stems
const char* heavenly_stems[CHINESE_STEMS_COUNT] = {
    "Jia", "Yi", "Bing", "Ding", "Wu", "Ji",
    "Geng", "Xin", "Ren", "Gui"
};

// Chinese earthly branches
const char* earthly_branches[CHINESE_BRANCHES_COUNT] = {
    "Zi", "Chou", "Yin", "Mao", "Chen", "Si",
    "Wu", "Wei", "Shen", "You", "Xu", "Hai"
};

const char* chinese_get_zodiac(int year) {
    // Chinese zodiac cycle starts from 1924 (Year of Rat)
    int zodiac_index = modulo(year - 1924, 12);
    return chinese_zodiac[zodiac_index];
}

void chinese_get_stem_branch(int year, char* result, size_t buffer_size) {
    if (!result) return;
    
    int stem_index = modulo(year - 1924, 10);
    int branch_index = modulo(year - 1924, 12);
    
    snprintf(result, buffer_size, "%s-%s", 
             heavenly_stems[stem_index], 
             earthly_branches[branch_index]);
}

int chinese_get_cycle_year(int year) {
    return modulo(year - 1924, 60) + 1;
}

ChineseDate* chinese_create_date(int day, int month, int year) {
    ChineseDate* date = malloc(sizeof(ChineseDate));
    if (!date) {
        return NULL;
    }
    
    date->base.day = day;
    date->base.month = month;
    date->base.year = year;
    date->leap_month = 0;  // Simplified - real implementation needs complex calculations
    strcpy(date->zodiac, chinese_get_zodiac(year));
    chinese_get_stem_branch(year, date->stem_branch, sizeof(date->stem_branch));
    date->cycle_year = chinese_get_cycle_year(year);
    
    return date;
}

void chinese_destroy_date(ChineseDate* date) {
    if (date) {
        free(date);
    }
}

void chinese_print_date(const ChineseDate* date) {
    if (!date) {
        printf("NULL date\n");
        return;
    }
    
    printf("Chinese: %d-%d-%d, Year of the %s (%s), Cycle year %d\n",
           date->base.year,
           date->base.month,
           date->base.day,
           date->zodiac,
           date->stem_branch,
           date->cycle_year);
}

CalendarResult chinese_from_gregorian(const GregorianDate* greg_date, ChineseDate* chinese_date) {
    if (!greg_date || !chinese_date) {
        return CALENDAR_ERROR_NULL_POINTER;
    }
    
    // Very simplified conversion - real implementation much more complex
    chinese_date->base.day = greg_date->base.day;
    chinese_date->base.month = greg_date->base.month;
    chinese_date->base.year = greg_date->base.year;
    chinese_date->leap_month = 0;
    strcpy(chinese_date->zodiac, chinese_get_zodiac(greg_date->base.year));
    chinese_get_stem_branch(greg_date->base.year, chinese_date->stem_branch, sizeof(chinese_date->stem_branch));
    chinese_date->cycle_year = chinese_get_cycle_year(greg_date->base.year);
    
    return CALENDAR_SUCCESS;
}