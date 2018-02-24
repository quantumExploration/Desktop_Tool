#include "parser.h"
#include <string.h>
#include <fstream>
#include <stdlib.h>

using namespace std;
string variables[] =
{
 "datadir",
 "dataname",
 "foldertostorefiles",
 "type",
 "planecenter",
 "planedirection",
 "ifcontour",
 "contours",
 "clusters",
 "step1",
 "weights",
 "layer",
 "bywhole",
 "magrange",
 "numofclusters",
 "step2",
 "symmetry",
 "planex",
 "planey",
 "angleuncertainty",
 "maguncertainty",
 "roi",
  "cuttingplane"
};
//need to be fixed!!! add line and isdATA
int Paser::GetLineLoc(int label, int &lineHead)
{
        int l1, l2;
        bool flag = true;
        int head=-1;
       cerr<<"============"<<endl; 
       int count = 0;
        for(int i=0;i<line.size();i++)
        {
           if(!isData[i])
           {
              int index=-1;
              for(int j=0;j<LABEL_NUM;j++)
              {
                if(!line[i].compare(variables[j]))
                {
                    index = j; break;
                }
               }
              GetType(l1, l2, index+1);
           }
           else
           {
              if(l2==label)
              {cerr<<"label "<<l2<<endl;
                 if(flag==true)
                 {
                    lineHead = i;
                    head = textindex[i];  flag = false;
                 }
                count++;
//                 line.erase(line.begin()+i);
//                 isData.erase(isData.begin()+i);
                 cerr<<configtext[textindex[i]]<<endl;
                 configtext.erase(configtext.begin()+textindex[i]);
              }
           }
        }
       line.erase(line.begin()+lineHead, line.begin()+lineHead+count);
       isData.erase(isData.begin()+lineHead, isData.begin()+lineHead+count);
       cerr<<"============="<<endl;
       return head;
}

string Paser::GetContourString(string data)
{
   char *cstr = new char[data.length()+1];

   strcpy(cstr, data.c_str());

   string results("");
  
   int i=0; 
   char *pch = strtok(cstr, " []:,()");
   while(pch != NULL)
   {
     string tmp(pch);
     tmp = LowerCase(tmp);
     if(i>0)
         results = results +" "+ tmp;
     else
         results = tmp;
     i++;
     pch = strtok(NULL, " []:,()");
   }

   delete [] cstr;
   return results;
}

void Paser::AddContourName(int layer, double value)
{
   char str[20];
   sprintf(str,"l %d %d a %g", layer, layer, value);
   string p(str);
   bool flag = true;
   int head;
   for(int i=0;i<property.contourname.size();i++)
   {
      char *cstr = new char[property.contourname[i].length()+1];
      strcpy(cstr, property.contourname[i].c_str());
      char *pch;
      pch  = strtok(cstr, " []:,()");
      while(pch != NULL)
      {
         if(!strcmp(pch,"g"))
         {
            pch = strtok(NULL, " []:,()");
            pch = strtok(NULL, " []:,()");
         }
         else if(!strcmp(pch,"l"))
         {
           pch = strtok(NULL, " []:,()");
           int layer1 = atoi(pch);
           pch = strtok(NULL, " []:,()");
           int layer2 = atoi(pch);
           pch = strtok(NULL, " []:,()");
           if(!strcmp(pch,"r"))
           {
              pch = strtok(NULL, " []:,()");
           }
           else
           {
              pch = strtok(NULL, " []:,()");
              double v = atof(pch);
              if(layer1 == layer && layer2 == layer && fabs(value-v)<1e-18)
              {
                 head=i;
                 flag = false;break;
              }
           }
         }
        pch = strtok(NULL, " []:,()");
        if(!flag)break;
       }
       delete [] cstr;
   }
   if(flag)
   {
       property.contourname.push_back(p);
   }
   else
   {
       property.contourname.erase(property.contourname.begin()+head);
       property.contourname.insert(property.contourname.begin()+head,p);
   }  
}
void Paser::AddContourName(double value)
{
   char str[20];
   sprintf(str,"g r %f", value);
   string p(str);
   bool flag = true;
   int head;
   for(int i=0;i<property.contourname.size();i++)
   {
      char *cstr = new char[property.contourname[i].length()+1];
      strcpy(cstr, property.contourname[i].c_str());
      char *pch = strtok(cstr, " []:,()");
      while(pch != NULL)
      {
         if(!strcmp(pch,"g"))
         {
            pch = strtok(NULL, " []:,()");
            if(!strcmp(pch,"r"))
            {
              pch = strtok(NULL, " []:,()");
              double v = atof(pch);
              if( fabs(value-v)<1e-18)
              {
                 head = i;
                 flag = false;
                 break;
              }
            }
            else
            {
              pch = strtok(NULL, " []:,()");
            }
         }
         else if(!strcmp(pch,"l"))
         {
           pch = strtok(NULL, " []:,()");
           pch = strtok(NULL, " []:,()");
           pch = strtok(NULL, " []:,()");
              pch = strtok(NULL, " []:,()");
         }
         pch = strtok(NULL, " []:,()");
         if(!flag)
         {
            break;
         }
       }
       delete [] cstr;
   }  
   if(flag)
   {
       property.contourname.push_back(p);
   }
   else
   {
       property.contourname.erase(property.contourname.begin()+head);
       property.contourname.insert(property.contourname.begin()+head,p);
   } 
}

string Paser::LowerCase(string data)
{
   char *cstr = new char[data.length()+1];
   
   strcpy(cstr, data.c_str());
   
   for(int i=0;i<data.length();i++)
   {
/*      if(cstr[i]>='a' && cstr[i]<='z')
      {
         cstr[i] = (char)((int)(cstr[i])-32);
      }
      else
*/
      if(cstr[i]>='A' && cstr[i]<='Z')
      {
         cstr[i] = (char)((int)(cstr[i])+32);
      }
   }
   string results(cstr);

   delete [] cstr;
   return results;
}

vector<double> Paser::StringtoNumbers(string str)
{
   vector<double> results;
   char *cstr = new char[str.length()+1];
   strcpy(cstr, str.c_str());
   char *pch = strtok(cstr, " []:,()");
   while(pch != NULL)
   { 
     double tmp = atof(pch);
     results.push_back(tmp);     
     pch = strtok(NULL, " []:,()");
   }
   delete [] cstr;
   return results;
}

Paser::Paser()
{
	update_contour = true;
	update_cluster = true;
	update_data = true;
        update_render = true;
        update_symmetry = true;

	InitFiles();
	SetVariables();
	SetFiles();
}

