#include "genVTKfromOrg.h"
#include <iostream>
#include <cmath>

bool Sort_Spin_Angle(const Spin &a, const Spin &b){
//  Spin t;
//  t.px = a.py * b.pz - a.pz * b.py;
//  t.py = a.pz * b.px - a.px * b.pz;
//  t.pz = a.px * b.py - a.py * b.px;
//  return (t.pz > 0);
  return (atan2(a.py, a.px) < atan2(b.py, b.px));
}
  
bool Sort_Spin_X(const Spin &a, const Spin &b)
{
 if (fabs(a.px - b.px)>0.0001)
 {
   return (a.px<b.px);
 }
 else if (fabs(a.py - b.py)>0.0001)
 {
   return (a.py<b.py);
 }

 return 0;
}

bool Sort_Spin_Y(const Spin &a, const Spin &b)
{
 if (fabs(a.py - b.py)>0.0001)
 {
   return (a.py<b.py);
 }
 else if (fabs(a.px - b.px)>0.0001)
 {
   return (a.px<b.px);
 }

 return 0;
}

bool Sort_Spin_Z(const Spin &a, const Spin &b)
{
 if (fabs(a.pz - b.pz)>0.0001)
 {
   return (a.pz<b.pz);
 }
 else if (fabs(a.py - b.py)>0.0001)
 {
   return (a.py<b.py);
 }
 else if (fabs(a.px - b.px)>0.0001)
 {
   return (a.px<b.px);
 }

 return 0;
}

bool linePlane(double * a, double* b, double* p, double *n)
{
        bool flag = false; 
        //svVector3 AP = p - a;
        //svVector3 BP = p - b;
        double AP[3];
	double BP[3];	
	AP[0] = p[0] - a[0];
        AP[1] = p[1] - a[1];
        AP[2] = p[2] - a[2];

        BP[0] = p[0] - b[0];
        BP[1] = p[1] - b[1];
        BP[2] = p[2] - b[2];
   
        double dd1 = AP[0]*n[0]+AP[1]*n[1]+AP[2]*n[2];//GetDot(AP, n);
        double dd2 = BP[0]*n[0]+BP[1]*n[1]+BP[2]*n[2];//GetDot(BP, n);
        if(dd1 * dd2 <= 0 ) flag = true;

        return flag;
}
bool planeBox(double* lb, double *rb, double * p, double *n)
{
        bool flag = false;

        double a[3],b[3];
        a[0] = lb[0];a[1] = lb[1]; a[2] = lb[2];
        b[0] = lb[0];b[1] = lb[1]; b[2] = rb[2];
        flag = linePlane(a, b, p, n);

        if(flag)        return flag;

        a[0] = rb[0];a[1] = lb[1]; a[2] = lb[2];
        b[0] = rb[0];b[1] = lb[1]; b[2] = rb[2];
        flag = linePlane(a, b, p, n);

        if(flag)        return flag;

        a[0] = rb[0];a[1] = rb[1]; a[2] = lb[2];
        b[0] = rb[0];b[1] = rb[1]; b[2] = rb[2];
        flag = linePlane(a, b, p, n);

        if(flag)        return flag;

        a[0] = lb[0];a[1] = rb[1]; a[2] = lb[2];
        b[0] = lb[0];b[1] = rb[1]; b[2] = rb[2];
        flag = linePlane(a, b, p, n);

        if(flag)        return flag;

        a[0] = lb[0];a[1] = lb[1]; a[2] = lb[2];
        b[0] = rb[0];b[1] = lb[1]; b[2] = lb[2];
        flag = linePlane(a, b, p, n);

        if(flag)        return flag;
        a[0] = rb[0];a[1] = lb[1]; a[2] = lb[2];
        b[0] = rb[0];b[1] = rb[1]; b[2] = lb[2];
        flag = linePlane(a, b, p, n);

        if(flag)        return flag;

        a[0] = rb[0];a[1] = rb[1]; a[2] = lb[2];
        b[0] = lb[0];b[1] = rb[1]; b[2] = lb[2];
        flag = linePlane(a, b, p, n);

        if(flag)        return flag;

        a[0] = lb[0];a[1] = rb[1]; a[2] = lb[2];
        b[0] = lb[0];b[1] = lb[1]; b[2] = lb[2];
        flag = linePlane(a, b, p, n);

        if(flag)        return flag;
        a[0] = lb[0];a[1] = lb[1]; a[2] = rb[2];
        b[0] = rb[0];b[1] = lb[1]; b[2] = rb[2];
        flag = linePlane(a, b, p, n);
        if(flag)        return flag;
        a[0] = rb[0];a[1] = lb[1]; a[2] = rb[2];
        b[0] = rb[0];b[1] = rb[1]; b[2] = rb[2];
        flag = linePlane(a, b, p, n);
        if(flag)        return flag;
        a[0] = rb[0];a[1] = rb[1]; a[2] = rb[2];
        b[0] = lb[0];b[1] = rb[1]; b[2] = rb[2];
        flag = linePlane(a, b, p, n);
        if(flag)        return flag;
        a[0] = lb[0];a[1] = rb[1]; a[2] = rb[2];
        b[0] = lb[0];b[1] = lb[1]; b[2] = rb[2];
        flag = linePlane(a, b, p, n);

        return flag;
}

genVTKfromOrg::genVTKfromOrg()
{
    dataSize = 0;
    planename = new char[100];
}

int genVTKfromOrg::CheckFormat(char *file)
{
  char filename[2056];
  sprintf(filename, "%s", file);

  ifstream infile(filename);

  if(infile.fail())
  {
	cerr<<filename<<" not found "<<endl;
	cerr<<"Load QDOT data failed. prog exit \n"<<endl;

	exit(-1);
  }
  else
  {
    cerr<<"Load data "<<filename<<endl;
  }

  double pos[3];
  double vel[3];
  double  den;

  string str;
  int count = 0;
  bool flag = false;

  getline(infile, str);

  for(int i=0;i<str.length(); i++)
  {

	if((str[i] < '0' 
	|| str[i] > '9')
	&& str[i] != '-'
	&& str[i] !='.'
        && str[i] !='e'
	&& flag == true)
	{
		count++;
		flag = false;
	}
	else if((str[i] >= '0' 
	&& str[i] <= '9')
	|| str[i] == '-'
	|| str[i] =='.'
        || str[i] == 'e')
	{
	//cerr<<str[i]<<endl;
		flag = true;
	}
  }
  count = count + 1;

  infile.close(); 
  
  return count; 
}

int genVTKfromOrg::getNumOfIntegerDigits(double num)
{
	  int multiplier = 0;

	  double numCopy = num;

	  if(numCopy<1&&numCopy!=0) 
	  {
	    while ((fabs(numCopy-1.)>0.000001)&&(numCopy-1.)<0.000001)
		{

		  multiplier--;//cerr<<num<<" "<<multiplier<<endl;
		  numCopy *= 10;
		}; // end while(1)
	  }
	  else if(numCopy==0)
		multiplier=0;
	  else if(numCopy>=1)
	  {
	    while(1)
		{
		  if(numCopy>=0 && numCopy <10)
		  {
		    break;
		  };
		  multiplier++;
		  numCopy /= 10.;
		}; // end while (1)
	  }; // end if(numCopy)

	  return multiplier;	
}

