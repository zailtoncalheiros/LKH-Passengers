#ifndef _FLAGS_CPP_
#define _FLAGS_CPP_

#include "../macros.h"

class Flags {
private:
    map<string, string> flagValues;

    void showErrorAndCrash (string flag) {
        cerr << "The flag (" << flag << ") does not exist." << endl;
        exit(0);
    }

    vector<string> splitString (const string &input) {
        vector<string> pieces;
        stringstream ss(input);
        string str;
        while (getline(ss, str, '=')) {
            pieces.pb (str);
        }
        return pieces;
    }

public:
    Flags (int argc, char* argv[]){
        FORR (i, argc) {
            string str (argv[i]);
            //cout << "aqui " << str << endl;
            if (sz(str)>1 && str[0]=='-') {
                if (str[1]=='-') {
                    str = str.substr(2);
                } else {
                    str = str.substr(1);
                }
                vector<string> pieces = splitString(str);
                if (sz(pieces)==0) {
                    cerr << "len(tokens) not expected for string: " << str << endl;
                } else if (sz(pieces)==1) {
                    flagValues[pieces[0]] = "";
                } else {
                    flagValues[pieces[0]] = pieces[1];
                    if (sz(pieces)>2) {
                        cerr << "Flag format is not expected, flag tokens:";
                        FORR (j, sz(pieces)) {
                            cerr << " " << pieces[j];
                        } 
                        cerr << endl;
                    }
                }
            }
        }
    }

    bool hasFlag(const string &flag) {
        return flagValues.find(flag)!=flagValues.end();
    }

    string getFlag(const string &flag) {
        if (!hasFlag(flag)) {
            showErrorAndCrash(flag);
        }
        return flagValues[flag];
    }

    int getFlagToInt(const string &flag) {
        try {
            return stoi(getFlag(flag));
        } catch (const std::invalid_argument& ia) {
            cerr << ia.what() << " : argument: flag -> " << flag << ", value -> " << getFlag(flag)<< endl;
            exit(0); 
        }
    }

    double getFlagToDouble (const string &flag) {
        return stod(getFlag(flag));
    }

};

#endif