void Paser::InitFiles()
{
	property.rawDir = new char[100];
	property.rawFile = new char[100];
	property.storeDir = new char[100];
	property.planename = new char[100];
	
    property.step1_kmeansproperty.file1 = new char[400];
    property.step1_kmeansproperty.file2 = new char[400];
    property.step2_kmeansproperty.file1 = new char[400];
    property.step2_kmeansproperty.file2 = new char[400];

    property.contourproperty.outputfile = new char[400];
    property.contourproperty.vtkdir = new char[200]; 
    property.contourproperty.wholefile = new char[400];
    property.contourproperty.planename = new char[100];

    property.symmetryproperty.inputfile = new char[400];
    property.symmetryproperty.datafile = new char[200];
    property.symmetryproperty.outputdir = new char[200];
    for(int i=0;i<SYMMETRY_TYPE;i++)
    {
      property.symmetryproperty.outputfile[i] = new char[400];
    }
   
    property.contourtreeproperty.dir = new char[200];
    property.contourtreeproperty.planename = new char[100];
}

void Paser::SetFiles()
{
    sprintf(property.planename,"%0.2f%0.2f%0.2f%0.2f%0.2f%0.2f", 
                   property.plane_center[0],
		   property.plane_center[1],
                   property.plane_center[2],
                   property.plane_vector[0],
                   property.plane_vector[1],
                   property.plane_vector[2]);

//----------contourtree----------------------------------------------				   
	sprintf(property.contourtreeproperty.dir, "%s/%s/",
	        property.storeDir,property.rawFile);
	sprintf(property.contourtreeproperty.planename, "%s", 
	        property.planename);

//----------------------contour--------------------------------------
    sprintf(property.contourproperty.planename, "%s", property.planename);
    sprintf(property.contourproperty.wholefile,
	        "%s/%s/contourall.txt", 
			property.storeDir, property.rawFile);
	sprintf(property.contourproperty.outputfile,  
	        "%s/%s/contour.txt", 
			property.storeDir, property.rawFile);
	sprintf(property.contourproperty.vtkdir,"%s/%s/", 
	        property.storeDir, property.rawFile);// = strdup(str);
	
//------------------------step1-------------------------------------
	sprintf(property.step1_kmeansproperty.file1, "%s/%s/input.txt", 
	        property.storeDir, property.rawFile);//= strdup(str);
	sprintf(property.step1_kmeansproperty.file2,"%s/%s/output.txt", 
	        property.storeDir, property.rawFile);// = strdup(str);

//--------------------------step2-----------------------------------
	sprintf(property.step2_kmeansproperty.file1,"%s/%s/input.txt", 
	        property.storeDir, property.rawFile);// = strdup(str);
	sprintf(property.step2_kmeansproperty.file2,"%s/%s/output.txt", 
	        property.storeDir, property.rawFile);// = strdup(str);

//----------------------------symmetry-------------------------------
    sprintf(property.symmetryproperty.datafile, "%s/%s/", 
	        property.storeDir, property.rawFile);//= strdup(str);
    if(!property.isContour)
    {
        sprintf(property.symmetryproperty.inputfile,"%s/%s/sort%s", 
		        property.storeDir, property.rawFile, property.planename);//=strdup(str);
    }
    else
    {
		sprintf(property.symmetryproperty.inputfile,"%s",
		        property.contourproperty.wholefile);
	}
    char *symmetrystr = new char[200];
    for(int j=0;j<200;j++) symmetrystr[j] = '\0';
    sprintf(property.symmetryproperty.outputdir, "%s/%s/", 
                property.storeDir, 
                property.rawFile);
    sprintf(symmetrystr, "%s%0.2f%0.2f%0.2f%0.2f%0.2f%0.2f", 
                  symmetrystr,
                  property.symmetryproperty.pos[0], 
                  property.symmetryproperty.pos[1], 
                  property.symmetryproperty.pos[2],
                  property.symmetryproperty.dir[0], 
                  property.symmetryproperty.dir[1], 
                  property.symmetryproperty.dir[2]);
    char *str = new char[400];
    for(int i=0;i<SYMMETRY_TYPE;i++)
    {
              switch(i)
              {
                 case 0:  sprintf(str,"%s/0xsyszs%s%s.txt",
                     property.symmetryproperty.outputdir, 
                     //property.isContour,
                     property.planename,
                     symmetrystr);
                     break;
                 case 1:  sprintf(str,"%s/0xsysza%s%s.txt",
                     property.symmetryproperty.outputdir, 
                     //property.isContour, 
                     property.planename,
                     symmetrystr);
                     break;
                 case 2:  sprintf(str,"%s/0xsyazs%s%s.txt",
                     property.symmetryproperty.outputdir, 
                    // property.isContour, 
                     property.planename,
                     symmetrystr);
                     break;
                 case 3:  sprintf(str,"%s/0xsyaza%s%s.txt",
                     property.symmetryproperty.outputdir, 
                     //property.isContour, 
                     property.planename,
                     symmetrystr); 
                     break;
                 case 4:  sprintf(str,"%s/0xayszs%s%s.txt",
                     property.symmetryproperty.outputdir, 
                     //property.isContour, 
                     property.planename,
                     symmetrystr);
                     break;
                 case 5:  sprintf(str,"%s/0xaysza%s%s.txt",
                     property.symmetryproperty.outputdir, 
                     //property.isContour, 
                     property.planename,
                     symmetrystr);
                     break;
                 case 6:  sprintf(str,"%s/0xayazs%s%s.txt",
                     property.symmetryproperty.outputdir, 
                     //property.isContour, 
                     property.planename,
                     symmetrystr);
                     break;
                 case 7:  sprintf(str,"%s/0xayaza%s%s.txt",
                     property.symmetryproperty.outputdir, 
                     //property.isContour,
                     property.planename,
                     symmetrystr);
                     break;
        }
        strcpy(property.symmetryproperty.outputfile[i],str);              
    }
    delete []symmetrystr;
    delete []str;
}

