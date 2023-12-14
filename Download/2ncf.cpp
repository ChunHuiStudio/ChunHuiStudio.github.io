#define GetValue 0
#include <string>
#include <iostream>
#include <sstream>
using namespace std;
class BigNumber {
public:
    string _numberString;
    constexpr BigNumber(string number);
    BigNumber(long long number);
    constexpr BigNumber add(BigNumber other);
    constexpr BigNumber subtract(BigNumber other);
    constexpr BigNumber multiply(BigNumber other);
    constexpr BigNumber pow(unsigned long long exponent);
    constexpr string getString();
    constexpr BigNumber setString(const string &newStr);
    constexpr BigNumber trimLeadingZeros();
    constexpr bool equals(const BigNumber &other);
    constexpr bool equals(const long long &other);    
    constexpr friend bool operator==(BigNumber b1, const BigNumber &b2);
    constexpr friend bool operator==(BigNumber b1, const long long &b2);
    constexpr friend bool operator>(BigNumber b1, const BigNumber &b2);
    constexpr friend bool operator<(BigNumber b1, const BigNumber &b2);
    constexpr BigNumber& operator=(const BigNumber &other);
    constexpr BigNumber& operator+=(const BigNumber &other);
    constexpr unsigned int operator[](int index);
};
constexpr BigNumber::BigNumber(string number) : _numberString(number.c_str()){}
BigNumber::BigNumber(long long number) : _numberString(to_string(number).c_str()){}
constexpr BigNumber BigNumber::add(BigNumber other) {
    BigNumber b1 = other > *this ? other : *this;
    BigNumber b2 = other > *this ? *this : other;
    string results;
    int carry = 0;
    int diff = int(b1._numberString.size() - b2._numberString.size());
    for (int i = 0; i < diff; ++i)
        b2._numberString.insert(b2._numberString.begin(), '0');
    for (int i = int(b1._numberString.size() - 1); i >= 0; --i) {
        int sum = (b1._numberString[i] - '0') + (b2._numberString[i] - '0') + carry;
        carry = 0;
        if (sum <= 9 || i == 0)
            results.insert(0, to_string(sum));
        else {
            results.insert(0, to_string(sum % 10));
            carry = 1;
        }
    }
    return BigNumber(results);
}
constexpr BigNumber BigNumber::subtract(BigNumber other) {
    BigNumber b1 = *this, b2 = other;
    string results;
    int n = 0, p = 0;
    bool takeOffOne = false;
    bool shouldBeTen = false;
    if (b1 < b2) return 0;
    if (b1._numberString.size() - b2.getString().size() > 1) {
        for (unsigned long i = 0; i < b1._numberString.size() - b2.getString().size() - 1; ++i)
            b2._numberString.insert(b2._numberString.begin(), '0');
    }
    int i = int(b1._numberString.size() - 1);
    for (int j = int(b2._numberString.size() - 1); j >= 0; --j,--i,n=0) {
        if (((b1._numberString[i] - '0') < (b2._numberString[j] - '0')) && i > 0) {
            n = char((b1._numberString[i] - '0') + 10);
            takeOffOne = true;
            if (j > 0 || b1._numberString[i - 1] != '0') {
                p = char((b1._numberString[i - 1] - '0') - 1);
                if (p == -1) {
                    p = 9;
                    shouldBeTen = true;
                }
                takeOffOne = false;
            }
            if (shouldBeTen) {
                int index = i - 1;
                for (int a = i - 1; (b1._numberString[a] - '0') == 0; --a) {
                    b1._numberString[a] = static_cast<char>(p + '0');
                    --index;
                }
                int t = (b1._numberString[index] - '0') - 1;
                b1._numberString[index] = static_cast<char>(t + '0');
            }
            b1._numberString[i - 1] = static_cast<char>(p + '0');
            shouldBeTen = false;
        }
        stringstream ss;
        if (((b1._numberString[i] - '0') == (b2._numberString[j] - '0')))
            ss << "0";
        else {
            if (n <= 0)
                ss << ((b1._numberString[i] - '0') - (b2._numberString[j] - '0'));
            else
                ss << (n - (b2._numberString[j] - '0'));
        }
        results.insert(0, ss.str());
    }
    if (takeOffOne) {
        string number = "";
        for (int j = b1._numberString.length() - b2._numberString.length() - 1; j >= 0; --j) {
            if (b1._numberString[j] == '0') {
                number += "0";
                continue;
            }
            else {
                number.insert(number.begin(), b1._numberString[j]);
                int t = atoi(number.c_str());
                --t;
                b1._numberString.replace(0, number.size(), to_string(t));
                break;
            }
        }
    }
    while (i >= 0) {
        stringstream ss;
        if (i == 0) {
            if (b1._numberString[i] - '0' != 0) {
                ss << (b1._numberString[i] - '0');
                results.insert(0, ss.str());
            }
        }
        else {
            ss << (b1._numberString[i] - '0');
            results.insert(0, ss.str());
        }

        --i;
    }
    if (results.find_first_not_of('0') == string::npos) {
        results = "0";
    }
    else if (results[0] == '0') {
        int index = results.find_first_not_of('0');
        results = results.substr(index, results.length() - 1);
    }
    return BigNumber(results);
}
constexpr BigNumber BigNumber::multiply(BigNumber other) {
    if (*this == 0 || other == 0) return 0;
    if (*this == 1) return other;
    if (other == 1) return *this;
    BigNumber b1 = other > *this ? other : *this;
    BigNumber b2 = other > *this ? *this : other;
    int carry = 0;
    int zeroCounter = 0;
    BigNumber b = 0;
    for (unsigned int i = 0; i < b1._numberString.size() - b2._numberString.size(); ++i)
        b2._numberString.insert(b2._numberString.begin(), '0');
    for (long long int i = (b2._numberString.size() - 1); i >= 0; --i,++zeroCounter) {
        string rr;
        for (long long int j = int(b1._numberString.size() - 1); j >= 0; --j) {
            int val = ((b2._numberString[i] - '0') * (b1._numberString[j] - '0')) + carry;
            carry = 0;
            if (val > 9 && j != 0) {
                carry = val / 10;
                rr.insert(0, to_string(val % 10));
            } else
                rr.insert(0, to_string(val));
        }
        if (zeroCounter > 0) {
            for (int x = 0; x < zeroCounter; ++x)
                rr.append("0");
        }
        b += BigNumber(rr);
    }
    b.setString(b._numberString.erase(0, b._numberString.find_first_not_of('0')));
    return b;
}
constexpr BigNumber powH(BigNumber base,unsigned long long exp,BigNumber powar = BigNumber("1")) {
    if (exp == 1) return base.multiply(powar);
    if (exp % 2 == 0)
        return powH(base.multiply(base),exp>>1,powar);
    else
        return powH(base,exp-1,powar.multiply(base));
}
constexpr BigNumber BigNumber::pow(unsigned long long exponent) {
    if (exponent == 0) return BigNumber("1");
    if (exponent == 1) return *this;
    return powH(*this,exponent);
}
constexpr string BigNumber::getString() {
    return this->_numberString;
}
constexpr BigNumber BigNumber::setString(const string &newStr) {
    this->_numberString = newStr;
    return *this;
}
constexpr BigNumber BigNumber::trimLeadingZeros() {
    BigNumber b = *this;
    if (b._numberString.find_first_not_of('0') != string::npos) {
        b.setString(b._numberString.erase(0, b._numberString.find_first_not_of('0')));
    }
    return b;
}
constexpr bool BigNumber::equals(const BigNumber &other) {
    return this->_numberString == other._numberString;
}
constexpr bool BigNumber::equals(const long long &other) {
    return this->getString() == to_string(other);
}
constexpr bool operator==(BigNumber b1, const BigNumber &b2) {
    return b1.equals(b2);
}
constexpr bool operator==(BigNumber b1, const long long &b2) {
    return b1.equals(b2);
}
constexpr bool operator>(BigNumber b1, const BigNumber &b2) {
    b1 = b1.trimLeadingZeros();
    auto c = BigNumber(b2);
    c = c.trimLeadingZeros();
    if (b1 == c)
        return false;
    if (b1._numberString.size() > c._numberString.size())
        return true;
    else if (c._numberString.size() > b1._numberString.size())
        return false;
    else {
        for (unsigned int i = 0; i < b1._numberString.size(); ++i) {
            if (b1[i] == static_cast<unsigned int>(c._numberString[i] - '0'))
                continue;
            return b1[i] > static_cast<unsigned int>(c._numberString[i] - '0');
        }
    }
    return false;
}
constexpr bool operator<(BigNumber b1, const BigNumber &b2) {
    return !(b1 == b2) && !(b1 > b2);
}
constexpr unsigned int BigNumber::operator[](int index) {
    return static_cast<unsigned int>(this->_numberString[index] - '0');
}
constexpr BigNumber& BigNumber::operator=(const BigNumber &other) {
    this->_numberString = other._numberString;
    return *this;
}
constexpr BigNumber& BigNumber::operator+=(const BigNumber &other) {
    *this = this->add(other);
    return *this;
}
constexpr BigNumber pow(unsigned long long exp) {
    if (exp ==     0) return BigNumber("2").pow(    0);
    if (exp ==     1) return BigNumber("2").pow(    1);
    if (exp ==     2) return BigNumber("2").pow(    2);
    if (exp ==     4) return BigNumber("2").pow(    4);
    if (exp ==     8) return BigNumber("2").pow(    8);
    if (exp ==    16) return BigNumber("2").pow(   16);
    if (exp ==    32) return BigNumber("2").pow(   32);
    if (exp ==    64) return BigNumber("2").pow(   64);
    if (exp ==   128) return BigNumber("2").pow(  128);
    if (exp ==   256) return BigNumber("2").pow(  256);
    if (exp ==   512) return BigNumber("2").pow(  512);
    if (exp ==  1024) return BigNumber("2").pow( 1024);
    if (exp ==  2048) return BigNumber("2").pow( 2048);
    if (exp ==  4096) return BigNumber("2").pow( 4096);
    if (exp ==  8192) return BigNumber("2").pow( 8192);
    if (exp == 16384) return BigNumber("2").pow(16384);
    if (exp == 32768) return BigNumber("2").pow(32768);
    if (exp == 65536) return BigNumber("2").pow(65536);
    return BigNumber("2").pow(exp);
}
int main(int argc,char const *argv[]) {
    unsigned long long n;
    if (argc <= 1)
        cin >> n;
    else
        n = stoull(argv[1]);
    constexpr BigNumber end = pow(GetValue);
    cout << end._numberString << endl;
}