void genVTKfromOrg::ReadRawData(char *file, int DEN_SCALAR)
{
  long double vx, vy, vz;
  long double px, py, pz;
  long double dummy;

  double pos[3];
  double vel[3];
  double  den;
  double a;
  double r;

  char filename[2056];
  sprintf(filename, "%s", file);//cerr<<filename<<endl;

  int data_type; 
  int count = format;//CheckFormat(filename);
  //cerr<<count<<endl;
/*  if(count == 9)
  {
	data_type = 1;//new
  }
  else if(count == 10)
  {
	data_type = 2;//old
  }
  else
  {
	data_type = 0;//not defined
	cerr << filename << " ";
	cerr << "Load QDOT data failed. Wrong format. Prog exit\n" <<endl;
	exit(-1);
  }
*/
  data_type = format;

  ifstream infile(filename);
  
  if(infile.fail())
  {
    cerr << filename<< "not found.\n";
	cerr << "Load QDOT data failed. prog exit\n" <<endl;
	exit(-1);
  }
  else
  {
	cerr << "Loading data: " << filename << endl;
  }
  min_den_zero = 9e+9;
  min_den = 9e+9;
  max_den = -1;  
  //cerr<<data_type<<endl;

   double tmpr;
  if(data_type == 4)
  {
	  while(infile>>pos[0] >> pos[1] >> pos[2] >> tmpr >> a >> vel[0] >> vel[1] >> vel[2] >> den>>r)
	  {
	
	    //vel*=VEL_SCALAR;
		double tmpden;
		tmpden = vel[0]*vel[0] + vel[1]*vel[1] + vel[2]*vel[2];
		tmpden = sqrt(tmpden);
		den = tmpden * den;
		if(tmpden > 0)
		{

		  vel[0] = vel[0]/tmpden;
		  vel[1] = vel[1]/tmpden;
		  vel[2] = vel[2]/tmpden;
		}
		
		Spin d;
		d.px = pos[0];
		d.py = pos[1];
		d.pz = pos[2];
		d.vx = vel[0];
		d.vy = vel[1];
		d.vz = vel[2];
		d.den = den;
		d.atom = a;
		d.region = tmpr;
		
	//outfile<<pos[0]<<" "<<pos[1]<<" "<<pos[2]<<" "<<1<<" "<<2<<" "<<vel[0]*den<<" "<<vel[1]*den<<" "<<vel[2]*den<<endl;
		data.push_back(d);
		
		if(min_den > d.den && d.den>0)
		{
		  min_den = d.den;
		}
                if(min_den_zero > d.den)
                {
                  min_den_zero = d.den;
                }

	        if(max_den < d.den)
                {
                 max_den = d.den;
                }	
		//dataSize ++;
	  }
  }
  else if(data_type == 3)
 {
	//cerr<<data_type<<endl;
	  while(infile>>pos[0] >> pos[1] >> pos[2] >> tmpr >> a>> vel[0] >> vel[1] >> vel[2] >> r)
	  {
	    //vel*=VEL_SCALAR;
	
		den = vel[0]*vel[0] + vel[1]*vel[1] + vel[2]*vel[2];
		den = sqrt(den);
		if(den>0)
		{
		  vel[0] = vel[0]/den;
		  vel[1] = vel[1]/den;
		  vel[2] = vel[2]/den;
		}

		Spin d;
		d.px = pos[0];
		d.py = pos[1];
		d.pz = pos[2];
		d.vx = vel[0];
		d.vy = vel[1];
		d.vz = vel[2];
		d.den = den;
		d.atom = a;
		d.region = r;		
		//cerr<<den<<endl;
		
	//outfile<<pos[0]<<" "<<pos[1]<<" "<<pos[2]<<" "<<1<<" "<<2<<" "<<vel[0]*den<<" "<<vel[1]*den<<" "<<vel[2]*den<<endl;
		data.push_back(d);
               if(min_den_zero > d.den)
                {
                  min_den_zero = d.den;
                }

		if(min_den > d.den && d.den>0)
		{
		  min_den = d.den;
		}		
		if(max_den < d.den)
                {
                   max_den = d.den;
                }
		//dataSize ++;
	  }
  }
  else if(data_type == 2)
  {
	  while(infile>>pos[0] >> pos[1] >> pos[2] >> r >> a >> vel[0] >> vel[1] >> vel[2] >> den)
	  {
	
	    //vel*=VEL_SCALAR;
		double tmpden;
		tmpden = vel[0]*vel[0] + vel[1]*vel[1] + vel[2]*vel[2];
		tmpden = sqrt(tmpden);
		den = tmpden * den;
		if(tmpden > 0)
		{

		  vel[0] = vel[0]/tmpden;
		  vel[1] = vel[1]/tmpden;
		  vel[2] = vel[2]/tmpden;
		}
		
		Spin d;
		d.px = pos[0];
		d.py = pos[1];
		d.pz = pos[2];
		d.vx = vel[0];
		d.vy = vel[1];
		d.vz = vel[2];
		d.den = den;
		d.atom = a;
		d.region = r;
		
	//outfile<<pos[0]<<" "<<pos[1]<<" "<<pos[2]<<" "<<1<<" "<<2<<" "<<vel[0]*den<<" "<<vel[1]*den<<" "<<vel[2]*den<<endl;
		data.push_back(d);
		
		if(min_den > d.den && d.den>0)
		{
		  min_den = d.den;
		}
                if(min_den_zero > d.den)
                {
                  min_den_zero = d.den;
                }

	        if(max_den < d.den)
                {
                 max_den = d.den;
                }	
		//dataSize ++;
	  }
  }
  else if(data_type == 1)
 {
	//cerr<<data_type<<endl;
	  while(infile>>pos[0] >> pos[1] >> pos[2] >> r >> a>> vel[0] >> vel[1] >> vel[2])
	  {
	    //vel*=VEL_SCALAR;
	
		den = vel[0]*vel[0] + vel[1]*vel[1] + vel[2]*vel[2];
		den = sqrt(den);
		if(den>0)
		{
		  vel[0] = vel[0]/den;
		  vel[1] = vel[1]/den;
		  vel[2] = vel[2]/den;
		}

		Spin d;
		d.px = pos[0];
		d.py = pos[1];
		d.pz = pos[2];
		d.vx = vel[0];
		d.vy = vel[1];
		d.vz = vel[2];
		d.den = den;
		d.atom = a;
		d.region = r;		
		//cerr<<den<<endl;
		
	//outfile<<pos[0]<<" "<<pos[1]<<" "<<pos[2]<<" "<<1<<" "<<2<<" "<<vel[0]*den<<" "<<vel[1]*den<<" "<<vel[2]*den<<endl;
		data.push_back(d);
               if(min_den_zero > d.den)
                {
                  min_den_zero = d.den;
                }

		if(min_den > d.den && d.den>0)
		{
		  min_den = d.den;
		}		
		if(max_den < d.den)
                {
                   max_den = d.den;
                }
		//dataSize ++;
	  }
  }  
  infile.close();

//  cerr<<den<<endl;

		 /* 
int  int_min_den = (double) getNumOfIntegerDigits(min_den);
		    //cerr<<min_den<<endl;
  if((-int_min_den) < DEN_SCALAR)
  {
	  int_min_den = DEN_SCALAR;
  }
  else
  {
	 int_min_den = -int_min_den;
  }
  
  lbbox[0] = 9e+9;lbbox[1] = 9e+9; lbbox[2] = 9e+9;
  rbbox[0] = -9e+9;rbbox[1] = -9e+9; rbbox[2] = -9e+9;

  min_region = 9e+1;
*/
  dataSize = data.size();
/*
  for(int i=0;i<dataSize;i++)
  {
    //data[i].den = data[i].den * pow(10., min_den);
    
    if(data[i].px < lbbox[0]) lbbox[0] = data[i].px;
    if(data[i].py < lbbox[1]) lbbox[1] = data[i].py;
    if(data[i].pz < lbbox[2]) lbbox[2] = data[i].pz;
    if(data[i].px > rbbox[0]) rbbox[0] = data[i].px;
    if(data[i].py > rbbox[1]) rbbox[1] = data[i].py;
    if(data[i].pz > rbbox[2]) rbbox[2] = data[i].pz;

    int flag = false;
    for(int j=0;j<region_count.size();j++)
    {
	if(data[i].region == unique_region[j])
	{
		flag = true;
		break;
	}
    }
    if(!flag)
    {
	unique_region.push_back(data[i].region);
	region_count.push_back(0);
    }
    
    if(data[i].region < min_region)
    {
	min_region = data[i].region;
    }
  }
//cerr<<min_region<<endl;
  for(int i=0;i<dataSize;i++)
  {
	region_count[data[i].region-min_region]++;
  }

  //outfile.close();
  //for(int i=0;i<zValue.size();i++)cerr<<denMax[i]<<" ";

  scaling =int_min_den;

  //cerr<<scaling<<endl;
  */
  sort(data.begin(), data.end(), Sort_Spin_Z); //cerr<<scaling<<endl;  
  sort(data.begin(), data.end(), Sort_Spin_Z); //cerr<<scaling<<endl;  
  sort(data.begin(), data.end(), Sort_Spin_Z);
 
  SplitData();

//========================================
  int  int_min_den = (double) getNumOfIntegerDigits(min_den);
		    //cerr<<min_den<<endl;
  if((-int_min_den) < DEN_SCALAR)
  {
	  int_min_den = DEN_SCALAR;
  }
  else
  {
	 int_min_den = -int_min_den;
  }
  
  lbbox[0] = 9e+9;lbbox[1] = 9e+9; lbbox[2] = 9e+9;
  rbbox[0] = -9e+9;rbbox[1] = -9e+9; rbbox[2] = -9e+9;

  min_region = 1;//9e+1;

  int inside = 1;
  int outside = inside + 1;
  dataSize = data.size();
  double zv = data[Zlayer_Num[1]].pz - data[0].pz;
  min_zdistance = zv;
  min_zlayer_value = data[0].pz;
  int inside_label = inside;
  int inside_index = (data[0].region==inside)?0:-1;;
  int shape_state = (data[0].region==inside)?0:-1;//cerr<<shape_state<<endl;
  if(inside_index>=0){
    region_z.push_back(data[0].pz);
    region_plane_z.push_back(data[0].pz);
  }
  for(int i=0;i<dataSize;i++){   
    if(data[i].px < lbbox[0]) lbbox[0] = data[i].px;
    if(data[i].py < lbbox[1]) lbbox[1] = data[i].py;
    if(data[i].pz < lbbox[2]) lbbox[2] = data[i].pz;
    if(data[i].px > rbbox[0]) rbbox[0] = data[i].px;
    if(data[i].py > rbbox[1]) rbbox[1] = data[i].py;
    if(data[i].pz > rbbox[2]) rbbox[2] = data[i].pz;

    int preinside_index = inside_index;
    if(data[i].region==inside){
     shape_state = (shape_state==0)?0:1;
     inside_index = i;
    }
    if(i<dataSize-1){
      if(fabs(data[i+1].pz - data[i].pz)>1e-8){
         shape_type.push_back(shape_state);
        if(data[i+1].region == inside){
         shape_state=0;
         if(region_z.size()==0){
           region_z.push_back(data[i+1].pz);
           region_plane_z.push_back(data[i+1].pz);
         }
        }
        else if(shape_state==0){
         shape_state = -1;
         region_plane_z.push_back(data[i].pz);
        }
        else{
         shape_state=-1;
        }
      }
    }
    if(data[i].region == outside){
     data[i].region = 0;
     continue;
    }
    if(preinside_index>=0){
      if(data[i].region==inside
        && data[preinside_index].region==inside
        && fabs(data[i].pz - data[preinside_index].pz)>zv){
        inside_label++;
        data[i].region = inside_label;
        region_z.push_back(data[preinside_index].pz);
        region_z.push_back(data[i].pz);
        region_plane_z.push_back(data[i].pz);
      }
    } 
  }
  region_z.push_back(data[inside_index].pz);
  shape_type.push_back(shape_state);
  for(int i=0;i<=inside_label;i++){
    unique_region.push_back(i);
  }
  region_count.resize(unique_region.size());
  for(int i=0;i<dataSize;i++){
	region_count[data[i].region]++;
  }
  scaling =int_min_den;

  //cerr<<region_z.size()<<" "<<region_z[0]<<" "<<region_z[1]<<endl;
  //cerr<<region_plane_z.size()<<" "<<region_plane_z[0]<<" "<<region_plane_z[1]<<endl;
}

void genVTKfromOrg::SplitData(double pos_x, double pos_y, double pos_z,
                        double dir_x, double dir_y, double dir_z,
			double distance)
{
        double sum = dir_x*dir_x+dir_y*dir_y+dir_z*dir_z;
        sum = sqrt(sum);
        dir_x = dir_x/sum;
        dir_y = dir_y/sum;
        dir_z = dir_z/sum;

//        sprintf(planename, "%0.2f%0.2f%0.2f%0.2f%0.2f%0.2f",
//                           pos_x, pos_y, pos_z,
//                           dir_x, dir_y, dir_z);

        double mind1 = 9e+9;
        double mind2 = 9e+9;
        for(int i=0;i<data.size();i++)
        {
            double d = (data[i].px - pos_x)*dir_x
                     + (data[i].py - pos_y)*dir_y
                     + (data[i].pz - pos_z)*dir_z;
            d = fabs(d);
            if(d<mind1||fabs(d-mind1)<1e-9)
            {
                mind1 = d;
            }
            else 
            {
                if(d<mind2)
                {
                   mind2 = d;
                }
            }
        }
//redefine the distance 
        distance = mind2 - mind1;

//========================================================================
	double min_p[3];
	double max_p[3];

	min_p[0] = pos_x; min_p[1] = pos_y; min_p[2] = pos_z;
	max_p[0] = pos_x; max_p[1] = pos_y; max_p[2] = pos_z;

	if(dir_z < 0)
	{
		dir_x = - dir_x;
		dir_y = - dir_y;
		dir_z = - dir_z;		
	}
	else if(dir_y < 0)
	{
                dir_x = - dir_x;
                dir_y = - dir_y;
                dir_z = - dir_z;
	}
	else if(dir_x < 0)
	{
                dir_x = - dir_x;
                dir_y = - dir_y;
                dir_z = - dir_z;
	}
        double dir[3];
        dir[0] = dir_x;
        dir[1] = dir_y;
        dir[2] = dir_z;

	while(1)
        {
                min_p[0] = min_p[0] - dir_x * distance;
                min_p[1] = min_p[1] - dir_y * distance;
                min_p[2] = min_p[2] - dir_z * distance;


                bool flag = planeBox(lbbox, rbbox, min_p, dir);

                if(flag == false)
                {
                        min_p[0] = min_p[0] + dir_x * distance;
                        min_p[1] = min_p[1] + dir_y * distance;
                        min_p[2] = min_p[2] + dir_z * distance;
                        break;
                }

//		cerr<<min_p[2]<<endl;
        }
        while(1)
        {

                max_p[0] = max_p[0] + dir_x * distance;
                max_p[1] = max_p[1] + dir_y * distance;
                max_p[2] = max_p[2] + dir_z * distance;
               bool flag = planeBox(lbbox, rbbox, max_p ,dir);

                if(flag == false)
                {
                        max_p[0] = max_p[0] - dir_x * distance;
                        max_p[1] = max_p[1] - dir_y * distance;
                        max_p[2] = max_p[2] - dir_z * distance;
                        break;
                }
        }
//redefine the num of planes=================
	int numPlane = sqrt((max_p[0] - min_p[0]) * (max_p[0] - min_p[0])
                + (max_p[1] - min_p[1]) * (max_p[1] - min_p[1])
                + (max_p[2] - min_p[2]) * (max_p[2] - min_p[2]))/distance + 1;
//cerr<<numPlane<<endl;
	plane_min.resize(3);
	plane_dir.resize(3);
	plane_min[0] = min_p[0];
        plane_min[1] = min_p[1];
        plane_min[2] = min_p[2];
	plane_dir[0] = dir[0];
        plane_dir[1] = dir[1];
        plane_dir[2] = dir[2];
	plane_distance = distance;

        splitIndex.resize(numPlane);	
	splitData.resize(numPlane);
        split2dData.resize(numPlane);
	layerDensity.resize(numPlane);

cerr<<"Split data by "<<numPlane<<" layers along direction ("<<dir[0]<<","<<dir[1]<<","<<dir[2]<<")"<<endl;
//cerr<<min_p[0]<<" "<<min_p[1]<<" "<<min_p[2]<<endl;
	for(int i=0;i<numPlane;i++)
	{
		layerDensity[i] = 0;	
	}

	for(int i=0;i<dataSize;i++)
	{
		double tmp[3];
		tmp[0] = data[i].px - min_p[0];
		tmp[1] = data[i].py - min_p[1];
		tmp[2] = data[i].pz - min_p[2];	

		double dot = tmp[0] * dir_x
				+ tmp[1] * dir_y
				+ tmp[2] * dir_z;

		if((fmod(dot, (double)distance)<1e-5)|| (distance-fmod(dot, (double)distance)<1e-5))
		{
			int index = (int)(dot/distance);
                //        if(index>=283)cerr<<data[i].px<<" "<<data[i].py<<" "<<data[i].pz<<" "<<index<<endl;
			splitData[index].push_back(data[i]);
                        splitIndex[index].push_back(i);
                        splitReverseIndex[0].push_back(index);
                        splitReverseIndex[1].push_back(splitIndex[index].size()-1); 
			if(data[i].den > layerDensity[index])
			{
				layerDensity[index] = data[i].den;
			}
		}	
	}			
        sprintf(planename, "%0.2f%0.2f%0.2f%0.2f%0.2f%0.2f",
                           min_p[0], min_p[1], min_p[2],//pos_y, pos_z,
                           dir_x, dir_y, dir_z);

//    cerr<<"split data"<<endl;
}
/*
void genVTKfromOrg::ComputeNeighbors(){
 for(int i=1;i<Zlayer_Num.size();i++){
   for(int j=Zlayer_Num[i-1]; j<Zlayer_Num[i];j++){
         
   }
 }
}
*/


