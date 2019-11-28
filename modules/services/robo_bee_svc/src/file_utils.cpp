
#include <algorithm>
#include <dirent.h>
#include <sys/types.h>

#include "file_utils.h"

void FileUtils::getSortedListOfFilesInDir(std::string dirName, std::vector<std::string> * fileNameList) {
   
   dirent* de;
   DIR* dp;
   errno = 0;
   dp = opendir( dirName.c_str() );
   if (dp)
   {
      while (true)
      {
         errno = 0;
         de = readdir( dp );
         if (de == nullptr) break;
         fileNameList->push_back( std::string( de->d_name ) );
      }
      closedir( dp );
      std::sort( fileNameList->begin(), fileNameList->end() );
   }
}