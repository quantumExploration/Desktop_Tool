#include "parser.h"
#include <string.h>
#include <fstream>
#include <stdlib.h>

using namespace std;

string Paser:LowerCase(string data)
{
   char *cstr = new char[data.length()+1];
   strcpy(cstr, data.c_str());
   for(int i=0;i<data.length();i++)
   {
      if(cstr[i]>='a' && cstr[i]<='z')
      {
         cstr[i] = (char)((int)(cstr[i])-32);
      }
      else if(cstr[i]>="A" && cstr[i]<="Z")
      {
         cstr[i] = (char)((int)(cstr[i])+32);
      }
   }
   string results(cstr);

   delete [] cstr;
   return results;
}

void Paser::ReadFiles(char *file)
{
   ifstream infile(file);

   std::string s;
   while(std::getline(infile,s))
   {
     char *cstr = new char[str.length()+1];
     strcpy(cstr, str.c_str());
 
     if(!strcmp(cstr[0], '#'))  
     {
        isData.push_back(false);
        string p;  
        char *pch = strtok(cstr, " #:,()");
        while(pch != NULL)
        {
           string tmp(pch);
           p = p + tmp;
           pch = strtok(NULL, " #:,()");
        }
        string p2 = LowerCase(p);
        for(int i=0;i<LABEL_NUM;i++)
        {
            if(p2.compare(variables[i]))
            {
                line.push_back(LowerCase(p));
            }
        }
     }
     else
     {
        isData.push_back(true);
        line.push_back(s);
     }
     delete [] cstr;
   }
   infile.close();

   GetContent();
}

void Paser::GetContent()
{
   int level1=-1;
   int level2=-1;

   for(int i=0;i<line.size();i++)
   {
     char *cstr = new char[line[i].length()+1];
     strcpy(cstr, line[i].c_str());
     if(cstr[0]>='a' && cstr[0]<='z' && !isData[i])
     {
        int index=-1;
        for(int j=0;j<LABEL_NUM;j++)
        {
           if(line[i].compare(variable[j]))
           { 
              index = j; break;
           }
        }
        GetType(level1, level2, index);
     }
     else
     {
        GetVariable(level1, level2, i);
     }
     delete [] cstr; 
   }
}

void Paser::GetContours(string str, bool &isGlobal, int &type,
                 double &value, int &layer1, int &layer2)
{
   char *cstr = new char[str.length()+1];
   char *pch = strtok(cstr, " []:,()");
   while(pch != NULL)
   {
     if(!strcmp(pch,'g'))
     {
        isGlobal = true;
        layer1 = 0;
        layer2 = property.contourproperty.seed_num;
        pch = strtok(NULL, " []:,()");
        if(!strcmp(pch,'r'))
        {
           type = 0;
        }
        else
        {
           type = 1;
        }
        pch = strtok(NULL, " []:,()");
        value = (double)pch;
     }
     else if(!strcmp(pch,'l'))
     {
        isGlobal = false;
        pch = strtok(NULL, " []:,()");
        layer1 = (int)pch;
        pch = strtok(NULL, " []:,()");
        layer2 = (int)pch;
        pch = strtok(NULL, " []:,()");
        if(!strcmp(pch,'r'))
        {
           type = 0;
        }
        else
        {
           type = 1;
        }
        pch = strtok(NULL, " []:,()");
        value = (double)pch;
     }
     pch = strtok(NULL, " []:,()");
   }
   delete [] cstr;
}

vector<double> Paser::StringtoNumbers(string str)
{
   vector<double> results;
   char *cstr = new char[str.length()+1];
   char *pch = strtok(cstr, " []:,()");
   while(pch != NULL)
   {
     double tmp = atof(str);
     results.push_back(tmp);     
     pch = strtok(NULL, " []:,()");
   }
   delete [] cstr;
   return results;
}