void genVTKfromOrg::ComputeMeshPoints(char *BIN_DIR, char *dir){
 for(int i=1;i<unique_region.size();i++){
       char *pointstr = new char[300];
       sprintf(pointstr, "%s/point%d", dir, unique_region[i]);
       ifstream infile(pointstr);
       char *str = new char[300];
       sprintf(str, "%s/region%d", dir, unique_region[i]);
       char *outstr = new char[300];
       sprintf(outstr, "%s/tregion%d", dir, unique_region[i]);
       char *exe = new char[1024];
       sprintf(exe, "%s/bin/ex_alpha_shapes_3 %s > %s 2>&1", BIN_DIR, str, pointstr);
       if(infile.is_open())
       {
           infile.close();
       }
       else
       {
           infile.close();
           system(exe);
       //    ComputeConvexMesh(BIN_DIR, outstr, pointstr, i);
       }
       delete [] pointstr;
       delete [] outstr;
       delete [] str;
       delete [] exe;
   }
}

//processing one qdot
void genVTKfromOrg::ComputeConvexMesh(char *src_dir, char *ifname, char *ofname, int region){
//=================Read Alpha Shape==================
 ifstream infile(ifname);
 int n;
 infile>>n;
 vector<Spin> pos;
 for(int i=0;i<n;i+=3){
  for(int j=0;j<3;j++){
   Spin p;
   infile>>p.px>>p.py>>p.pz;
   pos.push_back(p);
  }
 }
 infile.close(); 

 sort(pos.begin(), pos.end(), Sort_Spin_Z); //cerr<<scaling<<endl;  
 sort(pos.begin(), pos.end(), Sort_Spin_Z); //cerr<<scaling<<endl;  
 sort(pos.begin(), pos.end(), Sort_Spin_Z);
//====================Smooth mesh============================
 vector< vector<Spin> > tmesh;
 for(int i=0;i<pos.size()-1;i++){
   if(fabs(pos[i].px - pos[i+1].px)<1e-8
   && fabs(pos[i].py - pos[i+1].py)<1e-8
   && fabs(pos[i].pz - pos[i+1].pz)<1e-8){
     pos.erase(pos.begin()+(i+1));i--;
   }
 }
 vector< vector < vector<int> > > zline;
 vector<int> xline;
 vector< vector<int> > yline;
 bool first_pos = true;
 for(int i=0;i<pos.size()-1;i++){
   int zi = (pos[i].pz - min_zlayer_value)/min_zdistance;
   int state = shape_type[zi];
   if(fabs(pos[i].pz-pos[i+1].pz)<1e-8){
     if(fabs(pos[i].py-pos[i+1].py)<1e-8){
      if(first_pos == true){
        xline.push_back(i);
        first_pos = false;
      }
      else if(fabs(pos[i].px - pos[i+1].px)>min_xdistance){
        xline.push_back(i);
        xline.push_back(i+1);
      }
     }
     else{
       xline.push_back(i);
       yline.push_back(xline);
       xline.clear();
       first_pos = true; 
     }
   }
   else{
     xline.push_back(i);
     yline.push_back(xline);
     zline.push_back(yline);
     xline.clear();
     for(int ii=0;ii<yline.size();ii++){
        yline[ii].clear();
     }
     yline.clear();
     first_pos = true;
   } 
 }
 xline.push_back(pos.size()-1);
 yline.push_back(xline);
 zline.push_back(yline);
 xline.clear();
 for(int ii=0;ii<yline.size();ii++){
  yline[ii].clear();
 }
 tmesh.resize(zline.size());
//===============Compute Convex============================
 vector< vector<Spin> > points;
 points.resize(zline.size());
 for(int i=0;i<zline.size();i++){
   int zi = (pos[zline[i][0][0]].pz - min_zlayer_value)/min_zdistance;
   int state = shape_type[zi];
   if(state == 0){
     for(int j=0;j<zline[i].size();j++){
      if(zline[i][j].size()==2){
       for(int t=zline[i][j][0];t<=zline[i][j][1];t++){
         tmesh[i].push_back(pos[t]);
       }
      }
      else if(zline[i][j].size()>2){
        
         for(int t=zline[i][j][0];t<=zline[i][j][1]-1;t++){
           tmesh[i].push_back(pos[t]);
         }
         points[i].push_back(pos[zline[i][j][1]]);
         for(int t=zline[i][j][2]+1;t<=zline[i][j][3];t++){
           tmesh[i].push_back(pos[t]);
         }
         points[i].push_back(pos[zline[i][j][2]]);
      }
     }
   }
   else{
     for(int j=0;j<zline[i].size();j++){
       if(j==0 || j==zline[i].size()-1){
        if(zline[i][j].size()==1)
          tmesh[i].push_back(pos[zline[i][j][0]]);
        for(int t=zline[i][j][0];t<=zline[i][j][1];t++){
         tmesh[i].push_back(pos[t]);
        }
       }
       else{
         if(zline[i][j].size()==2){
           points[i].push_back(pos[zline[i][j][0]]);
           points[i].push_back(pos[zline[i][j][1]]);
         }
         else if(zline[i][j].size()>2){
           points[i].push_back(pos[zline[i][j][0]]);
           points[i].push_back(pos[zline[i][j][3]]);
         }
         else if(zline[i][j].size()>0)
           points[i].push_back(pos[zline[i][j][0]]);
       }
     }
   }
 }
//===========================CONVEX====================
 int meshsize = 0;
 for(int i=0;i<points.size();i++){
     if(points[i].size()==0)continue;
      ofstream outfile("tmpmesh");
      outfile<<points[i].size()<<endl;
      for(int j=0;j<points[i].size();j++){
         outfile<<points[i][j].px<<" "<<points[i][j].py<<" "<<points[i][j].pz<<endl;
      }
      outfile.close();

      char *exe = new char[1024];
      sprintf(exe, "%s/preprocessing/generateVTK/Convex_hull_2/vector_convex_hull_2 tmpmesh tmpregion %0.2f", src_dir, points[i][0].pz);
      //cerr<<exe<<endl;
      system(exe);
      delete [] exe;

      points[i].clear();
      ifstream infile("tmpregion");
      int n;
      infile>>n;
      for(int j=0;j<n;j++){
         Spin pp;
         infile>>pp.px>>pp.py>>pp.pz;
         points[i].push_back(pp);
      }
      infile.close();
      sort(points[i].begin(), points[i].end(), Sort_Spin_Angle);
      //sort(points[i].begin(), points[i].end(), Sort_Spin_X);

      int size = points[i].size();
      for(int j=0;j<size;j++){
       if(j<size-1){ 
         int y = fabs(points[i][j+1].py - points[i][j].py)/min_ydistance;
         double yd = points[i][j+1].py - points[i][j].py;
         double xd = points[i][j+1].px - points[i][j].px;
         int sign = yd>0?1:-1;
         for(int t=0;t<y-1;t++){
           Spin pp;
           pp.py = points[i][j].py + min_ydistance * (t+1)*sign;
           pp.pz = points[i][j].pz;
           pp.px = points[i][j].px + xd * ((pp.py - points[i][j].py)/yd);
           points[i].push_back(pp);
         }
       }
       else{
         int y = fabs(points[i][0].py - points[i][j].py)/min_ydistance;
         double yd = points[i][0].py - points[i][j].py;
         double xd = points[i][0].px - points[i][j].px;
         int sign = yd>0?1:-1;
         for(int t=0;t<y-1;t++){
           Spin pp;
           pp.py = points[i][j].py + min_ydistance * (t+1)*sign;
           pp.pz = points[i][j].pz;
           pp.px = points[i][j].px + xd * ((pp.py - points[i][j].py)/yd);
           points[i].push_back(pp);
         }
       }
/*         yd = points[i][j+3].py - points[i][j+1].py;
         y = yd/min_ydistance;
         xd = points[i][j+3].px - points[i][j+1].px;
         for(int t=0;t<y-1;t++){
           Spin pp;
           pp.py = points[i][j+1].py + min_ydistance * (t+1);
           pp.pz = points[i][j+1].pz;
           pp.px = points[i][j+1].px + xd * ((pp.py - points[i][j+1].py)/yd);
           points[i].push_back(pp);
         }
 */     }
      //sort(points[i].begin(), points[i].end(), Sort_Spin_Y);
      //sort(points[i].begin(), points[i].end(), Sort_Spin_Y); 

  }

  for(int i=0;i<points.size();i++){  
      for(int j=0;j<points[i].size();j++){
        tmesh[i].push_back(points[i][j]);
      }
      //points[i].clear();
      meshsize+=tmesh[i].size();
      sort(tmesh[i].begin(), tmesh[i].end(), Sort_Spin_Angle);
  //    sort(tmesh[i].begin(), tmesh[i].end(), Sort_Spin_Y);
 }
// points.clear();
 

/*
 cout<<points.size()<<endl;
 for(int i=0;i<points.size();i++){
   cout<<points[i].size()<<endl;
   for(int j=0;j<points[i].size();j++){
     cout<<points[i][j].px<<" "<<points[i][j].py<<" "<<points[i][j].pz<<endl;
   }  
 } 
*/
/*
 for(int i=1;i<pos.size()-1;i++){
   int zi = (pos[i].pz - min_zlayer_value)/min_zdistance;
   int state = shape_type[zi];
   if(fabs(pos[i].pz-pos[i+1].pz)<1e-8){
     if(fabs(pos[i].py-pos[i+1].py)<1e-8){
       if(state==0 && abs(pos[i+1].px-pos[i].px)>min_xdistance){
         tmesh.push_back(pos[i]);
         tmesh.push_back(pos[i+1]);
         if(slope1==-1){
           if(prepp1.size()==0){
             prepp1.push_back(tmesh.size()-2);
           } 
           else{
             slope1=(pos[i].px - tmesh[prepp1[0]].px)>0?0:1;
             prepp1.clear();
             prepp1.push_back(tmesh.size()-2);
           }
         }
         else{
          int tmps = (pos[i].px - tmesh[prepp1[0]].px)>0?0:1;
          if(tmps==1 && slope1==0){
           tmesh[prepp1[0]].px = pos[i].px;tmps = 0;
          }
          prepp1[0] = tmesh.size()-2;
          slope1 = tmps;
         }
         if(slope2==-1){
           if(prepp2.size()==0){
             prepp2.push_back(tmesh.size()-1);
           } 
           else{
             slope2=(pos[i+1].px - tmesh[prepp2[0]].px)>0?0:1;
             prepp2.clear();
             prepp2.push_back(tmesh.size()-1);
           }
         }
         else{
          int tmps = (pos[i+1].px - tmesh[prepp2[0]].px)>0?0:1;
          if(tmps==0 && slope2==1){
           tmesh[prepp2[0]].px = pos[i+1].px; tmps = 1;
          }
          prepp2[0] = tmesh.size()-1;
          slope2 = tmps;
         }
       } 
       else{
         if(state==0 || first_line){
            tmesh.push_back(pos[i]);
         }
       }
     }
     else{
       tmesh.push_back(pos[i]);
       tmesh.push_back(pos[i+1]);
       first_line = false;
     }
   }
   else{
     tmesh.push_back(pos[i]);
     //tmesh.push_back(pos[i+1]);
     first_line = true;
     slope1 = -1; prepp1.clear();
     slope2 = -1; prepp2.clear();
   }
 }
 tmesh.push_back(pos[pos.size()-1]);

 for(int i=1;i<pos.size()-1;i++){
   if(fabs(pos[i].px - pos[i-1].px)<1e-8
   && fabs(pos[i].py - pos[i-1].py)<1e-8
   && fabs(pos[i].pz - pos[i-1].pz)<1e-8){
       continue;
   }
*/
/*   if(fabs(pos[i+1].pz - pos[i].pz)<1e-8){
     if(fabs(pos[i+1].py - pos[i].py)<1e-8){
      if(fabs(pos[i+1].px - pos[i].px) > min_xdistance){
       if(prepp.size()!=0){
         if(pos[i].px>prepp[0].px){
           Spin tmp= pos[i];
           tmp.px = prepp[0].px;
           mesh.push_back(tmp);
         }
         if(pos[i+1].px<prepp[1].px){
           Spin tmp= pos[i+1];
           tmp.px = prepp[1].px;
           mesh.push_back(tmp);
         }
         prepp.clear();
       }
       prepp.push_back(mesh[mesh.size()-2]);
       prepp.push_back(mesh[mesh.size()-1]);
       i++;
       continue;
      }
     }
   }
   else{
    prepp.clear();
   }
   mesh.push_back(pos[i]); 
*/
 //}
//================save points=======================
 ofstream outfile(ofname);
 //cerr<<meshsize<<" "<<region_count[unique_region[region]]<<" "<<unique_region[region]<<endl;
 double array[2]={0,10};
 /*outfile<<meshsize*((array[1]-array[0])/min_zdistance+1)+region_count[unique_region[region]]<<endl;
 for(int i=0;i<tmesh.size();i++){
   for(int j=0;j<tmesh[i].size();j++){
     for(double t=min(array[0], tmesh[i][j].pz);t<=tmesh[i][j].pz;t+=min_zdistance)
      outfile<<tmesh[i][j].px<<" "<<tmesh[i][j].py<<" "<<t<<endl;//tmesh[i][j].pz<<endl;
   }
  // outfile<<tmesh[i].px<<" "<<tmesh[i].py<<" "<<tmesh[i].pz<<endl;
 }*/
 for(int i=0;i<dataSize;i++){
   if(data[i].region == unique_region[region])
   {  //outfile<<data[i].px<<" "<<data[i].py<<" "<<data[i].pz<<endl;
     for(double j=min(array[0], data[i].pz);j<=data[i].pz;j+=min_zdistance)
       outfile<<data[i].px<<" "<<data[i].py<<" "<<j<<endl;
   }
 }
 outfile.close();
}

