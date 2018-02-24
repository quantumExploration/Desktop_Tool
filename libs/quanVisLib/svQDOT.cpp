#include "svQDOTProcess.h"
#include "svQDOT.h"

namespace __svl_lib {

bool Sort_Spin_Z(const Spin &a, const Spin &b){
 if (fabs(a.pos[2] - b.pos[2])>0.0001){
   return (a.pos[2]<b.pos[2]);
 }
 else if (fabs(a.pos[1] - b.pos[1])>0.0001){
   return (a.pos[1]<b.pos[1]);
 }
 else if (fabs(a.pos[0] - b.pos[0])>0.0001){
   return (a.pos[0]<b.pos[0]);
 }
 return 0;
}

void svQDOT::SetData(int type, char *dir, char *tmpdir, char *file){
  if(qdotData.size()>0) qdotData.clear();
  char *str = new char[1024];
  minMag = 9e+9;
  minMagNonzero = 9e+9;
  maxMag = 0;
  lbbox[0]=9e+9;lbbox[1]=9e+9;lbbox[2]=9e+9;
  rbbox[0]=-9e+9;rbbox[1]=-9e+9;rbbox[2]=-9e+9;
  sprintf(str, "%s/%s", dir, file);
  switch(type) {
    case 1: 
        ReadData1(str);break;
    case 2:
        ReadData2(str);break;
    case 3:
        ReadData3(str);break;
    case 4:
        ReadData4(str);break;
  }
  maxExp = getNumOfIntegerDigits(maxMag);
  minExp = minMag>0?getNumOfIntegerDigits(minMag):getNumOfIntegerDigits(minMagNonzero);
  numOfPower = maxExp - minExp + 1;
  sort(qdotData.begin(), qdotData.end(), Sort_Spin_Z);
  ProcessData();
  ProcessFormat();
//cerr<<"============region======================"<<endl;
  ProcessRegion();
  char *name = new char[20];
  sprintf(name,"region");
  SaveRegiontoVTK(tmpdir, name);
  char *name2 = new char[20];
  sprintf(name2,"mesh");
  //GenerateMesh(tmpdir, name, name2);
  //GenerateBoundaryPoints(tmpdir);
//==============region done===============
  delete [] name;
  delete [] name2; 
  delete [] str;
}

void svQDOT::ProcessData(){
  for(int i=0;i<qdotData.size()-1;i++){
    if(qdotData[i+1].pos[2]>qdotData[i].pos[2]){
       minZDistance = qdotData[i+1].pos[2] - qdotData[i].pos[2];
       break;
    }
    else{
       if(qdotData[i+1].pos[1]>qdotData[i].pos[1]){
           minYDistance = qdotData[i+1].pos[1] - qdotData[i].pos[1];
       }
       else if(qdotData[i+1].pos[0]>qdotData[i].pos[0]){
           minXDistance = qdotData[i+1].pos[0] - qdotData[i].pos[0];
       }
    }
  }
}

void svQDOT::ProcessFormat(){
  int x=0;
  int y=0;
  int z=0;
  int i=0;
  vector<int> format;
  for(i=0;i<qdotData.size()-1;i++){
     format.push_back(x); format.push_back(y); format.push_back(z);
     qdotFormat.insert(std::pair<Spin*, vector<int> > (&qdotData[i], format));
     if(qdotData[i+1].pos[2]>qdotData[i].pos[2]){
        x=0; y=0; z++;
     }
     else if(qdotData[i+1].pos[1]>qdotData[i].pos[1]){
        x=0; y++;
     }
     else{
        x++;
     }
     format.clear();
  }
  format.push_back(x); format.push_back(y); format.push_back(z);
  qdotFormat.insert(std::pair<Spin*, vector<int> > (&qdotData[i], format));
  format.clear();
}

void svQDOT::ProcessRegion(){
  //0: outside region
  //1: in region1
  //2: in region2
  //......
  int count = 0;
  int regionIndex = 0;
  bool newRegion = false;
  bool newSphere = false;
//ignore the first z layer
  for(int i=0;i<qdotData.size()-1;i++){
    if(qdotData[i+1].pos[2] > qdotData[i].pos[2]){
      if(!fabs(qdotData[i].region - maxRegion)<1e-8){
         regionCount[regionIndex-1]++;
      }
      else{
         qdotData[i].region =0;
      }
      if(fabs(qdotData[i+1].region - maxRegion)<1e-8){
        if(newRegion && !newSphere){
          newRegion = false; newSphere = true;
          regionZ[1].push_back(qdotData[i].pos[2]);
        }
        qdotData[i+1].region = 0;    
      }
      else{
         if(!newRegion){
          newRegion = true;regionIndex++;newSphere=false;
          qdotData[i+1].region = regionIndex; 
          regionZ[0].push_back(qdotData[i+1].pos[2]);
          regionCount.push_back(1);
         }
         else{
          regionCount[regionIndex-1]++;//for i+1;
         }
      }
      i++;
    }
    else{
      if(fabs(qdotData[i].region - maxRegion)<1e-8){
        qdotData[i].region = 0;
      }
      else{
        qdotData[i].region = regionIndex;
        regionCount[regionIndex-1]++;
      } 
    }
  }
  regionSize = regionIndex;
}

void svQDOT::GenerateBoundaryPoints(char *dir){
  for(int i=0;i<regionSize;i++){
    char *inputfile = new char[400];
    sprintf(inputfile, "%s/tmppoint.txt", dir);
    int size = regionCount[i];
    ofstream outfile(inputfile);
    outfile<<size<<endl;
    for(int j=0;j<qdotData.size();j++){
       if(i+1 == qdotData[j].region){
          outfile<<qdotData[j].pos[0]<<" "
                 <<qdotData[j].pos[1]<<" "
                 <<qdotData[j].pos[2]<<endl;
       }
    }
    outfile.close();
    char *outputfile = new char[400];
    sprintf(outputfile, "%s/point%d.txt", dir, i+1);
    svMeshGenerator *mesh = new svMeshGenerator();
    mesh->GenerateBoundaryPoints(inputfile, outputfile);
    delete mesh;
    delete [] inputfile;
    delete [] outputfile;
  }
}

void svQDOT::SaveRegiontoVTK(char *dir, char *file){
  for(int i=1;i<regionSize+1;i++){
    int count =0;
    float minz = 9e+9; float maxz=  -9e+9;
    for(int j=0;j<qdotData.size();j++){
       if(qdotData[j].region == i){     
         if(qdotData[j].pos[2]>maxz){maxz = qdotData[j].pos[2];}
         if(qdotData[j].pos[2]<minz){minz = qdotData[j].pos[2];}
       }
    }
    for(int j=0;j<qdotData.size();j++){
       if(qdotData[j].pos[2]>minz-minZDistance*2
        && qdotData[j].pos[2]<maxz+minZDistance*2){
         count++;
       }
    }
    char *str = new char[400];
    sprintf(str, "%s/%s%d.vtk", dir, file,i);
    ifstream infile(str);
    if(infile.is_open()){infile.close();delete [] str; continue;}
    
    ofstream outfile(str);
    outfile<<"# vtk DataFile Version 2.0"<<endl;
    outfile<<str<<endl;
    outfile<<"ASCII"<<endl;
    outfile<<"DATASET UNSTRUCTURED_GRID"<<endl;
    outfile<<"POINTS "<<count<<" float"<<endl;
    for(int j=0;j<qdotData.size();j++){
      if(qdotData[j].pos[2]>minz-minZDistance*2
        && qdotData[j].pos[2]<maxz+minZDistance*2){
         outfile<<qdotData[j].pos[0]<<" "<<qdotData[j].pos[1]<<" "<<qdotData[j].pos[2]<<endl;
      }
    }
    outfile<<endl;
    outfile<<"POINT_DATA "<<count<<endl;
    outfile<<"SCALARS density float 1"<<endl;
    outfile<<"LOOKUP_TABLE default"<<endl;
    for(int j=0;j<qdotData.size();j++){
      if(qdotData[j].pos[2]>minz-minZDistance*2
        && qdotData[j].pos[2]<maxz+minZDistance*2){
        if(qdotData[j].region==0) outfile<<1<<endl;
        else outfile<<0<<endl;
      }
    }
    outfile.close();
    delete [] str;
  }
}

void svQDOT::GenerateMesh(char *dir, char *ifname, char *ofname){
  svMeshGenerator *mesh = new svMeshGenerator();
  for(int i=0;i<regionSize;i++){
    char *str = new char[400];
    sprintf(str, "%s/%s%d.vtk", dir, ifname,i+1);
    char *str2 = new char[400];
    sprintf(str2, "%s/%s%d.txt", dir, ofname,i+1);
    mesh->GenerateMesh(str, str2, regionZ[1][i], regionZ[0][i]);
    delete [] str; delete [] str2;
  }
  delete mesh;
}

void svQDOT::ReadData1(char *file){
  ifstream infile(file);
  Spin tmpspin;
  maxRegion = -1;
  while(infile>>tmpspin.pos[0]>>tmpspin.pos[1]>>tmpspin.pos[2]
             >>tmpspin.region>>tmpspin.atom
             >>tmpspin.dir[0]>>tmpspin.dir[1]>>tmpspin.dir[2]){
     if(tmpspin.atom > maxRegion) maxRegion = tmpspin.atom;
     tmpspin.den = tmpspin.dir[0]*tmpspin.dir[0]
                 + tmpspin.dir[1]*tmpspin.dir[1]
                 + tmpspin.dir[2]*tmpspin.dir[2];
     tmpspin.den = sqrt(tmpspin.den);
     if(tmpspin.den>0) tmpspin.dir = normalize(tmpspin.dir);
     tmpspin.exp = getNumOfIntegerDigits(tmpspin.den); 
     tmpspin.coe = tmpspin.den/pow(10., tmpspin.exp);
     tmpspin.xangle = acos(tmpspin.dir[0])*180./PI;
     tmpspin.yangle = acos(tmpspin.dir[1])*180./PI;
     tmpspin.zangle = acos(tmpspin.dir[2])*180./PI;
     qdotData.push_back(tmpspin);
     if(tmpspin.pos[0]>rbbox[0])rbbox[0]=tmpspin.pos[0];
     if(tmpspin.pos[1]>rbbox[1])rbbox[1]=tmpspin.pos[1];
     if(tmpspin.pos[2]>rbbox[2])rbbox[2]=tmpspin.pos[2];
     if(tmpspin.pos[0]<lbbox[0])lbbox[0]=tmpspin.pos[0];
     if(tmpspin.pos[1]<lbbox[1])lbbox[1]=tmpspin.pos[1];
     if(tmpspin.pos[2]<lbbox[2])lbbox[2]=tmpspin.pos[2];
     if(minMag > tmpspin.den) minMag = tmpspin.den;
     if(maxMag < tmpspin.den) maxMag = tmpspin.den;
     if(minMagNonzero > tmpspin.den && tmpspin.den>0) minMagNonzero = tmpspin.den;
  }
  infile.close(); 
}

void svQDOT::ReadData2(char *file){
  ifstream infile(file);
  Spin tmpspin;
  if(infile.fail())
  {
    cerr << file<< "not found.\n";
        cerr << "Load QDOT data failed. prog exit\n" <<endl;
        exit(-1);
  }
  else
  {
        cerr << "Loading data: " << file << endl;
  }

  while(infile>>tmpspin.pos[0]>>tmpspin.pos[1]>>tmpspin.pos[2]
             >>tmpspin.region>>tmpspin.atom
             >>tmpspin.dir[0]>>tmpspin.dir[1]>>tmpspin.dir[2]
             >>tmpspin.den){
     if(tmpspin.atom > maxRegion) maxRegion = tmpspin.atom;
     tmpspin.den = sqrt(tmpspin.dir[0]*tmpspin.dir[0]
                 + tmpspin.dir[1]*tmpspin.dir[1]
                 + tmpspin.dir[2]*tmpspin.dir[2]) * tmpspin.den;
     if(tmpspin.den>0) tmpspin.dir = normalize(tmpspin.dir);
     tmpspin.exp = getNumOfIntegerDigits(tmpspin.den);
     tmpspin.coe = tmpspin.den/pow(10., tmpspin.exp);
     tmpspin.xangle = acos(tmpspin.dir[0])*180./PI;
     tmpspin.yangle = acos(tmpspin.dir[1])*180./PI;
     tmpspin.zangle = acos(tmpspin.dir[2])*180./PI;
     qdotData.push_back(tmpspin);
     if(tmpspin.pos[0]>rbbox[0])rbbox[0]=tmpspin.pos[0];
     if(tmpspin.pos[1]>rbbox[1])rbbox[1]=tmpspin.pos[1];
     if(tmpspin.pos[2]>rbbox[2])rbbox[2]=tmpspin.pos[2];
     if(tmpspin.pos[0]<lbbox[0])lbbox[0]=tmpspin.pos[0];
     if(tmpspin.pos[1]<lbbox[1])lbbox[1]=tmpspin.pos[1];
     if(tmpspin.pos[2]<lbbox[2])lbbox[2]=tmpspin.pos[2];
     if(minMag > tmpspin.den) minMag = tmpspin.den;
     if(maxMag < tmpspin.den) maxMag = tmpspin.den;
     if(minMagNonzero > tmpspin.den && tmpspin.den>0) minMagNonzero = tmpspin.den;
  }
  infile.close();
}

void svQDOT::ReadData3(char *file){
  ifstream infile(file);
  Spin tmpspin;
  svInt tmpr;
  while(infile>>tmpspin.pos[0]>>tmpspin.pos[1]>>tmpspin.pos[2]
             >>tmpr>>tmpspin.atom
             >>tmpspin.dir[0]>>tmpspin.dir[1]>>tmpspin.dir[2]
             >>tmpspin.region){
     if(tmpspin.atom > maxRegion) maxRegion = tmpspin.atom;
     tmpspin.den = tmpspin.dir[0]*tmpspin.dir[0]
                 + tmpspin.dir[1]*tmpspin.dir[1]
                 + tmpspin.dir[2]*tmpspin.dir[2];
     tmpspin.den = sqrt(tmpspin.den);
     if(tmpspin.den>0) tmpspin.dir = normalize(tmpspin.dir);
     tmpspin.exp = getNumOfIntegerDigits(tmpspin.den);
     tmpspin.coe = tmpspin.den/pow(10., tmpspin.exp);
     tmpspin.xangle = acos(tmpspin.dir[0])*180./PI;
     tmpspin.yangle = acos(tmpspin.dir[1])*180./PI;
     tmpspin.zangle = acos(tmpspin.dir[2])*180./PI;
     qdotData.push_back(tmpspin);
     if(tmpspin.pos[0]>rbbox[0])rbbox[0]=tmpspin.pos[0];
     if(tmpspin.pos[1]>rbbox[1])rbbox[1]=tmpspin.pos[1];
     if(tmpspin.pos[2]>rbbox[2])rbbox[2]=tmpspin.pos[2];
     if(tmpspin.pos[0]<lbbox[0])lbbox[0]=tmpspin.pos[0];
     if(tmpspin.pos[1]<lbbox[1])lbbox[1]=tmpspin.pos[1];
     if(tmpspin.pos[2]<lbbox[2])lbbox[2]=tmpspin.pos[2];
     if(minMag > tmpspin.den) minMag = tmpspin.den;
     if(maxMag < tmpspin.den) maxMag = tmpspin.den;
     if(minMagNonzero > tmpspin.den && tmpspin.den>0) minMagNonzero = tmpspin.den;
  }
  infile.close();
}

void svQDOT::ReadData4(char *file){
  ifstream infile(file);
  Spin tmpspin;
  svInt tmpr;
  while(infile>>tmpspin.pos[0]>>tmpspin.pos[1]>>tmpspin.pos[2]
             >>tmpr>>tmpspin.atom
             >>tmpspin.dir[0]>>tmpspin.dir[1]>>tmpspin.dir[2]
             >>tmpspin.den>>tmpspin.region){
     if(tmpspin.atom > maxRegion) maxRegion = tmpspin.atom;
     tmpspin.den = sqrt(tmpspin.dir[0]*tmpspin.dir[0]
                 + tmpspin.dir[1]*tmpspin.dir[1]
                 + tmpspin.dir[2]*tmpspin.dir[2]) * tmpspin.den;
     if(tmpspin.den>0) tmpspin.dir = normalize(tmpspin.dir);
     tmpspin.exp = getNumOfIntegerDigits(tmpspin.den);
     tmpspin.coe = tmpspin.den/pow(10., tmpspin.exp);
     tmpspin.xangle = acos(tmpspin.dir[0])*180./PI;
     tmpspin.yangle = acos(tmpspin.dir[1])*180./PI;
     tmpspin.zangle = acos(tmpspin.dir[2])*180./PI;
     qdotData.push_back(tmpspin);
     if(tmpspin.pos[0]>rbbox[0])rbbox[0]=tmpspin.pos[0];
     if(tmpspin.pos[1]>rbbox[1])rbbox[1]=tmpspin.pos[1];
     if(tmpspin.pos[2]>rbbox[2])rbbox[2]=tmpspin.pos[2];
     if(tmpspin.pos[0]<lbbox[0])lbbox[0]=tmpspin.pos[0];
     if(tmpspin.pos[1]<lbbox[1])lbbox[1]=tmpspin.pos[1];
     if(tmpspin.pos[2]<lbbox[2])lbbox[2]=tmpspin.pos[2];
     if(minMag > tmpspin.den) minMag = tmpspin.den;
     if(maxMag < tmpspin.den) maxMag = tmpspin.den;
     if(minMagNonzero > tmpspin.den && tmpspin.den>0) minMagNonzero = tmpspin.den;

  }
  infile.close();
}

}