void Paser::SetVariables()
{
	sprintf(property.rawDir, "data");
	sprintf(property.rawFile,"spin_proj_norm_vs_pos_p_v_z_10z_newcolumn");
	sprintf(property.storeDir,"tmp");

    seed_num = 109;

	property.plane_center[0] = 0;
	property.plane_center[1] = 0;
	property.plane_center[2] = 0;
	property.plane_vector[0] = 0;
	property.plane_vector[1] = 0;
	property.plane_vector[2] = 1;
	property.plane_distance = 0.25;
	
    property.isContour = false;
   
    property.step1_kmeansproperty.isNormalize = true;
	property.step1_kmeansproperty.dimension = 7;
	property.step1_kmeansproperty.numCluster = 25;
	property.step1_kmeansproperty.layer1 = 0;
	property.step1_kmeansproperty.layer2 = seed_num-1;
	property.step1_kmeansproperty.bywhole = 1;
	for(int i=0;i<7;i++)
		property.step1_kmeansproperty.clusterWeight.add(1);
	for(int i=0;i<seed_num;i++)
		property.step1_kmeansproperty.clusterLayer.add(0);

	property.step2_kmeansproperty.isNormalize = true;
	property.step2_kmeansproperty.dimension = 7;
	property.step2_kmeansproperty.numCluster = 10;
	property.step2_kmeansproperty.layer1 = 0;
	property.step2_kmeansproperty.layer2 = -1;
	property.step2_kmeansproperty.bywhole = 1;
	for(int i=0;i<7;i++)
		property.step2_kmeansproperty.clusterWeight.add(1);
	for(int i=0;i<seed_num;i++)
		property.step2_kmeansproperty.clusterLayer.add(0);

	property.symmetryproperty.pos = property.plane_center;
	property.symmetryproperty.dir[0] = 1;
	property.symmetryproperty.dir[1] = 1;
	property.symmetryproperty.dir[2] = 0;
	property.symmetryproperty.x[0]=-1;
	property.symmetryproperty.x[1]=1;
	property.symmetryproperty.x[2]=0;
	property.symmetryproperty.y[0]=0;
	property.symmetryproperty.y[1]=0;
	property.symmetryproperty.y[2]=1;
	property.symmetryproperty.planepos = property.plane_center;
	property.symmetryproperty.planedir = property.plane_vector;
	property.symmetryproperty.planedistance = property.plane_distance;
	property.symmetryproperty.angle_uncertain = 0.1;
	property.symmetryproperty.mag_uncertain = 1e-8;
    property.symmetryproperty.regenerate = 0;//property.isContour;

	property.contourproperty.seed_num = seed_num;
	property.contourproperty.contourValues = new svScalarArray[seed_num];
	for(int i=0;i<seed_num;i++)
	{
		property.contourproperty.isUpdate.add(1);
		property.contourproperty.type.add(0);
		property.contourproperty.contourValues[i].add(0.5);
	}
	
	property.contourtreeproperty.seed_num = seed_num;
	
	property.contourname.resize(1);
    property.contourname[0] = "G R 0.5";
	
	property.magrange = new svScalarArray[2];
	property.magrange[0].add(0);
	property.magrange[0].add(1);
	property.magrange[1].add(1);
	property.magrange[1].add(0);
	
	property.format = 4;
}

void Paser::SetContourValues(svScalarArray density)
{
	for(int i=0;i<seed_num;i++)
	{
		if(property.contourproperty.type[i] == 0)//ratio
		{
			property.contourproperty.contourValues[i][0] = property.contourproperty.contourValues[i][0] * density[i];
		}
	}
}

void Paser::SetData(int data)
{
   property.isContour = data;

   int l1, l2;
   bool flag = true;
   int head=-1;
   int headLine = -1;

   head = GetLineLoc(IC, headLine);
   if(head>=0)
   {
      char str[10];
      sprintf(str,"%d",property.isContour);
      string s(str);
      configtext.insert(configtext.begin()+head, s);
      line.insert(line.begin()+headLine,s);
      isData.insert(isData.begin()+headLine,1);
   }
}

void Paser::SetLayer(int zmin, int zmax, int nonz, int repeat)
{
   property.zmin = zmin;
   property.zmax = zmax;
   property.nonz = nonz;
   property.layer_repeat = repeat;

        int l1, l2;
        bool flag = true;
        int head=-1;
        int headLine = -1;
        head = GetLineLoc(ROI, headLine);
        if(head>=0)
        {
          char str[10];
          if(repeat)
            sprintf(str,"%d %d %d",zmin, zmax, nonz);
          else
            sprintf(str,"%d %d",zmin, zmax);
          string s(str);
          isData.insert(isData.begin()+headLine,1); 
          line.insert(line.begin()+headLine,s);
          configtext.insert(configtext.begin()+head, s);
        }

}

void Paser::SetSeedNum(int seed)
{
   for(int i=0;i<seed_num;i++)
           property.contourproperty.contourValues[i].free();

   seed_num = seed;
   property.contourproperty.seed_num = seed;
   property.contourtreeproperty.seed_num = seed;
  
   delete [] property.contourproperty.contourValues; 
   property.contourproperty.contourValues = new svScalarArray[seed];
}

void Paser::SetPlane(svScalar distance, svVector3 c)
{
        property.plane_center = c;
	property.plane_distance = distance;
        property.symmetryproperty.planepos = c;
	property.symmetryproperty.planedistance = distance;
        SetFiles();
}

void Paser::ReadFiles(char *file)
{
/*   prespeciallines.clear();
   for(int i=0;i<speciallines.size();i++)
   {
       prespeciallines.push_back(speciallines[i]);
   }
*/
   clusterline1="";
   clusterline2="";
   line.clear();
   configtext.clear();
   speciallines.clear();
   textindex.clear();
//=======start=============================	
   ifstream infile(file);

   int count = 0;
   std::string str;
   while(std::getline(infile,str))
   {
         configtext.push_back(str);
	 char *tmpstr = new char[str.length()+1];
	 strcpy(tmpstr, str.c_str());
	 
	 int length = 0;
	 for(int i=0;i<strlen(tmpstr);i++)
	 {
		 if(tmpstr[i]!=' ')
		 {
			 length = i;
			 break;
		 }
	 }
/*     char *cstr = new char[strlen(tmpstr)-length];
     for(int i=length;i<strlen(tmpstr);i++)
	 {
		 cstr[i-length] = tmpstr[i];
	 }
*/
       char *cstr = new char[str.length()+1];
       strcpy(cstr, str.c_str());

     if(cstr[0]=='*')//!strcmp(cstr[0], '*'))  
     {
        isData.push_back(false);
        string p("\0");  
        char *pch = strtok(cstr, " *:,()");
        while(pch != NULL)
        {
           string tmp(pch);
           p = p + tmp;//cerr<<pch<<" "<<cstr<<" ";
           pch = strtok(NULL, " *:,()");
        }//cerr<<p<<" "<<endl;
        string p2 = LowerCase(p);
        for(int i=0;i<LABEL_NUM;i++)
        {
            if(!p2.compare(variables[i]))
            {
                textindex.push_back(count);
                line.push_back(p2);
		break;
            }
        }
     }
     else if(cstr[0]=='#')//!strcmp(cstr[0], '#'))
     {
     }
     else
     {
        line.push_back(str);//LowerCase(str));   
        isData.push_back(true);
        textindex.push_back(count);
     }
     count++;
     delete [] cstr;
	 delete [] tmpstr;
   }
   infile.close();

   GetContent();
   SetFiles();
cerr<<"============Set Files========="<<endl;
   if(precontourname.size()!=property.contourname.size())
    update_contour = true;

   precontourname.clear();
   for(int i=0;i<property.contourname.size();i++)
   {
      precontourname.push_back(property.contourname[i]);
   }
cerr<<"==========Read Config============"<<endl;
}