void genVTKfromOrg::ComputeMesh(char * src_dir, char *dir, char *outfname){
 for(int r=1;r<unique_region.size();r++){
  vector<Spin> tmpdata;
  char *str = new char[400];
  sprintf(str, "%s/%s%d", dir, outfname, r); 
  ifstream in(str);
  if(!in.is_open()){
//================================================
   for(int i=1;i<Zlayer_Num.size();i++){
      vector<int> pos; 
      int state = shape_type[(data[Zlayer_Num[i-1]].pz-min_zlayer_value)/min_zdistance];
      if(state == 0){
      for(int j=Zlayer_Num[i-1];j<Zlayer_Num[i];j++){
          if(data[j].region == unique_region[r]){
            tmpdata.push_back(data[j]);
           } 
        }
        continue;
      }
      for(int j=Zlayer_Num[i-1];j<Zlayer_Num[i];j++){
          if(data[j].region == unique_region[r]){
             pos.push_back(j);
          }
      }
      if(pos.size() == 0){
         continue;
      }
//=============================================
      ofstream outfile("tmpmesh");
      outfile<<pos.size()<<endl;
      for(int j=0;j<pos.size();j++){
         outfile<<data[pos[j]].px<<" "<<data[pos[j]].py<<" "<<data[pos[j]].pz<<endl;
      }
      outfile.close();
//=============================================
      char *exe = new char[1024];
      sprintf(exe, "%s/preprocessing/generateVTK/Convex_hull_2/vector_convex_hull_2 tmpmesh tmpregion %0.2f", src_dir, data[pos[0]].pz);
      cerr<<exe<<endl;
      system(exe);
      delete [] exe;
//=============================================
      ifstream infile("tmpregion");
      int n;
      infile>>n;
      for(int j=0;j<n;j++){
        Spin d;
        infile>>d.px>>d.py>>d.pz;
        tmpdata.push_back(d);
      }
      infile.close();
      pos.clear();
//========================================
   }
//===========================================
   ofstream out(str);
   out<<tmpdata.size()<<endl;
   for(int j=0;j<tmpdata.size();j++){
    out<<tmpdata[j].px<<" "<<tmpdata[j].py<<" "<<tmpdata[j].pz<<endl;
   }
   out.close();    
   tmpdata.clear();
   char *exe = new char[100];
   sprintf(exe, "rm tmpmesh");
   system(exe);
   sprintf(exe, "rm tmpregion");
   system(exe);
  }
  in.close();
 }
}
void genVTKfromOrg::Compute2DData()
{
   double xAxis[3], yAxis[3], z[3];
   z[0] = plane_dir[0];
   z[1] = plane_dir[1];
   z[2] = plane_dir[2];

   if(fabs(plane_dir[0])<1e-8
   && fabs(plane_dir[1])<1e-8
   && (fabs(plane_dir[2]-1)<1e-8||fabs(plane_dir[2]+1)<1e-8))
   {
       xAxis[0]=1; xAxis[1]=0;   xAxis[2]=0; 
       yAxis[0]=0;   yAxis[1]=1; yAxis[2]=0;
   }
   else
   {
       double v[3];
       v[0]=0;v[1]=0;v[2]=1;
       double u[3];
       u[0] =  plane_dir[1]*v[2] - plane_dir[2]*v[1];
       u[1] =  plane_dir[2]*v[0] - plane_dir[0]*v[2];
       u[2] =  plane_dir[0]*v[1] - plane_dir[1]*v[0];    

       double sum = u[0]*u[0]+u[1]*u[1]+u[2]*u[2];
       sum = sqrt(sum);
       u[0]= u[0]/sum;
       u[1]= u[1]/sum;
       u[2]= u[2]/sum;

       if(u[0]<0)
       {
          u[1] = -u[1];
          u[0] = -u[0];
       }
       else if(fabs(u[0])<1e-9)
       {
          if(u[1]<0)
          {
             u[1] = -u[1];
             u[0] = -u[0];
          }
       } 
//       xAxis[0]=lbbox[0]+u[0]; xAxis[1]=lbbox[1]+u[1];   xAxis[2]=lbbox[2]+u[2];

//       yAxis[0]=   plane_dir[1]*u[2] - plane_dir[2]*u[1];
//       yAxis[1] =  plane_dir[2]*u[0] - plane_dir[0]*u[2];
//       yAxis[2] =  plane_dir[0]*u[1] - plane_dir[1]*u[0];
//       yAxis[0] = yAxis[0]+lbbox[0];
//       yAxis[1] = yAxis[1]+lbbox[1];
//       yAxis[2] = yAxis[2]+lbbox[2];
         xAxis[0]=u[0];xAxis[1]=u[1];xAxis[2]=u[2];
         yAxis[0]=   plane_dir[1]*u[2] - plane_dir[2]*u[1];
         yAxis[1] =  plane_dir[2]*u[0] - plane_dir[0]*u[2];
         yAxis[2] =  plane_dir[0]*u[1] - plane_dir[1]*u[0];

         if(yAxis[2]<0)
         {
             yAxis[0] = -yAxis[0];
             yAxis[1] = -yAxis[1];
             yAxis[2] = -yAxis[2];
         }
         else if(fabs(yAxis[2])<1e-9)
         {
             if(yAxis[1]<0)
             {
                 yAxis[0] = -yAxis[0];
                 yAxis[1] = -yAxis[1];
                 yAxis[2] = -yAxis[2];
             }
             else if(fabs(yAxis[1])<1e-9)
             { 
                  if(yAxis[0]<0)
                  {
                     yAxis[0] = -yAxis[0];
                     yAxis[1] = -yAxis[1];
                     yAxis[2] = -yAxis[2];
                  }
             }
         }
   }

   double angle = acos(plane_dir[2]);
   double pi = 3.1415926;
   if(angle > pi/2.) angle = pi - angle;
   angle = pi/2. -angle;
   double dd;
   if(angle < pi/2.)
      dd = plane_distance/cos(angle); 
   else
      dd = plane_distance;

   for(int i=0;i<splitData.size();i++)
   {
        double p[3];
        p[0] = plane_min[0] + plane_dir[0] *(double)i * plane_distance;
        p[1] = plane_min[1] + plane_dir[1] *(double)i * plane_distance;
        p[2] = plane_min[2] + plane_dir[2] *(double)i * plane_distance;
        double d = (lbbox[0]-p[0])*plane_dir[0]
                  +(lbbox[1]-p[1])*plane_dir[1]
                  +(lbbox[2]-p[2])*plane_dir[2];
        double angle = acos(plane_dir[2]);
        double pi = 3.1415926;
        if(angle > pi/2.) angle = pi - angle;
        angle = pi/2. -angle;
        double dd; 
        if(angle < pi/2.)
           dd = d/cos(angle);
        else
           dd = d;

        double newlb[3];
        if(angle < pi/2.)
        {
           double vec[2];
           double ss = plane_dir[0]*plane_dir[0]+plane_dir[1]*plane_dir[1];
           ss= sqrt(ss);
           vec[0] = plane_dir[0]/ss;
           vec[1] = plane_dir[1]/ss;
           newlb[0] = lbbox[0] - dd*vec[0];
           newlb[1] = lbbox[1] - dd*vec[1];
           newlb[2] = lbbox[2] ;
        }
        else
        {
           newlb[0] = lbbox[0] - dd*plane_dir[0];
           newlb[1] = lbbox[1] - dd*plane_dir[1];
           newlb[2] = lbbox[2] - dd*plane_dir[2];
        }
//        double x[3], y[3];
//        x[0] = xAxis[0] - d*plane_dir[0];
//        x[1] = xAxis[1] - d*plane_dir[1];
//        x[2] = xAxis[2] - d*plane_dir[2];
//        y[0] = yAxis[0] - d*plane_dir[0];
//        y[1] = yAxis[1] - d*plane_dir[1];
//        y[2] = yAxis[2] - d*plane_dir[2];
         xCoor[0].push_back(xAxis[0]);
         xCoor[1].push_back(xAxis[1]);
         xCoor[2].push_back(xAxis[2]);
         yCoor[0].push_back(yAxis[0]);
         yCoor[1].push_back(yAxis[1]);
         yCoor[2].push_back(yAxis[2]);
         coor[0].push_back(newlb[0]);
         coor[1].push_back(newlb[1]);
         coor[2].push_back(newlb[2]);
//      double xAxis[3], yAxis[3], z[3];
//      z[0] = plane_dir[0];
//      z[1] = plane_dir[1];
//      z[2] = plane_dir[2];
//      if(splitData[i].size()>1)
//      {
//         GenerateCoordinates(splitData[i][0], splitData[i][1],
//                             z, xAxis, yAxis);

//         if(yAxis[2]<0)
//         {
//            yAxis[0] = -yAxis[0];
//            yAxis[1] = -yAxis[1];
//            yAxis[2] = -yAxis[2];
//         }
//         else if(fabs(yAxis[2])<1e-8)
//         {
//             if(yAxis[1]<0)
//             {
//                 yAxis[0] = -yAxis[0];
//                 yAxis[1] = -yAxis[1];
//                 yAxis[2] = -yAxis[2];
//             }
//             else if(fabs(yAxis[1])<1e-8) 
//             {
//                 if(yAxis[0]<0)
//                 {
//                   yAxis[0] = -yAxis[0];
//                   yAxis[1] = -yAxis[1];
//                   yAxis[2] = -yAxis[2];
//                 }
//             }
//         } 
//         xCoor[0].push_back(xAxis[0]);
//         xCoor[1].push_back(xAxis[1]);
//         xCoor[2].push_back(xAxis[2]);
//         yCoor[0].push_back(yAxis[0]);
//         yCoor[1].push_back(yAxis[1]);
//         yCoor[2].push_back(yAxis[2]);
//      }
//      else
//      {
//         Spin tmp = splitData[i][0];
//         tmp.px = 0; tmp.py = 0;
//         split2dData[i].push_back(tmp);
//         xCoor[0].push_back(0);
 //        xCoor[1].push_back(0);
//         xCoor[2].push_back(0);
//         yCoor[0].push_back(0);
 //        yCoor[1].push_back(0);
 //        yCoor[2].push_back(0);
 //        continue;
//      }
//------------------------------------------------------------//
      for(int j=0;j<splitData[i].size();j++)
      {
         Spin pp;
         
//redefine the num of planes=================
         pp.px = newlb[0];
         pp.py = newlb[1];
         pp.pz = newlb[2];
         double xx, yy;
         SetNewCoordinates(splitData[i][j], pp, 
                           xAxis, yAxis,
                           xx, yy);
         Spin tmp = splitData[i][j];
         tmp.px = xx;tmp.py=yy; tmp.pz=0;
         split2dData[i].push_back(tmp);
      }


      
   }//cerr<<"compute2D"<<endl;
}