void Paser::GetVariable(int level1, int level2)
{
   vector<double> tmp;
   switch(level2)
   {
       case DATADIR:
           sprintf(property.rawDir, "%s", line[index]);
           break;
       case DATANAME;
           sprintf(property.rawFile, "%s", line[index]);
           break; 
       case TMP:
           sprintf(property.storeDir, "%s", line[index]);
           break;
       case DATATYPE: 
           tmp = StringtoNumbers(line[index]);
           property.format = (int)tmp[0];
           break;
       case PC:
       {
          if(level1 == CUTTINGPLANE)
          {
              tmp = StringtoNumbers(line[index]);
              property.plane_center[0] = tmp[0];
              property.plane_center[1] = tmp[1];
              property.plane_center[2] = tmp[2];
          }          
          else if(level1 == SYMMETRY)
          {
              tmp = StringtoNumbers(line[index]);
              property.symmetryproperty.pos[0] = tmp[0];
              property.symmetryproperty.pos[1] = tmp[1];
              property.symmetryproperty.pos[2] = tmp[2];
          }
          break;
       }
       case PD:
       {
          tmp = StringtoNumbers(line[index]);
          if(level1 == CUTTINGPLANE)
          {
              property.plane_direction[0] = tmp[0];
              property.plane_direction[1] = tmp[1];
              property.plane_direction[2] = tmp[2];
              property.plane_direction.normalize();
          } 
          else if(level1 == SYMMETRY)
          {
              property.symmetryproperty.dir[0] = tmp[0];
              property.symmetryproperty.dir[1] = tmp[1];
              property.symmetryproperty.dir[2] = tmp[2];
              property.symmetryproperty.dir.normalize();
          }
          break;
       }
       case IC:
       {
          tmp = StringtoNumbers(line[index]);
          format.isContour = (svBool)tmp[0];
          break;
       }
       case CONTOUR:
       {
          bool isGlobal;
          int type;
          double value;
          int layer1, layer2;
          GetContours(line[index], isGlobal, type,
                 value, layer1, layer2);
          for(int i=0;i<property.contourproperty.seed_num;i++)
          {
             if(property.contourproperty.type[i].size()<0)
             {
                property.contourproperty.contourValues.add(value);
                property.contourproperty.type.add(type);
                property.contourproperty.isUpdate.add(1);
             }
             else if(property.contourproperty.type[i] == 0)
             {
                property.contourproperty.contourValues[i] = value;
                property.contourproperty.type[i] = type;
             }
             else if(property.contourproperty.type[i] == 1)
             {
                 if(type[i] == 1)
                 {
                      property.contourproperty.contourValues[i] = value;
                      property.contourproperty.type[i] = type;
                 }
             }
          }
          break;
       }
       case CWEIGHTS:
       {
          tmp = StringtoNumbers(line[index]);
          if(level1 == STEP1)
          {
             for(int i=0;i<7;i++)
                property.step1_kmeansproperty.clusterWeight.add(tmp[i]);
          }
          else if(level1 == STEP2)
          {
             for(int i=0;i<7;i++)
                property.step2_kmeansproperty.clusterWeight.add(tmp[i]);
          }
          break;
       }
       case CWHOLE:
       {
          tmp = StringtoNumbers(line[index]);
          if(level1 == STEP1)
          {
                property.step1_kmeansproperty.bywhole = tmp[0];
          }
          else if(level1 == STEP2)
          {
                property.step2_kmeansproperty.bywhole = tmp[1];
          }
          break;
       } 
       case CLAYER:
       {
          tmp = StringtoNumbers(line[index]);
          for(int i=0;i<property.step1_kmeansproperty.seed_num;i++)
          {
             property.step1_kmeansproperty.clusterLayer.add(-1);
          } 
          if(level1 == STEP1)
          {
             if(property.step1_kmeansproperty.bywhole)
             {
                for(int i=(int)tmp[0];i<=(int)tmp[1];i++)
                    property.step1_kmeansproperty.clusterLayer[i]++;
             }
             else
             {
                for(int i=(int)tmp[0];i<=(int)tmp[1];i++)
                {
                  if(i>(int)tmp[0])
                      property.step1_kmeansproperty.clusterLayer[i]=property.step1_kmeansproperty.clusterLayer[i-1]+1;
                  else
                      property.step1_kmeansproperty.clusterLayer[i]=0;
                }
             }
          }
          else if(level1 == STEP2)
          {
             if(property.step2_kmeansproperty.bywhole)
             {
                for(int i=(int)tmp[0];i<=(int)tmp[1];i++)
                    property.step2_kmeansproperty.clusterLayer[i]++;
             }
             else
             {
                for(int i=(int)tmp[0];i<=(int)tmp[1];i++)
                {    
                  if(i>(int)tmp[0])
                      property.step2_kmeansproperty.clusterLayer[i]=property.step2_kmeansproperty.clusterLayer[i-1]+1;
                  else
                      property.step2_kmeansproperty.clusterLayer[i]=0;
                }
             }
          }
          break;
       }
       case CMAG:
       {
         tmp = StringtoNumbers(line[index]);
         
         if(level1 == STEP1)
         {
           property.mag[0].add(tmp[0]);
           property.mag[0].add(tmp[1]);
         }
         else if(level1 == STEP2)
          {
           property.mag[1].add(tmp[0]);
           property.mag[1].add(tmp[1]);
          }
         break;
       }
       case CNUM:
       {
         tmp = StringtoNumbers(line[index]);
         if(level1 == STEP1)
         {
           property.step1_kmeansproperty.numCluster  = tmp[0];
         }
         else if(level1 == STEP2)
          {
           property.step2_kmeansproperty.numCluster  = tmp[0];
          }
         break;
       }
       case SPX:
       {
         tmp = StringtoNumbers(line[index]);
         property.symmetryproperty.x[0] = tmp[0];
         property.symmetryproperty.x[1] = tmp[1];
         property.symmetryproperty.x[2] = tmp[2];
         property.symmetryproperty.x.normalize();
         break;
       }
       case SPY:
       {
         tmp = StringtoNumbers(line[index]);
         property.symmetryproperty.y[0] = tmp[0];
         property.symmetryproperty.y[1] = tmp[1];
         property.symmetryproperty.y[2] = tmp[2];
         property.symmetryproperty.y.normalize();
         break;
       }
       case SANGLE:
       {
         tmp = StringtoNumbers(line[index]);
         property.symmetryproperty.angle_uncertain = tmp[0];
         break;
       }
       case SMAG:
       {
         tmp = StringtoNumbers(line[index]);
         property.symmetryproperty.mag_uncertain = tmp[0];
         break;
       }
       case ROI:
       {
         tmp = StringtoNumbers(line[index]);
         property.zmin = tmp[0];
         property.zmax = tmp[1];
         if(tmp.size() == 3)
         {
            property.nonz = tmp[2];
            property.layer_repeat = true;
         }
         break;
       }
    } 
}

