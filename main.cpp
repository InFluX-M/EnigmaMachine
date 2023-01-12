
#include <ctime>
#include <chrono>
#include <bits/stdc++.h>
using namespace std;

class Reflector
{
    map<char, char> reflector;

public:
    Reflector()
    {
        for (int i = 0; i < 13; i++)
        {
            reflector['A' + i] = 'Z' - i;
            reflector['Z' - i] = 'A' + i;
        }
    }
    char get(char c)
    {
        if (reflector.find(c) != reflector.end())
            return reflector[c];

        return c;
    }
};

class Plugboard
{
    map<char, char> plugboard;

public:
    Plugboard(string s)
    {
        for (int i = 0; i < s.size(); i += 2)
        {
            plugboard[s[i]] = s[i + 1];
            plugboard[s[i + 1]] = s[i];
        }
    }

    char get(char c)
    {
        if (plugboard.find(c) != plugboard.end())
            return plugboard[c];
        return c;
    }
};

class Rotor
{
public:
    map<char, char> rotor;
    int rotateCount = 0;
    Rotor(string s)
    {
        for (int i = 0; i < s.size(); i++)
        {
            char temp = 'A' + i;
            rotor[temp] = s[i];
        }
    }
    char get(char c)
    {
        if (rotor.find(c) != rotor.end())
        {
            int temp = int(c) - (rotateCount) % 26;
            if (temp < 'A')
            {
                temp += 26;
            }
            char c = temp;
            return rotor[c];
        }
        return c;
    }
    char find(char c)
    {
        for (pair<char, char> p : rotor)
        {
            if (p.second == c)
            {
                c = p.first;
                int temp = c + (rotateCount % 26);
                if (temp > 'Z')
                {
                    temp = 'A' + (temp - 'Z') - 1;
                }
                c = temp;
                break;
            }
        }
        return c;
    }

    void rotate()
    {
        reverse_iterator<map<char, char>::iterator> rit = rotor.rbegin();
        reverse_iterator<map<char, char>::iterator> rit2 = rotor.rbegin();
        rit2++;
        char temp = rit->second;

        for (rit; rit != rotor.rend(); rit++)
        {
            rit->second = rit2->second;
            rit2++;
        }
        rotor.begin()->second = temp;
    }
};

class Enigma
{
    string date;
    Rotor rotor1;
    Rotor rotor2;
    Rotor rotor3;
    Reflector reflector;
    Plugboard plugboard;

    char de_enCode(char c)
    {
        c = plugboard.get(c);
        c = rotor3.get(c);
        c = rotor2.get(c);
        c = rotor1.get(c);
        c = reflector.get(c);
        c = rotor1.find(c);
        c = rotor2.find(c);
        c = rotor3.find(c);
        c = plugboard.get(c);
        rotor3.rotateCount++;
        if (rotor3.rotateCount == 26)
        {
            rotor2.rotateCount++;
            rotor3.rotateCount = 0;
        }
        if (rotor2.rotateCount == 26)
        {
            rotor1.rotateCount++;
            rotor2.rotateCount = 0;
        }
        return c;
    }

public:
    Enigma(string date, string s1, string s2, string s3, string s4)
        : plugboard(s1), rotor3(s4), rotor2(s3), rotor1(s2), reflector() { this->date = date; }

    string de_encipher(string s)
    {
        string result = "";
        for (int i = 0; i < s.size(); i++)
            result.push_back(de_enCode(s[i]));
        return result;
    }
};

string toUpperCase(string s)
{
    for (int i = 0; i < s.size(); i++)
    {
        s[i] = toupper(s[i]);
    }
    return s;
}

string editPlugboard(string s)
{
    string plg = "";
    for (int i = 12; i < s.size() - 1; i++)
    {
        if (s[i] != ' ' && s[i] != ',')
            plg.push_back(s[i]);
    }

    return plg;
}
string editRotor(string s)
{
    string rtr = "";
    for (int i = 9; i < s.size() - 1; i++)
    {
        rtr.push_back(s[i]);
    }

    return rtr;
}
string editDate(string s)
{
    return s.substr(6);
}