void genVTKfromOrg::SplitData()
{
  int count = 0;
  
  Zlayer_Num.push_back(0);
  
  for(int i=1;i<dataSize;i++)
  {
    if(fabs(data[i].pz - data[i-1].pz) > 0.0001)
    {
      Zlayer_Num.push_back(i);
    }
    else{
     if(fabs(data[i].py - data[i-1].py) > 0.0001){
        min_ydistance = fabs(data[i].py - data[i-1].py);
     }
     else{
      if(fabs(data[i].px - data[i-1].px) > 0.0001){
       min_xdistance = fabs(data[i].px - data[i-1].px);
      }
     }
    }
  }
  
 // cerr<<Zlayer_Num.size();
  
  Zlayer_Num.push_back(dataSize);
}
void genVTKfromOrg::SaveDensityBylayer(char *dir, char *file)
{
   char *str = new char[400];
   sprintf(str, "%s/%s%s", dir, file, planename);
   ifstream infile(str);
   if(!infile.is_open()){
	ofstream outfile (str);

	outfile<<splitData.size()<<endl;
	for(int i=0;i<splitData.size();i++)
	{
		outfile<<layerDensity[i]<<endl;
	}

	outfile.close();
   }
   infile.close();
   delete [] str;	
}
void genVTKfromOrg::SaveSortedData(char *file)
{
  ofstream outfile;
 
 //=======================================================
  outfile.open(file);
  
  outfile<<dataSize<<endl;
  //outfile<<"Density_Scaling "<<scaling<<endl;
  //outfile<<"Zlayer_Num "<<Zlayer_Num.size()<<endl;
  //outfile<<"Z_Range "<<data[0].pz<<" "<<data[dataSize-1].pz<<endl;
  //outfile<<"Z_Distance "<<data[Zlayer_Num[1]].pz - data[0].pz<<endl;  

 for(int i=0;i<dataSize;i++)
 {
   double exp,coe;
   
   exp = (double)getNumOfIntegerDigits(data[i].den);
   coe = data[i].den/pow(10., exp);
   
   outfile<<data[i].px<<" "<<data[i].py<<" "<<data[i].pz<<" "
	  <<data[i].vx<<" "<<data[i].vy<<" "<<data[i].vz<<" "
	  <<data[i].den<<endl;//" "<<exp<<" "<<coe<<" "<<data[i].atom<<" "<<data[i].region<<endl;
 }
 outfile.close();
/*
//=======================================================
  char *str = new char[400];	//cerr<<file<<endl;
  for(int i=0;i<Zlayer_Num.size()-1;i++)
  {
  	double mean[6];
  	for(int ii=0;ii<6;ii++)
	{
		mean[ii] = 0;  
	}
	    sprintf(str, "%s_layer%d_sorted", file2, i);
	    cerr<<str<<endl;

//=======================================================
	    outfile.open(str);

	  outfile<<Zlayer_Num[i+1] - Zlayer_Num[i]<<endl;

	  for(int j=Zlayer_Num[i];j<Zlayer_Num[i+1];j++)
	  {
	   double exp,coe;
	   
	   exp = (double)getNumOfIntegerDigits(data[j].den);
	   coe = data[j].den/pow(10., exp);

	   outfile<<data[j].px<<" "<<data[j].py<<" "<<data[j].pz<<" "
		  <<data[j].vx<<" "<<data[j].vy<<" "<<data[j].vz<<" "
		  <<data[j].den<<" "<<exp<<" "<<coe<<endl;

		mean[0] = mean[0] + data[j].px;
		mean[1] = mean[1] + data[j].py;
		mean[2] = mean[2] + data[j].vx;
		mean[3] = mean[3] + data[j].vy;
		mean[4] = mean[4] + data[j].vz;
		mean[5] = mean[5] + data[j].den;
	  }
	  outfile.close();

//=======================================================
	 for(int ii=0;ii<6;ii++)
	 {
		mean[ii] = mean[ii]/(Zlayer_Num[i+1] - Zlayer_Num[i]);
	 }

	  double std[6];
	  for(int ii=0;ii<6;ii++)
	  {
		std[ii] = 0; 
	  }
	  for(int j=Zlayer_Num[i];j<Zlayer_Num[i+1];j++)
	  {
		std[0] = std[0] + (data[j].px - mean[0]) * (data[j].px - mean[0]);
		std[1] = std[1] + (data[j].py - mean[1]) * (data[j].py - mean[1]);
		std[2] = std[2] + (data[j].vx - mean[2]) * (data[j].vx - mean[2]);
		std[3] = std[3] + (data[j].vy - mean[3]) * (data[j].vy - mean[3]);
		std[4] = std[4] + (data[j].vz - mean[4]) * (data[j].vz - mean[4]);
		std[5] = std[5] + (data[j].den - mean[5]) * (data[j].den - mean[5]);
	  }
	  for(int ii=0;ii<6;ii++)
	  {
		std[ii] = sqrt(std[ii]/(Zlayer_Num[i+1] - Zlayer_Num[i]-1));
	  }

//==================================================================================
	    sprintf(str, "%s_layer%d_sorted.txt", file2, i);
	    cerr<<str<<endl;

	    outfile.open(str);

	//  outfile<<Zlayer_Num[i+1] - Zlayer_Num[i]<<endl;

	  for(int j=Zlayer_Num[i];j<Zlayer_Num[i+1];j++)
	  {
	   double exp,coe;
	   
	   exp = (double)getNumOfIntegerDigits(data[j].den);
	   coe = data[j].den/pow(10., exp);

	   outfile<<(data[j].px-mean[0])/std[0]<<" "<<(data[j].py-mean[1])/std[1]<<" "
		  <<(data[j].vx-mean[2])/std[2]<<" "<<(data[j].vy-mean[3])/std[3]<<" "<<(data[j].vz-mean[4])/std[4]<<" "
		  <<(data[j].den-mean[5])/std[5]<<endl;
	  }
	    
	    outfile.close();
//==================================================================================
	    sprintf(str, "%s_layer%d_sorted_ld.txt", file2, i);
	    cerr<<str<<endl;

	    outfile.open(str);

	//  outfile<<Zlayer_Num[i+1] - Zlayer_Num[i]<<endl;

	  for(int j=Zlayer_Num[i];j<Zlayer_Num[i+1];j++)
	  {
	   double exp,coe;
	   
	   exp = (double)getNumOfIntegerDigits(data[j].den);
	   coe = data[j].den/pow(10., exp);

	   outfile<<(data[j].px-mean[0])/std[0]<<" "<<(data[j].py-mean[1])/std[1]<<" "
		  <<(data[j].den-mean[5])/std[5]<<endl;
	  }
	    
	    outfile.close();
  }
  delete [] str;*/
//=======================================================
}

