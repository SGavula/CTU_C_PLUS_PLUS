#include "trie.hpp"
#include "test-helpers.hpp"
#include "catch.hpp"

/*
    Testy pro stage-04.

    Tento krok testuje
      * search_by_prefix
      * get_prefixes
      * sjednocení trií (operator|)
      * průnik trií (operator&)

    Testy v tomto kroku předpokládají, že metody testované v minulém
    kroku fungují správně, a tudíž se na nich mohou stavět testy pro další
    funkcionalitu.
*/


TEST_CASE("Basics: Search by prefix", "[search_by_prefix]") {
    trie trie;
    insert_all(trie, { "a", "aa", "aaa", "aabb", "aabab", "aaaab", "aaqqq" });
    SECTION("Search is inclusive 1") {
        VALIDATE_SETS(trie.search_by_prefix("aa"), as_vec({ "aa", "aaa", "aabb", "aabab", "aaaab", "aaqqq" }));
    }
    SECTION("Search is inclusive 2") {
        VALIDATE_SETS(trie.search_by_prefix("aaa"), as_vec({ "aaa", "aaaab" }));
    }
    SECTION("Prefix not in the trie 1") {
        VALIDATE_SETS(trie.search_by_prefix(""), extract_all(trie));
    }
    SECTION("Prefix not in the trie 2") {
        VALIDATE_SETS(trie.search_by_prefix("ab"), as_vec({}));
    }
    SECTION("Prefix not in the trie 3") {
        VALIDATE_SETS(trie.search_by_prefix("aaaa"), as_vec({ "aaaab" }));
    }
}

TEST_CASE("Complex: Search by prefix", "[search_by_prefix]") {
    trie trie;
    insert_all(trie, { "a", "aa", "aaa", "aabb", "aabab", "aaaab", "aaqqq", "b", "bb", "bbb", "bbaba", "bbbba", "bbxxx" });
    SECTION("Search is inclusive 1") {
        VALIDATE_SETS(trie.search_by_prefix("bb"), as_vec({ "bb", "bbb", "bbaba", "bbbba", "bbxxx" }));
    }
    SECTION("Search is inclusive 2") {
        VALIDATE_SETS(trie.search_by_prefix("bbb"), as_vec({ "bbb", "bbbba" }));
    }
    SECTION("Prefix not in the trie 1") {
        VALIDATE_SETS(trie.search_by_prefix(""), extract_all(trie));
    }
    SECTION("Prefix not in the trie 2") {
        VALIDATE_SETS(trie.search_by_prefix("ba"), as_vec({}));
    }
    SECTION("Prefix not in the trie 3") {
        VALIDATE_SETS(trie.search_by_prefix("bbbb"), as_vec({ "bbbba" }));
    }
}

TEST_CASE("Basics: Get prefixes", "[get_prefixes]") {
    trie trie;
    insert_all(trie, { "a", "aa", "aaa", "aabb", "aabab", "aaaab", "aaqqq" });
    SECTION("Search is inclusive") {
        VALIDATE_SETS(trie.get_prefixes("aabab"), as_vec({ "aa", "a", "aabab" }));
    }
    SECTION("Input is not in the trie") {
        VALIDATE_SETS(trie.get_prefixes("aaaaa"), as_vec({ "a", "aa", "aaa" }));
    }
}

TEST_CASE("Complex: Get prefixes", "[get_prefixes]") {
    trie trie;
    insert_all(trie, { "", "a", "aa", "aaa", "aabb", "aabab", "aaaab", "aaqqq", "b", "bb", "bbb", "bbaba", "bbbba", "bbxxx" });
    SECTION("Search is inclusive 1") {
        VALIDATE_SETS(trie.get_prefixes("bbxxx"), as_vec({ "", "bb", "b", "bbxxx" }));
    }
    SECTION("Search is inclusive 1") {
        VALIDATE_SETS(trie.get_prefixes("aaaab"), as_vec({ "", "a", "aa", "aaa", "aaaab" }));
    }
    SECTION("Input is not in the trie") {
        VALIDATE_SETS(trie.get_prefixes("bbbbb"), as_vec({ "b", "bb", "bbb", "" }));
    }
}

