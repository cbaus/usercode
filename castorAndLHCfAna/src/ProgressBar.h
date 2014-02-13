#ifndef PROGRESSBAR__H__
#define PROGRESSBAR__H__

#include <iostream>
#include <iomanip>
#include <time.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

namespace progress
{
class ProgressBar
{
 public:
  ProgressBar () : fWidth(50),fProgress(0.),fLast(-1) {}
  ~ProgressBar () {}
  bool SetStatus (double progress);
  void Print () const;
 private:
  bool SetSize (unsigned int width) const;
  mutable int fWidth;
  double fProgress;
  mutable int fLast;
};

bool ProgressBar::SetStatus(double progress)
{
  if ( progress > 1. )
    fProgress = 1.;
  else if ( progress < 0. )
    fProgress = 0.;
  else
    fProgress = progress;
  return true;
}
bool ProgressBar::SetSize(unsigned int width) const
{
  if (width > 5)
    fWidth = width;
  else
    fWidth = 5;
  return true;
}

void ProgressBar::Print() const
{
  const int percent = floor (fProgress*100.+0.5); // round
  if (fLast != percent)
    fLast=percent;
  else
    return; //don't update screen when percentage does not change
  
  //remove old text
  for (int i = 0; i < fWidth; i++)
    std::cout << "\b";

  //get new window size
  int cols=15;
  //int lines=10;
#ifdef TIOCGSIZE
  struct ttysize ts;
  ioctl(STDIN_FILENO, TIOCGSIZE, &ts);
  cols = ts.ts_cols;
  //lines = ts.ts_lines;
#elif defined(TIOCGWINSZ)
  struct winsize ts;
  ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);
  cols = ts.ws_col;
  //lines = ts.ws_row;
#endif /* TIOCGSIZE */

  this->SetSize(cols);

  std::cout << "|";
  int pos=1;
  while (pos < fWidth / 2 - 1)
    {
      if (pos <= (int)( (fWidth - 1) * fProgress ))
        std::cout << "=";
      if (pos > (int)( (fWidth - 1) * fProgress ))
        std::cout << "-";
      pos++;
    }
  
  std::streamsize p = std::cout.precision();
  std::cout << std::setprecision(0) << std::fixed << percent << "%";
  if (percent<10)
    pos+=2;
  else if (percent<100)
    pos+=3;
  else if (percent==100)
    pos+=4;

  while (pos < fWidth - 1)
    {
      if (pos <= (int)( (fWidth - 1) * fProgress ))
        std::cout << "=";
      if (pos > (int)( (fWidth - 1) * fProgress ))
        std::cout << "-";
      pos++;
    }
  std::cout << ">";
  pos++;

  std::cout.flush();
  std::cout.precision(p);
}//progress in 0 upto 1
}
#endif