void genVTKfromOrg::SaveDataByLayer(char *dir, char *rawfile, char *twodfile){
 char *str = new char[400];
 sprintf(str, "%s/%s%s", dir, rawfile, planename);

 ifstream infile(str);
 if(!infile.is_open()){
 ofstream outfile (str);
 outfile<<splitData.size()<<endl;
 for(int i=0;i<splitData.size();i++){
  outfile<<splitData[i].size()<<endl;
  for(int j=0;j<splitData[i].size();j++){
   outfile<<splitData[i][j].px<<" "<<splitData[i][j].py<<" "<<splitData[i][j].pz
          <<" "<<splitData[i][j].vx<<" "<<splitData[i][j].vy<<" "<<splitData[i][j].vz
          <<" "<<splitData[i][j].den
          <<" "<<splitData[i][j].atom
          <<" "<<splitData[i][j].region<<endl;
    //      <<" "<<1<<endl;//raw data
  }
 }
 outfile.close(); 
 }
 infile.close(); 

 sprintf(str, "%s/%s%s", dir, twodfile, planename);
 infile.open(str);
 if(!infile.is_open()){
 ofstream outfile(str);
 outfile<<splitData.size()<<endl;;
 for(int i=0;i<splitData.size();i++){
  outfile<<splitData[i].size()<<endl;
  for(int j=0;j<splitData[i].size();j++){
   outfile<<split2dData[i][j].px<<" "<<split2dData[i][j].py<<" "<<split2dData[i][j].pz
     <<" "<<splitData[i][j].vx<<" "<<splitData[i][j].vy<<" "<<splitData[i][j].vz
     <<" "<<splitData[i][j].den
     <<" "<<splitData[i][j].atom
     <<" "<<splitData[i][j].region<<endl;
   //  <<" "<<1<<endl;//rawdata
  }
 }
 outfile.close();
 }
 infile.close();
 delete [] str;
}

void genVTKfromOrg::SaveSplitData(char *file)
{
	ofstream outfile;

	char *str = new char[400];
	
	for(int i=0;i<splitData.size();i++)
	{
		sprintf(str, "%s/%s%d.txt", file, planename, i);
    		outfile.open(str);
		
		outfile<<splitData[i].size()<<endl;
		for(int j=0;j<splitData[i].size();j++)
		{
   			double exp,coe;
   			exp = (double)getNumOfIntegerDigits(splitData[i][j].den);
   			coe = splitData[i][j].den/pow(10., exp);

			outfile<<splitData[i][j].px<<" "<<splitData[i][j].py<<" "<<splitData[i][j].pz
				<<" "<<splitData[i][j].vx<<" "<<splitData[i][j].vy<<" "<<splitData[i][j].vz
				<<" "<<splitData[i][j].den<<endl;
		}

		outfile.close();

//===========================================================
                sprintf(str, "%s/layer%s%d.txt", file, planename, i);
                outfile.open(str);

                outfile<<splitData[i].size()<<endl;
                for(int j=0;j<splitData[i].size();j++)
                {
                        outfile<<split2dData[i][j].px<<" "<<split2dData[i][j].py<<" "<<split2dData[i][j].pz
                                <<" "<<splitData[i][j].vx<<" "<<splitData[i][j].vy<<" "<<splitData[i][j].vz
                                <<" "<<splitData[i][j].den<<endl;
                }

                outfile.close();
   
	}

        delete [] str;
}
void genVTKfromOrg::SaveIndex(char *file)
{
        ofstream outfile;
        char * str = new char[400];

        for(int i=0;i<splitData.size();i++)
        {
                sprintf(str, "%s/index%d.txt", file, i);
                outfile.open(str);
                outfile<<splitData[i].size()<<endl;
                for(int j=0;j<splitData[i].size();j++)
                {
                       outfile<<splitIndex[i][j]<<endl;
                }
                outfile.close();
        }

        sprintf(str, "%s/reverseindex.txt",file);
        outfile.open(str);
        outfile<<data.size()<<endl;
        for(int i=0;i<splitReverseIndex[0].size();i++)
            outfile<<splitReverseIndex[0][i]<<" "<<splitReverseIndex[1][i]<<endl;
        outfile.close();

        delete [] str;
}

void genVTKfromOrg::Save2DDensity(char *file)
{
    ofstream outfile;
    char *str = new char[400];

    for(int i=0;i<splitData.size();i++)
    {
        sprintf(str,"%s/density%s%d.txt", file, planename, i);
        ifstream infile(str);
        if(!infile.is_open()){
        outfile.open(str);
        vector<Spin> tmp;
        for(int j=0;j<splitData[i].size();j++)
           tmp.push_back(split2dData[i][j]);

        sort(tmp.begin(), tmp.end(), Sort_Spin_Y);
        sort(tmp.begin(), tmp.end(), Sort_Spin_Y);

        int ycount = 0;
        int xcount = 0;
        vector<int> layercount;
        double prevx, x;
        double prevy, y;
        int count = 0;
        for(int j=0;j<splitData[i].size();j++)
        {
          if(j==0)
          {
             prevx = tmp[j].px;
             prevy = tmp[j].py;
             count++;
          }
          else
          {
             x = tmp[j].px;
             y = tmp[j].py;
             if(fabs(y - prevy) > 0.0001)
             {
                if(count > xcount) xcount = count;
                layercount.push_back(count);
                count=1;
                ycount++;
             }
             else
             {
                count++;
             }
             prevx = x;
             prevy = y;
          }
        }
        ycount++;
        if(count > xcount) xcount = count;
        layercount.push_back(count);

//------------------------------------output files------------------------------
        count = 0;
        outfile<<1<<" "<<ycount<<" "<<xcount<<endl;
        for(int j=0;j<splitData[i].size();j++)
        {
          if(j==0)
          {
             prevx = tmp[j].px;
             prevy = tmp[j].py;
             outfile<<tmp[j].den<<" ";
          }
          else
          {
             x = tmp[j].px;
             y = tmp[j].py;

             if(fabs(y - prevy) > 0.0001)
             {
                if(layercount[count]< xcount)
                {
                    for(int t=0;t<xcount-layercount[count];t++)
                         outfile<<tmp[j-1].den<<" ";
                }
                outfile<<endl;
                outfile<<tmp[j].den<<" ";
                count++;
             }
             else
             {
                outfile<<tmp[j].den<<" ";
             }
             prevx = x;
             prevy = y;
          }
        }
        if(layercount[count]< xcount)
        {
          for(int t=0;t<xcount-layercount[count];t++)
               outfile<<tmp[splitData[i].size()-1].den<<" ";
        }
        outfile<<endl;
        outfile.close();

        tmp.clear();
        layercount.clear();
       }
       infile.close();
    }
//----------------------------------------------------------------------------
    delete [] str;
}

void genVTKfromOrg::SaveAllData(char *file)
{
        ofstream outfile;
        char * str = new char[400];

        sprintf(str, "%s/layerall.txt", file);
        outfile.open(str);
        outfile<<splitData.size()<<endl;
        for(int i=0;i<splitData.size();i++)
        {
              outfile<<splitData[i].size()<<endl;
                for(int j=0;j<splitData[i].size();j++)
                {
       //                 double exp,coe;
       //                 exp = (double)getNumOfIntegerDigits(splitData[i][j].den);
       //                 coe = splitData[i][j].den/pow(10., exp);

                        outfile<<splitData[i][j].px<<" "<<splitData[i][j].py<<" "<<splitData[i][j].pz
                                <<" "<<splitData[i][j].vx<<" "<<splitData[i][j].vy<<" "<<splitData[i][j].vz
                                <<" "<<splitData[i][j].den<<endl;
                  }

       }
      outfile.close();

      sprintf(str, "%s/zall.txt", file);
      outfile.open(str);
      outfile<<Zlayer_Num.size()-1<<endl;
      for(int i=1;i<Zlayer_Num.size();i++)
      { 
          outfile<<Zlayer_Num[i] - Zlayer_Num[i-1]<<endl;   
          for(int j=Zlayer_Num[i-1];j<Zlayer_Num[i];j++)
          {
              outfile<<data[j].px<<" "<<data[j].py<<" "<<data[j].pz<<" "
                     <<data[j].vx<<" "<<data[j].vx<<" "<<data[j].vz<<" "
                     <<data[j].den<<endl;
          }
      }
      outfile.close();

      delete [] str;
}

void genVTKfromOrg::SaveRegiontoFile(char *file){
  for(int i=1;i<region_count.size();i++){
	char *str = new char[400];
	sprintf(str, "%s/region%d", file, unique_region[i]);//i+min_region);
        ifstream infile(str);
        if(!infile.is_open()){
	  ofstream outfile(str);
	  outfile<<region_count[unique_region[i]]<<endl;  
          int count = 0;
	  for(int j=0;j<dataSize;j++){
		if(data[j].region == unique_region[i] )//i+min_region)
	      	{
			outfile<<data[j].px<<" "<<data[j].py<<" "<<data[j].pz<<endl;
	        }
	   }
	  outfile.close();

          double rlb[3]={9e+9,9e+9,9e+9};
          double rrb[3]={-9e+9,-9e+9,-9e+9};
          for(int j=1;j<Zlayer_Num.size();j++){
            if(data[Zlayer_Num[j-1]].pz>=region_plane_z[(i-1)*2+1]+min_zdistance
            && data[Zlayer_Num[j-1]].pz<=region_z[(i-1)*2+1]){
                 for(int jj=Zlayer_Num[j-1];jj<Zlayer_Num[j];jj++){
                  if(data[jj].region>0){
                   if(data[jj].px>rrb[0])rrb[0]=data[jj].px;
                   if(data[jj].py>rrb[1])rrb[1]=data[jj].py;
                   if(data[jj].pz>rrb[2])rrb[2]=data[jj].pz;
                   if(data[jj].px<rlb[0])rlb[0]=data[jj].px;
                   if(data[jj].py<rlb[1])rlb[1]=data[jj].py;
                   if(data[jj].pz<rlb[2])rlb[2]=data[jj].pz;
                  }
                 }
              }
          }
          rrb[2] += min_zdistance;
          cerr<<rrb[2]<<endl;
          int size = 0;
          vector<int> rindex;
          vector<Spin> newp;
          for(int j=1;j<Zlayer_Num.size();j++)
          {
            if(data[Zlayer_Num[j-1]].pz>=region_plane_z[(i-1)*2+1]+min_zdistance//region_z[(i-1)*2]-min_zdistance
            && data[Zlayer_Num[j-1]].pz<=region_z[(i-1)*2+1]+min_zdistance){
              //size+=Zlayer_Num[j]-Zlayer_Num[j-1];
              for(int jj=Zlayer_Num[j-1];jj<Zlayer_Num[j];jj++){
                 rindex.push_back(jj);
              }
            }/*
           else if(data[Zlayer_Num[j-1]].pz>=region_plane_z[(i-1)*2+1]+min_zdistance
             && data[Zlayer_Num[j-1]].pz<=region_z[(i-1)*2+1]+min_zdistance){
              for(int jj=Zlayer_Num[j-1];jj<Zlayer_Num[j];jj++){
                if(data[jj].px>=rlb[0] && data[jj].px<=rrb[0]
                && data[jj].py>=rlb[1] && data[jj].py<=rrb[1]
                && data[jj].pz>=rlb[2] && data[jj].pz<=rrb[2]){
                  rindex.push_back(jj);
                }
              }
            }*/
          }
          size = rindex.size();
          char *vtkstr = new char[400];
          sprintf(vtkstr, "%s/region%d.vtk", file, unique_region[i]);
          outfile.open(vtkstr);
          outfile<<"# vtk DataFile Version 2.0"<<endl;
          outfile<<vtkstr<<endl;
          outfile<<"ASCII"<<endl;
          outfile<<"DATASET UNSTRUCTURED_GRID"<<endl;
          outfile<<"POINTS "<<size<<" float"<<endl;
          for(int j=0;j<size;j++){
            int in = rindex[j];
            outfile<<data[in].px<<" "<<data[in].py<<" "<<data[in].pz<<endl;
          }
          outfile<<endl;
          outfile<<"POINT_DATA "<<size<<endl;
          outfile<<"SCALARS density float 1"<<endl;
          outfile<<"LOOKUP_TABLE default"<<endl;
          for(int j=0;j<size;j++){
            int in = rindex[j];
            int r = data[in].region>0?1:0;
            outfile<<r<<endl;
          }
          rindex.clear();
          /*for(int j=1;j<Zlayer_Num.size();j++)
          {
            if(data[Zlayer_Num[j-1]].pz>=region_z[(i-1)*2]
            && data[Zlayer_Num[j-1]].pz<=region_z[(i-1)*2+1]){
              for(int jj=Zlayer_Num[j-1];jj<Zlayer_Num[j];jj++)
              {
                 outfile<<data[jj].px<<" "<<data[jj].py<<" "<<data[jj].pz<<endl;
              }
            }
          }
          outfile<<endl;
          outfile<<"POINT_DATA "<<size<<endl;
          outfile<<"VECTORS velocity float"<<endl;
          for(int j=1;j<Zlayer_Num.size();j++)
          {
            if(data[Zlayer_Num[j-1]].pz>=region_z[(i-1)*2]
            && data[Zlayer_Num[j-1]].pz<=region_z[(i-1)*2+1]){
              for(int jj=Zlayer_Num[j-1];jj<Zlayer_Num[j];jj++)
              {
                 outfile<<data[jj].px<<" "<<data[jj].py<<" "<<data[jj].pz<<endl;
              }
            }
          }
          outfile<<endl;
          outfile<<"SCALARS density float 1"<<endl;
          outfile<<"LOOKUP_TABLE default"<<endl;    
          for(int j=1;j<Zlayer_Num.size();j++)
          {
            if(data[Zlayer_Num[j-1]].pz>=region_z[(i-1)*2]
            && data[Zlayer_Num[j-1]].pz<=region_z[(i-1)*2+1]){
              for(int jj=Zlayer_Num[j-1];jj<Zlayer_Num[j];jj++)
              {
                 outfile<<data[jj].region<<endl;
              }
            }
          }
          outfile<<endl; 
*/
          outfile.close();
          delete [] vtkstr;
        }
        delete [] str;
        infile.close();
  }	
}