void Paser::GetContent()
{
   int level1=-1;
   int level2=-1;

   for(int i=0;i<line.size();i++)
   {cerr<<line[i]<<endl;
     char *cstr = new char[line[i].length()+1];
     strcpy(cstr, line[i].c_str());
     if(cstr[0]>='a' && cstr[0]<='z' && !isData[i])
     {
        int index=-1;
        for(int j=0;j<LABEL_NUM;j++)
        {
           if(!line[i].compare(variables[j]))
           { 
              index = j; break;
           }
        }//cerr<<index<<endl;
        GetType(level1, level2, index+1);
     }
     else
     {//cerr<<level1<<" "<<level2<<" "<<i<<endl;
        GetVariable(level1, level2, i);cerr<<update_cluster<<endl;
     }
     delete [] cstr; 
   }
   cerr<<"===========Get Content=========="<<endl;
}

void Paser::SetContours(int layer, double value)
{
	property.contourproperty.contourValues[layer] = value;
	property.contourproperty.type[layer] = 1;
	property.contourproperty.isUpdate[layer] = 1;
        AddContourName(layer, value);
 
        int l1, l2;
        bool flag = true;
        int head=-1;
        int headLine = -1;
        head = GetLineLoc(CONTOUR, headLine);
        if(head>=0)
        {
           for(int j=0;j<property.contourname.size();j++)
           {cerr<<property.contourname[j]<<endl;
               isData.insert(isData.begin()+headLine,1);
               line.insert(line.begin()+headLine, property.contourname[j]);
               configtext.insert(configtext.begin()+head, property.contourname[j]);
           }
        }
}

void Paser::SetContours(double value)
{
	for(int i=0;i<property.contourproperty.seed_num;i++)
	{
		property.contourproperty.contourValues[i] = value;
		property.contourproperty.type[i] = 0;
		property.contourproperty.isUpdate[i] = 1;
	}
       AddContourName(value);
        int l1, l2;
        bool flag = true;
        int head=-1;
        int headLine = -1;
        head = GetLineLoc(CONTOUR, headLine);
        if(head>=0)
        {
           for(int j=0;j<property.contourname.size();j++)
           {
               isData.insert(isData.begin()+headLine,1);
               line.insert(line.begin()+headLine, property.contourname[j]);
               configtext.insert(configtext.begin()+head, property.contourname[j]);
           }
        }

}

void Paser::GetContours(string str, bool &isGlobal, int &type,
                 double &value, int &layer1, int &layer2)
{
   char *cstr = new char[str.length()+1];
   strcpy(cstr, str.c_str());
   char *pch = strtok(cstr, " []:,()");
   while(pch != NULL)
   {
     if(!strcmp(pch,"g"))
     {
        isGlobal = true;
        layer1 = 0;
        layer2 = property.contourproperty.seed_num-1;
        pch = strtok(NULL, " []:,()");
        if(!strcmp(pch,"r"))
        {
           type = 0;
        }
        else
        {
           type = 1;
        }
        pch = strtok(NULL, " []:,()");
        value = atof(pch);
     }
     else if(!strcmp(pch,"l"))
     {
        isGlobal = false;
        pch = strtok(NULL, " []:,()");
        layer1 = atoi(pch);
        pch = strtok(NULL, " []:,()");
        layer2 = atoi(pch);
        pch = strtok(NULL, " []:,()");
        if(!strcmp(pch,"r"))
        {
           type = 0;
        }
        else
        {
           type = 1;
        }
        pch = strtok(NULL, " []:,()");
        value = atof(pch);
     }
     pch = strtok(NULL, " []:,()");
   }
   delete [] cstr;
}

