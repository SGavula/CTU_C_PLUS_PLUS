#include "trie.hpp"
#include "catch.hpp"

/*
   Testy pro stage-01.

   Tyto testy po kouskách zkontrolují metody
     * trie() (základní konstruktor)
     * ~trie() (destruktor)
     * size()
     * empty()
     * insert()
     * contains()
     * erase()
     * trie(std::vector<std::string> const &strings) (konstruktor z vektoru stringů)

   Tyto metody je nutné přidat do projektu, aby bylo možné stage-01 přeložit a spustit.
   V dalších krocích se očekává, že vám metody z tohoto kroku fungují.
*/


namespace {
    void insert_all(trie& t, const std::vector<std::string>& strings) {
        for (const auto& string : strings) {
            t.insert(string);
        }
    }
}

TEST_CASE("Basics: easy insertion", "[stage1]") {
    trie trie;

    SECTION("Empty trie") {
        REQUIRE_FALSE(trie.contains(""));
        REQUIRE(trie.size() == 0);
        REQUIRE(trie.empty());
    }


    SECTION("Trivial inserts 1") {
        REQUIRE(trie.insert(""));
        REQUIRE(trie.contains(""));
        REQUIRE_FALSE(trie.contains("a"));
        REQUIRE(trie.size() == 1);

        REQUIRE_FALSE(trie.empty());
    }

    SECTION("Trivial inserts 1a") {
        REQUIRE(trie.insert(""));
        REQUIRE(trie.contains(""));
        REQUIRE_FALSE(trie.insert(""));
        REQUIRE(trie.size() == 1);
    }

    SECTION("Trivial inserts 2") {
        REQUIRE(trie.insert("a"));
        REQUIRE(trie.contains("a"));
        REQUIRE_FALSE(trie.contains(""));
        REQUIRE(trie.size() == 1);

        REQUIRE(trie.insert(""));
        REQUIRE(trie.contains(""));
        REQUIRE(trie.contains("a"));
        REQUIRE(trie.size() == 2);
        REQUIRE_FALSE(trie.contains("b"));
        REQUIRE_FALSE(trie.contains("ab"));

        REQUIRE_FALSE(trie.empty());
    }
}

TEST_CASE("Basics: inserting a long word", "[stage1]") {
    trie trie;

    SECTION("Long word insertion 1") {
        REQUIRE(trie.insert("abcd"));
        REQUIRE(trie.contains("abcd"));
        REQUIRE_FALSE(trie.contains(""));
        REQUIRE_FALSE(trie.contains("a"));
        REQUIRE_FALSE(trie.contains("ab"));
        REQUIRE_FALSE(trie.contains("abc"));
        REQUIRE(trie.size() == 1);
    }

    SECTION("Long word insertion 2") {
        REQUIRE(trie.insert("abcd"));
        REQUIRE(trie.contains("abcd"));
        REQUIRE(trie.size() == 1);

        REQUIRE(trie.insert("abc"));
        REQUIRE_FALSE(trie.contains(""));
        REQUIRE_FALSE(trie.contains("a"));
        REQUIRE_FALSE(trie.contains("ab"));
        REQUIRE(trie.contains("abc"));
        REQUIRE(trie.size() == 2);

        REQUIRE_FALSE(trie.empty());
    }

    SECTION("Long word insertion 3") {
        REQUIRE(trie.insert("abcd"));
        REQUIRE(trie.contains("abcd"));
        REQUIRE(trie.size() == 1);

        REQUIRE(trie.insert("abc"));
        REQUIRE(trie.contains("abc"));
        REQUIRE(trie.size() == 2);

        REQUIRE(trie.insert("ab"));
        REQUIRE(trie.contains("ab"));
        REQUIRE(trie.size() == 3);

        REQUIRE(trie.insert(""));
        REQUIRE(trie.contains(""));
        REQUIRE_FALSE(trie.contains("a"));
        REQUIRE(trie.size() == 4);

        REQUIRE_FALSE(trie.empty());
    }

    SECTION("Long word insertion 4") {
        REQUIRE(trie.insert("abcd"));
        REQUIRE(trie.contains("abcd"));
        REQUIRE(trie.size() == 1);

        REQUIRE(trie.insert("abce"));
        REQUIRE(trie.contains("abce"));
        REQUIRE(trie.size() == 2);

        REQUIRE_FALSE(trie.contains("a"));
        REQUIRE_FALSE(trie.contains("ab"));
        REQUIRE_FALSE(trie.contains("abc"));
    }
}