void genVTKfromOrg::SavetoVTK(char *file)
{
  ofstream outfile;
  
  char *str = new char[400];	//cerr<<file<<endl;

//cerr<<file<<" "<<Zlayer_Num.size()-1<<endl;
//=======================================================
/*  for(int i=1;i<Zlayer_Num.size();i++)
  {
    sprintf(str, "%s/%d.vtk", file, i-1);
    
    outfile.open(str);
  
    outfile<<"# vtk DataFile Version 2.0"<<endl;
    outfile<<str<<endl;
    outfile<<"ASCII"<<endl;
    outfile<<"DATASET UNSTRUCTURED_GRID"<<endl;   

    outfile<<"POINTS "<<Zlayer_Num[i] - Zlayer_Num[i-1]<<" float"<<endl;   
    for(int j=Zlayer_Num[i-1];j<Zlayer_Num[i];j++)
    {
      outfile<<data[j].px<<" "<<data[j].py<<" "<<data[j].pz<<endl;
    }
    outfile<<endl;
    
    outfile<<"POINT_DATA "<<Zlayer_Num[i] - Zlayer_Num[i-1]<<endl;
    outfile<<"VECTORS velocity float"<<endl;  
    for(int j=Zlayer_Num[i-1];j<Zlayer_Num[i];j++)
    {
      outfile<<data[j].vx<<" "<<data[j].vy<<" "<<data[j].vz<<endl;
    }
    outfile<<endl; 
    
    outfile<<"SCALARS density float 1"<<endl;
    outfile<<"LOOKUP_TABLE default"<<endl;    
    for(int j=Zlayer_Num[i-1];j<Zlayer_Num[i];j++)
    {
      outfile<<data[j].den<<endl;
    }
    outfile<<endl; 
    
    outfile.close();
  }
*/
    for(int i=0;i<splitData.size();i++)
    {
    sprintf(str, "%s/%s%d.vtk", file, planename, i);
//cerr<<str<<endl;   

    ifstream infile(str);
    if(!infile.is_open()){ 
    outfile.open(str);
  
    outfile<<"# vtk DataFile Version 2.0"<<endl;
    outfile<<str<<endl;
    outfile<<"ASCII"<<endl;
    outfile<<"DATASET UNSTRUCTURED_GRID"<<endl;   

    outfile<<"POINTS "<<splitData[i].size()<<" float"<<endl;   
/*
    Spin x,y;
    x = splitData[i][0];
    y = splitData[i][1];
    
    double xaxis[3];
    xaxis[0] = y.px - x.px;
    xaxis[1] = y.py - x.py;
    xaxis[2] = y.pz - x.pz;

    double sum = sqrt(xaxis[0]*xaxis[0]+xaxis[1]*xaxis[1]+xaxis[2]*xaxis[2]); 
    xaxis[0] = xaxis[0]/sum;
    xaxis[1] = xaxis[1]/sum;
    xaxis[2] = xaxis[2]/sum;

    double z[3];z[0]=plane_dir[0];z[1]=plane_dir[1];z[2]=plane_dir[2];
    sum = sqrt(z[0]*z[0]+z[1]*z[1]+z[2]*z[2]);
    z[0]=z[0]/sum;
    z[1]=z[1]/sum;
    z[2]=z[2]/sum;
    double yaxis[3];
    yaxis[0] = z[1]*xaxis[2] - z[2] * xaxis[1];
    yaxis[1] = z[2]*xaxis[0] - z[0] * xaxis[2];
    yaxis[2] = z[0]*xaxis[1] - z[1] * xaxis[0];

    sum = sqrt(yaxis[0]*yaxis[0]+yaxis[1]*yaxis[1]+yaxis[2]*yaxis[2]);
    yaxis[0] = yaxis[0]/sum;
    yaxis[1] = yaxis[1]/sum;
    yaxis[2] = yaxis[2]/sum;
*/
//    char *name = new char[100];
//    sprintf(name, "%s/coor%d.txt", file,i);
//    ofstream coorout(name);
//    coorout<<x.px<<" "<<x.py<<" "<<x.pz<<endl;
//    coorout<<xaxis[0]<<" "<<xaxis[1]<<" "<<xaxis[2]<<endl;
//    coorout<<yaxis[0]<<" "<<yaxis[1]<<" "<<yaxis[2]<<endl;
//    coorout<<z[0]<<" "<<z[1]<<" "<<z[2]<<endl;
//    coorout.close();

//    sprintf(name, "%s/format%d.txt",file,i);
//    ofstream formatout(name);

//    formatout<<splitData[i].size()<<endl;
//    int xi = 0; int yi =0;
//    double prevx, prevy;
//    double xvalue, yvalue;
//    prevx = 0; prevy = 0;
//    xvalue = 0; yvalue = 0;
//    int maxx = -1;
//    int maxy = -1;


//    cerr<<split2dData[i].size()<<endl;
    for(int j=0;j<splitData[i].size();j++)
    {
//      double tmp[3];
//      tmp[0] = splitData[i][j].px - x.px;
//      tmp[1] = splitData[i][j].py - x.py;
//      tmp[2] = splitData[i][j].pz - x.pz;

//      prevx = xvalue; prevy = yvalue;

//      xvalue = tmp[0] * xaxis[0] + tmp[1] * xaxis[1] + tmp[2] * xaxis[2];
//      yvalue = tmp[0] * yaxis[0] + tmp[1] * yaxis[1] + tmp[2] * yaxis[2]; 

//      formatout<<xi<<" "<<yi<<endl;
//      if(xvalue < prevx)
//      {
//        xi = 0;yi++;

//      else
//      {
//        xi++;
//      }
  //    outfile<<splitData[i][j].px<<" "<<splitData[i][j].py<<" "<<splitData[i][j].pz<<endl;
//    	outfile<<xvalue<<" "<<yvalue<<" 0"<<endl;
//        if(xvalue > maxx) maxx = xvalue;
//        if(yvalue > maxy) maxy = yvalue;
        outfile<<split2dData[i][j].px
              <<" "<<split2dData[i][j].py
              <<" "<<split2dData[i][j].pz<<endl; 
   }
    outfile<<endl;
//    formatout.close();

//    delete [] name;
 
    outfile<<"POINT_DATA "<<splitData[i].size()<<endl;
    outfile<<"VECTORS velocity float"<<endl;  
    for(int j=0;j<splitData[i].size();j++)
    {
      outfile<<splitData[i][j].vx<<" "<<splitData[i][j].vy<<" "<<splitData[i][j].vz<<endl;
    }
    outfile<<endl; 
    
    outfile<<"SCALARS density float 1"<<endl;
    outfile<<"LOOKUP_TABLE default"<<endl;    
    for(int j=0;j<splitData[i].size();j++)
    {
      outfile<<splitData[i][j].den<<endl;
    }
    outfile<<endl; 
    
    outfile.close();
    }
    infile.close();
  }
//=======================================================
/*
  int count = 0;
  for(int i=0;i<dataSize;i++){
    if(data[i].pz>-1.5 && data[i].pz<10.5){
       count++;
    }
  }
  sprintf(str, "%s/all.vtk", file);
  outfile.open(str);

    outfile<<"# vtk DataFile Version 2.0"<<endl;
    outfile<<str<<endl;
    outfile<<"ASCII"<<endl;
    outfile<<"DATASET UNSTRUCTURED_GRID"<<endl;   

    outfile<<"POINTS "<<count<<" float"<<endl;  

  for(int j=0;j<dataSize;j++)
  {
    if(data[j].pz>-1.5 && data[j].pz<10.5)
      outfile<<data[j].px<<" "<<data[j].py<<" "<<data[j].pz<<endl;
   }
    outfile<<endl;
    
    outfile<<"POINT_DATA "<<count<<endl;
    outfile<<"VECTORS velocity float"<<endl;  

  for(int j=0;j<dataSize;j++)
  {
    if(data[j].pz>-1.5 && data[j].pz<10.5)
      outfile<<data[j].vx<<" "<<data[j].vy<<" "<<data[j].vz<<endl;
    }
    outfile<<endl; 
    
    outfile<<"SCALARS density float 1"<<endl;
    outfile<<"LOOKUP_TABLE default"<<endl;   
 
  for(int j=0;j<dataSize;j++)
  {
    if(data[j].pz>-1.5 && data[j].pz<10.5)
      outfile<<data[j].region<<endl;
    }
    outfile<<endl;
*/

//  outfile.close();

//=======================================================
/*  for(int i=0;i<region_count.size();i++)
  {
	sprintf(str, "%s/region%d.vtk", file, i+min_region);
	outfile.open(str);

	    outfile<<"# vtk DataFile Version 2.0"<<endl;
	    outfile<<str<<endl;
	    outfile<<"ASCII"<<endl;
	    outfile<<"DATASET UNSTRUCTURED_GRID"<<endl;   

	    outfile<<"POINTS "<<region_count[i]<<" float"<<endl;  
		int count = 0;
	  for(int j=0;j<dataSize;j++)
	  {
		if(data[j].region == i+min_region)
	      	{
			outfile<<data[j].px<<" "<<data[j].py<<" "<<data[j].pz<<endl;
			count++;
		}
	   }
	//	cerr<<data[0].region<<" "<<i+min_region<<" "<<region_count[i]<<" "<<count<<endl;
	    outfile<<endl;
	    
	    outfile<<"POINT_DATA "<<region_count[i]<<endl;
	    outfile<<"VECTORS velocity float"<<endl;  

	  for(int j=0;j<dataSize;j++)
	  {
		if(data[j].region == i+min_region)
	     		outfile<<data[j].vx<<" "<<data[j].vy<<" "<<data[j].vz<<endl;
	    }
	    outfile<<endl; 
	    
	    outfile<<"SCALARS density float 1"<<endl;
	    outfile<<"LOOKUP_TABLE default"<<endl;   
	 
	  for(int j=0;j<dataSize;j++)
	  {
		if(data[j].region == i+min_region)
	      		outfile<<data[j].region<<endl;
	    }
	    outfile<<endl;

	outfile.close();
  }
*/
  delete [] str;
}
void genVTKfromOrg::SaveLayerFormat(char *file)
{
     char *str = new char[400];
//      sprintf(str, "%s/coor%s.txt", file, planename);

     for(int i=0;i<splitData.size();i++){
      sprintf(str, "%s/coor%s%d.txt", file, planename,i);
      ifstream infile(str);
      if(!infile.is_open()){
      ofstream outfile(str);
    //  outfile<<splitData.size()<<endl;;
//      for(int i=0;i<splitData.size();i++)
//      {
      //   sprintf(str, "%s/coor%s%d.txt", file, planename, i);
         outfile<<coor[0][i]<<" "<<coor[1][i]<<" "<<coor[2][i]<<endl;
         outfile<<xCoor[0][i]<<" "<<xCoor[1][i]<<" "<<xCoor[2][i]<<" "<<endl;
         outfile<<yCoor[0][i]<<" "<<yCoor[1][i]<<" "<<yCoor[2][i]<<" "<<endl;
         outfile<<plane_dir[0]<<" "<<plane_dir[1]<<" "<<plane_dir[2]<<endl;
//      }
      outfile.close();
     }
     infile.close();
   }
   delete [] str;
}
void genVTKfromOrg::SaveZFormat(char *file)
{
      ofstream outfile;
      char *str = new char[400];

      sprintf(str, "%s/zformat.txt",file);
      outfile.open(str);
      outfile<<data.size()<<endl;
      //Zlayer_Num.size()-1<<endl;
      int count_x = 0;
      int count_y = 0;
      for(int i=1;i<Zlayer_Num.size();i++)
      {
           count_x = 0;
           count_y = 0;
         //sprintf(str, "%s/format%d.txt", file, i);
//         outfile.open(str);
//          outfile<<Zlayer_Num[i] - Zlayer_Num[i-1]<<endl;
          for(int j=Zlayer_Num[i-1];j<Zlayer_Num[i];j++)
          {
//              outfile<<data[j].px<<" "<<data[j].py<<" "<<data[j].pz<<" "
//                     <<data[j].vx<<" "<<data[j].vx<<" "<<data[j].vz<<" "
//                     <<data[j].den<<endl;
                if(j>Zlayer_Num[i-1])
                {
                   if(fabs(data[j].py-data[j-1].py)<1e-3)
                   {
                      count_x++;
                   }
                   else
                   {
                      count_x = 0;
                      count_y++;
                  }
                }
                outfile<<count_x<<" ";
                outfile<<count_y<<" ";
                outfile<<i-1<<endl;
          }
//         outfile.close();
      }

     outfile.close();
}
void genVTKfromOrg::SaveFormat(char *dir, char *file){
 char *str = new char[400];
 sprintf(str, "%s/%s", dir, file);//, planename);

// ifstream infile(str);
// if(!infile.is_open()){
 ofstream outfile(str);

 outfile<<unique_region.size()<<endl;
 for(int i=0;i<unique_region.size();i++)
  outfile<<unique_region[i]<<" ";
 outfile<<endl;
 
 outfile<<splitData.size()<<endl; //# of Z layers
 outfile<<plane_min[0]<<" "<<plane_min[1]<<" "<<plane_min[2]<<endl;
 outfile<<plane_dir[0]<<" "<<plane_dir[1]<<" "<<plane_dir[2]<<endl;
 outfile<<plane_distance<<endl;

 outfile<<min_xdistance<<endl; //distance of two x values
 outfile<<min_ydistance<<endl; //distance of two y values
 outfile<<data[Zlayer_Num[1]].pz - data[0].pz<<endl; //distance of two z layers
 outfile<<scaling<<endl; //scalar for magnitude
 outfile<<min_den_zero<<" "<<min_den<<" "<<max_den<<endl;
 outfile<<getNumOfIntegerDigits(max_den)-getNumOfIntegerDigits(min_den)+1<<endl;
 outfile<<lbbox[0]<<" "<<lbbox[1]<<" "<<lbbox[2]<<endl;
 outfile<<rbbox[0]<<" "<<rbbox[1]<<" "<<rbbox[2]<<endl;

//format
 outfile<<Zlayer_Num.size()-1<<endl;
 for(int i=1;i<Zlayer_Num.size();i++){
   int count = 0;
   for(int j=Zlayer_Num[i-1]+1; j<Zlayer_Num[i];j++){
     if(data[j].py!=data[j-1].py){
       count++;
     }
   }
   count++;
   outfile<<count<<endl;
   outfile<<data[Zlayer_Num[i-1]].px<<" "<<data[Zlayer_Num[i-1]].py<<" "<<data[Zlayer_Num[i-1]].pz<<endl;
   for(int j=Zlayer_Num[i-1]+1; j<Zlayer_Num[i];j++){
    if(data[j].py != data[j-1].py){
     outfile<<data[j].px<<" "<<data[j].py<<" "<<data[j].pz<<endl;
    } 
   }
 }
 outfile.close(); 
// }
// infile.close();
 delete [] str;
}

