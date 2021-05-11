#ifndef VERSION_H_INCLUDED
#define VERSION_H_INCLUDED
#include <string>
#include <iostream>
class version
{
  public:
      static std::string getTitanVersion() // TiTan internal version - Do NOT touch
      {
          return "0.9 Alpha";
      }

      static std::string getUserAppVersion() // Your App version - U can edit
      {
          return "WIP";
      }
};
#endif // VERSION_H_INCLUDED
