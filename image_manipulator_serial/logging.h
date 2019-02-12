#ifndef  _LOGGING_H_
#define  _LOGGING_H_

#include <exception>
#include <stdio.h>
#include <fstream>

using std::exception;
using std::ofstream;

class DataFlowException : public exception
{
  public:
    DataFlowException(const char *type, const char *error);
    virtual const char  *what() const throw() { return msg; };

  protected:
    char        msg[1024];
};


class Logger
{
  public:
    static void     LogEvent(const char *event);
    static void     Finalize();

  private:
    static ofstream logger;
};

#endif