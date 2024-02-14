#include <iostream>
#include <map>
#include <string>
#include <optional>
#include <cassert>
#include <io.h>

using StringRef = const std::string &;

void warning(StringRef msg) {
    std::cout << "[WARNING] " << msg << std::endl;
}


class PhoneBook {
public:

    PhoneBook() = default;

    explicit PhoneBook(const std::map<std::string, std::string> &dict) {
        _lookup = dict;
        for (const auto &[name, phone_number]: dict) {
            _reverse_lookup[phone_number] = name;
        }
    }


    /// При перезаписи возвращает true и логирует это
    bool add(StringRef name, StringRef phone_number) {
        bool warn = false;
        if (_lookup.contains(name)) {
            warning(name + "(" + phone_number + ") already in book");
            warn = true;
        }

        _lookup[name] = phone_number;
        _reverse_lookup[phone_number] = name;

        return warn;
    }

    bool remove(StringRef name) {
        if (!_lookup.contains(name)) return false;

        std::string phone_number = _lookup[name];
        _lookup.erase(name);
        return _reverse_lookup.erase(phone_number);
    }


    [[nodiscard]] std::optional<std::string> search_by_name(StringRef name) const {
        return common_search(_lookup, name);

    }

    [[nodiscard]] std::optional<std::string> search_by_phone_number(StringRef phone_number) const {
        return common_search(_reverse_lookup, phone_number);
    }


    void print() {
        for (const auto &[name, phone_number]: _lookup) {
            std::cout << name << " - " << phone_number << std::endl;
        }
    }

private:

    static std::optional<std::string>
    common_search(const std::map<std::string, std::string> &data, const std::string &key) {
        auto search = data.find(key);

        if (search == data.end()) return std::nullopt;

        return std::optional<std::string>{search->second};
    }


    /// mapping name->phone number and reversed
    std::map<std::string, std::string> _lookup, _reverse_lookup;
};

PhoneBook get_test_data() {
    return PhoneBook(
            {
                    {"Amanda",      "466-768-4109x5156"},
                    {"Amber",       "362-554-5167"},
                    {"Amy",         "(804)378-6103"},
                    {"Ann",         "759-631-5160x969"},
                    {"Ashley",      "988.392.0816x0561"},
                    {"Austin",      "001-590-273-7515x68464"},
                    {"Brandon",     "+1-664-734-8591x887"},
                    {"Cameron",     "(701)870-0222x5319"},
                    {"Cassandra",   "001-444-504-4840x833"},
                    {"Charles",     "(521)718-8883x658"},
                    {"Christian",   "440-573-9266x496"},
                    {"Christopher", "514.414.1414x8121"},
                    {"Clayton",     "868-912-2076x738"},
                    {"Dana",        "833-416-2777x950"},
                    {"Dawn",        "674-969-3905x5604"},
                    {"Debbie",      "+1-368-610-7161"},
                    {"Denise",      "001-672-542-7477x720"},
                    {"Eduardo",     "(976)347-0816"},
                    {"Edward",      "530.604.6149x937"},
                    {"Edwin",       "616-615-7251x12853"},
                    {"Elizabeth",   "(460)897-4523x0209"},
                    {"Emily",       "+1-738-667-7116x42413"},
                    {"Eric",        "9902178859"},
                    {"Frank",       "281-514-7566"},
                    {"George",      "601.947.6970"},
                    {"Guy",         "753-894-0157"},
                    {"Jacob",       "(263)860-3574x243"},
                    {"James",       "+1-331-829-7468x19655"},
                    {"Jasmine",     "(769)644-6604"},
                    {"Jason",       "001-826-832-7810x1876"},
                    {"Jeffrey",     "935.823.3871x9259"},
                    {"Jennifer",    "2079289150"},
                    {"Jessica",     "+1-721-864-4029x0933"},
                    {"Juan",        "001-776-858-6805x0523"},
                    {"Katherine",   "001-593-211-8585x64993"},
                    {"Kathryn",     "(786)904-7443x0969"},
                    {"Kelly",       "+1-863-733-3652x01338"},
                    {"Kim",         "889-774-2447"},
                    {"Kristen",     "400-551-0301x039"},
                    {"Lisa",        "(801)377-8843"},
                    {"Lynn",        "001-538-326-0657"},
                    {"Mark",        "802.832.7876x159"},
                    {"Mary",        "432.414.0103"},
                    {"Matthew",     "811-342-3128"},
                    {"Maurice",     "001-700-602-7608"},
                    {"Michael",     "+1-297-398-2447x4608"},
                    {"Michele",     "838-970-4905"},
                    {"Michelle",    "9479803254"},
                    {"Monica",      "+1-431-528-6550"},
                    {"Morgan",      "(785)901-0340x1932"},
                    {"Natasha",     "(785)853-5266x6615"},
                    {"Oscar",       "8968211741"},
                    {"Patricia",    "001-681-363-6402x633"},
                    {"Phillip",     "766-955-9504x6685"},
                    {"Priscilla",   "347.339.4941x99951"},
                    {"Randall",     "399-629-8870"},
                    {"Randy",       "+1-637-980-1803x78244"},
                    {"Robert",      "564-271-8612"},
                    {"Robin",       "+1-475-298-4777"},
                    {"Ryan",        "+1-562-995-7435"},
                    {"Samantha",    "(431)953-7137x37417"},
                    {"Sarah",       "290.705.4587"},
                    {"Scott",       "(314)329-1091x934"},
                    {"Sean",        "468.286.1917"},
                    {"Stephen",     "200-585-4713x09665"},
                    {"Steven",      "+1-717-804-7210x63493"},
                    {"Susan",       "+1-221-884-2540x46446"},
                    {"Tanner",      "796.678.2912x845"},
                    {"Tony",        "708-271-7627x48058"},
                    {"Valerie",     "939-461-7085x56341"},
                    {"Veronica",    "+1-279-415-7572x37003"},
                    {"William",     "(624)289-1909"},
                    {"John",        "758-840-6809"},
            }
    );
}