void Paser::GetVariable(int &level1, int level2, int index) //need to improve. Save "cluster" and "contour" string first.
{
//cerr<<level1<<" "<<level2<<" "<<index<<endl;
   vector<double> tmp;
   switch(level2)
   {
       case DATADIR:
	{       if (strcmp(property.rawDir,line[index].c_str()) != 0) 
		   {cerr<<property.rawDir<<" "<<line[index].c_str()<<endl;
			   update_data = true;
		   }
             strcpy(property.rawDir, line[index].c_str());
             cerr<<property.rawDir<<endl;
        //   sprintf(property.rawDir, "%s", line[index]);
           break;
        }
       case DATANAME:
	       if (strcmp(property.rawFile,line[index].c_str()) != 0) 
		   {cerr<<property.rawFile<<" "<<line[index].c_str()<<endl;
			   update_data = true;
		   }
             strcpy(property.rawFile, line[index].c_str());
             cerr<<property.rawFile<<endl;
           break; 
       case TMP:
             strcpy(property.storeDir, line[index].c_str());
             cerr<<property.storeDir<<endl;
           break;
       case DATATYPE: 
           tmp = StringtoNumbers(line[index]);
           property.format = (int)tmp[0];
           cerr<<property.format<<endl;
           break;
       case PC:
       {
          if(level1 == CUTTINGPLANE)
          {
              tmp = StringtoNumbers(line[index]);
			  if(!(fabs(tmp[0]-property.plane_center[0])<1e-3
			  &&fabs(tmp[1]-property.plane_center[1])<1e-3
			  &&fabs(tmp[2]-property.plane_center[2])<1e-3))
			  {cerr<<tmp[0]<<" "<<tmp[1]<<" "<<tmp[2]<<" "<<property.plane_center[0]<<" "<<property.plane_center[1]<<" "<<property.plane_center[2]<<endl;
				  update_data = true;
			  }
              property.plane_center[0] = tmp[0];
              property.plane_center[1] = tmp[1];
              property.plane_center[2] = tmp[2];
              property.symmetryproperty.planepos[0] = tmp[0];
              property.symmetryproperty.planepos[1] = tmp[1];
              property.symmetryproperty.planepos[2] = tmp[2];
              cerr<<property.plane_center[0]<<" "<<property.plane_center[1]<<" "<<property.plane_center[2]<<endl;
          }          
          else if(level1 == SYMMETRY)
          {
              tmp = StringtoNumbers(line[index]);
			  if(!(fabs(tmp[0]-property.symmetryproperty.pos[0])<1e-3
			  &&fabs(tmp[1]-property.symmetryproperty.pos[1])<1e-3
			  &&fabs(tmp[2]-property.symmetryproperty.pos[2])<1e-3))
			  {
				  update_symmetry = true;
			  }
              property.symmetryproperty.pos[0] = tmp[0];
              property.symmetryproperty.pos[1] = tmp[1];
              property.symmetryproperty.pos[2] = tmp[2];
              cerr<<property.symmetryproperty.pos[0]<<" "<<property.symmetryproperty.pos[1]<<" "<<property.symmetryproperty.pos[2]<<endl;  
          }
          break;
       }
       case PD:
       {
          tmp = StringtoNumbers(line[index]);
		  double sum = tmp[0] * tmp[0]
		 + tmp[1] * tmp[1]
		 + tmp[2] * tmp[2];
		 sum = sqrt(sum);
		 tmp[0] = tmp[0]/sum;
		 tmp[1] = tmp[1]/sum;
		 tmp[2] = tmp[2]/sum;
          if(level1 == CUTTINGPLANE)
          {
			  if(!(fabs(tmp[0]-property.plane_vector[0])<1e-3
			  &&fabs(tmp[1]-property.plane_vector[1])<1e-3
			  &&fabs(tmp[2]-property.plane_vector[2])<1e-3))
			  {
				  update_data = true;
			  }
              property.plane_vector[0] = tmp[0];
              property.plane_vector[1] = tmp[1];
              property.plane_vector[2] = tmp[2];
              property.symmetryproperty.planedir[0] = tmp[0];
              property.symmetryproperty.planedir[1] = tmp[1];
              property.symmetryproperty.planedir[2] = tmp[2];
              property.symmetryproperty.planedir=normalize(property.symmetryproperty.planedir);
              property.plane_vector=normalize(property.plane_vector);
             cerr<<property.plane_vector[0]<<" "<<property.plane_vector[1]<<" "<<property.plane_vector[2]<<endl;
          } 
          else if(level1 == SYMMETRY)
          {
			  if(!(fabs(tmp[0]-property.symmetryproperty.dir[0])<1e-3
			  &&fabs(tmp[1]-property.symmetryproperty.dir[1])<1e-3
			  &&fabs(tmp[2]-property.symmetryproperty.dir[2])<1e-3))
			  {
				  update_symmetry = true;
			  }
              property.symmetryproperty.dir[0] = tmp[0];
              property.symmetryproperty.dir[1] = tmp[1];
              property.symmetryproperty.dir[2] = tmp[2];
              property.symmetryproperty.dir=normalize(property.symmetryproperty.dir);
              cerr<<property.symmetryproperty.dir[0]<<" "<<property.symmetryproperty.dir[1]<<" "<<property.symmetryproperty.dir[2]<<endl;
          }
          break;
       }
       case IC:
       {
          tmp = StringtoNumbers(line[index]);
		  if(!fabs(tmp[0]-property.isContour)<1e-3)
		  {
			  update_data = true;
		  }
          property.isContour = (svBool)tmp[0];
          property.symmetryproperty.regenerate = 0;//property.isContour;
          cerr<< property.isContour <<endl;
          break;
       }
       case CONTOUR:
       {
          /*bool isGlobal;
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
          }*/
         string cc = GetContourString(line[index]);
         if(update_contour!=true)
         {
	  bool flag = true;
	  for(int i=0;i<precontourname.size();i++)
	  {//cerr<<cc<<" "<<property.contourname[i]<<endl;
		if(cc.compare(precontourname[i]) == 0 )
		{
			flag = false;	
			break;
		}
	  }
	  if(flag)
	  {
               update_contour = true;
          }
         }//cerr<<level1<<endl; 
         if(level1 == -1)
         {
              for(int i=0;i<seed_num;i++)
              {
	         property.contourproperty.contourValues[i].free();
              }
            //  delete [] property.contourproperty.contourValues;
              property.contourproperty.isUpdate.free();
              property.contourproperty.type.free();
              property.contourname.clear();
          }
          level1 = 0;//cerr<<level1<<endl;
//          speciallines.push_back(cc);
	  property.contourname.push_back(cc);
          cerr<<cc<<endl;
          break;
       }
       case CWEIGHTS:
       {
          tmp = StringtoNumbers(line[index]);
          if(level1 == STEP1)
          {
	     for(int i=0;i<7;i++)
         	  {
				  if(property.step1_kmeansproperty.clusterWeight[i]!=tmp[i])
				  {
					  update_cluster = true;
					  break;
				  }
                               cerr<<tmp[i]<<" ";
			  }cerr<<endl;
              property.step1_kmeansproperty.clusterWeight.free();

             for(int i=0;i<7;i++)
                property.step1_kmeansproperty.clusterWeight.add(tmp[i]);
          }
          else if(level1 == STEP2)
          {
			  for(int i=0;i<7;i++)
			  {
				  if(property.step2_kmeansproperty.clusterWeight[i]!=tmp[i])
				  {
					  update_cluster = true;
					  break;
				  }
                             cerr<<tmp[i]<<" ";
			  }cerr<<endl;
			  property.step2_kmeansproperty.clusterWeight.free();
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
			  if(property.step1_kmeansproperty.bywhole != tmp[0])
				  update_cluster=true;
                property.step1_kmeansproperty.bywhole = tmp[0];cerr<<tmp[0]<<endl;
          }
          else if(level1 == STEP2)
          {
			  if(property.step2_kmeansproperty.bywhole != tmp[0])
				  update_cluster=true;
                property.step2_kmeansproperty.bywhole = tmp[0];cerr<<tmp[0]<<endl;
          }
          break;
       } 
       case CLAYER:
       {
          
          tmp = StringtoNumbers(line[index]);
          if(level1 == STEP1)
          {
                  clusterline1 = line[index];
                   cerr<<clusterline1<<endl;
		  if(tmp[0]!=property.step1_kmeansproperty.layer1)
			  update_cluster = true;
		  if(tmp[1]!=property.step1_kmeansproperty.layer2)
			  update_cluster = true;
           }
           else if(level1 == STEP2)
           {
                clusterline2 = line[index];
                cerr<<clusterline2<<endl;
               cerr<<tmp[0]<<" "<<property.step2_kmeansproperty.layer1<<" "<<tmp[1]<<" "<<property.step2_kmeansproperty.layer2<<endl; 
		if(tmp[0]!=property.step2_kmeansproperty.layer1)
			 update_cluster = true;
		if(tmp[1]!=property.step2_kmeansproperty.layer2)
			 update_cluster = true;
           }  
          /*for(int i=0;i<property.step1_kmeansproperty.seed_num;i++)
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
          }*/
          property.step1_kmeansproperty.clusterLayer.free();
          property.step2_kmeansproperty.clusterLayer.free();
	  speciallines.push_back(line[index]);
          break;
       }
       case CMAG:
       {
         tmp = StringtoNumbers(line[index]);
		 
         if(level1 == STEP1)
         {
		   if(fabs(property.magrange[0][0]-tmp[0])>1e-3)
			   update_cluster = true;
		   if(fabs(property.magrange[0][1]-tmp[1])>1e-3)
			   update_cluster = true;
		   property.magrange[0].free();
           property.magrange[0].add(tmp[0]);
           property.magrange[0].add(tmp[1]);
            cerr<<tmp[0]<<" "<<tmp[1]<<endl;
         }
         else if(level1 == STEP2)
          {
		   if(fabs(property.magrange[1][0]-tmp[0])>1e-3)
			   update_cluster = true;
		   if(fabs(property.magrange[1][1]-tmp[1])>1e-3)
			   update_cluster = true;
		   property.magrange[1].free();
           property.magrange[1].add(tmp[0]);
           property.magrange[1].add(tmp[1]);
           cerr<<tmp[0]<<" "<<tmp[1]<<endl;
          }
         break;
       }
       case CNUM:
       {
         tmp = StringtoNumbers(line[index]);
         if(level1 == STEP1)
         {
	   if(!fabs(property.step1_kmeansproperty.numCluster -tmp[0])<1e-3)
			   update_cluster = true;
           property.step1_kmeansproperty.numCluster  = tmp[0];
           cerr<<tmp[0]<<endl;
         }
         else if(level1 == STEP2)
          {
	  if(!fabs(property.step2_kmeansproperty.numCluster -tmp[0])<1e-3)
			   update_cluster = true;
           property.step2_kmeansproperty.numCluster  = tmp[0];
           cerr<<tmp[0]<<endl;
          }
         break;
       }
       case SPX:
       {
         tmp = StringtoNumbers(line[index]);
		 double sum = tmp[0] * tmp[0]
		 + tmp[1] * tmp[1]
		 + tmp[2] * tmp[2];
		 sum = sqrt(sum);
		 tmp[0] = tmp[0]/sum;
		 tmp[1] = tmp[1]/sum;
		 tmp[2] = tmp[2]/sum;
		 if(!(fabs(tmp[0]-property.symmetryproperty.x[0])<1e-3
			  &&fabs(tmp[1]-property.symmetryproperty.x[1])<1e-3
			  &&fabs(tmp[2]-property.symmetryproperty.x[2])<1e-3))
			  {
				  update_symmetry = true;
			  }
         property.symmetryproperty.x[0] = tmp[0];
         property.symmetryproperty.x[1] = tmp[1];
         property.symmetryproperty.x[2] = tmp[2];
         property.symmetryproperty.x.normalize();
         cerr<<property.symmetryproperty.x[0]<<" "<<property.symmetryproperty.x[1]<<" "<<property.symmetryproperty.x[2]<<endl;
         break;
       }
       case SPY:
       {
         tmp = StringtoNumbers(line[index]);
		 double sum = tmp[0] * tmp[0]
		 + tmp[1] * tmp[1]
		 + tmp[2] * tmp[2];
		 sum = sqrt(sum);
		 tmp[0] = tmp[0]/sum;
		 tmp[1] = tmp[1]/sum;
		 tmp[2] = tmp[2]/sum;
		 if(!(fabs(tmp[0]-property.symmetryproperty.y[0])<1e-3
			  &&fabs(tmp[1]-property.symmetryproperty.y[1])<1e-3
			  &&fabs(tmp[2]-property.symmetryproperty.y[2])<1e-3))
			  {
				  update_symmetry = true;
			  }
         property.symmetryproperty.y[0] = tmp[0];
         property.symmetryproperty.y[1] = tmp[1];
         property.symmetryproperty.y[2] = tmp[2];
         property.symmetryproperty.y.normalize();
         cerr<< property.symmetryproperty.y[0]<<" "<< property.symmetryproperty.y[1]<<" "<< property.symmetryproperty.y[2]<<endl;
         break;
       }
       case SANGLE:
       {
         tmp = StringtoNumbers(line[index]);
         tmp[0] = tmp[0]/180.*3.1415926;
	 if(fabs(tmp[0] - property.symmetryproperty.angle_uncertain)>1e-3)
			 update_symmetry = true;
         property.symmetryproperty.angle_uncertain = tmp[0];
         cerr<<tmp[0]<<endl;
         break;
       }
       case SMAG:
       {
         tmp = StringtoNumbers(line[index]);
		 if(fabs(tmp[0] - property.symmetryproperty.mag_uncertain)>1e-3)
			 update_symmetry = true;
         property.symmetryproperty.mag_uncertain = tmp[0];
         cerr<<tmp[0]<<endl;
         break;
       }
       case ROI:
       {
         tmp = StringtoNumbers(line[index]);
		 if(fabs(tmp[0] - property.zmin )>1e-3)
			 update_render = true;
		 if(fabs(tmp[0] - property.zmax )>1e-3)
			 update_render = true;
         property.zmin = tmp[0];
         property.zmax = tmp[1];
         cerr<<property.zmin<<" "<<property.zmax<<endl;
         if(tmp.size() == 3)
         {
			 if(!property.layer_repeat)
			 {
				 update_render = true;
			 }
			 if(fabs(property.nonz-tmp[2])>1e-3)
				 update_render = true;
            property.nonz = tmp[2];
            property.layer_repeat = true;
		
         }
	 else
	 {
		 if(property.layer_repeat)
		 {
			 update_render = true;
		 }
                 property.nonz = -1;
		 property.layer_repeat  = false;
	 }
         break;
       }
    } 
}

