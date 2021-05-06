// https://www.cplusplus.com/reference/regex/ECMAScript/

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <regex>
#include <vector>
#include <set>

using namespace std;

static vector<regex> res;
static set<string> good;
static set<string> seen;
static regex re_uncolor;


static char *opt_path_config = nullptr;
static bool opt_debug = false;
static bool opt_show_good = true;
static bool opt_show_seen = true;
static bool opt_show_bad = true;


static string normalize(string line)
{
    for(auto re: res) {
        line = regex_replace(line, re, "", std::regex_constants::match_default);
    }
    if(opt_debug) {
        cout << "\e[1;30m" << line << "\e[0m\n";
    }
    return line;
}


static void parse(string fname, bool good_file)
{
    cerr << "--- " 
         << (good_file ? "good" : "bad")
         << ": " << fname << "\n";

    if(fname == "-") {
        fname = "/dev/stdin";
    }

    ifstream file(fname);

    string l;
    while (getline(file, l)) {
        l = regex_replace(l, re_uncolor, "", std::regex_constants::match_default);
        auto ln = normalize(l);

        if(good_file) {
            good.insert(ln);
        } else {
            if(good.count(ln) > 0) {
                if(opt_show_good) {
                    cout << "\e[1;32m" << l << "\e[0m" << "\n";
                }
            } else if(seen.count(ln) > 0) {
                if(opt_show_seen) {
                    cout << "\e[1;33m" << l << "\e[0m" << "\n";
                }
            } else {
                if(opt_show_bad) {
                    cout << "\e[1;31m" << l << "\e[0m" << "\n";
                }
            }
        }

        seen.insert(ln);
    }
}


static void usage(const char *fname)
{
    cerr 
        << "usage: " << fname <<" [options] [good_file [...]] <bad_file>\n"
        << "\n"
        << "options:\n"
        << "\n"
        << "  -c FILE  load configuration from FILE. Defaults to ~/.glbl.conf\n"
        << "\n"
        << "  -g       omit good lines\n"
        << "  -b       omit bad lines\n"
        << "  -s       omit seen lines\n"
        << "\n"
        << "  -d       debug: show the normalized strings\n"
        << "  -h       show this help\n"
        << "\n"
        << "description:\n"
        << "\n"
        << "This tool reads zero or more 'good' files followed by one 'bad' file.\n"
        << "All line of the files are normalized by removing substrings that match\n"
        << "list of given regular expressions. When parsing the 'bad' file, lines\n"
        << "are classified into three groups that are displayed in different colors:\n"
        << "\n"
        << "  good: lines that were seen in one of the 'good' files\n"
        << "\n"
        << "  bad:  lines that are were not seen in one of the 'good' files\n"
        << "\n"
        << "  seen: 'bad' lines that are seen more then once\n"
        << "\n"
        ;
}

static void read_config(string fname)
{
    ifstream file(fname.c_str());

    cerr << "Reading config from " << fname << "\n";

    if(file.fail()) {
        cerr << "Error opening " << opt_path_config << ": " << strerror(errno) << "\n";
        exit(1);
    }

    string l;
    while (getline(file, l)) {
        if(l.substr(0, 5) == "drop ") {
            res.push_back(regex(l.substr(5, string::npos)));
        }
    }
}


int main(int argc, char **argv)
{
    int c;

    while((c = getopt(argc, argv, "c:gsbhd")) != -1) {
        switch(c) {
            case 'c':
                opt_path_config = optarg;
                break;
            case 'g':
                opt_show_good = false;
                break;
            case 's':
                opt_show_seen = false;
                break;
            case 'b':
                opt_show_bad = false;
                break;
            case 'd':
                opt_debug = true;
                break;
            case 'h':
                usage(argv[0]);
                exit(0);
            default:
                usage(argv[0]);
                exit(1);
        }
    }

    argv += optind;
    argc -= optind;

    re_uncolor = regex("\\x1b\\[[\\d;]+m");

    if(opt_path_config) {
        read_config(opt_path_config);
    } else {
        read_config(string(getenv("HOME")) + "/.glbl.conf");
    }

    for(int i=0; i<argc; i++) {
        parse(string(argv[i]), i < argc-1);
    }

    return 0;
}

// vi: ft=cpp ts=4 sw=4 et