void Paser::GetType(int &level1, int &level2, int index)
{
   switch(index)
   {
      case -1;
            break;
      case DATADIR:
            level1 = -1;
            level2 = DATADIR;
            break;
      case DATANAME:
            level1 = -1;
            level2 = DATANAME;
            break;
      case TMP:
            level1 = -1;
            level2 = TMP;
            break;
      case DATATYPE:
            level1 = -1;
            level2 = DATATYPE;
            break;
      case CUTTINGPLANE:
            level1 = CUTTINGPLANE;
            level2 = -1;
            break;
      case PC:
            level2 = PC;
            break;
      case PD:
            level2 = PD;
            break;
      case IC:
            level1 = -1;
            level2 = IC;
            break;
      case CONTOUR:
            level1 = -1;
            level2 = CONTOUR;
            break;
      case CLUSTER:
            level1 = -1;
            level2 = -1;
            break;
      case STEP1:
            level1 = STEP1;
            level2 = -1;
            break;
      case CWEIGHTS:
            level2 = CWEIGHTS;
            break;
      case CLAYER:
            level2 = CLAYER;
            break;
      case CWHOLE:
            level2 = CWHOLE;
            break;
      case CMAG:
            level2 = CMAG;
            break;
      case CNUM:
            level2 = CNUM;
            break;
      case STEP2:
            level1 = STEP1;
            level2 = -1;
            break;
      case SYMMETRY:
            level1 = SYMMETRY;
            level2 = -1;
            break;
      case SPX:
            level2 = SPX;
            break;
      case SPY:
            level2 = SPY;
            break;
      case SANGLE:
            level2 = SANGLE;
            break;
      case SMAG:
            level2 = SMAG;
            break;
      case ROI:
            level1 = ROI;
            level2 = -1;
            break;
   }
}


void Paser::SavetoFiles(char *ifile, char *ofile)
{

}

