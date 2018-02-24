/*  svRawQDOTData
 */
#include "svRawQDOTData.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

namespace __svl_lib {

svRawQDOTData::svRawQDOTData():svQDOTData()
{
  pixel=NULL;

  stype = NULL;
  glyphFormat[0] = NULL;
  glyphFormat[1] = NULL;
  glyphFormat[2] = NULL;

  inglyph =NULL;
  magdiff = NULL;
  dirdiff = NULL;
  inlayer = NULL;
  inindex = NULL;
  outlayer = NULL;
  outindex = NULL;

  ccluster = NULL;
  nlayer = NULL;
  nindex = NULL;

  for(int i=0;i<SYMMETRY_TYPE;i++){
      symmetrylist[i] = new svList(); 
  }
}
void svRawQDOTData::Reset(){
   svQDOTData::Reset();
   this->clean();
   dataSize = 0;
}
void svRawQDOTData::New()//svVectorField* f, int numPlane)
{
     svQDOTData::New();
     stype = new svIntArray[seed_num];
     pixel = new svVector3Array[seed_num];
     nlayer = new svVectorIntArray[seed_num];
     nindex = new svVectorIntArray[seed_num];
     ccluster = new svIntArray[seed_num];
     glyphFormat[0] = new svIntArray[seed_num];
     glyphFormat[1] = new svIntArray[seed_num];
     glyphFormat[2] = new svIntArray[seed_num];
}

void svRawQDOTData::Set2DData(char *infName){
   plb[0]=9e+9;plb[1]=9e+9;plb[2]=9e+9;
   prb[0]=-9e+9;prb[1]=-9e+9;prb[2]=-9e+9;

   ifstream infile(infName);
   if(!infile.is_open()){
      cout<<"Data misssing... Please reload the program... "<<endl;
   }
   int num;
   infile>>num;
   for(int i=0;i<num;i++){
     int n;
     infile>>n;
     int layer = i;
     for(int j=0;j<n;j++){
       svVector3 p,v; svScalar d,c,e;
       int a,r;
       infile>>p[0]>>p[1]>>p[2]>>v[0]>>v[1]>>v[2]>>d
             >>a>>r;
       if(p[0]>prb[0])prb[0]=p[0];
       if(p[1]>prb[1])prb[1]=p[1];
       if(p[2]>prb[2])prb[2]=p[2];
       if(p[0]<plb[0])plb[0]=p[0];
       if(p[1]<plb[1])plb[1]=p[1];
       if(p[2]<plb[2])plb[2]=p[2];
       v.normalize();
       pixel[layer].add(p);
     }
   }
   infile.close();
}

void svRawQDOTData::SetData(char *infName){
   dataSize = 0;
   ifstream infile(infName);
//   cerr<<infName<<endl;
   if(!infile.is_open()){
      cout<<"Data misssing... Please reload the program... "<<endl;
   }
   int num;
   infile>>num;
   for(int i=0;i<num;i++){
     int n;
     infile>>n;
     int layer = i;
     for(int j=0;j<n;j++){
       svVector3 p,v; svScalar d;
       int a,r;
       infile>>p[0]>>p[1]>>p[2]>>v[0]>>v[1]>>v[2]>>d
             >>a>>r;
       v.normalize();

       glyph[layer].add(p);
       dir[layer].add(v);
       mag[layer].add(d);
       svScalar tmpexp = getNumOfIntegerDigits(d);
       svScalar tmpcoe = d/pow(10.,tmpexp);
       exp[layer].add(tmpexp);
       coe[layer].add(tmpcoe);
       atom[layer].add(a);
       region[layer].add(r);

       glyphFormat[0][layer].add(0);
       glyphFormat[1][layer].add(0);
       glyphFormat[2][layer].add(0);

       stype[layer].add(-1);
       clusterLabel[layer].add(-1);
       roiLabel[layer].add(true);
       ccluster[layer].add(-1);

       if(d>0) visibleLabel[layer].add(true);
       else visibleLabel[layer].add(false);

       dataSize ++;
     }
   }
   infile.close();
}

void svRawQDOTData::SetFormat(char *format_fname){
 ifstream infile(format_fname);
 double tmp;
 double xd, yd, zd;
 infile>>tmp;
 infile>>tmp>>tmp;
 infile>>tmp;
 infile>>tmp>>tmp>>tmp;
 infile>>tmp>>tmp>>tmp;
 infile>>tmp;
 infile>>xd;
 infile>>yd;
 infile>>zd;
 infile>>tmp;
 infile>>tmp>>tmp>>tmp;
 infile>>tmp;
 infile>>tmp>>tmp>>tmp;
 infile>>tmp>>tmp>>tmp;
 infile>>tmp;

 svVector3Array *l = new svVector3Array[seed_num];
 for(int i=0;i<seed_num;i++){
   int n;
   infile>>n;
   for(int j=0;j<n;j++){
     svVector3 p;
     infile>>p[0]>>p[1]>>p[2];
     l[i].add(p);
   }
 }
 infile.close();

 for(int i=0;i<seed_num;i++){
   for(int j=0;j<glyph[i].size();j++){
     int zindex = (glyph[i][j][2] - l[0][0][2])/zd;
     int yindex = (glyph[i][j][1] - l[zindex][0][1])/yd;
     int xindex = (glyph[i][j][0] - l[zindex][yindex][0])/xd;
     glyphFormat[0][i][j] = xindex;
     glyphFormat[1][i][j] = yindex;
     glyphFormat[2][i][j] = zindex;
   }
 }

 for(int i=0;i<seed_num;i++){
  l[i].free();
 }
 delete [] l;
 l = NULL;
}

void svRawQDOTData::ResetData(int layer)
{
  svQDOTData::ResetData(layer);
  glyphFormat[0][layer].free();
  glyphFormat[1][layer].free();
  glyphFormat[2][layer].free();
  stype[layer].free();
}

void svRawQDOTData::GenerateClustersBySymmetry(KmeansProperty  property, 
                      SymmetryProperty  sproperty,
                      vector<int> symmetrytype)// const
{
      int max = -1;
      for(int i=0;i<property.clusterLayer.size();i++)
      {
         if(property.clusterLayer[i] > max)
            max = property.clusterLayer[i];
      }

    int count = 0;
    for(int i=0;i<seed_num;i++)
    {
         for(int j=0;j<glyph[i].size();j++)
         {
            if(roiLabel[i][j] == true)
            {
              bool flag;
              if(dot(glyph[i][j]-sproperty.pos, 
                          dir[i][j])>=0) flag = true;
              else flag = false;
              for(int t=0;t<symmetrytype.size();t++)
              {
                   if(symmetrytype[t]<0) continue;
                   int size = symmetrylist[symmetrytype[t]]->getSize(count);
                   if(size <= 1) continue;
                   else
                   {
                      int data1[size];
                      int data2[size];

                      symmetrylist[symmetrytype[t]]->getData(count, 
                                        data1, 
                                        data2);
                     roiLabel[data1[1]][data2[1]]=1-flag;
                     roiLabel[data1[0]][data2[0]]=flag;
                   }
              }
             }
            count++;
         }
    }
      for(int i=0;i<=max;i++)
      {
         SaveToFile(property.file1, property.clusterLayer,
                            property.dimension, i);
         GenerateCluster(property.file1, property.file2,
                                 property.isNormalize,
                                                 property.dimension,
                                                 property.clusterWeight,
                                                 property.numCluster);
         SetClusterLabelBySymmetry(property.file1, property.clusterLayer,i,symmetrytype);
      }
      ProcessClusterLabel();
}
void svRawQDOTData::SetNeighbors(char *file){
//===============read neighbors===============
 ifstream infile(file);
 int n;
 for(int i=0;i<seed_num;i++){
  for(int j=0;j<glyph[i].size();j++){
    infile>>n;
    vector<int> tempi; tempi.resize(n);
    vector<int> templ; templ.resize(n);
    for(int t=0;t<n;t++){
      infile>>templ[t]>>tempi[t];
    }
    nlayer[i].add(templ);
    nindex[i].add(tempi);
    templ.clear();
    tempi.clear();
  }  
 }  
 infile.close();

 //=============process connnected clusters==========
 svClusterConnect *connect = new svClusterConnect();
 connect->ComputeConnect(glyph, clusterLabel, ccluster,
                         maxClusterLabel, seed_num,
                         nlayer, nindex);
 delete connect;
 connect = NULL;  
}
void svRawQDOTData::SetNeighborLabel(vector<int> symmetrytype,SymmetryProperty &property, svChar *outputdir)
{
    char *symmetrystr = new char[200];
    for(int j=0;j<200;j++) symmetrystr[j] = '\0';
    sprintf(symmetrystr, "%s%0.2f%0.2f%0.2f%0.2f%0.2f%0.2f", symmetrystr,
                  property.pos[0], property.pos[1], property.pos[2],
                  property.dir[0], property.dir[1], property.dir[2]);
    sort(symmetrytype.begin(), symmetrytype.end());
    for(int i=0;i<symmetrytype.size();i++)
        sprintf(symmetrystr, "%s%d", symmetrystr, symmetrytype[i]);

    char *str = new char[400];
    sprintf(str,"%s/neighbor%s",outputdir, symmetrystr);
    cerr<<str<<endl;

    ifstream test(str);
    if(test.is_open())
    {
         test.close();
    }
    else
    {
      if(symmetrytype.size()>0)
      {
         vector<bool> label;
         label.resize(dataSize);

         int count = 0;
         for(int i=0;i<seed_num;i++)
         {
             for(int j=0;j<glyph[i].size();j++)
             {
                  bool flag = false;
                  for(int t=0;t<symmetrytype.size();t++)
                  {
                   int size = symmetrylist[symmetrytype[t]]->getSize(count);
                    if((t==0&&size>1)||(flag&&size>1))
                        flag = true;
                    else
                        flag = false;
                   }
                   if(flag)
                   {
                        label[count] = true;
                   }
                   else 
                        label[count] = false;
                count++;
             }
          }
 
        svConnect *ccl = new svConnect();
        char *inputfile = new char[400];
        sprintf(inputfile, "%s/neighbors.txt", outputdir);
        ifstream infile(inputfile);
        ofstream outfile(str);
        int n;
        infile>>n;
        outfile<<n<<endl;
        for(int i=0;i<n;i++)
        {
            int m;
            infile>>m;
            vector<int> index;
            for(int j=0;j<m;j++)
            {
                int ii;
                infile>>ii;
                if(label[ii]) index.push_back(ii);
            }

            if(!label[i])
            {
               outfile<<-1<<endl;
            }
            else
            {
                outfile<<index.size()<<" ";
                for(int j=0;j<index.size();j++)
                    outfile<<index[j]<<" ";
                outfile<<endl;
            }
        }
        infile.close();
        outfile.close();
//---------------------------------------------
        char *outputfile = new char[400];
        sprintf(outputfile,"%s/point%s", outputdir, symmetrystr);
        cerr<<outputfile<<endl;
        vector<int> unique_label = ccl->TwoPass(str, outputfile);
//-------------------------------------------------
        int label_size = unique_label.size();
        if(unique_label[0]<0) label_size  = label_size - 1;

        svVector3Array *labels = new svVector3Array[label_size];

        sprintf(str,"%s/meshsize%s",outputdir, symmetrystr);
        ofstream unique(str);
        unique<<unique_label.size()<<endl;
        for(int i=0;i<unique_label.size();i++)
        {
           unique<<unique_label[i]<<endl;
        }
        unique.close();        

        int shift;
        if(unique_label[0]<0) shift = 1;
        else shift = 0;

        infile.open(outputfile);
        infile>>n;
        for(int i=0;i<seed_num;i++)
        {
          for(int j=0;j<glyph[i].size();j++)
          {
             int ii;
             infile>>ii;
             if(ii>=0)
               labels[ii-shift].add(glyph[i][j]);
          }
        }
        infile.close();

        for(int i=0;i<label_size;i++)
        {
           ofstream outlabel;
           sprintf(str, "%s/region%s%d",outputdir, symmetrystr,
                              unique_label[i+shift]);
           outlabel.open(str);
           outlabel<<labels[i].size()<<endl;
           for(int j=0;j<labels[i].size();j++)
                 outlabel<<labels[i][j][0]<<" " 
                         <<labels[i][j][1]<<" "
                         <<labels[i][j][2]<<endl;
           outlabel.close();
           labels[i].free();
        }
        
        delete [] labels;
        delete [] outputfile;
        delete [] inputfile;
        delete ccl;
      }
    }
    delete [] str;
    delete [] symmetrystr;
}

void svRawQDOTData::GenerateNeighbors(NeighborProperty &property)
{
    svNeighbor *neighbor = new svNeighbor();

    neighbor->ComputeNeighbors(property);

    delete neighbor;
}


void svRawQDOTData::SetInside(char *directory, char *name){
 inglyph = new svVector3Array[unique_region.size()-1];
 magdiff = new svScalarArray[unique_region.size()-1];
 dirdiff = new svScalarArray[unique_region.size()-1];
 inlayer = new svIntArray[unique_region.size()-1];
 outlayer = new svIntArray[unique_region.size()-1];
 inindex = new svIntArray[unique_region.size()-1];
 outindex = new svIntArray[unique_region.size()-1];
//=========================================================
 for(int i=1;i<unique_region.size();i++){
  char *outstr = new char[300];
  sprintf(outstr, "%s/%s%d", directory, name, unique_region[i]);
  ifstream infile(outstr);
  int n;
  infile>>n;
  vector< vector<double> > tmpp;
  for(int j=0;j<n;j++){
    vector<double> p(3);
    infile>>p[0]>>p[1]>>p[2];
    bool flag = true;
    for(int t=0;t<tmpp.size();t++){
      if(fabs(p[0]-tmpp[t][0])<1e-8
      && fabs(p[1]-tmpp[t][1])<1e-8
      && fabs(p[2]-tmpp[t][2])<1e-8){
          flag = false;
      }
    }
    if(flag) tmpp.push_back(p);
  }  
  for(int j=0;j<tmpp.size();j++){
    svVector3 p;
    p[0] = tmpp[j][0];p[1] = tmpp[j][1];p[2] = tmpp[j][2];
    //infile>>p[0]>>p[1]>>p[2];
    inglyph[i-1].add(p);
    inlayer[i-1].add(-1);
    inindex[i-1].add(-1);
    outlayer[i-1].add(-1);
    outindex[i-1].add(-1);
  }
  for(int ii=0;ii<tmpp.size();ii++){
    tmpp[i].clear();
  }tmpp.clear();
  infile.close();
  delete [] outstr;      
//=========================================================
  svVector3Array *tempin = new svVector3Array[seed_num];
  svVector3Array *tempout = new svVector3Array[seed_num];
  svIntArray *templayer1 = new svIntArray[seed_num];
  svIntArray *templayer2 = new svIntArray[seed_num];
  svIntArray *tempindex1 = new svIntArray[seed_num];
  svIntArray *tempindex2 = new svIntArray[seed_num];
  for(int ii=0;ii<seed_num;ii++){
   for(int jj=0;jj<glyph[ii].size();jj++){
    if(region[ii][jj] == unique_region[i]){
     tempin[ii].add(glyph[ii][jj]);   
     templayer1[ii].add(ii);  
     tempindex1[ii].add(jj);
     //cerr<<glyph[ii][jj][0]<<" "<<glyph[ii][jj][1]<<" "<<glyph[ii][jj][2]<<endl;
    }
    else{
     templayer2[ii].add(ii);
     tempindex2[ii].add(jj);
     tempout[ii].add(glyph[ii][jj]);
    }
   }
  }
//=========================================================
  svNear *near = new svNear();
  near->ComputeNear(tempin, seed_num,
                   inglyph[i-1],i-1, 
                   inlayer[i-1], inindex[i-1]);
  near->ComputeNear(tempout, seed_num,
                   inglyph[i-1],i-1,
                   outlayer[i-1], outindex[i-1]);
  delete near;
  near = NULL;
//=======================================================
  for(int j=0;j<inglyph[i-1].size();j++){
   int layer1 = outlayer[i-1][j];
   int index1 = outindex[i-1][j];
   int layer2 = inlayer[i-1][j];
   int index2 = inindex[i-1][j];

   outlayer[i-1][j] = templayer2[layer1][index1];
   outindex[i-1][j] = tempindex2[layer1][index1];
   inlayer[i-1][j] = templayer1[layer2][index2];
   inindex[i-1][j] = tempindex1[layer2][index2];

   layer1 = outlayer[i-1][j];
   index1 = outindex[i-1][j];
   layer2 = inlayer[i-1][j];
   index2 = inindex[i-1][j];

//   cerr<<inglyph[i-1][j][0]<<" "<<inglyph[i-1][j][1]<<" "<<inglyph[i-1][j][2]<<endl;
//   cerr<<glyph[layer2][index2][0]<<" "<<glyph[layer2][index2][1]<<" "<<glyph[layer2][index2][2]<<endl;
   magdiff[i-1].add(fabs(exp[layer1][index1] - exp[layer2][index2]));
   dirdiff[i-1].add(acos(dot(dir[layer1][index1], dir[layer2][index2])));
  }
//=======================================================
  for(int ii=0;ii<seed_num;ii++){
   tempin[ii].free();
   tempout[ii].free();
   templayer1[ii].free();
   tempindex1[ii].free();
   templayer2[ii].free();
   tempindex2[ii].free();
  } 
  delete [] tempin;
  delete [] tempout;
  delete [] tempindex1;
  delete [] templayer1;
  delete [] tempindex2;
  delete [] templayer2;
 }
}

void svRawQDOTData::SaveSymmetry(int symmetrytype,
                           char *str1, char *str2,
                           SymmetryProperty property)
{
    ofstream outfile1(str1);
    ofstream outfile2(str2);
    svVector3Array p1, p2;
    for(int i=0;i<seed_num;i++)
    {
         for(int j=0;j<glyph[i].size();j++)
         {
              if(stype[i][j] == symmetrytype&&visibleLabel[i][j])
              {              
                 double dd = dot(glyph[i][j]-property.pos, property.dir);
                 if(dd>0)
                 {
                   p1.add(glyph[i][j]);
                 }
                 else if(dd<0)
                 {
                   p2.add(glyph[i][j]);
                 }
              }
         }
       }
    outfile1<<p1.size()<<endl;
    outfile2<<p2.size()<<endl;
    for(int i=0;i<p1.size();i++)
        outfile1<<p1[i][0]<<" "<<p1[i][1]<<" "<<p1[i][2]<<endl;
    for(int i=0;i<p2.size();i++)
        outfile2<<p2[i][0]<<" "<<p2[i][1]<<" "<<p2[i][2]<<endl;
    outfile1.close();
    outfile2.close();
    p1.free();
    p2.free();

//    }
}
void svRawQDOTData::SetSampling(svInt frequency)
{
    for(int i=0;i<seed_num;i++)
      for(int j=0;j<glyph[i].size();j++)
         visibleLabel[i][j] = false;

    for(int i=0;i<seed_num;i++)
    {
         for(int j=0;j<glyph[i].size();j++)
         {
                   int x = glyphFormat[0][i][j];
                   int y = glyphFormat[1][i][j];
                   int z = glyphFormat[2][i][j];
                   if(mag[i][j]>0&&x%frequency ==0 && y%frequency ==0 && z%frequency==0)
                   {
                       visibleLabel[i][j] = true;
                   }
         }
    }
}


void svRawQDOTData::GenerateSymmetry(SymmetryProperty property)
{
//   for(int i=0;i<seed_num;i++)
//     stype[i].free();
       
   svSymmetry *symmetry = new svSymmetry();

        sprintf(property.outputdir, "%s", property.datafile);

//         sprintf(property.inputfile, "%s/layerall.txt",property.datafile);
         symmetry->ComputeSymmetry(property);
  
        dataSize = 0;
        for(int i=0;i<seed_num;i++)
            for(int j=0;j<glyph[i].size();j++)
               dataSize++;
        symmetrytopcount = 0;
        int sum = 0;
        for(int i=0;i<SYMMETRY_TYPE;i++)
        {
          symmetrycount[i] = 0;
          symmetrylist[i]->destroy();
          symmetrylist[i]->new_list(dataSize);

            ifstream infile(property.outputfile[i]);
            int n;
            infile>>n;
             int count = 0;
             for(int ii=0;ii<seed_num;ii++)
             {//cerr<<ii<<endl;
                 for(int j=0;j<glyph[ii].size();j++)
                {
                 int m;
                 infile>>m;//cerr<<m<<endl;
                 vector<int> value; value.resize(m*2);
                 for(int t=0;t<m*2;t++)
                 {
                    infile>>value[t];
                    
                 }     
                 if(m > 1)
                 {
                     stype[ii][j] = i;   
                     symmetrycount[i]++;    
                     sum++;           
                 } 
                 symmetrylist[i]->add_next(count, value);
                 count++;
                 value.clear();
                 }   
             }//cerr<<count<<endl;
             infile.close();
            // delete [] symmetrystr;
            // delete [] str;
           if(symmetrycount[i] > symmetrytopcount)
                 symmetrytopcount = symmetrycount[i];
       }

   symmetrycount[SYMMETRY_TYPE] = dataSize - sum;
   if(symmetrytopcount <  symmetrycount[SYMMETRY_TYPE])
        symmetrytopcount =  symmetrycount[SYMMETRY_TYPE];  
   delete symmetry;

//    for(int i=0;i<SYMMETRY_TYPE+1;i++)
//           cerr<<symmetrycount[i]<<" ";
//    cerr<<endl;
}


void svRawQDOTData::SetSymmetryVisible(vector<int> symmetrytype)
{
//   if(symmetrytype.size()>0)
//   {
    int count = 0;
    int cc = 0;
    for(int i=0;i<seed_num;i++)
    {//cerr<<i<<" "<<glyph[i].size()<<" "<<visibleLabel[i].size()<<" "<<stype[i].size()<<endl;
         for(int j=0;j<glyph[i].size();j++)
         {
            if(visibleLabel[i][j] == true)
            {
              bool flag = false;
              for(int t=0;t<symmetrytype.size();t++)
              {//if(i==108)cerr<<t<<endl;
                if(symmetrytype[t] >= 0)
                {
                    int size = symmetrylist[symmetrytype[t]]->getSize(count);
//if(i==108)cerr<<size<<endl;
                    if(size>1)//(t==0&&size>1)||(flag&&size>1))
                    {
                          flag = true;
                          break;
                    }
                }
                else
                {
                    if(stype[i][j] == symmetrytype[t])
                    {
                           flag = true;
                           break;
                    } 
                }
              }
//if(i==108)cerr<<flag<<endl;
              if(flag) 
              {
               if(stype[i][j]>=0)
               {
                for(int t=0;t<symmetrytype.size();t++)
                {
//if(i==108)cerr<<t<<endl;
                  if(symmetrytype[t]>=0)
                  {
                   int size = symmetrylist[symmetrytype[t]]->getSize(count);
                   int data1[size], data2[size];
//if(i==108)cerr<<size<<endl;
                   symmetrylist[symmetrytype[t]]->getData(count, data1, data2);
                    for(int m=0;m<size;m++)
                    {
//if(i==108)cerr<<data1[m]<<" "<<data2[m]<<endl;
                          visibleLabel[data1[m]][data2[m]] = true;
                          cc++;
                    }
                  }         
                }
               }
            }
            else
            {
                //cerr<<i<<" "<<j<<endl;
                visibleLabel[i][j] = false;
            }
         }
          count++;
       }
//    }
/*
    ofstream outfile("test");
    outfile<<cc<<endl;
    for(int i=0;i<seed_num;i++)
    {
        for(int j=0;j<glyph[i].size();j++)
        {
         if(visibleLabel[i][j] == true)
            {
              outfile<<glyph[i][j][0]<<" "<<glyph[i][j][1]<<" "<<glyph[i][j][2]<<endl;
           }
        }
    }
    outfile.close();

*/
   }
}
void svRawQDOTData::SetRegionVisible(){

 svIntArray *temp = new svIntArray[seed_num];
 for(int i=0;i<seed_num;i++){
  for(int j=0;j<glyph[i].size();j++){
   temp[i].add(0);
  }
 }
 for(int i=0;i<unique_region.size()-1;i++){
  for(int j=0;j<inglyph[i].size();j++){
   int l1 = outlayer[i][j];
   int i1 = outindex[i][j];
   int l2 = inlayer[i][j];
   int i2 = inindex[i][j];
   if(visibleLabel[l1][i1]){
    temp[l1][i1] = true;
   }
   if(visibleLabel[l2][i2]){
    temp[l2][i2] = 1;
   }
  }
 }
 for(int i=0;i<seed_num;i++){
  for(int j=0;j<glyph[i].size();j++){
   if(visibleLabel[i][j]  && (temp[i][j])){// || region[i][j]!=unique_region[0])){
    visibleLabel[i][j] = true;
   }
   else{
    visibleLabel[i][j] = false;
   }
  }
 }

 for(int i=0;i<seed_num;i++) temp[i].free();
 delete [] temp;
 temp = NULL;
}

void svRawQDOTData::SetClusterLabelBySymmetry(char *clusterfile, svIntArray clusterLayer, int clusterIndex,  vector<int> symmetrytype)
{
       int max = maxClusterLabel;
       ifstream infile(clusterfile);
       for(int i=0;i<seed_num;i++)
       {
             if(clusterLayer[i] == clusterIndex)
             {
                int s = glyph[i].size();
                for(int j=0;j<s;j++)
                {
                    if(roiLabel[i][j])
                    {
                         int cluster;
                         infile>>cluster;
                         clusterLabel[i][j] = cluster+maxClusterLabel+1;
                         
                         if(clusterLabel[i][j] > max) max = clusterLabel[i][j];
                    }
                    ccluster[i][j] = -1;
                }
             }
       }
       infile.close();
       maxClusterLabel = max;

       int count = 0;
       for(int i=0;i<seed_num;i++)
       {
         for(int j=0;j<glyph[i].size();j++)
         {
              for(int t=0;t<symmetrytype.size();t++)
              {
                      if(symmetrytype[t]<0)  continue;
                      int size = symmetrylist[symmetrytype[t]]->getSize(count);
                      if(size <= 1) continue;

                      int data1[size];
                      int data2[size];

                      symmetrylist[symmetrytype[t]]->getData(count,
                                        data1,
                                        data2);
                     if(clusterLabel[data1[0]][data2[0]]>-1){ 
                             clusterLabel[data1[1]][data2[1]] = clusterLabel[data1[0]][data2[0]];
                     }
                     else if (clusterLabel[data1[1]][data2[1]]>-1){
                             clusterLabel[data1[0]][data2[0]] = clusterLabel[data1[1]][data2[1]];
                     }
              }
              count++;
         }
       }
}

svScalarArray *svRawQDOTData::GetSymmetryMagProb(int & topvalue)
{
   svScalar level;
   svScalar max= -9e+9;
   for(int i=0;i<seed_num;i++)
   {
     for(int j=0;j<glyph[i].size();j++)
     {
        if(exp[i][j] > max && coe[i][j]>0)
           max = exp[i][j];
     }
   } 

   level = max - 4;
   svScalarArray *prob = new svScalarArray[4];
   for(int i=0;i<SYMMETRY_TYPE+1;i++)
   {
     prob[0].add(0);
     prob[1].add(0);
     prob[2].add(0);
     prob[3].add(0);
   }

   for(int i=0;i<seed_num;i++)
   {
      for(int j=0;j<glyph[i].size();j++)
      {
         if(exp[i][j] > level&&coe[i][j]>0)
         {
            int ii;
            if(stype[i][j]>=0)ii = stype[i][j];
            else ii = SYMMETRY_TYPE;

            if(ii== SYMMETRY_TYPE && fabs(exp[i][j]-level-4)<1e-3)

            if(fabs(exp[i][j]-level-1)<1e-3)
                 prob[3][ii] = prob[3][ii]+1;
            if(fabs(exp[i][j]-level-2)<1e-3)
                 prob[2][ii] = prob[2][ii]+1;
            if(fabs(exp[i][j]-level-3)<1e-3)
                 prob[1][ii] = prob[1][ii]+1;
            if(fabs(exp[i][j]-level-4)<1e-3)
                 prob[0][ii] = prob[0][ii]+1;
         }
      }
    }

   topvalue = 0;
   for(int i=0;i<=SYMMETRY_TYPE;i++)
   {
     svScalar sum = prob[0][i] + prob[1][i] + prob[2][i] + prob[3][i];
//     cerr<<prob[0][i]<<" "<<prob[1][i]<<" "<<prob[2][i]<<" "<<prob[3][i]<<endl;
     prob[0][i] = prob[0][i]/sum;
     prob[1][i] = prob[1][i]/sum;
     prob[2][i] = prob[2][i]/sum;
     prob[3][i] = prob[3][i]/sum;
     //if(topvalue < sum)
       // topvalue = sum;
   }

   topvalue = 1;

   return prob;
}
svRawQDOTData::~svRawQDOTData()
{
  this->clean();
}

void svRawQDOTData::clean(int j)
{
    pixel[j].free();
    glyphFormat[0][j].free();
    glyphFormat[1][j].free();
    glyphFormat[2][j].free();
    stype[j].free();
    ccluster[j].free();
}

void svRawQDOTData::clean()
{
  glyphSize = 0;

  if (pixel != NULL) {
    for (int i=0; i<seed_num; i++)
        pixel[i].free();
    delete [] pixel;
    pixel=NULL;
  };
  if (glyphFormat[0]!=NULL) {
    for(int i=0; i<seed_num; i++)
      glyphFormat[0][i].free();
    delete [] glyphFormat[0];
    glyphFormat[0]=NULL;
  };
  if (glyphFormat[1]!=NULL) {
    for(int i=0; i<seed_num; i++)
      glyphFormat[1][i].free();
    delete [] glyphFormat[1];
    glyphFormat[1]=NULL;
  };
  if (glyphFormat[2]!=NULL) {
    for(int i=0; i<seed_num; i++)
      glyphFormat[2][i].free();
    delete [] glyphFormat[2];
    glyphFormat[2]=NULL;
  };
  if(stype != NULL) {
      for(int i=0;i<seed_num;i++)
         stype[i].free();
      delete [] stype;
      stype = NULL;
   }

  int regionsize = unique_region.size()-1;
  if(inglyph!=NULL){
    for(int i=0;i<regionsize;i++){
      inglyph[i].free();
    } 
    delete [] inglyph;
    inglyph = NULL;
  }
  if(inlayer!=NULL){
    for(int i=0;i<regionsize;i++){
      inlayer[i].free();
    }
    delete [] inlayer;
    inlayer = NULL;
  }
  if(inindex!=NULL){
    for(int i=0;i<regionsize;i++){
      inindex[i].free();
    }
    delete [] inindex;
    inindex = NULL;
  }
  if(outlayer!=NULL){
    for(int i=0;i<regionsize;i++){
      outlayer[i].free();
    }
    delete [] outlayer;
    outlayer = NULL;
  }
  if(outindex!=NULL){
    for(int i=0;i<regionsize;i++){
      outindex[i].free();
    }
    delete [] outindex;
    outindex = NULL;
  }
  if(magdiff!=NULL){
    for(int i=0;i<regionsize;i++){
      magdiff[i].free();
    } 
    delete [] magdiff;
    magdiff = NULL;
  }
  if(dirdiff!=NULL){
    for(int i=0;i<regionsize;i++){
      dirdiff[i].free();
    } 
    delete [] dirdiff;
    dirdiff = NULL;
  }
  if(ccluster!=NULL){
    for(int i=0;i<seed_num;i++){
      ccluster[i].free();
    }
    delete [] ccluster;
    ccluster = NULL;
  }
  if(nlayer!=NULL){
   for(int i=0;i<seed_num;i++){
      for(int j=0;j<nlayer[i].size();i++){
        nlayer[i][j].clear();
      }
      nlayer[i].free();
    }
    delete [] nlayer;
    nlayer = NULL;
  }
  if(nindex!=NULL){
   for(int i=0;i<seed_num;i++){
      for(int j=0;j<nindex[i].size();i++){
        nindex[i][j].clear();
      }
      nindex[i].free();
    }
    delete [] nindex;
    nindex = NULL;
  }
}

}