TEST_CASE("Complex: inserts", "[stage1]") {
    trie trie;

    SECTION("Repeated inserts do not reinsert given string 1") {
        REQUIRE(trie.insert("abcd"));
        REQUIRE_FALSE(trie.insert("abcd"));
        REQUIRE(trie.insert("abc"));
        REQUIRE_FALSE(trie.insert("abc"));

        REQUIRE(trie.contains("abcd"));
        REQUIRE(trie.contains("abc"));
        REQUIRE(trie.size() == 2);
    }

    SECTION("Repeated inserts do not reinsert given string 2") {
        REQUIRE(trie.insert("ABCD"));
        REQUIRE_FALSE(trie.insert("ABCD"));
        REQUIRE(trie.insert("ABDE"));
        REQUIRE_FALSE(trie.insert("ABDE"));
        REQUIRE(trie.insert("AB"));
        REQUIRE_FALSE(trie.insert("AB"));

        REQUIRE(trie.contains("ABCD"));
        REQUIRE(trie.contains("ABDE"));
        REQUIRE(trie.contains("AB"));
        REQUIRE(trie.size() == 3);
    }

    SECTION("Repeated inserts do not reinsert given string 3") {
        REQUIRE(trie.insert("12345"));
        REQUIRE_FALSE(trie.insert("12345"));
        REQUIRE(trie.insert("#23@"));
        REQUIRE_FALSE(trie.insert("#23@"));
        REQUIRE(trie.insert("(&&)"));
        REQUIRE_FALSE(trie.insert("(&&)"));
        REQUIRE(trie.insert(":-)"));
        REQUIRE_FALSE(trie.insert(":-)"));

        REQUIRE(trie.contains("12345"));
        REQUIRE(trie.contains("#23@"));
        REQUIRE(trie.contains("(&&)"));
        REQUIRE(trie.contains(":-)"));
        REQUIRE(trie.size() == 4);
    }
}

TEST_CASE("Basics: erase", "[stage1]") {
    trie trie;

    SECTION("Trivial erase 1") {
        REQUIRE(trie.insert(""));
        REQUIRE(trie.erase(""));
        REQUIRE_FALSE(trie.contains(""));
        REQUIRE(trie.size() == 0);
        REQUIRE(trie.empty());
    }
    SECTION("Trivial erase 2") {
        REQUIRE(trie.insert("a"));
        REQUIRE(trie.erase("a"));
        REQUIRE_FALSE(trie.contains("a"));
        REQUIRE(trie.size() == 0);
        REQUIRE(trie.empty());

        REQUIRE(trie.insert("bcd"));
        REQUIRE(trie.erase("bcd"));
        REQUIRE_FALSE(trie.contains("bcd"));
        REQUIRE(trie.size() == 0);
        REQUIRE(trie.empty());
    }

    SECTION("Trivial erase 3") {
        REQUIRE(trie.insert(""));
        REQUIRE(trie.insert("a"));
        REQUIRE(trie.erase("a"));
        REQUIRE(trie.contains(""));
        REQUIRE_FALSE(trie.contains("a"));
        REQUIRE(trie.size() == 1);
        REQUIRE_FALSE(trie.empty());
    }

    SECTION("Erase in the middle of a link") {
        insert_all(trie, { "", "a", "ab", "abc", "abcd" });
        REQUIRE(trie.erase("ab"));
        REQUIRE(trie.size() == 4);
        REQUIRE_FALSE(trie.contains("ab"));

        REQUIRE(trie.erase("abc"));
        REQUIRE(trie.size() == 3);
        REQUIRE_FALSE(trie.contains("abc"));

        REQUIRE(trie.contains("abcd"));
    }

    SECTION("Erase in the middle of a trie") {
        insert_all(trie, { "", "aa", "ac", "acb", "acc", "be", "bfa", "bfb", "bg" });
        REQUIRE(trie.size() == 9);
        REQUIRE_FALSE(trie.erase("bf"));
        REQUIRE(trie.size() == 9);
        REQUIRE(trie.erase("bfb"));
        REQUIRE(trie.size() == 8);
        REQUIRE_FALSE(trie.contains("bfb"));
        REQUIRE(trie.contains("bfa"));
        REQUIRE(trie.contains("be"));

        REQUIRE(trie.erase("acb"));
        REQUIRE(trie.size() == 7);
        REQUIRE_FALSE(trie.contains("acb"));
        REQUIRE(trie.contains("acc"));
        REQUIRE(trie.contains("ac"));
    }
}