void Paser::ProcessContour()
{
    bool isGlobal;
    int type;
    double value;
    int l1, l2;
    
    for(int i=0;i<seed_num;i++)
    {
                property.contourproperty.contourValues[i].add(-1);
                property.contourproperty.type.add(-1);
                property.contourproperty.isUpdate.add(1);
    }
    for(int i=0;i<property.contourname.size();i++)
    {
        GetContours(property.contourname[i], isGlobal, type,
                 value, l1, l2);

//cerr<<isGlobal<<" "<<type<<" "<<value<<" "<<l1<<" "<<l2<<endl; 
        for(int j=l1;j<=l2;j++)
        {
           if(property.contourproperty.contourValues[j][0]<0)
           {
                property.contourproperty.contourValues[j][0]=value;
                property.contourproperty.type[j]=type;
           }
           else 
           {
               if(isGlobal == 0)
               {
                      property.contourproperty.contourValues[j][0] = value;
                      property.contourproperty.type[j] = type;
               }
           }
        }
    }
/*	int index = -1;
	for(int i=0;i<speciallines.size();i++)
	{
		char *cstr = new char[data.length()+1];
		strcpy(cstr, speciallines[i].c_str());
		
		if((cstr[0] >= 'a' && cstr[0] <= 'z')
			index = i;
			break;
		}
		delete [] cstr;
	}
	
	if(index >= 0)
	{
	    GetContours(speciallines[index], isGlobal, type,
                 value, l1, l2);
        for(int i=0;i<property.contourproperty.seed_num;i++)
        {
			if(isGlobal || (!isGlobal && i>=l1 && i<=l2))
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
                    SetFiles(); }
             else if(property.contourproperty.type[i] == 1)
             {
                 if(type == 1)
                 {
                      property.contourproperty.contourValues[i] = value;
                      property.contourproperty.type[i] = type;
                 }
             }
			}
        }
	}
*/
}

