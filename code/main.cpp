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
   str_str_map map;

   for (int i = 0; i < argc; i++)
   {
      regex comment_regex{R"(^\s*(#.*)?$)"};
      regex key_value_regex{R"(^\s*(.*?)\s*=\s*(.*?)\s*$)"};
      regex trimmed_regex{R"(^\s*([^=]+?)\s*$)"};
      string line;

      bool file_input;
      ifstream file(argv[i]);

      if (i == 0)
      {
         if (argc == 1)
         {
            file_input = false;
         }
         else
         {
            continue;
         }
      }
      else
      {
         if (argv[i] == string("-"))
         {
            file_input = false;
         }
         else
         {
            if (!file.is_open())
            {
               cout << "Error: Cannot open file" << endl;
               exit(-1);
            }
            file_input = true;
         }
      }

      istream &input = file_input ? file : cin;

      int line_num = 0;
      while (getline(input, line))
      {
         line_num++;
         smatch result;

         cout << argv[i] << ": " << line_num << ": " << line << endl;
         if (regex_search(line, result, comment_regex)) // comments/blank lines
         {
            continue;
         }
         else if (regex_search(line, result, key_value_regex))
         {
            if (result[1] != "" && result[2] == "") // key =
            {
               map.erase(map.find(result[1]));
               continue;
            }
            if (result[1] != "" && result[2] != "") // key = value
            {
               str_str_pair pair(result[1], result[2]);
               map.insert(pair);
               continue;
            }
            if (result[1] == "" && result[2] != "") // = value
            {
               str_str_map values;
               for (auto pair : map)
               {
                  if (pair.second == result[2])
                  {
                     values.insert(pair);
                  }
               }
               values.print_all();
               continue;
            }
            if (result[1] == "" && result[2] == "") // =
            {
               map.print_all();
               continue;
            }
         }
         else if (regex_search(line, result, trimmed_regex)) // key
         {
            map.print(result[1], map.find(result[1]));
            continue;
         }
         else
         {
            cout << "This cannot happen.";
            exit(-1);
         }
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