void genVTKfromOrg::SaveDataFormat(char *file)
{
	ofstream outfile(file);

        outfile<<unique_region.size()<<endl;
        for(int i=0;i<unique_region.size();i++)
            outfile<<unique_region[i]<<" ";
        outfile<<endl;

	vector<double> temp_x;
	for(int i=0;i<dataSize;i++)
	{
	  temp_x.push_back(data[i].px);
	}
	sort(temp_x.begin(), temp_x.end());
	temp_x.erase(unique(temp_x.begin(), temp_x.end()), temp_x.end());
	sort(temp_x.begin(), temp_x.end());
	double min_xdistance = temp_x[1] - temp_x[0];
	
	vector<double> temp_y;
	for(int i=0;i<dataSize;i++)
	{
	  temp_y . push_back(data[i].py);
	}
	sort(temp_y.begin(), temp_y.end());
	temp_y.erase(unique(temp_y.begin(), temp_y.end()), temp_y.end() );
	sort(temp_y.begin(), temp_y.end());
	double min_ydistance = temp_y[1] - temp_y[0]; 	
	
	//Overall 
	outfile<<splitData.size()<<endl; //# of Z layers
	outfile<<plane_min[0]<<" "<<plane_min[1]<<" "<<plane_min[2]<<endl;
	outfile<<plane_dir[0]<<" "<<plane_dir[1]<<" "<<plane_dir[2]<<endl;
	outfile<<plane_distance<<endl;

	outfile<<min_xdistance<<endl; //distance of two x values
	outfile<<min_ydistance<<endl; //distance of two y values
	outfile<<data[Zlayer_Num[1]].pz - data[0].pz<<endl; //distance of two z layers
	outfile<<scaling<<endl; //scalar for magnitude
        outfile<<min_den_zero<<" "<<min_den<<" "<<max_den<<endl;
        outfile<<getNumOfIntegerDigits(max_den)-getNumOfIntegerDigits(min_den)+1<<endl;
	outfile<<lbbox[0]<<" "<<lbbox[1]<<" "<<lbbox[2]<<endl;
	outfile<<rbbox[0]<<" "<<rbbox[1]<<" "<<rbbox[2]<<endl;

	//Details
	//z_start_value z_start_index z_end_value z_end_index z_distance
	outfile<<data[0].pz<<" "<<0
	  <<" "<<data[dataSize-1].pz<<" "<<dataSize-1
	  <<" "<<data[Zlayer_Num[1]].pz - data[0].pz<<endl; 
	  
	outfile<<Zlayer_Num.size()-1<<endl;
	for(int i=1;i<Zlayer_Num.size();i++)
	{
	  double y_distance;

	  vector<int> x_index;
	  x_index.push_back(Zlayer_Num[i-1]);
	  
	  for(int j=Zlayer_Num[i-1]+1; j<Zlayer_Num[i];j++)
	  {
	    if(data[j].py > data[j-1].py)
	    {
	      x_index.push_back(j);
	    }
	  }
	  x_index.push_back(Zlayer_Num[i]);
	  y_distance = data[x_index[1]].py - data[x_index[0]].py;
	  
	  //y_start_value y_start_index y_end_value y_end_index y_distance
	  outfile<<data[Zlayer_Num[i-1]].py<<" "<<Zlayer_Num[i-1]
	    <<" "<<data[Zlayer_Num[i]-1].py<<" "<<Zlayer_Num[i]-1
	    <<" "<<y_distance<<endl;
	    
	  outfile<<x_index.size()-1<<endl;	    
	  for(int j=1;j<x_index.size();j++)
	  {
	    double x_distance = data[x_index[j-1]+1].px - data[x_index[j-1]].px;
	    //x_start_value x_start_index x_end_value x_end_index x_distance
	    outfile<<data[x_index[j-1]].px<<" "<<x_index[j-1]
	      <<" "<<data[x_index[j]-1].px<<" "<<x_index[j]-1
	      <<" "<<x_distance<<endl;;
	  }
	  
	  x_index.clear();
	}

	outfile.close();
	
	temp_x.clear();
	temp_y.clear();
}
void genVTKfromOrg::GenerateCoordinates(Spin p1, Spin p2, double *z, double *xAxis, double *yAxis)
{
    xAxis[0] = p2.px - p1.px;
    xAxis[1] = p2.py - p1.py;
    xAxis[2] = p2.pz - p1.pz;

    double sum = xAxis[0]*xAxis[0]
               + xAxis[1]*xAxis[1]
               + xAxis[2]*xAxis[2];
    sum = sqrt(sum);
    xAxis[0] = xAxis[0]/sum;
    xAxis[1] = xAxis[1]/sum;
    xAxis[2] = xAxis[2]/sum;

    yAxis[0] = z[1]*xAxis[2] - z[2] * xAxis[1];
    yAxis[1] = z[2]*xAxis[0] - z[0] * xAxis[2];
    yAxis[2] = z[0]*xAxis[1] - z[1] * xAxis[0];
}
void genVTKfromOrg::SetNewCoordinates(Spin p, Spin origin, double *xAxis, double *yAxis,
                           double &x, double &y)
{
    double v[3];
    v[0] = p.px - origin.px;
    v[1] = p.py - origin.py;
    v[2] = p.pz - origin.pz;
    x  = v[0] * xAxis[0]
              + v[1] * xAxis[1]
              + v[2] * xAxis[2];
    y = v[0] * yAxis[0]
              + v[1] * yAxis[1] 
              + v[2] * yAxis[2];
}
void genVTKfromOrg::cleanup()
{
  data.clear();
  splitReverseIndex[0].clear();
  splitReverseIndex[1].clear();
  Zlayer_Num.clear();
  region_count.clear();
  unique_region.clear();
  dataSize = 0;
  plane_min.clear();
  plane_dir.clear();

  for(int i=0;i<splitData.size();i++)
  {
                split2dData[i].clear();
		splitData[i].clear();
                splitIndex[i].clear();
   }
  split2dData.clear();
  splitData.clear();
  splitIndex.clear();
  
  for(int i=0;i<3;i++)
  {
     xCoor[i].clear();
     yCoor[i].clear();
     coor[i].clear();
  }
}
