#include <check.h>
#include "dictionary.h"
// #include "spell.c"
#include <stdlib.h>

#define DICTIONARY "wordlist.txt"
#define TESTDICT "test_wordlist.txt"

START_TEST(test_dictionary_normal)
{
    hashmap_t hashtable[HASH_SIZE];
    ck_assert(load_dictionary(TESTDICT, hashtable));
    // Here we can test if certain words ended up in certain buckets
    // to ensure that our load_dictionary works as intended. I leave
    // this as an exercise.
}
END_TEST

START_TEST(test_check_word_normal)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    const char* correct_word = "Justice";
    const char* punctuation_word_2 = "pl.ace";
    const char* quotation_word = "\"hello\"";
    ck_assert(check_word(correct_word, hashtable));
    ck_assert(!check_word(punctuation_word_2, hashtable));
    ck_assert(!check_word(quotation_word, hashtable));
}
END_TEST

START_TEST(test_check_words_normal)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    char* expected[3];
    expected[0] = "sogn";
    expected[1] = "skyn";
    expected[2] = "betta";
    char *misspelled[MAX_MISSPELLED];
    FILE *fp = fopen("test1.txt", "r");
    int num_misspelled = check_words(fp, hashtable, misspelled);
    ck_assert(num_misspelled == 3);
    bool test = strlen(misspelled[0]) == strlen(expected[0]);
    int len1 = strlen(misspelled[0]);
    int len2 = strlen(expected[0]);
    ck_assert_msg(test, "%d!=%d", len1, len2);
    ck_assert_msg(strcmp(misspelled[0], expected[0]) == 0);
    ck_assert_msg(strcmp(misspelled[1], expected[1]) == 0);
    ck_assert_msg(strcmp(misspelled[2], expected[2]) == 0);
}
END_TEST

// START_TEST(test_remove_punctuation_and_uppercase_characters) 
// {
//     char * correct_word = "hello";
//     char * word_with_punctuation = "hell.o!";
//     char * word_with_uppercase = "Hello";
//     char * word_with_both = "Hello!";
//     char * returned_word_punctuation_test = remove_punctuation_and_uppercase_characters(word_with_punctuation);
//     char * returned_word_uppercase_test = remove_punctuation_and_uppercase_characters(word_with_uppercase);
//     char * returned_word_both_test = remove_punctuation_and_uppercase_characters(word_with_both);
//     ck_assert_msg(strcmp(correct_word, returned_word_punctuation_test), "%s!=%s", correct_word, returned_word_punctuation_test);
//     ck_assert_msg(strcmp(correct_word, returned_word_uppercase_test), "%s!=%s", correct_word, returned_word_uppercase_test);
//     ck_assert_msg(strcmp(correct_word, returned_word_both_test), "%s!=%s", correct_word, returned_word_both_test);
// }
// END_TEST
Suite *
check_word_suite(void)
{
    Suite * suite;
    TCase * check_word_case;
    suite = suite_create("check_word");
    check_word_case = tcase_create("Core");
    tcase_add_test(check_word_case, test_check_word_normal);
    tcase_add_test(check_word_case, test_check_words_normal);
    // tcase_add_test(check_word_case, test_remove_punctuation_and_uppercase_characters);
    suite_add_tcase(suite, check_word_case);

    return suite;
}


int
main(void)
{
    int failed;
    Suite *suite;
    SRunner *runner;
    
    suite = check_word_suite();
    runner = srunner_create(suite);
    srunner_run_all(runner, CK_NORMAL);
    failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    return (failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}