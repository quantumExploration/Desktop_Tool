
#include "svContourTree.h"
#include <fstream>
#include <algorithm>

using namespace std;

namespace __svl_lib {

bool comparetop(ContourTreeArc a1, ContourTreeArc a2){
    if(a1.topvalue<a2.topvalue)
        return true;
    else
        return false;
}

void ContourTree::Construct(char *file){
    ifstream infile(file);
    int n;
    infile>>n;
    contour_tree.resize(n);
    vector<int> nodeid;
    for(int i=0;i<n;i++){
       infile>>contour_tree[i].topid>>contour_tree[i].bottomid
             >>contour_tree[i].topvalue>>contour_tree[i].bottomvalue;
       nodeid.push_back(contour_tree[i].topid);
       nodeid.push_back(contour_tree[i].bottomid);
//       svScalar value = (contour_tree[i].topvalue-contour_tree[i].bottomvalue)/contour_tree[i].topvalue;
//       contour_tree[i].reheight = value;
       infile>>contour_tree[i].volume;
       contour_tree[i].active = true;
       contour_tree[i].depth = -1;
    }
    infile.close();
    sort(nodeid.begin(), nodeid.end());
    std::vector<int>::iterator it;
    it = unique(nodeid.begin(), nodeid.end());
    nodeid.resize(std::distance(nodeid.begin(),it));
    int max = nodeid[nodeid.size()-1];
    contour_node.resize(max+1);

    sort(contour_tree.begin(), contour_tree.end(), comparetop);
    for(int i=0;i<n;i++){
       contour_node[contour_tree[i].topid].bottomArc.push_back(i);
       contour_node[contour_tree[i].bottomid].topArc.push_back(i);
    }
    sort(contour_tree.begin(), contour_tree.end(), comparetop);
    for(int i=0;i<n;i++){
       for(int j=0;j<n;j++){
         if(i!=j)
         {
             if(contour_tree[i].topid == contour_tree[j].bottomid)
             {
                contour_tree[i].topArc.push_back(j);
                contour_tree[j].bottomArc.push_back(i);
             }
             else if(contour_tree[i].bottomid == contour_tree[j].topid)
             {
                contour_tree[i].bottomArc.push_back(j);
                contour_tree[j].topArc.push_back(i);
             }
         }
       }
    }
/*
    int d = 0;
    for(int i=n-1;i>=0;i--)
    {
         int dd=-1;
         for(int j =0;j<contour_tree[i].bottomArc.size();j++)
         {
            int arc = contour_tree[i].bottomArc[j];
            if(contour_tree[arc].depth >-1)
            {
               dd = contour_tree[arc].depth;
               break;
            }
         }
        if(dd>-1)
        { 
         for(int j=0;j<contour_tree..bottomArc.size();j++)
         {
            int arc = contour_tree[i].bottomArc[j];
            contour_tree[arc].depth = dd;
         }
         contour_tree[i].depth = dd+1;
         
        }
    }
*/
}

void ContourTree::SetVolumeBound(svInt v1, svInt v2, svInt step){
   volumebound[0] = v1;
   volumebound[1] = v2;
   volumestep = step;
   for(int i=0;i<isoCount;i++)
   {
      isoArc[i].free();
   }
   delete [] isoArc;
   isoCount = (v2-v1)/step + 1;
   isoArc = new svIntArray[isoCount];
}

void ContourTree::PruneVolume(){
 bool smallBranchExist = true;
 int count = 0;
 while(smallBranchExist){//cut all small branches
  smallBranchExist = false;
  for(int i=0;i<contour_tree.size();i++){
    if(contour_tree[i].topArc.size()==0
    && contour_tree[i].bottomArc.size()==0) continue;

    if(contour_tree[i].topArc.size()==0){//cut top
      if(contour_tree[i].volume<50){
        smallBranchExist = true;
        int id = contour_tree[i].bottomid;
        //remove
        for(int j=0;j<contour_node[id].topArc.size();j++){
           if(contour_node[id].topArc[j] == i){
              contour_node[id].topArc.erase(contour_node[id].topArc.begin()+j);
              break;
           }
        }
        for(int j=0;j<contour_node[id].bottomArc.size();j++){
           int index=contour_node[id].bottomArc[j];
           for(int t=0;t<contour_tree[index].topArc.size();t++){
              if(contour_tree[index].topArc[t] == i){
                contour_tree[index].topArc.erase(contour_tree[index].topArc.begin()+t);
                break;
              }
           }
        }
        //dead arc
        contour_tree[i].topArc.clear();
        contour_tree[i].bottomArc.clear();
      }
    }
    else if(contour_tree[i].bottomArc.size()==0){//cut leaf
      if(contour_tree[i].volume<50){
        smallBranchExist = true;
        int id = contour_tree[i].topid;
        for(int j=0;j<contour_node[id].bottomArc.size();j++){
           if(contour_node[id].bottomArc[j] == i){
              contour_node[id].bottomArc.erase(contour_node[id].bottomArc.begin()+j);
              break;
           }
        }
        for(int j=0;j<contour_node[id].topArc.size();j++){
           int index=contour_node[id].topArc[j];
           for(int t=0;t<contour_tree[index].bottomArc.size();t++){
              if(contour_tree[index].bottomArc[t] == i){
                contour_tree[index].bottomArc.erase(contour_tree[index].bottomArc.begin()+t);
                break;
              }
           }
        }
        //dead arc
        contour_tree[i].topArc.clear();
        contour_tree[i].bottomArc.clear();
      }
    } 
  }
 }
 MergeBranches();
}

void ContourTree::MergeBranches(){
 for(int i=0;i<contour_node.size();i++){
   if(contour_node[i].bottomArc.size()==1 
   && contour_node[i].topArc.size()==1){
     int topid = contour_node[i].topArc[0];
     int bottomid = contour_node[i].bottomArc[0];
     //combine the two branches
     svScalar volume = contour_tree[topid].volume+contour_tree[bottomid].volume;
     svScalar topvalue = contour_tree[topid].topvalue;
     svScalar bottomvalue = contour_tree[bottomid].bottomvalue;
     svInt topnode = contour_tree[topid].topid;
     svInt bottomnode = contour_tree[bottomid].bottomid;
     vector<int> topArc; 
     for(int j=0;j<contour_tree[topid].topArc.size();j++){
       topArc.push_back(contour_tree[topid].topArc[j]);
       int index = contour_tree[topid].topArc[j];
       for(int t=0;t<contour_tree[index].bottomArc.size();t++){
         if(contour_tree[index].bottomArc[t] == topid){
            contour_tree[index].bottomArc[t] = bottomid;
         } 
       }
     }
     for(int j=0;j<contour_node[topnode].bottomArc.size();j++){
       if(contour_node[topnode].bottomArc[j] == topid){
         contour_node[topnode].bottomArc[j] = bottomid;
       }
     }
     vector<int> bottomArc;
     for(int j=0;j<contour_tree[bottomid].bottomArc.size();j++){
       bottomArc.push_back(contour_tree[bottomid].bottomArc[j]);
     }
     //update bottom arc
     contour_tree[bottomid].topvalue = topvalue;
     contour_tree[bottomid].bottomvalue = bottomvalue; 
     contour_tree[bottomid].topid = topnode;
     contour_tree[bottomid].bottomid = bottomnode;
     contour_tree[bottomid].topArc.clear();
     for(int j=0;j<topArc.size();j++){
        contour_tree[bottomid].topArc.push_back(topArc[j]);
     } 
     contour_tree[bottomid].bottomArc.clear();
     for(int j=0;j<bottomArc.size();j++){
        contour_tree[bottomid].bottomArc.push_back(bottomArc[j]);
     }
     topArc.clear();
     bottomArc.clear();
     //dead node
     contour_node[i].topArc.clear();
     contour_node[i].bottomArc.clear();
     //dead arc
     contour_tree[topid].topArc.clear();
     contour_tree[topid].bottomArc.clear();
   }
 }
}

void ContourTree::GenerateIsoValues(){
  isoValue.clear();
  int index;
  for(int i=0;i<contour_tree.size();i++){
    if(contour_tree[i].topArc.size() >0 
      || contour_tree[i].bottomArc.size()>0){
      index = i;
      svScalar value = contour_tree[i].bottomvalue;
      if(contour_tree[i].bottomArc.size()==0){
        if(value + contour_tree[i].topvalue>0)
             isoValue.push_back((value + contour_tree[i].topvalue)/2.);
      }
      else if(value>0)isoValue.push_back(value);
    }
  }
  if(isoValue.size()==0){
    index = 0;
    isoValue.push_back(log10(contour_tree[index].topvalue)
                + log10(contour_tree[index].bottomvalue));
    isoValue[0] = isoValue[0]/2.;
    isoValue[0] = pow(10., isoValue[0]);
  }
  else if(isoValue.size()==1){
    isoValue[0] = log10(contour_tree[index].topvalue)
                + log10(contour_tree[index].bottomvalue);
    isoValue[0] = isoValue[0]/2.;
    isoValue[0] = pow(10., isoValue[0]);
  }
  else{
    svScalar threshold = 0.5;
    sort(isoValue.begin(), isoValue.end());
    for(int i=0;i<isoValue.size()-1;i++){
       svScalar distance = log10(isoValue[i+1]) - log10(isoValue[i]);
       if(distance < threshold){
          isoValue.erase(isoValue.begin()+i+1);
          i--;
       }
    }
  }
}

void ContourTree::Prune(){
   vector<ContourTreeArc> prune_tree;

   for(int ii=0;ii<isoCount;ii++)
   {
      svIntArray label;

      for(int i=0;i<prune_tree.size();i++)
      {
        prune_tree[i].topArc.clear();
        prune_tree[i].bottomArc.clear();
      }
      prune_tree.clear();

      prune_tree.resize(contour_tree.size());

      svInt bound = volumestep*ii + volumebound[0];

 //cut all small leafs
      for(int i=0;i<contour_tree.size();i++)
      {
         label.add(1);
         prune_tree[i].topvalue = contour_tree[i].topvalue;
         prune_tree[i].bottomvalue = contour_tree[i].bottomvalue;
         prune_tree[i].topid = contour_tree[i].topid;
         prune_tree[i].bottomid = contour_tree[i].bottomid;
         for(int j=0;j<contour_tree[i].topArc.size();j++)
              prune_tree[i].topArc.push_back(contour_tree[i].topArc[j]);
         for(int j=0;j<contour_tree[i].bottomArc.size();j++)
              prune_tree[i].bottomArc.push_back(contour_tree[i].bottomArc[j]);
         prune_tree[i].volume = contour_tree[i].volume;

         if(prune_tree[i].volume <= bound)
         {
            if(prune_tree[i].topArc.size()==0)
            {
               label[i] = 0;
               for(int j=0;j<prune_tree[i].bottomArc.size();j++)
               {
                   int theArc = prune_tree[i].bottomArc[j];
                   for(int t=0;t<prune_tree[theArc].topArc.size();t++)
                   {
                      if(prune_tree[theArc].topArc[t]==i)
                      {
                          prune_tree[theArc].topArc.erase(prune_tree[theArc].topArc.begin()+t);
                          break;  
                      }
                   }
               }
            } 
         }
      } 

 //cut from bottom to top
      for(int i=prune_tree.size()-1;i>=0;i--)
      {
         if(label[i])
         {
           if(prune_tree[i].volume <= bound)
           {
            if(prune_tree[i].bottomArc.size()==0)
            {
               label[i] = 0;
               for(int j=0;j<prune_tree[i].topArc.size();j++)
               {
                   int theArc = prune_tree[i].topArc[j];
                   for(int t=0;t<prune_tree[theArc].bottomArc.size();t++)
                   {
                      if(prune_tree[theArc].bottomArc[t]==i)
                      {
                          prune_tree[theArc].bottomArc.erase(prune_tree[theArc].bottomArc.begin()+t);
                          break;
                      }
                   }
               }
            } 
           }
         } 
      }

 //cut new small leafs again
      for(int i=prune_tree.size()-1;i>=0;i--)
      {
         if(label[i] && prune_tree[i].volume > bound)
         {
             isoArc[ii].add(i);
         }
      }
      label.free();
   }

      for(int i=0;i<prune_tree.size();i++)
      {
        prune_tree[i].topArc.clear();
        prune_tree[i].bottomArc.clear();
      }
      prune_tree.clear();
}

void ContourTree::GetDisplayIndex(svInt bound, svScalar threshold, svIntArray &index)
{
    svIntArray ii;
    svScalarArray dd;

    int boundindex = (bound - volumebound[0])/volumestep;
    svScalar prevalue = 0;//contour_tree[isoArc[boundindex][0]].bottomvalue;
    for(int i=0;i<isoArc[boundindex].size();i++)
//isoArc[boundindex].size()-1;i>=0;i--)
    {
       int arc = isoArc[boundindex][i];

       if(contour_tree[arc].volume <= bound)
       {
         continue;
       }

       if(contour_tree[arc].topArc.size()<=1
        &&contour_tree[arc].bottomArc.size()<=1)
       { 
         continue;
       }

       svScalar value;
//     cerr<<contour_tree[arc].bottomvalue<<" ";
       if(contour_tree[arc].bottomvalue>0)
       {
          value = contour_tree[arc].bottomvalue;
       }
       else if(contour_tree[arc].topvalue>0)
       {
          value = (contour_tree[arc].bottomvalue+contour_tree[arc].topvalue)/2.;
       }
       else
          continue;
//     cerr<<contour_tree[arc].bottomvalue<<" "<<prevalue<<" ";
          svScalar ratio;
          if(prevalue>0)
          {   ratio = (prevalue-value)/prevalue;//-log10(value)+log10(prevalue);
// (contour_tree[arc].bottomvalue - prevalue)/contour_tree[arc].bottomvalue;
//              cerr<<ratio<<" "<<threshold<<" ";
              if(ratio > threshold)
              {
                ii.add(arc);
                dd.add(ratio);
               // index.add(arc);
                prevalue = value;    
              }
          }
          else
          {
                ii.add(arc);
                dd.add(1.);
             // index.add(arc);
              prevalue = value;
          }
    }
    if(0)//ii.size()>5)
    {
       vector<double> tmpdd;
       for(int i=0;i<ii.size();i++)
            tmpdd.push_back(dd[i]);
       sort(tmpdd.begin(), tmpdd.end());
       for(int i=4;i>=0;i--)
       {
         for(int j=0;j<ii.size();j++)
         {
           if(fabs(dd[j]-tmpdd[ii.size()-1-i])<1e-9)
           {//cerr<<ii[j]<<" ";
               index.add(ii[j]);break;
           }
         }
       }//cerr<<endl;
       tmpdd.clear();
    }
    else
    {
         for(int i=0;i<ii.size();i++)
             index.add(ii[i]);
    }
    dd.free();
    ii.free();
/*    if(index.size()==0)
    {
       index.add(isoArc[boundindex][isoArc[boundindex].size()-1]);
    }
*/ 
//   cerr<<isoArc[boundindex].size()<<" "<<index.size()<<endl;

}
svScalar ContourTree::GetIsoValue(int index)
{
    return isoValue[index];
/*
    if(contour_tree[index].bottomvalue>0)
         return contour_tree[index].bottomvalue;
    else if(contour_tree[index].topvalue>0)
         return (contour_tree[index].bottomvalue+contour_tree[index].topvalue)/2.;
    else 
         return 0;

*/
}
ContourTree::~ContourTree()
{
   for(int i=0;i<contour_tree.size();i++)
   {
      contour_tree[i].topArc.clear();
      contour_tree[i].bottomArc.clear();
   }
   contour_tree.clear();

   for(int i=0;i<contour_node.size();i++){
      contour_node[i].topArc.clear();
      contour_node[i].bottomArc.clear();
   }
   contour_node.clear();

   for(int i=0;i<isoCount;i++)
   {
     isoArc[i].free();
   }
   isoValue.clear();
   delete [] isoArc;
}

}