TEST_CASE("Trie union", "[union]") {
    trie t1, t2;
    SECTION("Empty tries") {
        auto res = t1 | t2;
        CHECK(res.empty());
        REQUIRE(res.size() == 0);
    }
    SECTION("One trie empty") {
        t1 = trie({ "aa", "aab", "aabb", "zzz" });
        SECTION("LHS trie empty") {
            auto res = t2 | t1;
            CHECK(res.size() == 4);
            REQUIRE(res == t1);
        }
        SECTION("RHS trie empty") {
            auto res = t1 | t2;
            CHECK(res.size() == 4);
            REQUIRE(res == t1);
        }
    }

    SECTION("No empty tries") {
        t1 = trie({ "queue", "quiz", "quizzical", "quilt" });
        SECTION("Identical tries") {
            auto res = t1 | t1;
            CHECK(res.size() == 4);
            REQUIRE(res == t1);
        }
        SECTION("Overlapping tries") {
            t2 = trie({ "quilt", "queue", "quitter" });
            auto res = t1 | t2;
            CHECK(res.size() == 5);
            REQUIRE(res == trie({ "queue", "quiz", "quizzical", "quilt", "quitter" }));
        }
        SECTION("Distinct tries") {
            t2 = trie({ "abra", "kadabra","abrakadabra" });
            auto res = t1 | t2;
            CHECK(res.size() == 7);
            REQUIRE(res == trie({ "queue", "quiz", "quizzical", "quilt", "abra", "kadabra", "abrakadabra" }));
        }
    }
    SECTION("Chained unions") {
        t1 = trie({ "aaa", "bbb", "ccc", "eee" });
        t2 = trie({ "bbb", "ccc", "ddd" });
        trie t3({ "aaa", "eee", "fff", "ggg" });
        trie t4({ "ggg", "hhh", "iii", "ccc" });
        auto res = t1 | t2 | t3 | t4;
        CHECK(res.size() == 9);
        REQUIRE(res == trie({ "aaa", "bbb", "ccc", "ddd", "eee", "fff", "ggg", "hhh" ,"iii" }));
    }
    SECTION("Correct tries") {
        t1 = trie({ "queue", "quiz", "quizzical", "quilt" });
        SECTION("Identical tries") {
            auto res = t1 | t1;
            CHECK(res.size() == 4);
            REQUIRE(res == t1);
            res.insert("zzz");
            CHECK(res.size() == 5);
            REQUIRE(res == trie({ "queue", "quiz", "quizzical", "quilt", "zzz" }));
            res.erase("queue");
            CHECK(res.size() == 4);
            REQUIRE(res == trie({ "quiz", "quizzical", "quilt", "zzz" }));
            res = res | t1;
            CHECK(res.size() == 5);
            REQUIRE(res == trie({ "queue", "quiz", "quizzical", "quilt", "zzz" }));
        }
        SECTION("Overlapping tries") {
            t2 = trie({ "quilt", "queue", "quitter" });
            auto res = t1 | t2;
            CHECK(res.size() == 5);
            REQUIRE(res == trie({ "queue", "quiz", "quizzical", "quilt", "quitter" }));
            res.insert("zzz");
            CHECK(res.size() == 6);
            REQUIRE(res == trie({ "queue", "quiz", "quizzical", "quilt", "quitter", "zzz" }));
            res.erase("queue");
            CHECK(res.size() == 5);
            REQUIRE(res == trie({ "quiz", "quizzical", "quilt", "quitter", "zzz" }));
        }
        SECTION("Distinct tries") {
            t2 = trie({"abra", "kadabra", "abrakadabra"});
            auto res = t1 | t2;
            CHECK(res.size() == 7);
            REQUIRE(res == trie({"queue", "quiz", "quizzical", "quilt", "abra", "kadabra", "abrakadabra"}));
            res.insert("abb");
            res.insert("kabb");
            CHECK(res.size() == 9);
            REQUIRE(res ==
                    trie({"queue", "quiz", "quizzical", "quilt", "abra", "kadabra", "abrakadabra", "abb", "kabb"}));
            res.erase("queue");
            res.erase("kadabra");
            CHECK(res.size() == 7);
            REQUIRE(res == trie({"quiz", "quizzical", "quilt", "abra", "abrakadabra", "abb", "kabb"}));
            res = res | t2;
            CHECK(res.size() == 8);
            REQUIRE(res == trie({"quiz", "quizzical", "quilt", "abra", "kadabra", "abrakadabra", "abb", "kabb"}));
        }
    }
}