void filePanel()
{
    cout << "Enter a date: ";
    string dateAim;
    cin >> dateAim;

    ifstream fin("/media/influx/Programming/Projects/Enigma/EnigmaFile.txt");
    string line;
    bool flag = false;

    Enigma *enigmaBurlin;
    Enigma *enigmaMunich;

    while (getline(fin, line))
    {
        string date = line;
        date = editDate(date);
        if (date == dateAim)
        {
            string plugboard;
            getline(fin, plugboard);
            plugboard = editPlugboard(plugboard);
            plugboard = toUpperCase(plugboard);
            string rotor1;
            getline(fin, rotor1);
            rotor1 = editRotor(rotor1);
            rotor1 = toUpperCase(rotor1);
            string rotor2;
            getline(fin, rotor2);
            rotor2 = editRotor(rotor2);
            rotor2 = toUpperCase(rotor2);
            string rotor3;
            getline(fin, rotor3);
            rotor3 = editRotor(rotor3);
            rotor3 = toUpperCase(rotor3);
            enigmaBurlin = new Enigma(date, plugboard, rotor1, rotor2, rotor3);
            enigmaMunich = new Enigma(date, plugboard, rotor1, rotor2, rotor3);
            flag = true;
            break;
        }
        else
        {
            string plugboard;
            getline(fin, plugboard);
            string rotor1;
            getline(fin, rotor1);
            string rotor2;
            getline(fin, rotor2);
            string rotor3;
            getline(fin, rotor3);
        }
    }
    fin.close();

    if (!flag)
    {
        cout << "Date not found!\n";
        return;
    }

    while (true)
    {
        cout << "InputBody: ";
        string s;
        cin >> s;
        s = toUpperCase(s);

        // chrono::steady_clock::time_point tend0 = chrono::steady_clock::now() + chrono::seconds(2);
        // while (chrono::steady_clock::now() < tend0)
        // {
        // }
        string a = enigmaMunich->de_encipher(s);
        cout << "Encode...: ";
        // chrono::steady_clock::time_point tend1 = chrono::steady_clock::now() + chrono::seconds(2);
        // while (chrono::steady_clock::now() < tend1)
        // {
        // }
        cout << a << '\n';
        cout << "Decode...: ";
        // chrono::steady_clock::time_point tend2 = chrono::steady_clock::now() + chrono::seconds(2);
        // while (chrono::steady_clock::now() < tend2)
        // {
        // }
        cout << enigmaBurlin->de_encipher(a) << '\n';
        cout << "-----------------------------------------\n";
    }
}

void panel()
{
    cin.ignore();
    string s1, s2, s3, s4;
    cout << "Plugboard: ";
    getline(cin, s1);
    cout << "Rotor1: ";
    getline(cin, s2);
    cout << "Rotor2: ";
    getline(cin, s3);
    cout << "Rotor3: ";
    getline(cin, s4);

    string date = "2022/12/23";

    s1 = editPlugboard(s1);
    s2 = editRotor(s2);
    s3 = editRotor(s3);
    s4 = editRotor(s4);

    Enigma enigmaBurlin(date, toUpperCase(s1), toUpperCase(s2), toUpperCase(s3), toUpperCase(s4));
    Enigma enigmaMunich(date, toUpperCase(s1), toUpperCase(s2), toUpperCase(s3), toUpperCase(s4));

    while (true)
    {
        cout << "InputBody: ";
        string s;
        cin >> s;
        s = toUpperCase(s);
        // chrono::steady_clock::time_point tend0 = chrono::steady_clock::now() + chrono::seconds(2);
        // while (chrono::steady_clock::now() < tend0)
        // {
        // }
        string a = enigmaMunich.de_encipher(s);
        cout << "Encode...: ";
        // chrono::steady_clock::time_point tend1 = chrono::steady_clock::now() + chrono::seconds(2);
        // while (chrono::steady_clock::now() < tend1)
        // {
        // }
        cout << a << '\n';
        cout << "Decode...: ";
        // chrono::steady_clock::time_point tend2 = chrono::steady_clock::now() + chrono::seconds(2);
        // while (chrono::steady_clock::now() < tend2)
        // {
        // }
        cout << enigmaBurlin.de_encipher(a) << '\n';
        cout << "-----------------------------------------\n";
    }
}

int main()
{
    cout << "1- Manual input\n2- File input\n";
    int n;
    cin >> n;
    if (n == 1)
        panel();
    else
        filePanel();
    return 0;
}