void Paser::ProcessClusterLayers()
{cerr<<clusterline1<<endl;
cerr<<clusterline2<<endl;
   vector<double> tmp = StringtoNumbers(clusterline1);
cerr<<clusterline1<<" "<<clusterline2<<" "<<tmp[0]<<" "<<tmp[1]<<endl;
   if(tmp.size()==2)
   {
     property.step1_kmeansproperty.layer1 = tmp[0];
     property.step1_kmeansproperty.layer2 = tmp[1];
     for(int i=0;i<seed_num;i++)
     {
       property.step1_kmeansproperty.clusterLayer.add(-1);
     }
     if(property.step1_kmeansproperty.bywhole)
     {
       for(int i=(int)tmp[0];i<=(int)tmp[1];i++)
       {
          property.step1_kmeansproperty.clusterLayer[i]++;
       }
     }
     else
     {
       for(int i=(int)tmp[0];i<=(int)tmp[1];i++)
       {
         if(i>(int)tmp[0])
         {
              property.step1_kmeansproperty.clusterLayer[i]=property.step1_kmeansproperty.clusterLayer[i-1]+1;
         }
         else
         {
              property.step1_kmeansproperty.clusterLayer[i]=0;
         } 
       }
      }
   }

   tmp = StringtoNumbers(clusterline2);
   if(tmp.size()==2)
   {
     if(tmp[1]>=tmp[0])
     {
     if(tmp[0] < property.step1_kmeansproperty.layer1)
     {
        tmp[0] = property.step1_kmeansproperty.layer1;
     }
     else if(tmp[1] < property.step1_kmeansproperty.layer2)
     {
        tmp[1] = property.step1_kmeansproperty.layer2;
     }
     }
     property.step2_kmeansproperty.layer1 = tmp[0];
     property.step2_kmeansproperty.layer2 = tmp[1];

     for(int i=0;i<seed_num;i++)
     {
       property.step2_kmeansproperty.clusterLayer.add(-1);
     }
     if(property.step2_kmeansproperty.bywhole)
     {
       for(int i=(int)tmp[0];i<=(int)tmp[1];i++)
       {
          property.step2_kmeansproperty.clusterLayer[i]++;
       }
     }
     else
     {
       for(int i=(int)tmp[0];i<=(int)tmp[1];i++)
       {
         if(i>(int)tmp[0])
         {
              property.step2_kmeansproperty.clusterLayer[i]=property.step2_kmeansproperty.clusterLayer[i-1]+1;
         }
         else
         {
              property.step2_kmeansproperty.clusterLayer[i]=0;
         } 
       }
      }
   }
/*	bool isGlobal;
    int type;
    double value;
    int l1, l2;
    
	int index = -1;
	for(int i=0;i<speciallines.size();i++)
	{
		char *cstr = new char[data.length()+1];
		strcpy(cstr, speciallines[i].c_str());
		
		if((cstr[0] >= 'a' && cstr[0] <= 'z')
			||(cstr[0] >= 'A' && cstr[0] <= 'Z'))
		{
			index = i;
		}
		
		delete [] cstr;
	}
	if(speciallines.size()>index+1)
	{	index = index+1;
	
    vector<double> tmp = StringtoNumbers(speciallines[index]);
	property.step1_kmeansproperty.layer1 = tmp[0];
	property.step1_kmeansproperty.layer2 = tmp[0];
    for(int i=0;i<property.step1_kmeansproperty.seed_num;i++)
    {
        property.step1_kmeansproperty.clusterLayer.add(-1);
    } 
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
    }

	if(speciallines.size()>index+1)
	{	index = index+1;
	tmp.clear();
	tmp = StringtoNumbers(speciallines[index]);
	property.step2_kmeansproperty.layer1 = tmp[0];
	property.step2_kmeansproperty.layer2 = tmp[0];
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
	}
*/
}

void Paser::GetType(int &level1, int &level2, int index)
{
   switch(index)
   {
      case -1:
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
            level1 = CUTTINGPLANE;//cerr<<"level"<<level1<<endl;
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
            level1 = STEP2;
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
            level1 = -1;
            level2 = ROI;
            break;
   }
}

void Paser::SavetoFile(char *file)
{
   ofstream outfile(file);
   for(int i=0;i<configtext.size();i++)
       outfile<<configtext[i]<<endl;
   outfile.close();
}