TEST_CASE("Complex: repeated interaction", "[stage1]") {
    trie trie;

    SECTION("Reinserting and reerasing the same element works properly 1") {
        insert_all(trie, { "", "a", "aa", "ab", "ac" });
        REQUIRE(trie.erase("a"));
        REQUIRE_FALSE(trie.contains("a"));
        REQUIRE(trie.insert("a"));
        REQUIRE(trie.contains("a"));
        REQUIRE(trie.erase("a"));
        REQUIRE_FALSE(trie.erase("a"));
        REQUIRE(trie.contains("aa"));
        REQUIRE(trie.contains("ab"));
        REQUIRE(trie.contains("ac"));

        REQUIRE(trie.size() == 4);
    }

    SECTION("Reinserting and reerasing the same element works properly 2") {
        insert_all(trie, { "Veselo", "Vesela", "Veselovsky", "Vesely", "Veselovska", "Novak" });
        REQUIRE(trie.erase("Veselovsky"));
        REQUIRE_FALSE(trie.contains("Veselovsky"));
        REQUIRE(trie.insert("Veselovsky"));
        REQUIRE(trie.contains("Veselovsky"));
        REQUIRE(trie.erase("Veselovska"));
        REQUIRE(trie.erase("Veselovsky"));
        REQUIRE_FALSE(trie.contains("Vesel"));
        REQUIRE(trie.contains("Vesela"));
        REQUIRE(trie.contains("Vesely"));
        REQUIRE(trie.contains("Veselo"));

        REQUIRE(trie.erase("Veselo"));
        REQUIRE_FALSE(trie.contains("Vesel"));
        REQUIRE(trie.contains("Vesela"));
        REQUIRE(trie.contains("Vesely"));
        REQUIRE_FALSE(trie.contains("Veselo"));

        REQUIRE(trie.contains("Novak"));
        REQUIRE(trie.size() == 3);
    }
}

TEST_CASE("Vector constructor", "[stage1]") {
    trie trie({ "abc", "bc", "a", "bc", "d", "", "d", "abcd", "abc" });
    REQUIRE(trie.size() == 6);
    REQUIRE(trie.contains(""));
    REQUIRE(trie.contains("a"));
    REQUIRE(trie.contains("d"));
    REQUIRE_FALSE(trie.contains("b"));
    REQUIRE(trie.contains("bc"));
    REQUIRE_FALSE(trie.contains("ab"));
    REQUIRE(trie.contains("abc"));
    REQUIRE(trie.contains("abcd"));
}

TEST_CASE("Destructor", "[stage1]") {
    std::vector<std::string> vec{ "abc", "a", "bc", "da", "", "d", "abcd", "bcd" };

    SECTION("Destruction of trie") {
        trie* tree = new trie(vec);
        REQUIRE(tree->size() == 8);
        REQUIRE(tree->contains("da"));
        delete tree;
        tree = new trie({"a", "b", "c"});
        REQUIRE(tree->size() == 3);
        REQUIRE_FALSE(tree->empty());
        REQUIRE(tree->contains("a"));
        REQUIRE_FALSE(tree->contains(""));
        REQUIRE_FALSE(tree->contains("da"));
        delete tree;
    }

    SECTION("Deletion of trie") {
        trie* tree = new trie(vec);
        size_t count = tree->size();
        REQUIRE(count == 8);
        for (size_t i = 0; i < count; ++i) {
            tree->erase(vec[i]);
            REQUIRE(tree->size() == count - i - 1);
        };
        REQUIRE(tree->empty());
        REQUIRE(tree->size() == 0);
        tree->insert("Hotovo");
        REQUIRE_FALSE(tree->empty());
        REQUIRE(tree->size() == 1);
        delete tree;
    }

}
