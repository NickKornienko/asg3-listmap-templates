// $Id: main.cpp,v 1.13 2021-02-01 18:58:18-08 - - $

#include <cstdlib>
#include <exception>
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <regex>

using namespace std;

#include "listmap.h"
#include "xpair.h"
#include "util.h"

using str_str_map = listmap<string, string>;
using str_str_pair = str_str_map::value_type;

void scan_options(int argc, char **argv)
{
   opterr = 0;
   for (;;)
   {
      int option = getopt(argc, argv, "@:");
      if (option == EOF)
         break;
      switch (option)
      {
      case '@':
         debugflags::setflags(optarg);
         break;
      default:
         complain() << "-" << char(optopt) << ": invalid option"
                    << endl;
         break;
      }
   }
}

int main(int argc, char **argv)
{
   sys_info::execname(argv[0]);
   scan_options(argc, argv);
   for (int i = 0; i < argc; i++)
   {
      cout << "\n"
           << argv[i] << "\n";
      regex comment_regex{R"(^\s*(#.*)?$)"};
      regex key_value_regex{R"(^\s*(.*?)\s*=\s*(.*?)\s*$)"};
      regex trimmed_regex{R"(^\s*([^=]+?)\s*$)"};

      string line;
      if (argv[i] == string("-"))
      {
         while (getline(cin, line))
         {
            cout << "input: \"" << line << "\"" << endl;
            smatch result;
            if (regex_search(line, result, comment_regex))
            {
               cout << "Comment or empty line." << endl;
            }
            else if (regex_search(line, result, key_value_regex))
            {
               cout << "key  : \"" << result[1] << "\"" << endl;
               cout << "value: \"" << result[2] << "\"" << endl;
            }
            else if (regex_search(line, result, trimmed_regex))
            {
               cout << "query: \"" << result[1] << "\"" << endl;
            }
            else
            {
               cout << "This cannot happen.";
               // assert (false and "This can not happen.");
            }
         }
      }
      else
      {
         ifstream file(argv[i]);
         if (!file.is_open())
         {
            cout << "Error" << endl;
            exit(-1);
         }

         while (getline(file, line))
         {
            cout << "input: \"" << line << "\"" << endl;
            smatch result;
            if (regex_search(line, result, comment_regex))
            {
               cout << "Comment or empty line." << endl;
            }
            else if (regex_search(line, result, key_value_regex))
            {
               cout << "key  : \"" << result[1] << "\"" << endl;
               cout << "value: \"" << result[2] << "\"" << endl;
            }
            else if (regex_search(line, result, trimmed_regex))
            {
               cout << "query: \"" << result[1] << "\"" << endl;
            }
            else
            {
               cout << "This cannot happen.";
               // assert (false and "This can not happen.");
            }
         }
         file.close();
      }
   }
   return 0;
   /*str_str_map test;
   cout << test << endl;
   for (char** argp = &argv[optind]; argp != &argv[argc]; ++argp) {
      str_str_pair pair (*argp, to_string<int> (argp - argv));
      cout << "Before insert: " << pair << endl;
      test.insert (pair);
   }

   cout << test.empty() << endl;
   for (str_str_map::iterator itor = test.begin();
        itor != test.end(); ++itor) {
      cout << "During iteration: " << *itor << endl;
   }

   str_str_map::iterator itor = test.begin();
   test.erase (itor);

   cout << "EXIT_SUCCESS" << endl;
   return EXIT_SUCCESS;*/
}