void test_print() {
    PhoneBook book = get_test_data();
    book.print();

}

void test_search_by_name() {
    PhoneBook book = get_test_data();


    auto s1 = book.search_by_name("John");
    assert(s1.has_value());
    assert(s1.value() == "758-840-6809");

    s1 = book.search_by_name("Maurice");
    assert(s1.has_value());
    assert(s1.value() == "001-700-602-7608");

    s1 = book.search_by_name("Amanda");
    assert(s1.has_value());
    assert(s1.value() == "466-768-4109x5156");

    s1 = book.search_by_name("iurync394m8mry3984");
    assert(s1->empty());
}

void test_search_by_phone_number() {
    PhoneBook book = get_test_data();

    auto s1 = book.search_by_phone_number("758-840-6809");
    assert(s1.has_value());
    assert(s1.value() == "John");

    s1 = book.search_by_phone_number("001-700-602-7608");
    assert(s1.has_value());
    assert(s1.value() == "Maurice");

    s1 = book.search_by_phone_number("466-768-4109x5156");
    assert(s1.has_value());
    assert(s1.value() == "Amanda");

    s1 = book.search_by_phone_number("iurync394m8mry3984");
    assert(s1->empty());
}

void test_add() {
    PhoneBook book;

    bool warn;
    warn = book.add("Test1", "+123");
    assert(!warn);

    warn = book.add("Test2", "+123");
    assert(!warn);

    warn = book.add("Test1", "+123");
    assert(warn);

    warn = book.add("Test3", "+123");
    assert(!warn);
}

void test_remove() {
    PhoneBook book;

    book.add("Test1", "+123");
    book.add("Test2", "+123");
    book.add("Test3", "+123");


    auto contains = book.search_by_name("Test1");
    assert(contains.has_value());

    bool deleted = book.remove("Test1");
    assert(deleted);
    contains = book.search_by_name("Test1");
    assert(!contains.has_value());
    deleted = book.remove("Test1");
    assert(!deleted);

    contains = book.search_by_name("Test2");
    assert(contains.has_value());
    contains = book.search_by_name("Test3");
    assert(contains.has_value());

    book.add("Test1", "+3234");
    contains = book.search_by_name("Test1");
    assert(contains.has_value());

}

int main() {
    test_print();
    test_search_by_name();
    test_search_by_phone_number();
    test_add();
    test_remove();
}