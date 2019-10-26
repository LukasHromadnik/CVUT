//
//  main.cpp
//  prog1
//
//  Created by Lukáš Hromadník on 11.03.14.
//  Copyright (c) 2014 Lukáš Hromadník. All rights reserved.
//

#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
using namespace std;
#endif /* __PROGTEST__ */

typedef map<string, map<string, int> >::iterator it;
typedef map<string, int>::iterator it2;

string trim(string& str) {
    str.erase(0, str.find_first_not_of(' '));
    str.erase(str.find_last_not_of(' ') + 1);
    return str;
}

bool is_alpha_or_numeric(string s) {
    for (int i = 0; i < (int) s.length(); i++)
        if ( ! isalnum(s[i]))
            return false;
    
    return true;
}

bool transform(const char * inFile, const char * outFile) {
    map<string, map<string, int> > result;
    map<string, int> test_sub;
    map<string, int> test_teach;
    string line;
    string teacher;
    string subjects;
    string subject;
    ifstream input;
    ofstream output;
    string del1 = ":";
    string del2 = ",";
    size_t pos = 0;
    int iterace = 0;
    int line_before = 0;
    
    input.open(inFile);
    
    if (input.is_open()) {
        
        while (getline(input, line)) {
            
            if (line.length() > 0 && line.at(0) != '#') {
                line = trim(line);
                pos = line.find(del1);
                if (pos - del1.length() == 0)
                    return false;
                if (pos == string::npos)
                    return false;
                teacher = line.substr(0, pos);
                teacher = trim(teacher);
                if (teacher.length() == 0)
                    return false;
                if ( ! is_alpha_or_numeric(teacher))
                    return false;
                
                subjects = line.substr(pos + del1.length(), line.length());
                
                while ((pos = subjects.find(del2)) != string::npos) {
                    subject = subjects.substr(0, pos);
                    subject = trim(subject);
                    if ( ! is_alpha_or_numeric(subject))
                        return false;
                    if (subject.length() == 0)
                        return false;
                    if (test_sub.find(subject) != test_sub.end())
                        return false;
                    if (test_teach.find(teacher) != test_teach.end())
                        return false;
                    result[subject][teacher] = 1;
                    test_sub[subject] = 1;
                    subjects.erase(0, pos + del1.length());
                    iterace++;
                }
                subjects = trim(subjects);
                if ( ! is_alpha_or_numeric(subjects))
                    return false;
                if (test_sub.find(subjects) != test_sub.end())
                    return false;
                if (test_teach.find(teacher) != test_teach.end())
                    return false;
                if (subjects.length() == 0 && iterace != 0)
                    return false;
                result[subjects][teacher] = 1;
                test_sub = map<string, int>();
                test_teach[teacher] = 1;
                iterace = 0;
                line_before++;
            }
        }
        input.close();
    }
    else
        return false;
    
    output.open(outFile);
    
    if (output.is_open()) {
    
        for (it i = result.begin(); i != result.end(); i++) {
            if (i->first.length()) {
                output << i->first << ": ";
            
                for (it2 j = i->second.begin(); j != i->second.end(); j++) {
                    if (j == i->second.begin())
                        output << j->first;
                    else
                        output << ", " << j->first;
                }
            
                output << endl;
            }
        }
        
        output.close();
    }
    else
        return false;
    
    return true;
}

#ifndef __PROGTEST__
int main(int argc, const char * argv[]) {

    transform("/Users/Luke/Documents/FIT/Programming/prog1/1.txt", "/Users/Luke/Documents/FIT/Programming/prog1/out.txt");
    
    return 0;
}
#endif /* __PROGTEST__ */

