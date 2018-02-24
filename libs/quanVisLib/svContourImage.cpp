#include "svContourImage.h"
#include "svVectorField.h"
#include <fstream>

using namespace std;

namespace __svl_lib {

svContourImage::svContourImage(svQDOTData *d):svImage(d){
  seed_num = 0;
}

svContourImage::svContourImage():svImage()
{
   contour = new svVector3ArrayPtr[1];
   contour_pixel = new svVector3ArrayPtr[1];
   contour[0] = new svVector3Array[1];
   contour_pixel[0] = new svVector3Array[1];
   contour_count.add(0);
   contour_valid = new svVector3Array[1];
   contour_valid_pixel = new svVector3Array[1];
   contour_display = new svIntArray[1];
   contour_tree.resize(1);
   contour_tree[0] = new ContourTree();
}

void svContourImage::ResetContourTree()
{
 if(seed_num>0){
   cleanupcontour();
   cleanupselectcontour();
 }
}


void svContourImage::ResetContourTree(int seed)
{
 if(seed_num>0){
   cleanupcontour();
   cleanupselectcontour();
 }
}

void svContourImage::SetBound(svInt v1, svInt v2, svInt step)
{
   contour_tree.resize(seed_num);

   for(int i=0;i<seed_num;i++)
   { 
      contour_tree[i] = new ContourTree();
      contour_tree[i]->SetVolumeBound(v1,v2,step);
   }
}

void svContourImage::GenerateContourTree(ContourTreeProperty property){
   svVectorField *field;
   svContourTree *contourtree;

   contourtree = new svContourTree();
   contourtree->ComputeContourTree(property);
   for(int i=0;i<seed_num;i++)
   {
        char *str = new char[400];
        sprintf(str,"%s/contourtree%s%d.txt",property.dir, property.planename,i);
//cerr<<str<<endl;
        contour_tree[i]->Construct(str);
        contour_tree[i]->Prune();
        delete [] str;
   }   
}

void svContourImage::GenerateContours(char *vtkdir, char *planename)
{
//    cleanupcontour();

    contour = new svVector3ArrayPtr[seed_num];
    contour_pixel = new svVector3ArrayPtr[seed_num];
    contour_display = new svIntArray[seed_num];
//    contour_tree.resize(seed_num);
    char *contourfile = new char[400];

    //#pragma omp parallel for
    for(int i=0;i<seed_num;i++)
    {//cerr<<i<<" "<<seed_num<<endl;
         int size = contour_tree[i]->GetNodeSize(); 
         contour[i] = new svVector3Array[size];
         contour_pixel[i] = new svVector3Array[size];
         contour_count.add(size);
         //#pragma omp parallel for
         for(int j=0;j<size;j++)
         {
//            char *contourfile = new char[400];
//cerr<<j<<" "<<size<<endl;
            if(contour_tree[i]->GetIsoValue(j)>0)
            {
//cerr<<j<<" "<<size<<endl;
            sprintf(contourfile, "%s/contour2d%d%6.2e.svl", //name needs to be fixed
                                  vtkdir,i,
                                  contour_tree[i]->GetIsoValue(j));
            ifstream infile(contourfile);
            if(!infile.is_open())
            {
                infile.close();
                  cerr<<" open"<<endl;
                  GenerateContour(contourfile, vtkdir, planename,
                            i , 
                            contour_tree[i]->GetIsoValue(j),
                            j);
            }
            else
            {//cerr<<endl;
               infile.close();
            }

            SetContours(contourfile, i,j);

//            contour_display[i].add(0);
            }
         //   delete [] contourfile;
 //           cerr<<"--"<<endl;
         }
    }
    delete [] contourfile;
cerr<<"done!"<<endl;
    GeneratePixel();
}

void svContourImage::GenerateContour(char *vtkdir, char *planename)
{
    int i = select_layer;
    int index = select_pixel;

    svScalar mag = myData->GetMag(i,index);
    char *contourfile = new char[400];
    sprintf(contourfile, "%s/contour2d%d%6.2e.svl",
                                  vtkdir,i,
                                  mag);
    ifstream infile(contourfile);
    cerr<<contourfile<<endl;
    if(!infile.is_open())
          GenerateContour(contourfile, vtkdir, planename,
                            i,
                            mag);
     else
              infile.close();

         contour_valid[i].free();
         contour_valid_pixel[i].free();

         SetContours(contourfile, i);

       svScalar ratio;
       if(select_layer == i)
       {
           ratio = (rbbox[1]-lbbox[1])/(sidey * 3);
       }
       else
           ratio = (rbbox[1]-lbbox[1])/sidey;

       svScalar x = image_pos[i][0];
       svScalar y = image_pos[i][1];

         for(int j=0;j<contour_valid[i].size();j++)
         {
               contour_valid_pixel[i][j][0] = contour_valid[i][j][0]
                                                  -lbbox[0];
               contour_valid_pixel[i][j][1] = contour_valid[i][j][1]
                                                  -lbbox[1];
               contour_valid_pixel[i][j][2] = contour_valid[i][j][2]
                                                  -lbbox[2];

               contour_valid_pixel[i][j][0] = contour_valid[i][j][0]/ratio;
               contour_valid_pixel[i][j][1] = contour_valid[i][j][1]/ratio;

               contour_valid_pixel[i][j][0] = contour_valid[i][j][0] + x;
               contour_valid_pixel[i][j][1] = contour_valid[i][j][1] + y;
         }
         delete [] contourfile;
}

void svContourImage::GenerateSelectedContours(ContourProperty property)
{
//    cleanupselectcontour();

    contour_valid = new svVector3Array[seed_num];
    contour_valid_pixel = new svVector3Array[seed_num];

    #pragma omp parallel for
    for(int i=0;i<seed_num;i++)
    {
        char *contourfile = new char[400];
         sprintf(contourfile, "%s/contour2d%d%6.2e.svl",
                                  property.vtkdir,i,
                                  property.contourValues[i][0]);
         ifstream infile(contourfile);
         if(!infile.is_open())
              GenerateContour(contourfile, property.vtkdir, property.planename,
                            i,
                            property.contourValues[i][0]);
         else
              infile.close();

         SetContours(contourfile, i);

       svScalar ratio;
       if(select_layer == i)
       {
           ratio = (rbbox[1]-lbbox[1])/(sidey * 3);
       }
       else
           ratio = (rbbox[1]-lbbox[1])/sidey;

       svScalar x = image_pos[i][0];
       svScalar y = image_pos[i][1];

         for(int j=0;j<contour_valid[i].size();j++)
         {
               contour_valid_pixel[i][j][0] = contour_valid[i][j][0]
                                                  -lbbox[0];
               contour_valid_pixel[i][j][1] = contour_valid[i][j][1]
                                                  -lbbox[1];
               contour_valid_pixel[i][j][2] = contour_valid[i][j][2]
                                                  -lbbox[2];

               contour_valid_pixel[i][j][0] = contour_valid[i][j][0]/ratio;
               contour_valid_pixel[i][j][1] = contour_valid[i][j][1]/ratio;

               contour_valid_pixel[i][j][0] = contour_valid[i][j][0] + x;
               contour_valid_pixel[i][j][1] = contour_valid[i][j][1] + y;
         }
         delete [] contourfile;
    }
}

void svContourImage::GenerateVisibility(svInt bound, svScalar threshold)
{
    for(int i=0;i<seed_num;i++)
    {
//cerr<<contour_count[i]<<" "<<contour_display[i].size()<<endl;
        for(int j=0;j<contour_count[i];j++)
               contour_display[i][j] = 0;
//cerr<<i<<endl;
        svIntArray index;
        contour_tree[i]->GetDisplayIndex(bound, threshold, index);

        for(int j=0;j<index.size();j++)
        {
               contour_display[i][index[j]] = 1;
        }
        index.free();
//cerr<<i<<endl;
    }
}

void svContourImage::GenerateContour(char *contourfile, char *vtkdir, char *planename, int layer, float contourValue, int index)
{
    char *vtkName = new char[200];
    sprintf(vtkName, "%s/%s%d.vtk", vtkdir, planename, layer);

    svVectorField *field = new svVectorField;
    svContour *contourField = new svContour();
    contourField->ComputeContourLines(vtkName, contourfile, contourValue);
    delete contourField; 
    delete field;

    delete [] vtkName;
}

void svContourImage::GenerateContour(char *contourfile, char *vtkdir, char *planename, int layer, float contourValue)
{
    char *vtkName = new char[200];
    sprintf(vtkName, "%s/%s%d.vtk", vtkdir, planename, layer);

    svVectorField *field = new svVectorField;
    svContour *contourField = new svContour();
    contourField->ComputeContourLines(vtkName, contourfile, contourValue);
    delete contourField;
    delete field;

    delete [] vtkName;
}

void svContourImage::SetContours(char *svlfile, int layer, int index)
{
    ifstream infile(svlfile);
    int n;
    infile>>n;
    for(int i=0;i<n;i++)
    {
         int m;
         infile>>m;
         for(int j=0;j<m;j++)
         {
            svVector3 pos;
            infile>>pos[0]>>pos[1]>>pos[2];
            contour[layer][index].add(pos);
            contour_pixel[layer][index].add(pos);
         }
    }
    infile.close();
}

void svContourImage::SetContours(char *svlfile, int layer)
{
    ifstream infile(svlfile);
    int n;
    infile>>n;
    for(int i=0;i<n;i++)
    {
         int m;
         infile>>m;
         for(int j=0;j<m;j++)
         {
            svVector3 pos;
            infile>>pos[0]>>pos[1]>>pos[2];
            contour_valid[layer].add(pos);
            contour_valid_pixel[layer].add(pos);
         }
    }
    infile.close();
}


void svContourImage::GeneratePixel()
{
   svScalar ratio; 
   svScalar x,y;
   for(int i=0;i<seed_num;i++)
   {
//cerr<<i<<endl;
       if(select_layer == i)
       {
           ratio = (rbbox[1]-lbbox[1])/(sidey * 3);
       }
       else
           ratio = (rbbox[1]-lbbox[1])/sidey;

       svScalar x = image_pos[i][0];
       svScalar y = image_pos[i][1];

       for(int j=0;j<contour_count[i];j++)
       {
           for(int t=0;t<contour[i][j].size();t++)
           {
               contour_pixel[i][j][t][0] = contour[i][j][t][0]-lbbox[0];
               contour_pixel[i][j][t][1] = contour[i][j][t][1]-lbbox[1];
               contour_pixel[i][j][t][2] = contour[i][j][t][2]-lbbox[2];
 
               contour_pixel[i][j][t][0] = contour_pixel[i][j][t][0]/ratio;
               contour_pixel[i][j][t][1] = contour_pixel[i][j][t][1]/ratio;
               contour_pixel[i][j][t][2] = contour_pixel[i][j][t][2]/ratio;

               contour_pixel[i][j][t][0] = contour_pixel[i][j][t][0] + x;
               contour_pixel[i][j][t][1] = contour_pixel[i][j][t][1] + y;
           }
           contour_display[i].add(0);
       }
   }
}

void svContourImage::SetContourPosition()
{
   svScalar ratio;
   svScalar x,y;

   for(int i=0;i<seed_num;i++)
   {
       if(select_layer == i)
       {
           ratio = (rbbox[1]-lbbox[1])/(sidey * largeratio);
       }
       else
           ratio = (rbbox[1]-lbbox[1])/sidey;

       svScalar x = image_pos[i][0];
       svScalar y = image_pos[i][1];

       for(int j=0;j<contour_count[i];j++)
       {
          if(contour_display[i][j])
          {
           for(int t=0;t<contour[i][j].size();t++)
           {
               contour_pixel[i][j][t][0] = contour[i][j][t][0]-lbbox[0];
               contour_pixel[i][j][t][1] = contour[i][j][t][1]-lbbox[1];
               contour_pixel[i][j][t][2] = contour[i][j][t][2]-lbbox[2];

               contour_pixel[i][j][t][0] = contour_pixel[i][j][t][0]/ratio;
               contour_pixel[i][j][t][1] = contour_pixel[i][j][t][1]/ratio;
               contour_pixel[i][j][t][2] = contour_pixel[i][j][t][2]/ratio;

               contour_pixel[i][j][t][0] = contour_pixel[i][j][t][0] + x;
               contour_pixel[i][j][t][1] = contour_pixel[i][j][t][1] + y;
           }
         }
       }

         for(int j=0;j<contour_valid[i].size();j++)
         {
               contour_valid_pixel[i][j][0] = contour_valid[i][j][0]
                                                  -lbbox[0];
               contour_valid_pixel[i][j][1] = contour_valid[i][j][1]
                                                  -lbbox[1];
               contour_valid_pixel[i][j][2] = contour_valid[i][j][2]
                                                  -lbbox[2];

               contour_valid_pixel[i][j][0] = contour_valid_pixel[i][j][0]/ratio;
               contour_valid_pixel[i][j][1] = contour_valid_pixel[i][j][1]/ratio;

               contour_valid_pixel[i][j][0] = contour_valid_pixel[i][j][0] + x;
               contour_valid_pixel[i][j][1] = contour_valid_pixel[i][j][1] + y;
         }
   }

}

void svContourImage::SaveContourtoFile(char *file)
{
   ofstream outfile(file);
   int n = (rbbox[0] - lbbox[0])/minD[0]+1;
   int m = (rbbox[1] - lbbox[1])/minD[1]+1;
 cerr<<minD[0]<<" "<<minD[1]<<" "<<n<<" "<<m<<endl; 
   svVector3Array *tmpColor = new svVector3Array[seed_num]; 
   outfile<<seed_num<<endl;
   svVector3 white;
   svVector3 black;
   svVector3 red;
   red[0]=255;red[1]=0;red[2]=0;
   black[0]=0;black[1]=0;black[2]=0;
   white[0]=255;white[1]=255;white[2]=255;
/*   for(int i=0;i<seed_num;i++)
   {
      outfile<<m<<" "<<n<<endl;
      for(int j=0;j<m;j++)
      {
           for(int t=0;t<n;t++)
           {
               svScalar y = (svScalar)j * (svScalar)n *minD[1];
               svScalar x = (svScalar)t * minD[0];
               bool isContour = false;
               for(int jj=0;jj<contour[i].size();jj++)
               {
                   svVector3 tmpp = contour[i][jj]-lbbox;//+image_pos[i];
                   if(tmpp[0]>x && tmpp[0]<x+minD[0]
                    && tmpp[1]>y && tmpp[1] <y+minD[1])
                   {
                      isContour = true;break;
                   }
               }
               if(isContour)
                      outfile<<0<<" "<<0<<" "<<0<<endl;
               else
                      outfile<<255<<" "<<255<<" "<<255<<endl;
           }
      } 

  }
*/
  for(int i=0;i<seed_num;i++)
  {
//cerr<<i<<endl;
     outfile<<m<<" "<<n<<endl;
     for(int j=0;j<m;j++)
     {
        for(int t=0;t<n;t++)
        {
           tmpColor[i].add(white);
        }
     }
//cerr<<"color"<<endl;
     for(int j=0;j<contour_count[i];j++)
     {
         for(int t=0;t<contour[i][j].size();t++)
         {
            svVector3 p;
            p = contour[i][j][t];// - lbbox;
            int x = (p[0] - lbbox[0])/minD[0];
            int y = (p[1] - lbbox[1])/minD[1];
//cerr<<x<<" "<<y<<" "<<p[0]<<" "<<p[1]<<" "<<tmpColor[i].size()<<endl;
             tmpColor[i][x + y * n ]=black;
         }
     }
//cerr<<"count"<<endl;
     for(int j=0;j<contour_valid[i].size();j++)
     {
            svVector3 p;
            p = contour_valid[i][j];// - lbbox;
            int x = (p[0] - lbbox[0])/minD[0];
            int y = (p[1] - lbbox[1])/minD[1];
            tmpColor[i][x + y * n ]=red;
     }
//cerr<<"valid"<<endl;
     for(int j=0;j<tmpColor[i].size();j++)
    {
        outfile<<tmpColor[i][j][0]<<" "<<tmpColor[i][j][1]<<" "<<tmpColor[i][j][2]<<endl;
    }

  }
  outfile.close();


  for(int i=0;i<seed_num;i++)
      tmpColor[i].free();
  delete [] tmpColor;

}
 
svContourImage::~svContourImage()
{
   cleanupcontour();
   cleanupselectcontour();
}

void svContourImage::cleanupcontour()
{
   for(int i=0;i<seed_num;i++)
   {
       for(int j=0;j<contour_count[i];j++)
       {
           contour[i][j].free();
           contour_pixel[i][j].free();
       }//cerr<<"tree"<<endl;
       contour_display[i].free();//cerr<<"tree"<<endl;
       delete [] contour[i];//cerr<<"tree"<<endl;
       delete [] contour_pixel[i];//cerr<<"tree"<<endl;
       delete contour_tree[i];//cerr<<"tree"<<endl;
   }
   contour_tree.clear();
   delete [] contour_display;
   delete [] contour;
   delete [] contour_pixel;

   contour_count.free();
}

void svContourImage::cleanupselectcontour()
{
   for(int i=0;i<seed_num;i++)
   {
       contour_valid[i].free();
       contour_valid_pixel[i].free();
   }
   delete [] contour_valid;
   delete [] contour_valid_pixel;
}

}
