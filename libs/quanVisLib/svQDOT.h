
#ifndef __SV_QDOT_H
#define __SV_QDOT_H

#include <math.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <assert.h>
#include <ctime>
#include <unistd.h>
#include <time.h>       /* time_t, struct tm, time, localtime */
//#include <X11/Xlib.h>
#include <sys/stat.h>

#include <vector>
#include <queue>
#include "svType.h"
#include "svParticle.h"
#include "svArray.h"
#include "svUtil.h"
//#include "svQDOTProcess.h"
#include "svConfig.h"
#include <map>

#ifndef M_PI
#define M_PI 3.1415926
#endif

#ifndef PI
#define PI 3.1415926
#endif

using namespace std;

namespace __svl_lib {

struct Spin{
 svVector3 pos;
 svVector3 dir;
 svScalar den;
 svScalar exp;
 svScalar coe;
 svScalar region;
 svScalar atom; 
 svScalar xangle;
 svScalar yangle;
 svScalar zangle;
};
typedef Spin Spin;

struct svQDOT{
  svQDOT(){};
  ~svQDOT(){qdotData.clear();}

  void SetData(int type, char *dir, char *tmpdir, char *file);
  void ProcessData();
  void ProcessFormat();
//  void SaveSymmetry(char *dir, char *file){}//TODO
//  void SaveFormat(char *dir, char *file, char *name){}//TODO
  void ProcessRegion();
  void SaveRegiontoVTK(char *dir,  char *name);
  void GenerateMesh(char *dir, char *ifname, char *ofname);
  void GenerateBoundaryPoints(char *dir);

  void ReadData1(char *file);
  void ReadData2(char *file);
  void ReadData3(char *file);
  void ReadData4(char *file);

  int maxRegion;
  int regionSize;
  svVector3 lbbox;
  svVector3 rbbox;
  vector<Spin> qdotData;
  map<Spin *, vector<int> > qdotFormat;//for sampling
  vector<float> regionZ[3];
  vector<int> regionCount;
  svScalar maxMag;
  svScalar minMag;
  svScalar minMagNonzero;
  svScalar maxExp;
  svScalar minExp;
  svInt numOfPower;
  svScalar minXDistance;
  svScalar minYDistance;
  svScalar minZDistance;
};
typedef svQDOT svQDOT;
}
#endif