void Paser::SavetoFiles(char *file)
{
/*
    ofstream outfile(file);
	outfile<<"#datadir:"<<endl;
	outfile<<property.rawDir<<endl;
	outfile<<"#dataname:"<<endl;
	outfile<<property.rawFile<<endl;
	outfile<<"#folderToStoreFiles:"<<endl;
	outfile<<property.storeDir<<endl;
	outfile<<"#type"<<endl;
	outfile<<property.format<<endl;
	outfile<<"#cuttingPlane"<<endl;
	outfile<<"#planeCenter"<<endl;
	outfile<<property.plane_center[0]
	<<" "<<property.plane_center[1]
	<<" "<<property.plane_center[2]<<endl;
	outfile<<"#planeDirection"<<endl;
	outfile<<property.plane_vector[0]
	<<" "<property.plane_vector[1]
	<<" "<<property.plane_vector[2]<<endl;
	outfile<<"#ifcontour"<<endl;
	outfile<<property.isContour<<endl;
	outfile<<"#contours:"<<endl;
	for(int i=0;i<property.contourname.size();i++)//need to implement contour name
	{
//need to add previous contour values
        char *cstr = new char[property.contourname[i].length()+1];
        char *pch = strtok(cstr, " []:,()");
        while(pch != NULL)
        {
			if(pch[0] == "L")
			{
				outfile<<"L ";
	            pch = strtok(NULL, " []:,()");
				outfile<<atoi(pch)<<" ";
				pch = strtok(NULL, " []:,()");
				if(pch[0] == "R")
					outfile<<"R ";
				else
					outfile<<"A ";
				pch = strtok(NULL, " []:,()");
				outfile<<atof(pch)<<endl;
				pch = strtok(NULL, " []:,()");
			}    
			else
			{
				outfile<<"G ";
				pch = strtok(NULL, " []:,()");
				if(pch[0] == "R")
					outfile<<"R ";
				else
					outfile<<"A ";
				pch = strtok(NULL, " []:,()");
				outfile<<atof(pch)<<endl;
				pch = strtok(NULL, " []:,()");
			} 	
        }
        delete [] cstr;		
	}
	outfile<<"#clusters:"<<endl;
	outfile<<"#step1:"<<endl;
	outfile<<"#weights:"<<endl;
	for(int i=0;i<7;i++)
		outfile<<property.step1_kmeansproperty.weights[i]<<" ";
	outfile<<endl;
	outfile<<"#bywhole:"<<endl;
	outfile<<property.step1_kmeansproperty.bywhole<<endl;
	outfile<<"#layer:"<<endl;
	outfile<<property.step1_kmeansproperty.layer1<<" "<<property.step1_kmeansproperty.layer2<<endl;
	outfile<<"#magrange:"<<endl;
	outfile<<property.magrange[0][0]<<" "<<property.magrange[0][1]<<endl;
	outfile<<"#numofclusters:"<<endl;
	outfile<<property.step1_kmeansproperty.numCluster<<endl;
	outfile<<"#step2:"<<endl;
	outfile<<"#weights:"<<endl;
	for(int i=0;i<7;i++)
		outfile<<property.step2_kmeansproperty.weights[i]<<" ";
	outfile<<endl;
	outfile<<"#bywhole:"<<endl;
	outfile<<property.step2_kmeansproperty.bywhole<<endl;
	outfile<<"#layer:"<<endl;
	outfile<<property.step2_kmeansproperty.layer1<<" "<<property.step2_kmeansproperty.layer2<<endl;
	outfile<<"#magrange:"<<endl;
	outfile<<property.magrange[0][0]<<" "<<property.magrange[0][1]<<endl;
	outfile<<"#numofclusters:"<<endl;
	outfile<<property.step2_kmeansproperty.numCluster<<endl;
	outfile<<"#symmetry"<<endl;
	outfile<<"#planeCenter"<<endl;
	outfile<<property.symmetryproperty.pos[0]
	<<" "<<property.symmetryproperty.pos[1]
	<<" "<<property.symmetryproperty.pos[2]<<endl;
	outfile<<"#planeDirection(planeZ)"<<endl;
	outfile<<property.symmetryproperty.dir[0]
	<<" "<<property.symmetryproperty.dir[1]
	<<" "<<property.symmetryproperty.dir[2]<<endl;
	outfile<<"#planeX"<<endl;
	outfile<<property.symmetryproperty.planeX[0]
	<<" "<<property.symmetryproperty.planeX[1]
	<<" "<<property.symmetryproperty.planeX[2]<<endl;
	outfile<<"#planeY"<<endl;
	outfile<<property.symmetryproperty.planeY[0]
	<<" "<<property.symmetryproperty.planeY[1]
	<<" "<<property.symmetryproperty.planeY[2]<<endl;
	outfile<<"#angleUncertainty"<<endl;
	outfile<<property.symmetryproperty.angleUncertainty<<endl;
	outifle<<"#magUncertainty"<<endl;
	outfile<<property.symmetryproperty.magUncertainty<<endl;
	outfile<<"#ROI"<<endl;
	if(property.layer_repeat)
	{
		outfile<<3<<endl;
		outfile<<property.zmin<<" "<<property.zmax<<" "<<property.nonz<<endl;//add these into config property.
	}
	else
	{
		outfile<<2<<endl;
		outfile<<property.zmin<<" "<<property.zmax<<endl;
	}
	outfile.close();
*/
}


Paser::~Paser()
{
  cleanup();
  cleanfiles();

  precontourname.clear();
  line.clear();
  isData.clear();
  speciallines.clear();
  prespeciallines.clear();
  configtext.clear();
  textindex.clear();
}

void Paser::cleanup()
{	
  property.step1_kmeansproperty.clusterLayer.free();
  property.step1_kmeansproperty.clusterWeight.free();
  property.step2_kmeansproperty.clusterLayer.free();
  property.step2_kmeansproperty.clusterWeight.free(); 

  for(int i=0;i<seed_num;i++)
  {
	  property.contourproperty.contourValues[i].free();
  }
  delete [] property.contourproperty.contourValues;
  property.contourproperty.type.free();
  property.contourproperty.isUpdate.free();

  property.magrange[0].free();
  property.magrange[1].free();
  property.contourname.clear();	
  
  update_cluster = false;
  update_contour = false;
  update_data = false;
}

void Paser::cleanfiles()
{
	delete [] property.rawDir;
	delete [] property.rawFile;
	delete [] property.storeDir;
	delete [] property.planename;
	
    delete [] property.step1_kmeansproperty.file1;
    delete [] property.step1_kmeansproperty.file2;
    delete [] property.step2_kmeansproperty.file1;
    delete [] property.step2_kmeansproperty.file2;

    delete [] property.contourproperty.outputfile;
    delete [] property.contourproperty.vtkdir; 
    delete [] property.contourproperty.wholefile;
    delete [] property.contourproperty.planename;

    delete [] property.symmetryproperty.inputfile;
    delete [] property.symmetryproperty.datafile;
    delete [] property.symmetryproperty.outputdir;
    for(int i=0;i<SYMMETRY_TYPE;i++)
    {
      delete [] property.symmetryproperty.outputfile[i];
    }
   
    delete [] property.contourtreeproperty.dir;
    delete [] property.contourtreeproperty.planename;	
}