TEST_CASE("Trie intersection", "[intersection]") {
    trie t1, t2;
    SECTION("Empty tries") {
        auto res = t1 & t2;
        CHECK(res.empty());
        REQUIRE(res.size() == 0);
    }

    SECTION("One trie empty") {
        t1 = trie({ "aa", "aab", "aabb", "zzz" });
        SECTION("LHS trie empty") {
            auto res = t2 & t1;
            CHECK(res.size() == 0);
            REQUIRE(res == t2);
        }
        SECTION("RHS trie empty") {
            auto res = t1 & t2;
            CHECK(res.size() == 0);
            REQUIRE(res == t2);
        }
    }

    SECTION("No empty tries") {
        t1 = trie({ "queue", "quiz", "quizzical", "quilt" });
        SECTION("Identical tries") {
            auto res = t1 & t1;
            CHECK(res.size() == 4);
            REQUIRE(res == t1);
        }
        SECTION("Overlapping tries") {
            t2 = trie({ "quilt", "queue", "quitter" });
            auto res = t1 & t2;
            CHECK(res.size() == 2);
            REQUIRE(res == trie({ "queue", "quilt" }));
        }
        SECTION("Distinct tries") {
            t2 = trie({ "abra", "kadabra","abrakadabra" });
            auto res = t1 & t2;
            CHECK(res.size() == 0);
            REQUIRE(res == trie{});
        }
    }
    SECTION("Chained intersections") {
        t1 = trie({ "aaa", "bbb", "ccc", "ddd" });
        t2 = trie({ "bbb", "ccc", "ddd", "eee" });
        trie t3({ "ccc", "ddd", "eee", "fff" });
        trie t4({ "ddd", "eee", "fff", "ggg" });
        auto res = t1 & t2 & t3 & t4;
        CHECK(res.size() == 1);
        REQUIRE(res == trie({ "ddd" }));
    }
    SECTION("Correct tries") {
        t1 = trie({ "queue", "quiz", "quizzical", "quilt" });
        SECTION("Identical tries") {
            auto res = t1 & t1;
            CHECK(res.size() == 4);
            REQUIRE(res == t1);
            res.insert("zzz");
            CHECK(res.size() == 5);
            REQUIRE(res == trie({ "queue", "quiz", "quizzical", "quilt", "zzz" }));
            res.erase("queue");
            CHECK(res.size() == 4);
            REQUIRE(res == trie({ "quiz", "quizzical", "quilt", "zzz" }));
            res = res | t1;
            CHECK(res.size() == 5);
            REQUIRE(res == trie({ "queue", "quiz", "quizzical", "quilt", "zzz" }));
        }
        SECTION("Overlapping tries") {
            t2 = trie({ "quilt", "queue", "quitter" });
            auto res = t1 & t2;
            CHECK(res.size() == 2);
            REQUIRE(res == trie({ "queue", "quilt" }));
            res.insert("zzz");
            CHECK(res.size() == 3);
            REQUIRE(res == trie({ "queue", "quilt", "zzz" }));
            res.erase("queue");
            CHECK(res.size() == 2);
            REQUIRE(res == trie({ "quilt", "zzz" }));
        }
        SECTION("Distinct tries") {
            t2 = trie({ "abra", "kadabra","abrakadabra" });
            auto res = t1 & t2;
            CHECK(res.size() == 0);
            REQUIRE(res == trie{});
            res.insert("abb");
            res.insert("kadabra");
            CHECK(res.size() == 2);
            REQUIRE(res == trie({ "abb", "kadabra" }));
            res.erase("queue");
            CHECK(res.size() == 2);
            REQUIRE(res == trie({ "abb", "kadabra"}));
            res = res & t2;
            CHECK(res.size() == 1);
            REQUIRE(res == trie({ "kadabra" }));
        }
    }
}
